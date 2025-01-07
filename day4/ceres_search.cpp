#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <mdspan>
#include <algorithm>
#include <ranges>

namespace stdr = std::ranges;
namespace stdv = std::views;

static constexpr auto term = std::array{'X', 'M', 'A', 'S'};
static constexpr auto x_term = std::array{'M', 'A', 'S'};

static constexpr auto deltas = std::array{std::array{-1, -1},
					  std::array{-1, 0},
					  std::array{-1, 1},
					  std::array{0, -1},
					  std::array{0, 1},
					  std::array{1, -1},
					  std::array{1, 0},
					  std::array{1, 1}};

// static constexpr auto x_deltas = std::array{std::array{-1, -1},
// 					    std::array{-1, 1},
// 					    std::array{1, -1},
// 					    std::array{1, 1}};
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
    auto const grid = std::mdspan{input.data(), num_rows, num_cols};

    {
	auto result = int{};
	for (auto row = 0z; row < num_rows; ++row) {
	    for (auto col = 0z; col < num_cols; ++col) {
		if (grid[row, col] == term[0]) {
		    for (auto const &delta: deltas) {
			auto valid = true;
			for (auto idx = 1z; idx < ssize(term); ++idx) {
			    auto const neighbor_row = row + idx * delta[0];
			    auto const neighbor_col = col + idx * delta[1];
			    if (neighbor_row < 0z || neighbor_row >= num_rows ||
				neighbor_col < 0z || neighbor_col >= num_cols || 
				grid[row + idx * delta[0], col + idx * delta[1]]
				    != term[idx]) {
				valid = false;
				break;
			    }
			}
			if (valid) {
			    ++result;
			}
		    } 
		}
	    }
	}
	std::cout << result << '\n';
    }
    {
	auto result = int{};
	for (auto row = 1z; row < num_rows - 1; ++row) {
	    for (auto col = 1z; col < num_cols - 1; ++col) {
		if (grid[row, col] == x_term[1]) {
		    if (((grid[row - 1, col - 1] == x_term[0] &&
			  grid[row + 1, col + 1] == x_term[2]) ||
		         (grid[row - 1, col - 1] == x_term[2] &&
			  grid[row + 1, col + 1] == x_term[0])) &&
		        ((grid[row - 1, col + 1] == x_term[0] &&
			  grid[row + 1, col - 1] == x_term[2]) ||
		         (grid[row - 1, col + 1] == x_term[2] &&
			  grid[row + 1, col - 1] == x_term[0]))) {
			++result;
		    }
		}
	    }
	}
	std::cout << result << '\n';
    }

    return 0;
}

