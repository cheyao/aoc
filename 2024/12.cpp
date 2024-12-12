#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <sys/types.h>
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

using pos = pair<uint64_t, uint64_t>;

uint64_t find(const decltype(get())& map, vector<pos>& found, pos p) {
	auto inside = [&map](const pos p) {
		return p.first >= 0 && p.second >= 0 && p.first < map[0].size() && p.second < map.size();
	};
	auto at = [&map](const pos p) { return map[p.second][p.first]; };
	auto add = [](pos p, const uint64_t x, const uint64_t y) {
		p.first += x;
		p.second += y;
		return p;
	};
	auto neighbors = [&add](pos cell) -> vector<pos> {
		return {add(cell, 1, 0), add(cell, -1, 0), add(cell, 0, -1), add(cell, 0, 1)};
	};

	uint64_t fences = 0;
	uint64_t area = 0;

	if (ranges::find(found, p) != found.end()) {
		return 0;
	}

	deque<pos> front;
	front.emplace_back(p);
	vector<pos> reached;

	const char id = at(p);
	while (!front.empty()) {
		const auto cell = front.back();
		front.pop_back();

		if (!inside(cell) || at(cell) != id) {
			++fences;

			continue;
		}

		if (ranges::find(reached, cell) != reached.end()) {
			continue;
		}

		reached.emplace_back(cell);
		found.emplace_back(cell);

		++area;
		for (const auto& next : neighbors(cell)) {
			if (ranges::find(reached, next) == reached.end()) {
				front.emplace_back(next);
			}
		}
	}

	return fences * area;
}

void part1() {
	uint64_t sum = 0;

	const auto map = get();
	vector<pos> found;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			sum += find(map, found, make_pair(x, y));

			found.emplace_back(make_pair(x, y));
		}
	}

	cout << "Part 1:" << sum << endl;
}

enum s { N, E, S, W };
using side = pair<pos, s>;

uint64_t bulk(const decltype(get())& map, vector<pos>& found, pos p) {
	auto inside = [&map](const pos p) {
		return p.first >= 0 && p.second >= 0 && p.first < map[0].size() && p.second < map.size();
	};
	auto at = [&map](const pos p) { return map[p.second][p.first]; };
	auto add = [](pos p, const uint64_t x, const uint64_t y) {
		p.first += x;
		p.second += y;
		return p;
	};
	auto neighbors = [&add](pos cell) -> array<side, 4> {
		return {make_pair(add(cell, 1, 0), E), make_pair(add(cell, -1, 0), W),
				make_pair(add(cell, 0, -1), N), make_pair(add(cell, 0, 1), S)};
	};

	uint64_t area = 0;

	if (ranges::find(found, p) != found.end()) {
		return 0;
	}

	deque<side> front;
	front.emplace_back(make_pair(p, N));
	vector<pos> reached;
	vector<side> fences;

	const char id = at(p);
	while (!front.empty()) {
		const auto cell = front.back();
		front.pop_back();

		if (!inside(cell.first) || at(cell.first) != id) {
			fences.emplace_back(cell);

			continue;
		}

		if (ranges::find(reached, cell.first) != reached.end()) {
			continue;
		}

		reached.emplace_back(cell.first);
		found.emplace_back(cell.first);

		++area;
		for (const auto& next : neighbors(cell.first)) {
			if (ranges::find(reached, next.first) == reached.end()) {
				front.emplace_back(next);
			}
		}
	}

	// Now count the fucking fence side count!
	auto sideNeighbors = [&add](const side s) -> array<side, 2> {
		switch (s.second) {
			case N:
				return {make_pair(add(s.first, 1, 0), N), make_pair(add(s.first, -1, 0), N)};

			case E:
				return {make_pair(add(s.first, 0, 1), E), make_pair(add(s.first, 0, -1), E)};

			case S:
				return {make_pair(add(s.first, 1, 0), S), make_pair(add(s.first, -1, 0), S)};

			case W:
				return {make_pair(add(s.first, 0, 1), W), make_pair(add(s.first, 0, -1), W)};
		};

		return {};
	};

	uint64_t sides = 0;
	while (!fences.empty()) {
		auto fence = fences.back();
		fences.pop_back();

		++sides;

		deque<side> toV;
		toV.emplace_back(fence);
		vector<side> v;

		while (!toV.empty()) {
			auto c = toV.back();
			toV.pop_back();

			for (const auto& neighbor : sideNeighbors(c)) {
				if (auto p = ranges::find(fences, neighbor); p != fences.end()) {
					if (p != fences.end()) {
						std::swap(*p, fences.back());
					}

					fences.pop_back();

					if (ranges::find(v, neighbor) == v.end()) {
						toV.emplace_back(neighbor);
						v.emplace_back(neighbor);
					}
				}
			}
		}
	}

	return area * sides;
}

void part2() {
	uint64_t sum = 0;

	const auto map = get();
	vector<pos> found;
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			sum += bulk(map, found, make_pair(x, y));

			found.emplace_back(make_pair(x, y));
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
