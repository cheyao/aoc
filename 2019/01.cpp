#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../algorithms.hpp"

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

void part1() {
	std::cout << ranges::fold_left(
		get() | ranges::views::transform([](const auto& b) { return std::atoi(b.data()); }), 0,
		[](const auto& a, const auto& c) { return a + (c / 3 - 2); }) << std::endl;
}

int calc(int i) {
	auto out = i / 3 - 2;
	if (i < 0) {
		return 0;
	}
	return calc(out) + i;
}

void part2() {
	std::cout << ranges::fold_left(
		get() | ranges::views::transform([](const auto& b) { return std::atoi(b.data()); }), 0,
		[](const auto& a, const auto& c) { return calc(c / 3 - 2) + a; }) << std::endl;
}

int main() {
	part1();
	part2();
}
