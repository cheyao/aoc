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
#include <unordered_set>
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

int prog(vector<int> program) {
	int ic = 0;
	while (true) {
		switch (program[ic]) {
			case 1:
				program[program[ic + 3]] = program[program[ic + 1]] + program[program[ic + 2]];
				break;
			case 2:
				program[program[ic + 3]] = program[program[ic + 1]] * program[program[ic + 2]];
				break;
			case 99:
				goto end;
		}
		ic += 4;
	}
end:

	return program[0];
}

void part1() {
	auto program = get()[0] | ranges::views::split(',') |
				   ranges::views::transform([](const auto& b) { return std::atoi(b.data()); }) |
				   ranges::to<vector<int>>();

	program[1] = 12;
	program[2] = 2;

	std::cout << prog(program) << std::endl;
}

void part2() {
	auto program = get()[0] | ranges::views::split(',') |
				   ranges::views::transform([](const auto& b) { return std::atoi(b.data()); }) |
				   ranges::to<vector<int>>();

	program[1] = 84;
	program[2] = 78;

	std::cout << prog(program) << std::endl;
}

int main() {
	part1();
	part2();
}
