#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
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

using score = uint64_t;
using pos = pair<pair<score, vec2>, vec2>;

vector<pos> getNext(const pos cur) {
	if (cur.first.second == UP) {
		return {
			make_pair(make_pair(cur.first.first + 1001, LEFT), cur.second + LEFT),
			make_pair(make_pair(cur.first.first + 1, UP), cur.second + UP),
			make_pair(make_pair(cur.first.first + 1001, RIGHT), cur.second + RIGHT),
		};
	}
	if (cur.first.second == DOWN) {
		return {
			make_pair(make_pair(cur.first.first + 1001, LEFT), cur.second + LEFT),
			make_pair(make_pair(cur.first.first + 1, DOWN), cur.second + DOWN),
			make_pair(make_pair(cur.first.first + 1001, RIGHT), cur.second + RIGHT),
		};
	}
	if (cur.first.second == RIGHT) {
		return {
			make_pair(make_pair(cur.first.first + 1001, UP), cur.second + UP),
			make_pair(make_pair(cur.first.first + 1, RIGHT), cur.second + RIGHT),
			make_pair(make_pair(cur.first.first + 1001, DOWN), cur.second + DOWN),
		};
	}
	if (cur.first.second == LEFT) {
		return {
			make_pair(make_pair(cur.first.first + 1001, UP), cur.second + UP),
			make_pair(make_pair(cur.first.first + 1, LEFT), cur.second + LEFT),
			make_pair(make_pair(cur.first.first + 1001, DOWN), cur.second + DOWN),
		};
	}

	std::unreachable();
}

uint64_t sum1 = 0;
unordered_map<vec2, vec2> llast;
unordered_map<vec2, uint64_t> ccost_so_far;

void part1() {
	auto map = get();
	vec2 start;
	vec2 end;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == 'S') {
				start = make_pair(x, y);
				continue;
			}

			if (map[y][x] == 'E') {
				end = make_pair(x, y);
				continue;
			}
		}
	}

	auto cmp = [](const pos& a, const pos& b) { return a.first.first > b.first.first; };

	priority_queue<pos, vector<pos>, decltype(cmp)> front(cmp);
	front.emplace(make_pair(make_pair(0, RIGHT), start));
	ccost_so_far[start] = 0;
	llast[start] = make_pair(-1, -1);

	while (!front.empty()) {
		const auto cur = front.top();
		front.pop();

		for (const auto& next : getNext(cur)) {
			if (at(map, next.second) == '#') {
				continue;
			}

			if (ccost_so_far.contains(next.second) &&
				ccost_so_far[next.second] <= next.first.first) {
				continue;
			}

			[[unlikely]] if (next.second == end) { sum1 = next.first.first; }

			ccost_so_far[next.second] = next.first.first;
			front.push(next);
			llast[next.second] = cur.second;
		}
	}

	cout << "Part 1:" << sum1 << endl;
}

void part2() {
	uint64_t sum = 0;

	auto map = get();
	vec2 start;
	vec2 end;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == 'S') {
				end = make_pair(x, y);
				continue;
			}

			if (map[y][x] == 'E') {
				start = make_pair(x, y);
				continue;
			}
		}
	}

	auto cmp = [](const pos& a, const pos& b) { return a.first.first > b.first.first; };

	priority_queue<pos, vector<pos>, decltype(cmp)> front(cmp);
	front.emplace(make_pair(make_pair(0, LEFT), start));
	front.emplace(make_pair(make_pair(0, DOWN), start));
	unordered_map<vec2, uint64_t> cost_so_far;
	unordered_map<vec2, vec2> last;
	cost_so_far[start] = 0;
	last[start] = make_pair(-1, -1);
	deque<vec2> dst;

	while (!front.empty()) {
		const auto cur = front.top();
		front.pop();

		if (ccost_so_far[cur.second] + cur.first.first == sum1) {
			++sum;
		}

		for (const auto& next : getNext(cur)) {
			if (at(map, next.second) == '#') {
				continue;
			}

			if (cost_so_far.contains(next.second) && cost_so_far[next.second] <= next.first.first) {
				continue;
			}

			if (next.second == end) {
				dst.emplace_back(cur.second);
			}

			cost_so_far[next.second] = next.first.first;
			front.push(next);
			last[next.second] = cur.second;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main(int argc, char** argv) {
	part1();
	part2();
}
