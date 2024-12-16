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
#include <queue>
#include <ranges>
#include <stdexcept>
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
			make_pair(make_pair(cur.first.first + 1000, LEFT), cur.second),
			make_pair(make_pair(cur.first.first + 1, UP), cur.second + UP),
			make_pair(make_pair(cur.first.first + 1000, RIGHT), cur.second),
		};
	}
	if (cur.first.second == DOWN) {
		return {
			make_pair(make_pair(cur.first.first + 1000, LEFT), cur.second),
			make_pair(make_pair(cur.first.first + 1, DOWN), cur.second + DOWN),
			make_pair(make_pair(cur.first.first + 1000, RIGHT), cur.second),
		};
	}
	if (cur.first.second == RIGHT) {
		return {
			make_pair(make_pair(cur.first.first + 1000, UP), cur.second),
			make_pair(make_pair(cur.first.first + 1, RIGHT), cur.second + RIGHT),
			make_pair(make_pair(cur.first.first + 1000, DOWN), cur.second),
		};
	}
	if (cur.first.second == LEFT) {
		return {
			make_pair(make_pair(cur.first.first + 1000, UP), cur.second),
			make_pair(make_pair(cur.first.first + 1, LEFT), cur.second + LEFT),
			make_pair(make_pair(cur.first.first + 1000, DOWN), cur.second),
		};
	}
	cout << "ERR!";
	return {};
}

void part1() {
	uint64_t sum = 0;

	auto map = get();
	vec2 start;
	vec2 end;
	for (const auto& [y, line] : views::enumerate(map)) {
		for (const auto& [x, c] : views::enumerate(line)) {
			if (c == 'S') {
				start = make_pair(x, y);
			}

			if (c == 'E') {
				end = make_pair(x, y);
			}
		}
	}

	auto cmp = [](const pos& a, const pos& b) { return a.first.first > b.first.first; };

	priority_queue<pos, vector<pos>, decltype(cmp)> front(cmp);
	vector<pair<vec2, vec2>> came;
	front.emplace(make_pair(make_pair(0, RIGHT), start));
	unordered_map<vec2, uint64_t> cost_so_far;
	cost_so_far[start] = 0;

	while (!front.empty()) {
		const auto cur = front.top();
		front.pop();

		for (const auto& next : getNext(cur)) {
			if (at(map, next.second) == '#') {
				continue;
			}

			if (ranges::find(came, make_pair(next.first.second, next.second)) != came.end() &&
				cost_so_far[next.second] <= next.first.first) {
				continue;
			}

			if (at(map, next.second) == 'E') {
				sum = next.first.first;

				goto end;
			}

			came.emplace_back(make_pair(next.first.second, next.second));
			front.push(next);
			cost_so_far[next.second] = next.first.first;
		}
	}

end:
	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	cout << "Part 2:" << sum << endl;
}

int main(int argc, char** argv) {
	part1();
	part2();
}
