#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
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

void part1() {
	uint64_t sum = 0;

	vector<vector<string>> locks;
	vector<vector<string>> keys;
	vector<string> cur(5);

	for (const auto& line : get()) {
		if (line.empty()) {
			if (cur[0][0] == '#') {
				locks.emplace_back(cur);
			} else {
				keys.emplace_back(cur);
			}

			for (size_t i = 0; i < 5; ++i) {
				cur[i] = "";
			}
		} else {
			for (size_t i = 0; i < 5; ++i) {
				cur[i] += line[i];
			}
		}
	}

	for (const auto& key : keys) {
		for (const auto& lock : locks) {
			bool full = true;

			for (size_t i = 0; i < 5; ++i) {
				if ((ranges::count(key[i], '#') + ranges::count(lock[i], '#')) > 7) {
					full = false;

					break;
				}
			}

			if (full) {
				++sum;
			}
		}
	}

	cout << "Part 1:" << sum << endl;
}

void part2() { cout << "Part 2: Click the button!\n"; }

int main() {
	part1();
	part2();
}
