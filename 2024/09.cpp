#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <string>
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

void part1() {
	uint64_t sum = 0;

	vector<uint64_t> disk;
	constexpr const static uint64_t max = numeric_limits<uint64_t>::max();
	for (const auto& line : get()) {
		uint64_t pos = 0;

		for (size_t i = 0; i < line.size(); ++i) {
			const uint64_t id = i % 2 == 0 ? i / 2 : max;

			disk.insert(disk.end(), line[i] - '0', id);
		}
	}

	for (size_t i = 0; i < disk.size(); ++i) {
		while (disk.back() == max) {
			disk.pop_back();
		}

		if (disk[i] == max) {
			std::swap(disk[i], disk.back());
		}
	}

	while (disk.back() == max) {
		disk.pop_back();
	}

	for (size_t i = 0; i < disk.size(); ++i) {
		sum += i * disk[i];
	}

	cout << "Part 1:" << sum << endl;
}

struct block {
	int64_t size;
	uint64_t pos;
};

void part2() {
	uint64_t sum = 0;

	const string line = get()[0];

	vector<block> blocks;
	blocks.reserve(line.size() / 2 + 1);
	vector<block> spaces;
	spaces.reserve(line.size() / 2 + 1);

	constexpr const static uint64_t max = numeric_limits<uint64_t>::max();
	uint64_t pos = 0;

	for (size_t i = 0; i < line.size(); ++i) {
		if (i % 2 == 0) {
			blocks.emplace_back(block{line[i] - '0', pos});
		} else {
			spaces.emplace_back(block{line[i] - '0', pos});
		}

		pos += line[i] - '0';
	}

	for (size_t i = blocks.size(); i != 0; --i) {
		for (auto& space : spaces) {
			if (space.size < blocks[i - 1].size) {
				continue;
			}

			if (space.pos >= blocks[i - 1].pos) {
				break;
			}

			blocks[i - 1].pos = space.pos;
			space.size -= blocks[i - 1].size;
			space.pos += blocks[i - 1].size;

			break;
		}
	}

	for (size_t b = 0; b < blocks.size(); ++b) {
		for (size_t i = blocks[b].pos; i < blocks[b].pos + blocks[b].size; ++i) {
			sum += i * b;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
