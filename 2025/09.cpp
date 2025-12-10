#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <ranges>
#include <ratio>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

#include "../vec2.hpp"

using namespace std;

static vector<string> get() {
	static vector<string> out;

	if (out.empty()) {
		ifstream file(__FILE__ ".txt");
		string line;

		while (getline(file, line)) {
			if (!line.empty() && ranges::count(line, '.') != line.length()) {
				out.emplace_back(std::move(line));
			}
		}
	}

	return out;
}

struct rect {
	uint64_t size;
	uintptr_t a, b;
};

auto part1() {
	uint64_t sum = 0;

	vector<vec2> tiles;
	for (const auto& l : get()) {
		const auto c = l | views::split(',') |
					   views::transform([](const auto& s) { return atoll(s.data()); }) |
					   ranges::to<vector<uint64_t>>();
		tiles.emplace_back(c[0], c[1]);
	}

	vector<rect> rects;
	for (uint64_t i = 0; i < tiles.size(); ++i) {
		for (uint64_t j = 0; j < i; ++j) {
			const auto diff = tiles[i] - tiles[j];

			rects.emplace_back((abs(diff.first) + 1) * (abs(diff.second) + 1), i, j);
		}
	}

	return ranges::max(rects, {}, &rect::size).size;
}
bool contains(const auto& compressedX, const auto& compressedY, const uint64_t x,
			  const uint64_t y) {
	bool cx = false; // This is the y coords
	for (const auto& r : compressedX.at(x)) {
		if (r.first <= y && y <= r.second) {
			cx = true;
			break;
		}
	}
	bool cy = false; // This is the x coords
	for (const auto& r : compressedY.at(y)) {
		if (r.first <= x && x <= r.second) {
			cy = true;
			break;
		}
	}
	return cx && cy;
};

auto part2() {
	uint64_t sum = 0;

	vector<vec2> tiles;
	for (const auto& l : get()) {
		const auto c = l | views::split(',') |
					   views::transform([](const auto& s) { return atoll(s.data()); }) |
					   ranges::to<vector<uint64_t>>();
		tiles.emplace_back(c[0], c[1]);
	}

	// X Mapping
	map<uint64_t, uint64_t> xmap;
	ranges::sort(tiles, {}, &vec2::first);
	uint64_t i = 0;
	for (const auto& t : tiles) {
		if (!xmap.contains(t.first)) {

			xmap[t.first] = i;
			++i;
		}
	}
	// Y Mapping
	map<uint64_t, uint64_t> ymap;
	ranges::sort(tiles, {}, &vec2::second);
	i = 0;
	for (const auto& t : tiles) {
		if (!ymap.contains(t.second)) {
			ymap[t.second] = i;
			++i;
		}
	}

	vector<string> map(ymap.size() + 1, string(xmap.size() + 1, '.'));
	vector<vec2> mappedTiles;
	for (const auto& t : tiles) {
		mappedTiles.emplace_back(xmap[t.first], ymap[t.second]);
	}

	vector<rect> rects;
	for (uint64_t i = 0; i < tiles.size(); ++i) {
		map[mappedTiles[i].second][mappedTiles[i].first] = '#';

		for (uint64_t j = 0; j < i; ++j) {
			const auto diff = tiles[i] - tiles[j];

			rects.emplace_back((abs(diff.first) + 1) * (abs(diff.second) + 1), i, j);
		}
	}

	ranges::sort(rects, ranges::greater(), &rect::size);
	{
		vector<vec2> tilesd = mappedTiles;
		ranges::sort(tilesd, {}, &vec2::second);

		vector<vec2> line;
		uint64_t y = 0;
		for (const auto t : tilesd) {
			if (t.second != y) {
				ranges::sort(line, {}, &vec2::first);

				// Get pairs and thing
				for (const auto elem : line | views::slide(2)) {
					uint64_t xmin = min(elem[0].first, elem[1].first);
					uint64_t xmax = max(elem[0].first, elem[1].first);

					for (uint64_t x = xmin; x <= xmax; ++x) {
						map[y][x] = '$';
					}
				}

				y = t.second;
				line.clear();
			}

			line.emplace_back(t);
		}

		ranges::sort(line, {}, &vec2::first);
		for (const auto elem : line | views::slide(2)) {
			uint64_t xmin = min(elem[0].first, elem[1].first);
			uint64_t xmax = max(elem[0].first, elem[1].first);

			for (uint64_t x = xmin; x <= xmax; ++x) {
				map[y][x] = '$';
			}
		}

		// Now on X side
		ranges::sort(tilesd, {}, &vec2::first);
		line.clear();
		uint64_t x = 0;
		for (const auto t : tilesd) {
			if (t.first != x) {
				ranges::sort(line, {}, &vec2::second);

				// Get pairs and thing
				for (const auto elem : line | views::slide(2)) {
					uint64_t ymin = min(elem[0].second, elem[1].second);
					uint64_t ymax = max(elem[0].second, elem[1].second);

					for (uint64_t y = ymin; y <= ymax; ++y) {

						map[y][x] = '$';
					}
				}

				x = t.first;
				line.clear();
			}

			line.emplace_back(t);
		}

		ranges::sort(line, {}, &vec2::second);
		for (const auto elem : line | views::slide(2)) {
			uint64_t ymin = min(elem[0].second, elem[1].second);
			uint64_t ymax = max(elem[0].second, elem[1].second);

			for (uint64_t y = ymin; y <= ymax; ++y) {

				map[y][x] = '$';
			}
		}
	}

	// Get a point inside
	vec2 point = make_pair(0, 0);
	bool inside = false;
	for (uint64_t x = 0; x <= xmap.size(); ++x) {
		if (map[ymap.size() / 2][x] != '.') {
			inside = true;
			continue;
		}

		if (inside) {
			point = make_pair(x, ymap.size() / 2);
			break;
		}
	}

	// Fill!
	queue<vec2> q;
	q.emplace(point);
	while (!q.empty()) {
		const auto c = q.front();
		q.pop();

		if (map[c.second][c.first] != '.') {
			continue;
		}

		map[c.second][c.first] = '#';

		auto l = [&](const auto& c) {
			if (valid(map, c)) {
				q.emplace(c);
			}
		};
		l(c + UP);
		l(c + DOWN);
		l(c + LEFT);
		l(c + RIGHT);
	}

	// Check if is inside
	for (const auto rect : rects) {
		const auto a = mappedTiles[rect.a];
		const auto b = mappedTiles[rect.b];

		uint64_t min_x = min(a.first, b.first);
		uint64_t min_y = min(a.second, b.second);
		uint64_t max_x = max(a.first, b.first);
		uint64_t max_y = max(a.second, b.second);

		bool ok = true;

		for (uint64_t x = min_x; x <= max_x; ++x) {
			if (map[max_y][x] == '.' || map[min_y][x] == '.') {
				ok = false;
				break;
			}
		}

		for (uint64_t y = min_y; y <= max_y; ++y) {
			if (map[y][max_x] == '.' || map[y][min_x] == '.') {
				ok = false;
				break;
			}
		}

		if (ok) {
			sum = rect.size;

			break;
		}
	}

	return sum;
}

int main() {
	auto tries = 1;
	auto ignore = 0;

	double time = 0;
	uint64_t result = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part1();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= ignore) {
			time += ms_double.count();
		}
	}
	cout << "Part 1: " << result << endl;
	std::cout << time / (tries - ignore) << "us\n";

	time = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part2();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= ignore) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - ignore) << "us\n";
}
