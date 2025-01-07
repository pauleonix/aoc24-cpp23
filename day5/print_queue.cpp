#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <tuple>

namespace stdr = std::ranges;

using Rule = std::tuple<int, int>;

int main() {
    auto input_stream = std::ifstream{"input"};
    auto line = std::string{};
    auto rules = std::map<Rule, bool>{};
    auto comp = [&rules](int left, int right){
		    return rules[Rule{left, right}]; 
		};
    auto rules_done = false;
    auto result = int{};
    auto result_corr = int{};
    while (getline(input_stream, line)) {
	if (empty(line)) {
	    rules_done = true;
	    continue;
	}
	auto line_stream = std::istringstream{line};
	if (!rules_done) {
	    auto vbar = char{};
	    auto left = int{};
	    auto right = int{};
	    if (!(line_stream >> left >> vbar >> right) || vbar != '|') {
		throw std::runtime_error{"Parse error\n"};
	    }
	    rules.emplace(Rule{left, right}, true);
	    rules.emplace(Rule{right, left}, false);
	}
	else {
	    auto page_str = std::string{};
	    auto update = std::vector<int>{};
	    while (getline(line_stream, page_str, ',')) {
		auto page_number = std::stoi(page_str);
		update.push_back(page_number);
	    }
	    if (stdr::is_sorted(update, comp)) {
		auto middle_page = update[ssize(update) / 2];
		result += middle_page;
	    } else {
		stdr::sort(update, comp);
		auto middle_page = update[ssize(update) / 2];
		result_corr += middle_page;
	    }
	}
    }
    std::cout << result << '\n' << result_corr << '\n';
    return 0;
}

