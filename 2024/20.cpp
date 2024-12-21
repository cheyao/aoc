#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <queue>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <unordered_map>
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

vector<vec2> getNext(const vec2 cur) { return {cur + UP, cur + LEFT, cur + DOWN, cur + RIGHT}; }
vector<vec2> cheat(const vec2 cur) {
	return {cur + UP + UP,	   cur + UP + RIGHT,  cur + RIGHT + RIGHT, cur + RIGHT + DOWN,
			cur + DOWN + DOWN, cur + DOWN + LEFT, cur + LEFT + LEFT,   cur + LEFT + UP};
}

void part1() {
	uint64_t sum = 0;

	auto map = get();
	vec2 start;
	vec2 end;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == 'S') {
				start = make_pair(x, y);
			}

			if (map[y][x] == 'E') {
				end = make_pair(x, y);
			}
		}
	}

	queue<vec2> frontiere;
	frontiere.emplace(start);
	unordered_map<vec2, vec2> came_from;
	came_from[start] = make_pair(-1, -1);
	unordered_map<vec2, uint64_t> cost;
	cost[start] = 0;

	while (!frontiere.empty()) {
		const auto current = frontiere.front();
		frontiere.pop();

		if (current == end) {
			break;
		}

		const auto new_cost = cost[current] + 1;
		for (const auto next : getNext(current)) {
			if (!came_from.contains(next) && at(map, next) != '#') {
				frontiere.emplace(next);
				came_from[next] = current;
				cost[next] = new_cost;
			}
		}
	}

	vector<vec2> pos = {};

	vec2 cur = end;
	while (cur != start) {
		at(map, cur) = 'o';

		pos.emplace_back(cur);

		cur = came_from[cur];
	}

	at(map, cur) = 'o';
	pos.emplace_back(cur);

	// Construct a new cost map
	constexpr const static int64_t MAX = numeric_limits<int64_t>::max();
	vector<vector<int64_t>> cmap(map.size());
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] != 'o') {
				cmap[y].emplace_back(MAX);
			} else {
				cmap[y].emplace_back(cost[make_pair(x, y)]);
			}
		}
	}

	for (const auto& cell : pos) {
		for (const auto& cheats : cheat(cell)) {
			if (valid(map, cheats) && at(map, cheats) != '#') {
				sum += (at(cmap, cheats) - at(cmap, cell) - 2) >= 100;
			}
		}
	}

	cout << "Part 1: " << sum << '\n';
}

void part2() {
	uint64_t sum = 0;

	auto map = get();
	vec2 start;
	vec2 end;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == 'S') {
				start = make_pair(x, y);
			}

			if (map[y][x] == 'E') {
				end = make_pair(x, y);
			}
		}
	}

	queue<vec2> frontiere;
	frontiere.emplace(start);
	unordered_map<vec2, vec2> came_from;
	came_from[start] = make_pair(-1, -1);
	unordered_map<vec2, uint64_t> cost;
	cost[start] = 0;

	while (!frontiere.empty()) {
		const auto current = frontiere.front();
		frontiere.pop();

		if (current == end) {
			break;
		}

		const auto new_cost = cost[current] + 1;
		for (const auto next : getNext(current)) {
			if (!came_from.contains(next) && at(map, next) != '#') {
				frontiere.emplace(next);
				came_from[next] = current;
				cost[next] = new_cost;
			}
		}
	}

	vector<vec2> pos = {};

	vec2 cur = end;
	while (cur != start) {
		at(map, cur) = 'o';
		pos.emplace_back(cur);

		cur = came_from[cur];
	}
	at(map, cur) = 'o';
	pos.emplace_back(cur);

	// Construct a new cost map
	constexpr const static int64_t MAX = numeric_limits<int64_t>::max();
	vector<vector<int64_t>> cmap(map.size());
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] != 'o') {
				cmap[y].emplace_back(MAX);
			} else {
				cmap[y].emplace_back(cost[make_pair(x, y)]);
			}
		}
	}

	for (const auto& cell : pos) {
		for (const auto& cheats : pos) {
			int64_t distance = dist(cell, cheats);
			if (distance <= 20 && valid(map, cheats) && at(map, cheats) != '#') {
				int64_t s = (at(cmap, cheats) - at(cmap, cell) - distance);
				if (s >= 100) {
					sum++;
				}
			}
		}
	}

	cout << "Part 2: " << sum << endl;
}

int main() {
	part1();
	part2();
}
