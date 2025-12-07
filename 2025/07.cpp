#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ratio>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

using namespace std;

static vector<string> get() {
	static vector<string> out;

	if (out.empty()) {
		ifstream file(__FILE__ ".txt");
		string line;

		while (getline(file, line)) {
			if (!line.empty() && ranges::count(line, '.') != line.length()) {
				out.emplace_back(std::move(line));
			}
		}
	}

	return out;
}

auto part1() {
	uint64_t sum = 0;

	vector<bool> rays;
	for (const auto& l : get()) {
		[[unlikely]] if (rays.empty()) {
			rays = vector<bool>(l.size(), false);
			rays[distance(l.begin(), ranges::find(l, 'S'))] = true;
            continue;
		}

		for (uint64_t i = 0; i < l.size(); ++i) {
			if (l[i] != '^' || !rays[i]) {
				continue;
			}

			++sum;
			rays[i] = false;
			rays[i + 1] = true;
			rays[i - 1] = true;
		}
	}

	return sum;
}

auto part2() {
    uint64_t sum = 0;
	vector<uint64_t> rays;

	for (const auto& l : get()) {
		[[unlikely]] if (rays.empty()) {
			rays = vector<uint64_t>(l.size(), 0);
			rays[distance(l.begin(), ranges::find(l, 'S'))] = 1;

            continue;
		}

		for (uint64_t i = 0; i < l.size(); ++i) {
			if (l[i] != '^' || rays[i] == 0) {
				continue;
			}

			rays[i + 1] += rays[i];
			rays[i - 1] += rays[i];
            rays[i] = 0;
		}
	}

	return std::accumulate(rays.begin(), rays.end(), 0ull); // this 0 fucked me over...
}

int main() {
	auto tries = 300;

	double time = 0;
	uint64_t result = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part1();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 1: " << result << endl;
	std::cout << time / (tries - 3) << "us\n";

	time = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part2();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - 3) << "us\n";
}
