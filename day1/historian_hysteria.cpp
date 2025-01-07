#include <set>
#include <iostream>
#include <fstream>
#include <functional>
#include <numeric>

int main() {
    auto input_stream = std::ifstream{"input"};
    auto list_a = std::multiset<int>{};
    auto list_b = std::multiset<int>{};
    {
	int a{};
	int b{};
	while (input_stream >> a >> b) {
	    list_a.insert(a);
	    list_b.insert(b);
	}
    }
    std::cout << std::transform_reduce(list_a.cbegin(), list_a.cend(),
				       list_b.cbegin(),
				       0,
				       std::plus<>{},
				       [](int a, int b) {
				           return std::abs(a - b);
				       }) << '\n';

    std::cout << std::transform_reduce(list_a.cbegin(), list_a.cend(),
				       0,
				       std::plus<>{},
				       [&list_b](int a) {
				           return list_b.count(a) * a;
				       }) << '\n';


    return 0;
}
