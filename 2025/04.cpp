#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <ratio>
#include <string>
#include <utility>
#include <vector>

using namespace std;

static vector<string> get() {
	static vector<string> out;

	if (out.empty()) {
		ifstream file(__FILE__ ".txt");
		string line;

		while (getline(file, line)) {
			if (!line.empty()) {
				out.emplace_back(std::move(line));
			}
		}
	}

	return out;
}

constexpr const bool isEmpty(const char c) { return c == '.'; }
bool check(const vector<string>& map, int64_t x, int64_t y) {
	if (x < 0 || y < 0 || y >= map.size() || x >= map[0].size()) {
		return true;
	}

	return isEmpty(map[y][x]);
}
bool removable(const vector<string>& map, int64_t x, int64_t y) {
	return (check(map, x + 1, y + 0) + check(map, x + 1, y + 1) + check(map, x + 1, y - 1) +
			check(map, x - 1, y + 1) + check(map, x - 1, y + 0) + check(map, x - 1, y - 1) +
			check(map, x + 0, y + 1) + check(map, x + 0, y - 1)) >= 5;
}

// Check part 2
auto part1() {
	uint64_t sum = 0;

	const auto map = get();

	for (uint64_t y = 0; y < map.size(); ++y) {
		for (uint64_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == '.') {
				continue;
			}

			if (removable(map, x, y)) {
				++sum;
			}
		}
	}

	return sum;
}

bool valid(const vector<string>& map, const int64_t x, const int64_t y) {
	return !(x < 0 || y < 0 || y >= map.size() || x >= map[0].size()) && map[y][x] == '@';
}

auto part2() {
	uint64_t sum = 0;

	auto map = get();

	queue<pair<int64_t, int64_t>> toRemove;

	for (uint64_t y = 0; y < map.size(); ++y) {
		for (uint64_t x = 0; x < map[0].size(); ++x) {
			if (map[y][x] == '@' && removable(map, x, y)) {
				toRemove.push(make_pair(x, y));
			}
		}
	}

	while (!toRemove.empty()) {
		const auto [x, y] = toRemove.front();
		toRemove.pop();
		if (map[y][x] != '@') {
			continue;
		}

		++sum;
		map[y][x] = '.';

		for (const auto [dx, dy] :
			 {make_pair(1, 0), make_pair(1, 1), make_pair(1, -1), make_pair(0, 1), make_pair(0, -1),
			  make_pair(-1, 0), make_pair(-1, 1), make_pair(-1, -1)}) {

			if (valid(map, x + dx, y + dy) && map[y + dy][x + dx] == '@' &&
				removable(map, x + dx, y + dy)) {
				toRemove.push(make_pair(x + dx, y + dy));
			}
		}
	}

	return sum;
}

int main() {
	auto tries = 250;

	double time = 0;
	uint64_t result = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part1();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::milli> ms_double = t2 - t1;
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 1: " << result << endl;
	std::cout << time / (tries - 3) << "ms\n";

	time = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part2();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::milli> ms_double = t2 - t1;
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - 3) << "ms\n";
}
