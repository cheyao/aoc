#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <queue>
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

void part2() {
	ifstream file("15.input");

	string line;
	getline(file, line);
	vector<string> strings = line | views::split(","sv) | ranges::to<vector<string>>();

	uint64_t sum = 0;

	vector<pair<string, uint64_t>> boxes[256];
	for (auto s : strings) {
		string label;
		bool remove;
		int number;
		int i = s.size() - 1;
		for (; isdigit(s[i]); i--) {
		};

		remove = (s[i] == '-');
		if (!remove)
			number = stoi(s.substr(i + 1, s.size() - i - 1));
		label = s.substr(0, i);

		// Now process the boxes
		int lHash = HASH(label);

		int boxEntry = -1;
		for (int i = 0; i < boxes[lHash].size(); ++i) {
			if (boxes[lHash][i].first == label) {
				boxEntry = i;
				break;
			}
		}

		if (remove) {
			if (boxEntry != -1) {
				boxes[lHash].erase(boxes[lHash].begin() + boxEntry);
			}
		} else {
			// Add
			if (boxEntry != -1) {
				boxes[lHash][boxEntry].second = number;
			} else {
				boxes[lHash].emplace_back((pair<string, uint64_t>) {label, number});
			}
		}
	}

	for (int i = 0; i < 256; i++) {
		for (int l = 0; l < boxes[i].size(); ++l) {
			sum += (i + 1) * (l + 1) * (boxes[i][l].second);
		}
	}

	cout << "Part 2: " << sum << endl;
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

	cout << "Part 1: " << sum << endl;
}

int main() {
	part1();
	part2();
}

