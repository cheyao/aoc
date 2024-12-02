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

	for (const auto& line : get()) {
		const auto v = line | views::split(" "sv) |
					   views::transform([](const auto s) { return atoll(s.data()); }) |
					   ranges::to<vector<uint64_t>>();

		if (ranges::all_of(
				v | views::slide(2),
				[](const auto v) { return v[0] < v[1] && v[1] - v[0] >= 1 && v[1] - v[0] <= 3; }) ||
			ranges::all_of(v | views::slide(2), [](const auto v) {
				return v[0] > v[1] && v[0] - v[1] >= 1 && v[0] - v[1] <= 3;
			})) {
			++sum;
		}
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	for (const auto& line : get()) {
		const auto v = line | views::split(" "sv) |
					   views::transform([](const auto s) { return atoll(s.data()); }) |
					   ranges::to<vector<uint64_t>>();
		auto s = v | views::slide(2);
		cout << v[0];

		if (ranges::all_of(
				v | views::slide(2),
				[](const auto v) { return v[0] < v[1] && v[1] - v[0] >= 1 && v[1] - v[0] <= 3; }) ||
			ranges::all_of(v | views::slide(2), [](const auto v) {
				return v[0] > v[1] && v[0] - v[1] >= 1 && v[0] - v[1] <= 3;
			})) {
			++sum;
			continue;
		}

		auto a = ranges::find_if(
			s, [](const auto v) { return v[0] < v[1] && v[1] - v[0] >= 1 && v[1] - v[0] <= 3; });

		if (a != s.end()) {
			auto n = v;
			n.erase(ranges::find(n, (*a)[1]));

			if (ranges::all_of(n | views::slide(2), [](const auto v) {
					return v[0] < v[1] && v[1] - v[0] >= 1 && v[1] - v[0] <= 3;
				})) {
				++sum;
				continue;
			}
		}

		auto b = ranges::find_if(
			s, [](const auto v) { return (v[0] > v[1] && v[0] - v[1] >= 1 && v[0] - v[1] <= 3); });

		if (b != s.end()) {
			auto n = v;
			n.erase(ranges::find(n, (*b)[1]));
			cout << ' ' <<(*b)[1] << endl;

			if (ranges::all_of(n | views::slide(2), [](const auto v) {
					return (v[0] > v[1] && v[0] - v[1] >= 1 && v[0] - v[1] <= 3);
				})) {
				++sum;

				continue;
			}
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
