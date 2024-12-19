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
#include <string>
#include <string_view>
#include <sys/types.h>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;
	string buf;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

bool make_pattern(const string_view pattern, const span<string> towels) {
	static unordered_map<string, bool> cache;

	string s(pattern);
	if (cache.contains(s)) {
		return cache[s];
	}

	for (const auto& towel : towels) {
		if (towel.size() > pattern.size()) {
			continue;
		}

		bool full = true;
		for (size_t i = 0; i < towel.size(); ++i) {
			if (pattern[i] != towel[i]) {
				full = false;
				break;
			}
		}

		if (!full) {
			continue;
		}

		if (towel.size() == pattern.size()) {
			return true;
		}

		if (make_pattern(string_view(pattern.data() + towel.size(), pattern.size() - towel.size()),
						 towels)) {
			cache[s] = true;
			return cache[s];
		};
	}

	cache[s] = false;
	return cache[s];
}

void part1() {
	uint64_t sum = 0;

	const auto input = get();
	vector<string> towels = input[0] | views::split(", "sv) | ranges::to<vector<string>>();

	for (const auto& pattern : span(input.begin() + 2, input.end())) {
		sum += make_pattern(pattern, towels);
	}

	cout << "Part 1: " << sum << '\n';
}

uint64_t count_patterns(const string_view pattern, const span<string> towels) {
	static unordered_map<string, uint64_t> cache;

	string s(pattern);
	if (cache.contains(s)) {
		return cache[s];
	}

	uint64_t times = 0;
	for (const auto& towel : towels) {
		if (towel.size() > pattern.size()) {
			continue;
		}

		bool full = true;
		for (size_t i = 0; i < towel.size(); ++i) {
			if (pattern[i] != towel[i]) {
				full = false;
				break;
			}
		}

		if (!full) {
			continue;
		}

		if (towel.size() == pattern.size()) {
			++times;
			continue;
		}

		times += count_patterns(
			string_view(pattern.data() + towel.size(), pattern.size() - towel.size()), towels);
	}

	cache[s] = times;

	return times;
}

void part2() {
	uint64_t sum = 0;

	const auto input = get();
	vector<string> towels = input[0] | views::split(", "sv) | ranges::to<vector<string>>();

	for (const auto& pattern : span(input.begin() + 2, input.end())) {
		sum += count_patterns(pattern, towels);
	}

	cout << "Part 2: " << sum << endl;
}

int main() {
	part1();
	part2();
}
