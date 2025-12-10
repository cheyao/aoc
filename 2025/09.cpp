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

	uint64_t max_x = 0;
	uint64_t max_y = 0;
	uint64_t min_x = 999999999999;
	uint64_t min_y = 999999999999;

	vector<vec2> tiles;
	for (const auto& l : get()) {
		const auto c = l | views::split(',') |
					   views::transform([](const auto& s) { return atoll(s.data()); }) |
					   ranges::to<vector<uint64_t>>();
		tiles.emplace_back(c[0], c[1]);

		max_x = max(max_x, c[0]);
		max_y = max(max_y, c[1]);
	}

	vector<vector<char>> map(max_y + 1, vector<char>(max_x + 1, '.'));

	vector<rect> rects;
	for (uint64_t i = 0; i < tiles.size(); ++i) {
		map[tiles[i].second][tiles[i].first] = '#';

		for (uint64_t j = 0; j < i; ++j) {
			const auto diff = tiles[i] - tiles[j];

			rects.emplace_back((abs(diff.first) + 1) * (abs(diff.second) + 1), i, j);
		}
	}

	std::map<uint64_t, vector<pair<uint64_t, uint64_t>>> compressedY;
	std::map<uint64_t, vector<pair<uint64_t, uint64_t>>> compressedX;

	ranges::sort(rects, ranges::greater(), &rect::size);
	{
		vector<vec2> tilesd = tiles;
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
					compressedY[y].emplace_back(xmin, xmax);
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
			compressedY[y].emplace_back(xmin, xmax);
		}

		// For X!
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
					compressedX[x].emplace_back(ymin, ymax);
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
			compressedX[x].emplace_back(ymin, ymax);
		}
	}

	// Check if is inside
	for (const auto rect : rects) {
		const auto a = tiles[rect.a];
		const auto b = tiles[rect.b];

		uint64_t min_x = min(a.first, b.first);
		uint64_t min_y = min(a.second, b.second);
		uint64_t max_x = max(a.first, b.first);
		uint64_t max_y = max(a.second, b.second);

		bool ok = true;
		for (uint64_t x = min_x; x <= max_x; ++x) {
			if (!contains(compressedX, compressedY, x, max_y) ||
				!contains(compressedX, compressedY, x, min_y)) {
				ok = false;
				break;
			}
		}
		for (uint64_t y = min_y; y <= max_y; ++y) {
			if (!contains(compressedX, compressedY, max_x, y) ||
				!contains(compressedX, compressedY, min_x, y)) {
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
