#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <unordered_set>
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

template <> struct std::hash<pair<uint32_t, uint32_t>> {
	std::size_t operator()(pair<uint32_t, uint32_t> const& p) const noexcept {
		return static_cast<uint64_t>(p.first) | (static_cast<uint64_t>(p.second) << 32);
	}
};

void part1() {
	uint64_t sum = 0;

	auto map = get();
	auto inbounds = [&map](pair<uint32_t, uint32_t> p) {
		if (p.first >= map[0].size() || p.second >= map.size()) {
			return false;
		}

		return true;
	};
	auto getCell = [&map, &inbounds](const pair<uint32_t, uint32_t> p) {
		if (!inbounds(p)) {
			return static_cast<char>(0);
		}
		return map[p.second][p.first];
	};
	auto add = [](pair<uint32_t, uint32_t> a, const uint32_t x, const uint32_t y) {
		a.first += x;
		a.second += y;
		return a;
	};

	vector<pair<uint32_t, uint32_t>> heads;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == '0') {
				heads.emplace_back(make_pair(x, y));
			}
		}
	}

	deque<pair<uint32_t, uint32_t>> grids;
	unordered_set<pair<uint32_t, uint32_t>> top;
	for (const auto head : heads) {
		assert(grids.empty());
		top.clear();

		grids.emplace_back(head);

		while (!grids.empty()) {
			const auto cell = grids.front();
			const auto dst = getCell(cell) + 1;
			grids.pop_front();

			// cout << cell.first << ' ' << cell.second << endl;

			if (dst == ('9' + 1)) {
				top.insert(cell);

				continue;
			}

			if (getCell(add(cell, 0, 1)) == dst) {
				grids.emplace_back(add(cell, 0, 1));
			}

			if (getCell(add(cell, 0, -1)) == dst) {
				grids.emplace_back(add(cell, 0, -1));
			}

			if (getCell(add(cell, 1, 0)) == dst) {
				grids.emplace_back(add(cell, 1, 0));
			}

			if (getCell(add(cell, -1, 0)) == dst) {
				grids.emplace_back(add(cell, -1, 0));
			}
		}

		sum += top.size();
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	auto map = get();
	auto inbounds = [&map](pair<uint32_t, uint32_t> p) {
		if (p.first >= map[0].size() || p.second >= map.size()) {
			return false;
		}

		return true;
	};
	auto getCell = [&map, &inbounds](const pair<uint32_t, uint32_t> p) {
		if (!inbounds(p)) {
			return static_cast<char>(0);
		}
		return map[p.second][p.first];
	};
	auto add = [](pair<uint32_t, uint32_t> a, const uint32_t x, const uint32_t y) {
		a.first += x;
		a.second += y;
		return a;
	};

	vector<pair<uint32_t, uint32_t>> heads;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == '0') {
				heads.emplace_back(make_pair(x, y));
			}
		}
	}

	deque<pair<uint32_t, uint32_t>> grids;
	for (const auto head : heads) {
		assert(grids.empty());

		grids.emplace_back(head);

		while (!grids.empty()) {
			const auto cell = grids.front();
			const auto dst = getCell(cell) + 1;
			grids.pop_front();

			// cout << cell.first << ' ' << cell.second << endl;

			if (dst == ('9' + 1)) {
				++sum;

				continue;
			}

			if (getCell(add(cell, 0, 1)) == dst) {
				grids.emplace_back(add(cell, 0, 1));
			}

			if (getCell(add(cell, 0, -1)) == dst) {
				grids.emplace_back(add(cell, 0, -1));
			}

			if (getCell(add(cell, 1, 0)) == dst) {
				grids.emplace_back(add(cell, 1, 0));
			}

			if (getCell(add(cell, -1, 0)) == dst) {
				grids.emplace_back(add(cell, -1, 0));
			}
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
