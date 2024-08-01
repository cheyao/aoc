#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

using namespace std;
using std::operator""sv;

constexpr const char* FILE_NAME = "04.input";

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
	auto lines = getFile();

	uint64_t cnt = 0;
	for (const auto& line : lines) {
		vector<vector<uint64_t>> pairs =
			line | views::split(","sv) | views::transform([](const auto& p) {
				return p | views::split("-"sv) | ranges::to<vector<string>>() |
					   views::transform([](const string& s) { return stoull(s); });
			}) |
			ranges::to<vector<vector<uint64_t>>>();

		pair<uint64_t, uint64_t> a = {pairs[0][0], pairs[0][1]};
		pair<uint64_t, uint64_t> b = {pairs[1][0], pairs[1][1]};

		if (a.first <= b.first && a.second >= b.second) {
			cnt++;
			continue;
		}

		if (a.first >= b.first && a.second <= b.second) {
			cnt++;
			continue;
		}
	}

	return cnt;
}

uint64_t part2() {
	auto lines = getFile();

	uint64_t cnt = 0;
	for (const auto& line : lines) {
		vector<vector<uint64_t>> pairs =
			line | views::split(","sv) | views::transform([](const auto& p) {
				return p | views::split("-"sv) | ranges::to<vector<string>>() |
					   views::transform([](const string& s) { return stoull(s); });
			}) |
			ranges::to<vector<vector<uint64_t>>>();

		pair<uint64_t, uint64_t> a = {pairs[0][0], pairs[0][1]};
		pair<uint64_t, uint64_t> b = {pairs[1][0], pairs[1][1]};

		// A: |----|
		// B:  |--|
		//
		// A:  |--|
		// B: |----|

		if (b.first >= a.first && b.first <= a.second) {
			cnt++;
			continue;
		}

		if (b.second >= a.first && b.second <= a.second) {
			cnt++;
			continue;
		}

		if (a.first >= b.first && a.second <= b.second) {
			cnt++;
			continue;
		}
	}

	return cnt;
}
