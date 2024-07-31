#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

using namespace std;

constexpr const char* FILE_NAME = "01.input";

queue<string> getFile() {
	queue<string> lines;

	ifstream file(FILE_NAME);
	string str;
	while (getline(file, str)) {
		lines.emplace(str);
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
	auto lines = getFile();

	uint64_t max = 0;
	uint64_t cur = 0;
	while (!lines.empty()) {
		auto line = lines.front();
		lines.pop();

		if (line.empty()) {
			max = std::max(max, cur);
			cur = 0;

			continue;
		}

		cur += stoull(line);
	}
	max = std::max(max, cur);

	return max;
}

uint64_t part2() {
	auto lines = getFile();

	std::vector<uint64_t> elfs;
	uint64_t cur = 0;
	while (!lines.empty()) {
		auto line = lines.front();
		lines.pop();

		if (line.empty()) {
			elfs.emplace_back(cur);
			cur = 0;

			continue;
		}

		cur += stoull(line);
	}
	elfs.emplace_back(cur);

	ranges::sort(elfs, std::greater<uint64_t>());

	return elfs[0] + elfs[1] + elfs[2];
}
