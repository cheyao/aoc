#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

constexpr const char* FILE_NAME = "02.input";

vector<string> getFile() {
	vector<string> lines;

	ifstream file(FILE_NAME);
	string str;
	while (getline(file, str)) {
		lines.emplace_back(str);
	}
	file.close();

	return lines;
}

uint64_t part1();
uint64_t part2();

int main(int argc, char** argv) {
	cout << "Day " << FILE_NAME << " part 1 : " << part1() << endl;
	cout << "Day " << FILE_NAME << " part 2 : " << part2() << endl;
}

uint64_t part1() {
	const unordered_map<char, uint64_t> MM = {
		{'X', 1},
		{'Y', 2},
		{'Z', 3}
	};

	auto lines = getFile();

	uint64_t score = 0;
	for (const std::string& line : lines) {
		const char O = line[0] - 'A';
		const char Y = line[2] - 'X';

		score += MM.at(line[2]);

		if (O == Y) {
			score += 3;
			continue;
		}

		if ((Y - O + 3) % 3 == 1) {
			score += 6;
		}
	}

	return score;
}

uint64_t part2() {
	auto lines = getFile();

	uint64_t score = 0;
	for (const std::string& line : lines) {
		const char O = line[0] - 'A';
		const char Y = line[2] - 'X';

		score += Y * 3;

		if (Y == 1) {
			score += O + 1;
			continue;
		}

		if (Y == 2) {
			score += (O + 1) % 3 + 1;
			continue;
		}

		if (Y == 0) {
			score += (O - 1 + 3) % 3 + 1;
			continue;
		}

		unreachable();
	}

	return score;
}
