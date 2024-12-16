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
#include <ranges>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

#include "../vec2.hpp"

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;
	string buf;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

vec2 inst2dir(const char inst) {
	switch (inst) {
		case '<':
			return make_pair(-1, 0);
		case '>':
			return make_pair(1, 0);
		case '^':
			return make_pair(0, -1);
		case 'v':
			return make_pair(0, 1);
		default:
			throw runtime_error("UH OH!");
	}
}

bool tryPropag(vector<string>& map, const vec2 pos, const vec2 dir) {
	if (at(map, pos) == '#') {
		return false;
	}

	if (at(map, pos) == 'O') {
		if (!tryPropag(map, pos + dir, dir)) {
			return false;
		}

		at(map, pos) = '.';
		at(map, pos + dir) = 'O';
	}

	return true;
}

void part1() {
	uint64_t sum = 0;

	bool part = true;
	vector<string> map;
	string cmd;
	vec2 player;

	for (const auto& line : get()) {
		if (part) {
			if (line.empty()) {
				part = false;

				continue;
			}

			for (const auto [i, c] : views::enumerate(line)) {
				if (c == '@') {
					player = make_pair(i, map.size());

					break;
				}
			}

			map.emplace_back(line);
		} else {
			cmd += line;
		}
	}

	at(map, player) = '.';

	for (const auto& inst : cmd) {
		const auto dir = inst2dir(inst);

		if (tryPropag(map, player + dir, dir)) {
			player += dir;
		}
	}

	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == 'O') {
				sum += y * 100 + x;
			}
		}
	}

	cout << "Part 1:" << sum << endl;
}

bool tryPropag2(vector<string>& map, const vec2 pos, const vec2 dir) {
	if (at(map, pos) == '#') {
		return false;
	}

	if (at(map, pos) == '[' && dir.second != 0) {
		if (!tryPropag2(map, pos + dir, dir) ||
			!tryPropag2(map, pos + dir + make_pair(1l, 0l), dir)) {
			return false;
		}
	}

	if (at(map, pos) == ']' && dir.second != 0) {
		if (!tryPropag2(map, pos + dir, dir) ||
			!tryPropag2(map, pos + dir + make_pair(-1l, 0l), dir)) {
			return false;
		}
	}

	if ((at(map, pos) == '[' || at(map, pos) == ']') && dir.second == 0) {
		if (!tryPropag2(map, pos + dir, dir)) {
			return false;
		}
	}

	return true;
}

bool propag2(vector<string>& map, const vec2 pos, const vec2 dir) {
	if (at(map, pos) == '#') {
		cout << '?';
		return false;
	}

	if (at(map, pos) == '[' && dir.second != 0) {
		if (!propag2(map, pos + dir, dir) || !propag2(map, pos + dir + make_pair(1l, 0l), dir)) {

			cout << '?';
			return false;
		}

		at(map, pos) = '.';
		at(map, pos + make_pair(1l, 0l)) = '.';
		at(map, pos + dir) = '[';
		at(map, pos + dir + make_pair(1l, 0l)) = ']';
	}

	if (at(map, pos) == ']' && dir.second != 0) {
		if (!propag2(map, pos + dir, dir) || !propag2(map, pos + dir + make_pair(-1l, 0l), dir)) {
			cout << '?';
			return false;
		}

		at(map, pos) = '.';
		at(map, pos + make_pair(-1l, 0l)) = '.';
		at(map, pos + dir) = ']';
		at(map, pos + dir + make_pair(-1l, 0l)) = '[';
	}

	if ((at(map, pos) == '[' || at(map, pos) == ']') && dir.second == 0) {
		if (!propag2(map, pos + dir, dir)) {
			cout << '?';
			return false;
		}

		at(map, pos + dir) = at(map, pos);
		at(map, pos) = '.';
	}

	return true;
}

void part2() {
	uint64_t sum = 0;

	bool part = true;
	vector<string> map;
	string cmd;
	vec2 player;

	for (const auto& line : get()) {
		if (part) {
			if (line.empty()) {
				part = false;

				continue;
			}

			map.emplace_back("");

			for (const auto [i, c] : views::enumerate(line)) {
				if (c == '@') {
					player = make_pair(i * 2, map.size() - 1);

					map.back() += "..";

					continue;
				} else {
					switch (c) {
						case '#':
							map.back() += "##";
							break;

						case 'O':
							map.back() += "[]";
							break;

						case '.':
							map.back() += "..";
							break;
					}
				}
			}
		} else {
			cmd += line;
		}
	}

	for (const auto& inst : cmd) {
		const auto dir = inst2dir(inst);

		if (tryPropag2(map, player + dir, dir)) {
			propag2(map, player + dir, dir);

			player += dir;
		}
	}

	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == '[') {
				sum += y * 100 + x;
			}
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
