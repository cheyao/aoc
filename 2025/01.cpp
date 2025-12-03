#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
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

	int64_t pos = 50;
	for (const auto& line : get()) {
		int c = std::atoi(&line[1]);

		pos += ((line[0] == 'L') ? -1 : 1) * c;
		pos %= 100;
		if (pos < 0) {
			pos += 100;
		}

		sum += pos == 0;
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	int64_t pos = 50;
	int64_t posl = pos;
	for (const auto& line : get()) {
		int c = std::atoi(&line[1]);

		posl = pos;
		pos += ((line[0] == 'L') ? -1 : 1) * c;
		sum += abs(pos / 100) + ((pos < 0) && (posl != 0)) + (pos == 0);

		pos %= 100;
		if (pos < 0) {
			pos += 100;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
