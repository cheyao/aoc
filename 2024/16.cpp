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

template <> struct std::hash<pos> {
	std::size_t operator()(pos const& p) const noexcept {
		return static_cast<uint64_t>(p.second.first) |
			   (static_cast<uint64_t>(p.second.second) << 32) +
				   static_cast<uint64_t>(
					   ((p.first.second.first + 2) + (p.first.second.second + 2) * 4) << 20);
	}
};

void part2() {
	uint64_t sum = 0;

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
	auto san = [](pos p) {
		p.first.first = 0;
		return p;
	};

	priority_queue<pos, vector<pos>, decltype(cmp)> front(cmp);
	front.emplace(make_pair(make_pair(0, RIGHT), start));

	unordered_map<pos, uint64_t> cost_so_far;
	cost_so_far[front.top()] = 0;
	unordered_map<pos, vector<pos>> last;

	deque<pos> dst;

	while (!front.empty()) {
		const auto cur = front.top();
		front.pop();

		if (cur.second == end && cur.first.first == sum1) {
			dst.emplace_back(cur);

			continue;
		}

		for (const auto& next : getNext(cur)) {
			if (at(map, next.second) == '#') {
				continue;
			}

			const auto current_cost = next.first.first;

			if (!cost_so_far.contains(san(next)) || cost_so_far[san(next)] >= current_cost) {
				cost_so_far[san(next)] = current_cost;
				front.push(next);

				if (ranges::find(last[next], cur) == last[next].end()) {
					last[next].emplace_back(cur);
				}
			}
		}
	}

	while (!dst.empty()) {
		const auto cur = dst.front();
		dst.pop_front();

		at(map, cur.second) = 'O';

		for (const auto& p : last[cur]) {
			if (at(map, p.second) != 'O') {
				dst.emplace_back(p);
			}
		}
	}

	for (const auto& l : map) {
		for (const auto c : l) {
			sum += c == 'O';
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main(int argc, char** argv) {
	part1();
	part2();
}
