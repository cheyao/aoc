#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

using namespace std;

constexpr const char* FILE_NAME = "00.input";

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
	return 0;
}

uint64_t part2() {
	auto lines = getFile();
	return 0;
}
