#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

	ranges::sort(a);
	ranges::sort(b);

	for (const auto& [ia, ib] : views::zip(a, b)) {
		sum += abs(static_cast<int64_t>(ia - ib));
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
