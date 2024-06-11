#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <utility>
#include <vector>
#include <ranges>
#include <iostream>

using namespace std;
using std::operator""sv;

bool check(string s, vector<uint64_t> dst) {
	vector<uint64_t> src;
	bool checked;

	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '#') {
			if (!checked) {
				src.emplace_back(1);
			} else {
				src[src.size() - 1]++;
			}

			checked = true;
		} else {
			checked = false;
		}
	}

	return src == dst;
}

uint64_t posibility(pair<string, vector<uint64_t>> row) {
	uint64_t count = 0;

	string m = row.first;
	vector<uint64_t>& spring = row.second;
	vector<char*> rand;

	for (int i = 0; i < m.size(); i++) {
		if (m[i] == '?') {
			rand.emplace_back(&(m[i]));
		}
	}

	uint64_t pos = std::count(m.begin(), m.end(), '?');
	uint64_t pos2 = std::pow(2, pos);
	uint64_t mask = 0;
	for (uint64_t i = 0; i < pos2; i++) {
		uint64_t maskCopy = mask;
		// Reset m
		for (auto c : rand) {
			*c = '?';
		}

		for (uint64_t j = 0; j < pos; j++) {
			if ((mask >> j) & 1) {
				*rand[j] = '#';
			} else {
				*rand[j] = '.';
			}
		}

		if (check(m, spring)) {
			count++;
			// cout << "Check " << i << " with " << m << endl;
		}

		mask++;
	}

	return count;
}

void part1() {
	ifstream file("12.input");
	vector<pair<string, vector<uint64_t>>> springMap;

	while(1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		string a;
		vector<uint64_t> b;
		a = line.substr(0, line.find(' '));
		line.erase(0, line.find(' ') + 1);
		b = line | views::split(","sv) | views::transform([](auto s)->uint64_t{ return atol(s.data()); }) | ranges::to<vector<uint64_t>>();
		springMap.emplace_back((pair<string, vector<uint64_t>>) {a, b});
	}

	// Brute force our way through
	uint64_t sum = 0;
	for (auto springRow : springMap) {
		sum += posibility(springRow);
	}

	cout << "Day 12 part 1: " << sum << endl;
}

int main() { part1(); }
