#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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
	uint64_t sum = 0;

	auto map = get();
	unordered_map<char, vector<pair<int64_t, int64_t>>> dict;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] != '.') {
				dict[map[y][x]].emplace_back(make_pair(x, y));
			}
		}
	}

	auto inbounds = [&map](int64_t x, int64_t y) {
		if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size()) {
			return false;
		}

		return true;
	};

	vector<pair<int64_t, int64_t>> antis;
	for (const auto freq : dict) {
		for (size_t c = 0; c < freq.second.size(); ++c) {
			for (size_t d = c + 1; d < freq.second.size(); ++d) {
				// a = 1 2
				// b = 2 5

				// a-b =a + -1 -3
				// b-a =b + 1 3

				// b = 1 2
				// a = 2 5

				// a-b = a + 1 3
				// b-a = b + 1 3

				int64_t x1 = freq.second[c].first + freq.second[c].first - freq.second[d].first;
				int64_t y1 = freq.second[c].second + freq.second[c].second - freq.second[d].second;

				int64_t x2 = freq.second[d].first + freq.second[d].first - freq.second[c].first;
				int64_t y2 = freq.second[d].second + freq.second[d].second - freq.second[c].second;

				if (inbounds(x1, y1)) {
					antis.emplace_back(make_pair(x1, y1));
				}

				if (inbounds(x2, y2)) {
					antis.emplace_back(make_pair(x2, y2));
				}
			}
		}
	}

	for (const auto [x, y] : antis) {
		map[y][x] = '#';
	}

	for (const auto& l : map) {
		for (const auto c : l) {
			if (c == '#') {
				++sum;
			}
		}
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	auto map = get();
	vector<string> outm = map;
	unordered_map<char, vector<pair<int64_t, int64_t>>> dict;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] != '.') {
				dict[map[y][x]].emplace_back(make_pair(x, y));
			}
		}
	}

	auto inbounds = [&map](int64_t x, int64_t y) {
		if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size()) {
			return false;
		}

		return true;
	};

	vector<pair<int64_t, int64_t>> antis;
	for (const auto freq : dict) {
		for (size_t c = 0; c < freq.second.size(); ++c) {
			for (size_t d = c + 1; d < freq.second.size(); ++d) {
				int64_t x1 = freq.second[c].first - freq.second[d].first;
				int64_t y1 = freq.second[c].second - freq.second[d].second;

				int64_t x2 = freq.second[d].first - freq.second[c].first;
				int64_t y2 = freq.second[d].second - freq.second[c].second;

				for (int64_t x = freq.second[c].first, y = freq.second[c].second; inbounds(x, y); x += x1, y += y1) {
					outm[y][x] = '#';
				}

				for (int64_t x = freq.second[c].first, y = freq.second[c].second; inbounds(x, y); x += x2, y += y2) {
					outm[y][x] = '#';
				}
			}
		}
	}

	for (const auto& l : outm) {
		for (const auto c : l) {
			if (c == '#') {
				++sum;
			}
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
