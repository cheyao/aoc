#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <span>
#include <string>
#include <vector>

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

vector<uint64_t> solve1(uint64_t a, span<uint64_t> s) {
	if (s.empty()) {
		return {a};
	}

	auto prev = solve1(s[0], span(&s[1], s.size() - 1));

	vector<uint64_t> t;

	for (const auto v : prev) {
		t.emplace_back(v * a);
		t.emplace_back(v + a);
	}

	return t;
}

void part1() {
	uint64_t sum = 0;

	for (const auto& line : get()) {
		auto parts = line | views::split(": "sv) | ranges::to<vector<string>>();
		uint64_t dst = stoul(parts[0]);

		auto numbs = parts[1] | views::split(' ') |
					 views::transform([](const auto a) { return atoll(a.data()); }) |
					 ranges::to<vector<uint64_t>>();
		ranges::reverse(numbs);

		auto solutions = solve1(numbs[0], span(numbs.begin() + 1, numbs.end()));

		if (ranges::find(solutions, dst) != solutions.end()) {
			sum += dst;
		}
	}

	cout << "Part 1:" << sum << endl;
}

uint64_t l(int64_t n) {
	int length = 1;
	while (n /= 10) {
		length++;
	}
	return length;
}

vector<uint64_t> solve2(uint64_t a, span<uint64_t> s) {
	if (s.empty()) {
		return {a};
	}

	auto prev = solve2(s[0], span(&s[1], s.size() - 1));

	vector<uint64_t> t;

	for (const auto v : prev) {
		t.emplace_back(v * a);
		t.emplace_back(v + a);
		t.emplace_back((v * (powl(10, l(a)))) + a);
	}

	return t;
}

void part2() {
	uint64_t sum = 0;

	for (const auto& line : get()) {
		auto parts = line | views::split(": "sv) | ranges::to<vector<string>>();
		uint64_t dst = stoul(parts[0]);

		auto numbs = parts[1] | views::split(' ') |
					 views::transform([](const auto a) { return atoll(a.data()); }) |
					 ranges::to<vector<uint64_t>>();
		ranges::reverse(numbs);

		auto solutions = solve2(numbs[0], span(numbs.begin() + 1, numbs.end()));

		if (ranges::find(solutions, dst) != solutions.end()) {
			sum += dst;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
