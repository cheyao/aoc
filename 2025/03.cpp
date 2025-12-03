#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <ratio>
#include <string>
#include <utility>
#include <vector>

using namespace std;

static vector<string> get() {
	static vector<string> out;

	if (out.empty()) {
		ifstream file(__FILE__ ".txt");
		string line;

		while (getline(file, line)) {
			if (!line.empty()) {
				out.emplace_back(std::move(line));
			}
		}
	}

	return out;
}

// Check part 2
auto part1() {
	uint64_t sum = 0;
	uint64_t n = 0;

	for (const auto& bank : get()) {
		const auto numbers = bank | views::transform([](const auto& c) { return c - '0'; });
		const auto view = numbers | views::take(numbers.size() - 1);

		const auto max1 = ranges::max_element(view);
		const auto max2 =
			ranges::max(numbers | views::drop(ranges::distance(view.begin(), max1) + 1));

		sum += *max1 * 10 + max2;
	}

	return sum;
}

auto part2() {
	constexpr const uint64_t size = 12;

	uint64_t sum = 0;
	uint64_t n = 0;

	for (const auto& bank : get()) {
		const auto& numbers = bank | views::transform([](const auto& c) { return c - '0'; });

		const auto& view = numbers | views::take(numbers.size() - (size - 1));
		auto last_element = ranges::max_element(view);
		n = *last_element;

		for (int64_t i = size - 2 /* 2 cuz we processed one ^ */; i >= 0; --i) {
			const auto& view = numbers | views::take(numbers.size() - i) |
							   views::drop(distance(numbers.begin(), last_element) + 1);

			last_element = ranges::max_element(view);

			n = n * 10 + *last_element;
		}

		sum += n;
		n = 0;
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
		if (i >= 3) {
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
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - 3) << "ms\n";
}
