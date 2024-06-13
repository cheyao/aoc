#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>

using namespace std;
using std::operator""sv;

int HASH(string s) {
	uint64_t cur = 0;

	for (char c : s) {
		cur += c;
		cur *= 17;
		cur %= 256;
	}

	return cur;
}

void part1() {
	ifstream file("15.input");

	string line;
	getline(file, line);
	vector<string> strings = line | views::split(","sv) | ranges::to<vector<string>>();

	uint64_t sum = 0;
	for (auto s : strings) {
		sum += HASH(s);
	}

	cout << sum << endl;
}

int main() {
	part1();
}

