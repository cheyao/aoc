#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <ranges>
#include <string>
#include <vector>

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;

	while (getline(file, line)) {
		if (!line.empty()) {
			out.emplace_back(std::move(line));
		}
	}

	return out;
}

void part1() {
	uint64_t sum = 0;

	vector<uint64_t> a;
	vector<uint64_t> b;
	for (const auto& line : get()) {
		auto parts = line | views::split("   "sv) | ranges::to<vector<string>>();

		a.emplace_back(stol(parts[0]));
		b.emplace_back(stol(parts[1]));
	}

	uint64_t i = a.size();
	while (i--) {
		auto amin = ranges::min_element(a);
		auto bmin = ranges::min_element(b);

		assert(*amin != std::numeric_limits<uint64_t>::max());
		assert(*bmin != std::numeric_limits<uint64_t>::max());

		auto adist = distance(a.begin(), amin);
		auto bdist = distance(b.begin(), bmin);

		sum += abs(static_cast<int64_t>(*bmin - *amin));

		*amin = std::numeric_limits<uint64_t>::max();
		*bmin = std::numeric_limits<uint64_t>::max();
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	vector<uint64_t> a;
	vector<uint64_t> b;
	for (const auto& line : get()) {
		auto parts = line | views::split("   "sv) | ranges::to<vector<string>>();

		a.emplace_back(stol(parts[0]));
		b.emplace_back(stol(parts[1]));
	}

	uint64_t i = a.size();
	while (i--) {
		sum += a[i] * ranges::count(b, a[i]);
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
