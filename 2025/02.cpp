#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cmath>
#include <concepts>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <sys/types.h>
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

auto exp(std::integral auto base, std::unsigned_integral auto pow) {
	using result_type = std::common_type_t<decltype(base), decltype(pow)>;
	auto result = result_type(1);
	while (pow > 0) {
		if (pow & 1)
			result *= base;
		base *= base;
		pow >>= 1;
	}

	return result;
}

uint64_t old_part1() {
	uint64_t sum = 0;

	for (const auto& line : views::split(get()[0], ',')) {
		const auto range = views::split(line, '-') |
						   views::transform([](const auto& s) { return atoll(s.data()); }) |
						   ranges::to<vector<uint64_t>>();

		for (auto i = range[0]; i <= range[1];) {
			const auto s = to_string(i);

			if (s.length() % 2) {
				i = exp(10, static_cast<uint64_t>(floor(log10(i))) + 1);

				continue;
			}

			if (ranges::equal(s | views::take(s.length() / 2), s | views::drop(s.length() / 2))) {
				sum += i;

				i += pow(10, s.length() / 2);
			} else {
				++i;
			}
		}
	}

	return sum;
}

// Check part 2
auto part1() {
	uint64_t sum = 0;

	for (const auto& line : views::split(get()[0], ',')) {
		const auto range = views::split(line, '-') |
						   views::transform([](const auto& s) { return atoll(s.data()); }) |
						   ranges::to<vector<uint64_t>>();
		const auto lowers = to_string(range[0]);
		const auto uppers = to_string(range[1]);

		std::vector<uint64_t> ids;

		for (uint64_t d = 1; d <= uppers.length() / 2; ++d) {
			uint64_t r = 2;

			const uint64_t exp10m1 = exp(10, d) - 1;
			const uint64_t f = (exp(10, d * r) - 1) / (exp10m1);

			if (f > range[1]) {
				continue;
			}

			const uint64_t k_min = exp(10, d - 1);
			const uint64_t k_max = exp10m1;

			const uint64_t k_l = max((range[0] + f - 1) / f, k_min);
			const uint64_t k_h = min(range[1] / f, k_max);

			if (k_h < k_l) {
				continue;
			}

			for (uint64_t k = k_l; k <= k_h; ++k) {
				const uint64_t n = k * f;
				assert(range[0] <= n && n <= range[1]);
				ids.push_back(n);
			}
		}

		ranges::sort(ids);
		const auto [first, last] = ranges::unique(ids);
		ids.erase(first, last);
		sum += ranges::fold_left(ids, 0, plus<uint64_t>());
	}

	return sum;
}

auto part2() {
	uint64_t sum = 0;

	for (const auto& line : views::split(get()[0], ',')) {
		const auto range = views::split(line, '-') |
						   views::transform([](const auto& s) { return atoll(s.data()); }) |
						   ranges::to<vector<uint64_t>>();
		const auto lowers = to_string(range[0]);
		const auto uppers = to_string(range[1]);

		std::vector<uint64_t> ids;

		// k: the number repeted
		// d: size of block
		// r: repetitions

		// 10^(d-1) <= k <= 10^d-1
		// lower/f <= k <= upper/f

		// length of repeted block, max length is half total length of str for 2 repetetions
		for (uint64_t d = 1; d <= uppers.length() / 2; ++d) {
			// repetitions of the block
			for (uint64_t r = 2; r <= uppers.length() / d; ++r) {
				// generates a sequence like 001001001 or 01010101
				const uint64_t exp10m1 = exp(10, d) - 1;
				const uint64_t f = (exp(10, d * r) - 1) / (exp10m1);

				// f too big
				if (f > range[1]) {
					continue;
				}

				const uint64_t k_min = exp(10, d - 1);
				const uint64_t k_max = exp10m1;

				// ciel()
				const uint64_t k_l = max((range[0] + f - 1) / f, k_min);
				// floor()
				const uint64_t k_h = min(range[1] / f, k_max);

				if (k_h < k_l) {
					continue; // No possible solutions
				}

				for (uint64_t k = k_l; k <= k_h; ++k) {
					const uint64_t n = k * f; // This is out candidate number
					assert(range[0] <= n && n <= range[1]);
					ids.push_back(n);
				}
			}
		}

		ranges::sort(ids);
		const auto [first, last] = ranges::unique(ids);
		ids.erase(first, last);
		sum += ranges::fold_left(ids, 0, plus<uint64_t>());
	}

	return sum;
}

int main() {
	auto tries = 250;

	double time = 0;
	uint64_t result = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part1();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::milli> ms_double = t2 - t1;
		if (i > 2) {
			time += ms_double.count();
		}
	}
	cout << "Part 1: " << result << endl;
	std::cout << time / (tries - 3) << "ms\n";

	time = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part2();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::milli> ms_double = t2 - t1;
		if (i > 2) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - 3) << "ms\n";
}
