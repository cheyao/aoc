#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <ranges>
#include <span>
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

using score = pair<uint64_t, vec2>;

uint64_t WIDTH = 71;

vector<vec2> getNext(const vec2 cur) { return {cur + UP, cur + DOWN, cur + LEFT, cur + RIGHT}; }

void part1() {
	uint64_t sum = 0;

	vector<string> map(WIDTH, string(WIDTH, '.'));
	uint64_t i = 0;
	for (const auto& line : get()) {
		++i;

		vector<string> l = line | views::split(',') | ranges::to<vector<string>>();
		map[stoull(l[1])][stoull(l[0])] = '#';

		if (i == 1024) {
			break;
		}
	}
	vec2 start = make_pair(0, 0);
	vec2 end = make_pair(WIDTH - 1, WIDTH - 1);

	auto cmp = [](const score& a, const score& b) { return a.first > b.first; };

	deque<score> frontiere;
	frontiere.push_back(make_pair(0, start));
	unordered_map<vec2, uint64_t> visited;
	visited[start] = 0;

	while (!frontiere.empty()) {
		const auto cur = frontiere.front();
		frontiere.pop_front();

		if (cur.second == end) {
			sum = cur.first;

			break;
		}

		for (const auto& next : getNext(cur.second)) {
			if (!valid(map, next) || at(map, next) == '#') {
				continue;
			}

			auto next_score = cur.first + 1;

			if (!visited.contains(next)) {
				frontiere.push_back(make_pair(next_score, next));
				visited[next] = next_score;
			}
		}
	}

	cout << "Part 1: " << sum << '\n';
}

vector<vec2> getPath(const span<string> map) {
	bool route = false;
	constexpr vec2 start = make_pair(0, 0);
	vec2 end = make_pair(WIDTH - 1, WIDTH - 1);

	auto cmp = [](const score& a, const score& b) { return a.first > b.first; };

	deque<vec2> frontiere;
	frontiere.push_back(start);
	unordered_map<vec2, vec2> came_from;
	came_from[start] = make_pair(0, 0);

	while (!frontiere.empty()) {
		const auto cur = frontiere.front();
		frontiere.pop_front();

		if (cur == end) {
			route = true;

			break;
		}

		for (const auto& next : getNext(cur)) {
			if (!valid(map, next) || at(map, next) == '#') {
				continue;
			}

			if (!came_from.contains(next)) {
				frontiere.push_back(next);
				came_from[next] = cur;
			}
		}
	}

	if (!route) {
		return {};
	}

	vector<vec2> path;
	vec2 last = end;
	while (last != start) {
		path.emplace_back(last);
		last = came_from[last];
	}

	return path;
}

void part2() {
	vec2 sum;

	vector<string> map(WIDTH, string(WIDTH, '.'));
	auto path = getPath(map);

	for (const auto& line : get()) {
		vector<string> l = line | views::split(',') | ranges::to<vector<string>>();
		sum = make_pair(stoull(l[0]), stoull(l[1]));
		at(map, sum) = '#';

		if (ranges::find(path, sum) != end(path)) {
			path = getPath(map);

			if (path.empty()) {
				break;
			}
		}
	}

	cout << "Part 2: " << sum.first << ',' << sum.second << endl;
}

int main(int argc, char** argv) {
	if (argc == 2) {
		WIDTH = atoll(argv[1]);
	}

	part1();
	part2();
}
