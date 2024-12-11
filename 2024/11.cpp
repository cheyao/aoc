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
#include <sys/types.h>
#include <unordered_map>
#include <utility>
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

constexpr int64_t ipow(int64_t base, uint8_t exp) {
	static const uint8_t highest_bit_set[] = {
		0,
		1,
		2,
		2,
		3,
		3,
		3,
		3,
		4,
		4,
		4,
		4,
		4,
		4,
		4,
		4,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		5,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		6,
		255, // anything past 63 is a guaranteed
			 // overflow with base > 1
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
		255,
	};

	int64_t result = 1;

	switch (highest_bit_set[exp]) {
		case 255: // we use 255 as an overflow marker and return 0 on overflow/underflow
			if (base == 1) {
				return 1;
			}

			if (base == -1) {
				return 1 - 2 * (exp & 1);
			}

			return 0;
		case 6:
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		case 5:
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		case 4:
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		case 3:
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		case 2:
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		case 1:
			if (exp & 1)
				result *= base;
		default:
			return result;
	}
}

constexpr uint64_t len(uint64_t n) noexcept {
	uint64_t length = 1;

	while (n /= 10) {
		length++;
	}

	return length;
}

array<unordered_map<uint64_t, uint64_t>, 76> cache;
uint64_t getSize(uint64_t stone, uint64_t times) noexcept {
	if (cache[times].contains(stone)) {
		return cache[times][stone];
	}

	if (times == 0) {
		return 1;
	}

	if (stone == 0) {
		cache[times][0] = getSize(1, times - 1);
		return cache[times][stone];
	}

	uint64_t l = len(stone);
	if (l % 2 == 0) {
		int64_t base = ipow(10, l / 2);
		cache[times][stone] = getSize(stone / base, times - 1) + getSize(stone % base, times - 1);
		return cache[times][stone];
	}

	cache[times][stone] = getSize(stone * 2024, times - 1);
	return cache[times][stone];
}

void part1() {
	uint64_t sum = 0;

	auto stones = get()[0] | views::split(' ') |
				  views::transform([](const auto& s) { return atoll(s.data()); }) |
				  ranges::to<vector<uint64_t>>();

	for (const auto& stone : stones) {
		sum += getSize(stone, 25);
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	auto stones = get()[0] | views::split(' ') |
				  views::transform([](const auto& s) { return atoll(s.data()); }) |
				  ranges::to<vector<uint64_t>>();

	for (const auto& stone : stones) {
		sum += getSize(stone, 75);
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
