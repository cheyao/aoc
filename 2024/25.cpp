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

#include "../algorithms.hpp"

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

	for (const auto& line : get()) {
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	cout << "Part 2: Click the button!\n";
}

int main() {
	part1();
	part2();
}
