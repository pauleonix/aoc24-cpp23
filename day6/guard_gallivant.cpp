#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <mdspan>
#include <algorithm>
#include <ranges>
#include <iterator>
#include <functional>

namespace stdr = std::ranges;
namespace stdv = std::views;

enum Dir {up, right, down, left};

static constexpr auto deltas = std::array{
    std::array{-1z, 0z},
    std::array{0z, 1z},
    std::array{1z, 0z},
    std::array{0z, -1z}};
static constexpr auto deltas_3d = std::array{
    std::array{0z, -1z, 0z},
    std::array{0z, 0z, 1z},
    std::array{0z, 1z, 0z},
    std::array{0z, 0z, -1z}};

template <typename T, std::size_t N>
auto operator+(std::array<T, N> const &left, std::array<T, N> const &right)
	-> std::array<T, N> {
    auto result = std::array<T, N>{};
    stdr::transform(left, right, begin(result), std::plus<>{});
    return result;
}

int main() {
    auto input_stream = std::ifstream{"input"};
    auto line = std::string{};
    auto input = std::vector<char>{};
    auto num_rows = 0z;
    while (std::getline(input_stream, line)) {
	auto line_stream = std::istringstream{line};
	stdr::copy(stdv::istream<char>(line_stream), back_inserter(input));
	if (!empty(line)) {
	    ++num_rows;
	}
    }
    assert(ssize(input) % num_rows == 0z);
    auto const num_cols = ssize(input) / num_rows;
    auto const guard_pos_1d = stdr::distance(begin(input), stdr::find(input, '^'));
    auto const init_guard_pos = std::array{guard_pos_1d / num_cols,
					   guard_pos_1d % num_cols};
    auto const init_guard_dir = up;

    {
	auto const is_on_map = [num_rows, num_cols](auto pos) {
				   return pos[0] >= 0z && pos[0] < num_rows &&
					  pos[1] >= 0z && pos[1] < num_cols;
			       };
	auto lab_map_buffer = input;
	auto const lab_map = std::mdspan{lab_map_buffer.data(), num_rows, num_cols};
	auto guard_pos = init_guard_pos;
	auto guard_dir = init_guard_dir;
	do {
	    auto const next_pos = guard_pos + deltas[guard_dir];
	    if (is_on_map(next_pos) && lab_map[next_pos] == '#') {
		guard_dir = static_cast<Dir>((guard_dir + 1) % 4);
	    }
	    else {
		lab_map[guard_pos] = 'X';
		guard_pos = next_pos;
	    }
	} while (is_on_map(guard_pos));
	std::cout << stdr::count(lab_map_buffer, 'X') << '\n';
    }

    {
	auto const is_on_map = [num_rows, num_cols](auto pos) {
				   return pos[1] >= 0z && pos[1] < num_rows &&
					  pos[2] >= 0z && pos[2] < num_cols;
			       };
	auto lab_map_3d_buffer = std::vector<char>(size(input) * 4uz); 
	for (int dir = 0z; dir < 4z; ++dir) {
	    stdr::copy(input, begin(lab_map_3d_buffer) + dir * ssize(input));
	}
	auto lab_map_3d = std::mdspan{lab_map_3d_buffer.data(), 4, num_rows, num_cols};
	auto guard_pos = std::array{0z, init_guard_pos[0], init_guard_pos[1]};
	for (int dir = 1z; dir < 4z; ++dir) {
	    lab_map_3d[dir, guard_pos[1], guard_pos[2]] = '.';
	}
	auto obstacle_count = int{};
	do {
	    auto const next_pos = guard_pos + deltas_3d[guard_pos[0]];
	    if (is_on_map(next_pos) && lab_map_3d[next_pos] == '#') {
		lab_map_3d[guard_pos] = 'X';
		guard_pos[0] = (guard_pos[0] + 1) % 4;
	    }
	    else {
		if (is_on_map(next_pos) &&
		    lab_map_3d[0z, next_pos[1], next_pos[2]] != 'X' &&
		    lab_map_3d[1z, next_pos[1], next_pos[2]] != 'X' &&
		    lab_map_3d[2z, next_pos[1], next_pos[2]] != 'X' &&
		    lab_map_3d[3z, next_pos[1], next_pos[2]] != 'X') {
		    auto lab_map_3d_buffer_copy = lab_map_3d_buffer;
		    auto const lab_map_3d_copy = std::mdspan{lab_map_3d_buffer_copy.data(),
							     4, num_rows, num_cols};
		    auto guard_pos_copy = guard_pos;
		    lab_map_3d_copy[0, next_pos[1], next_pos[2]] = '#';
		    lab_map_3d_copy[1, next_pos[1], next_pos[2]] = '#';
		    lab_map_3d_copy[2, next_pos[1], next_pos[2]] = '#';
		    lab_map_3d_copy[3, next_pos[1], next_pos[2]] = '#';
		    do {
			auto const next_pos_copy = guard_pos_copy +
						   deltas_3d[guard_pos_copy[0]];
			if (is_on_map(next_pos_copy) &&
			    lab_map_3d_copy[next_pos_copy] == '#') {
			    lab_map_3d_copy[guard_pos_copy] = 'X';
			    guard_pos_copy[0] = (guard_pos_copy[0] + 1) % 4;
			}
			else if(is_on_map(next_pos_copy) &&
				lab_map_3d_copy[next_pos_copy] == 'X') {
			    ++obstacle_count;
			    break;
			}
			else {
			    lab_map_3d_copy[guard_pos_copy] = 'X';
			    guard_pos_copy = next_pos_copy;
			}
		    } while (is_on_map(guard_pos_copy));

		}
		lab_map_3d[guard_pos] = 'X';
		guard_pos = next_pos;
	    }
	} while (is_on_map(guard_pos));
	std::cout << obstacle_count << '\n';
    }

    return 0;
}

