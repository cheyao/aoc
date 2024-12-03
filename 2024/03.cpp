#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
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

	const regex r("mul\\((\\d*),(\\d*)\\)");
	for (const auto& line : get()) {
		std::smatch base_match;

		auto words_begin = std::sregex_iterator(line.begin(), line.end(), r);
		auto words_end = std::sregex_iterator();

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			if (match.size() == 3) {
				sum += stoull(match[1]) * stoull(match[2]);
			}
		}
	}

	cout << "Part 1: " << sum << endl;
}

void part2() {
	uint64_t sum = 0;
	bool active = true;

	const regex r("mul\\((\\d*),(\\d*)\\)|do\\(\\)|don\'t\\(\\)");
	for (const auto& line : get()) {
		std::smatch base_match;

		auto words_begin = std::sregex_iterator(line.begin(), line.end(), r);
		auto words_end = std::sregex_iterator();

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;

			if (match.str()[0] == 'm') {
				if (active) {
					sum += stoull(match[1]) * stoull(match[2]);
				}
			} else if (match[0].str()[2] == 'n') {
				active = false;
			} else {
				active = true;
			}
		}
	}

	cout << "Part 2: " << sum << endl;
}

int main() {
	part1();
	part2();
}
