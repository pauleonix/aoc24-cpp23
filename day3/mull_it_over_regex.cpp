#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <string_view>
#include <regex>

namespace stdr = std::ranges;

static constexpr auto do_instr = std::string_view{"do()"};
static constexpr auto dont_instr = std::string_view{"don't()"};

int main() {
    auto input_stream = std::ifstream{"input"};
    auto const mul_instr_regex = std::regex{"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"};
    auto const mul_and_sum = [](int sum, auto match){
				 return sum + stoi(match[1].str()) *
					      stoi(match[2].str());
			     };
    auto input = std::string{std::istreambuf_iterator<char>{input_stream},
			     {}};
    {
	auto matches = stdr::subrange{std::sregex_iterator{cbegin(input), cend(input),
							   mul_instr_regex},
				      std::sregex_iterator{}};
	std::cout << stdr::fold_left(matches, 0, mul_and_sum) << '\n';
    }

    auto window = stdr::subrange{input};
    auto result = int{};
    while (!empty(window)) {
	auto const found_dont = stdr::search(window, dont_instr);
	using RegexIter = std::regex_iterator<stdr::const_iterator_t<decltype(window)>>;
	auto matches = stdr::subrange{RegexIter{cbegin(window), cbegin(found_dont),
						mul_instr_regex},
				      RegexIter{}};
	result += stdr::fold_left(matches, 0, mul_and_sum);
	auto const found_do = stdr::search(stdr::subrange{cend(found_dont), cend(window)},
					   do_instr);
	window = stdr::subrange{cend(found_do), cend(window)};
    }
    std::cout << result << '\n';

    return 0;
}
