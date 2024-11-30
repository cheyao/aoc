#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>

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
	uint64_t sum;

	for (const auto& line : get()) {
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum;

	for (const auto& line : get()) {
	}

	cout << "Part 1:" << sum << endl;
}

int main() {
	part1();

	part2();
}
