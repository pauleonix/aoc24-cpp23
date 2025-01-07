#include <cmath>
#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <iterator>
#include <generator>
#include <vector>

namespace stdr = std::ranges;
namespace stdv = std::views;

using Level = int;
using Report = std::vector<Level>;

auto iterate_reports() -> std::generator<Report const &> {
    auto input_stream = std::ifstream{"input"};
    auto line = std::string{};
    while (std::getline(input_stream, line)) {
	auto line_stream = std::istringstream{line};
	auto levels = Report{};
	stdr::copy(stdv::istream<int>(line_stream), back_inserter(levels));
	co_yield levels;
    }
}

struct IsSafe {
    template <class Levels>
    auto operator()(Levels &&levels) const noexcept -> bool {
	return stdr::all_of(levels
			    | stdv::pairwise_transform(std::minus<>{})
			    | stdv::pairwise,
			    [](auto pair_of_diffs) {
				auto const [left_diff, right_diff] = pair_of_diffs;
				return left_diff != 0 && right_diff != 0 &&
				    std::abs(left_diff) <= 3 && std::abs(right_diff) <= 3 &&
				    (left_diff > 0) == (right_diff > 0);
			    }); 
    }
};

struct IsSafeWithProblemDampener {
    template <class Levels>
    auto mask_level(Levels &&levels, int ignore_idx) const noexcept {
	return levels
	       | stdv::enumerate
	       | stdv::filter([ignore_idx](auto const &idx_and_level) {
			          auto const [idx, level] = idx_and_level;
				  return idx != ignore_idx;
			      })
	       | stdv::values;
    }

    template <class Levels>
    auto operator()(Levels &&levels) const noexcept -> bool {
	auto const num_levels = static_cast<int>(ssize(levels));
	return stdr::any_of(stdv::iota(0, num_levels),
			    [&](int ignore_idx) {
				return IsSafe{}(mask_level(levels, ignore_idx));
			    });
    }
};

int main() {
    std::cout << stdr::count_if(iterate_reports(), IsSafe{}) << '\n';
    std::cout << stdr::count_if(iterate_reports(), IsSafeWithProblemDampener{}) << '\n';

    return 0;
}
