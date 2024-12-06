#include <algorithm>
#include <cassert>
#include <cctype>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
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

void part1() {
	uint64_t sum = 0;

	int64_t xoff = 0, yoff = -1;
	int64_t xpos = 0, ypos = 0;

	vector<string> map = get();

	for (size_t i = 0; i < map.size(); ++i) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[i][x] == '^') {
				xpos = x;
				ypos = i;

				break;
			}
		}
	}

	auto inbounds = [&map](int64_t x, int64_t y) {
		if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size()) {
			return false;
		}

		return true;
	};

	while (inbounds(xpos, ypos)) {
		map[ypos][xpos] = 'X';

		if (inbounds(xpos + xoff, ypos + yoff) && map[ypos + yoff][xpos + xoff] == '#') {
			if (xoff == 0 && yoff == 1) {
				xoff = -1;
				yoff = 0;
			} else if (xoff == 0 && yoff == -1) {
				xoff = 1;
				yoff = 0;
			} else if (xoff == 1 && yoff == 0) {
				xoff = 0;
				yoff = 1;
			} else if (xoff == -1 && yoff == 0) {
				xoff = 0;
				yoff = -1;
			}
		}

		xpos += xoff;
		ypos += yoff;
	}

	for (const auto& line : map) {
		sum += ranges::count(line, 'X');
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	int64_t xoff = 0, yoff = -1;
	int64_t xpos = 0, ypos = 0;
	int64_t xposs = 0, yposs = 0;

	vector<string> map = get();

	for (size_t i = 0; i < map.size(); ++i) {
		for (size_t x = 0; x < map[0].size(); ++x) {
			if (map[i][x] == '^') {
				xpos = x;
				ypos = i;
				xposs = x;
				yposs = i;

				map[i][x] = 1;
			}

			if (map[i][x] != '#') {
				map[i][x] = 1;
			}
		}
	}

	auto inbounds = [&map](int64_t x, int64_t y) {
		if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size()) {
			return false;
		}

		return true;
	};

	auto tryp = [&map, &inbounds](int64_t x, int64_t y, int64_t x_offset, int64_t y_offset) {
		vector<string> m = map;
		m[y + y_offset][x + x_offset] = '#';

		while (inbounds(x + x_offset, y + y_offset)) {
			if (x_offset == 0 && y_offset == 1) {
				if (m[y][x] & 0b10) {
					return true;
				}

				m[y][x] |= 0b10;
			} else if (x_offset == 0 && y_offset == -1) {
				if (m[y][x] & 0b100) {
					return true;
				}

				m[y][x] |= 0b100;
			} else if (x_offset == 1 && y_offset == 0) {
				if (m[y][x] & 0b1000) {
					return true;
				}

				m[y][x] |= 0b1000;
			} else if (x_offset == -1 && y_offset == 0) {
				if (m[y][x] & 0b10000) {
					return true;
				}

				m[y][x] |= 0b10000;
			}

			// Turn if next block is out of radius
			if (m[y + y_offset][x + x_offset] == '#') {
				if (x_offset == 0 && y_offset == 1) {
					x_offset = -1;
					y_offset = 0;
				} else if (x_offset == 0 && y_offset == -1) {
					x_offset = 1;
					y_offset = 0;
				} else if (x_offset == 1 && y_offset == 0) {
					x_offset = 0;
					y_offset = 1;
				} else if (x_offset == -1 && y_offset == 0) {
					x_offset = 0;
					y_offset = -1;
				}
			}

			if (m[y + y_offset][x + x_offset] != '#') {
				x += x_offset;
				y += y_offset;
			}
		}

		return false;
	};

	while (inbounds(xpos + xoff, ypos + yoff)) {
		if (map[ypos + yoff][xpos + xoff] == '#') {
			if (xoff == 0 && yoff == 1) {
				xoff = -1;
				yoff = 0;
			} else if (xoff == 0 && yoff == -1) {
				xoff = 1;
				yoff = 0;
			} else if (xoff == 1 && yoff == 0) {
				xoff = 0;
				yoff = 1;
			} else if (xoff == -1 && yoff == 0) {
				xoff = 0;
				yoff = -1;
			}
		}

		if (map[ypos + yoff][xpos + xoff] != 0b1000000 &&
			!((ypos + yoff) == yposs && (xpos + xoff) == xposs)) {
			if (tryp(xpos, ypos, xoff, yoff)) {
				++sum;
			}
		}

		map[ypos][xpos] = 0b1000000;

		xpos += xoff;
		ypos += yoff;
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
