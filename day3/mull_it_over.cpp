#include <iostream>
#include <fstream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <string>
#include <string_view>
#include <regex>

namespace stdr = std::ranges;
namespace stdv = std::views;

static constexpr auto max_digits = 3;
static constexpr auto head_marker = std::string_view{"mul("};
static constexpr auto min_size = ssize(std::string_view{"mul(0,0)"});

bool is_digit(char const input) {
    return char >= '0' && char <= '9';
}


class Parser {
    enum State {First, Second, Invalid};
    State state = First;
    int left_factor{};
    int right_factor{};
    void parse(char const input) {
	switch (state) {
	    case First:
		if (input >= '0' && input <= '9') {
		    left_factor = 10 * left_factor + (input - '0');
		}
		else if (input == ',') {
		    state = Second;
		}
	    case Second:
	}
    }
};

int main() {
    auto input_stream = std::ifstream{"input"};
    auto line = std::string{};
    while (std::getline(input_stream, line)) {
	auto window = stdr::subrange{line};
	while (ssize(window) >= min_size) {
	    auto const head = stdr::search(window, head_marker);
	    window = stdr::subrange{cend(head), cend(window)};
	    auto valid = true;
	    auto left_value = int{};
	    auto digit_count = int{};
	    for (auto digit : window | stdv::take(max_digits)) {
		if (digit >= '0' && digit <= '9') {
		    left_value = 10 * left_value + digit;
		    ++digit_count;
		}
		else if (digit == ',') {
		    break;
		}
		else {
		    valid = false;
		    break;
		}
	    }
	    if (digit_count < 1) {
		valid = false;
	    }

	}

    }
    std::cout << std::transform_reduce(list_a.cbegin(), list_a.cend(),
				       0,
				       std::plus<>{},
				       [&list_b](int a) {
				           return list_b.count(a) * a;
				       }) << '\n';


    return 0;
}
