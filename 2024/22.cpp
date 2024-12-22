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

uint64_t genNumber(uint64_t init, uint64_t r);

uint64_t genNumberI(uint64_t init, uint64_t r) {
	[[unlikely]] if (r == 0) { return init; }

	if (r != 1) {
		init = genNumber(init, r - 1);
	}

	init ^= init * 64;
	init %= 16777216;
	init ^= init / 32;
	init %= 16777216;
	init ^= init * 2048;
	init %= 16777216;

	return init;
}

uint64_t genNumber(const uint64_t init, uint64_t r) {
	static array<unordered_map<uint64_t, uint64_t>, 2001> cache;

	if (!cache[r].contains(init)) {
		cache[r][init] = genNumberI(init, r);
	}

	return cache[r][init];
}

void part1() {
	uint64_t sum = 0;

	for (const auto code : get()) {
		sum += genNumber(stoull(code), 2000);
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	unordered_map<string, uint64_t> bananas;
	for (const auto code : get()) {
		const uint64_t seed = stoull(code);

		unordered_map<string, bool> sold;
		for (uint64_t i = 0; i <= 2000 - 4; ++i) {
			const int64_t a = genNumber(seed, i) % 10;
			const int64_t b = genNumber(seed, i + 1) % 10;
			const int64_t c = genNumber(seed, i + 2) % 10;
			const int64_t d = genNumber(seed, i + 3) % 10;
			const int64_t e = genNumber(seed, i + 4) % 10;

			const string s = to_string(b - a) + ',' + to_string(c - b) + ',' + to_string(d - c) +
							 ',' + to_string(e - d);
			if (!sold[s]) {
				bananas[s] += e;
				sold[s] = 1;
			}
		}
	}

	for (const auto& [s, i] : bananas) {
		sum = max(sum, i);
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
