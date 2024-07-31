#include <sys/types.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

std::queue<std::string> file;
std::string empty = "";

void readFile() {
	while (!file.empty()) {
		file.pop();
	}

	std::ifstream input("05.input");
	std::string line;

	if (!input.is_open()) {
		std::cerr << "Failed to open input" << std::endl;
		exit(1);
	}

	while (!input.eof()) {
		file.push(line);
		std::getline(input, line);
	}

	file.pop();
}

typedef struct conv {
	uint64_t dst, src, count;
} conv;

void null() {}

std::vector<uint64_t> output;
std::vector<conv> convmap;
void map_stuff(std::vector<uint64_t>& input) {
	output.clear();
	convmap.clear();
	file.pop();
	file.pop();

	// Seed to soil map
	while (file.front() != empty) {
		auto sts = std::stringstream(file.front());

		uint64_t dst, src, count;
		sts >> dst >> src >> count;

		convmap.emplace_back((conv){dst, src, count});

		file.pop();
	}

	for (auto& i : input) {
		for (auto& b : convmap) {
			if (b.src <= i && i < b.src + b.count) {
				output.emplace_back(i + b.dst - b.src);
				goto idk;
			}
		}

		output.emplace_back(i);
	idk:
		null();
	}

	std::swap(input, output);
}

void part1() {
	std::vector<uint64_t> input;

	auto seeds = file.front();
	seeds.erase(0, seeds.find(':') + 1);
	auto seedstream = std::stringstream(seeds);
	while (!seedstream.eof()) {
		uint64_t tmp;
		seedstream >> tmp;
		input.emplace_back(tmp);
	}

	file.pop();

	map_stuff(input);
	map_stuff(input);
	map_stuff(input);
	map_stuff(input);
	map_stuff(input);
	map_stuff(input);
	map_stuff(input);

	uint64_t smallest = input[0];
	for (auto& i : input) {
		if (i < smallest) {
			smallest = i;
		}
	}

	std::cout << "Day 5 part 1: " << smallest << std::endl;
}

typedef struct {
	uint64_t start, end, size;
	uint64_t destination;
} mapEntry;  // Ignore the naming plz

void printqueue(std::queue<std::pair<uint64_t, uint64_t>> seeds) {
	std::cout << "Queue: [";
	while (!seeds.empty()) {
		std::cout << seeds.front().first << ":" << seeds.front().second
			  << "], [";
		seeds.pop();
	}
	std::cout << "]" << std::endl;
}

void mappart2(std::queue<std::pair<uint64_t, uint64_t>>& seeds) {
	std::vector<mapEntry> seedmap;
	std::queue<std::pair<uint64_t, uint64_t>> seeds_out;

	// Over the white spaces
	file.pop();
	file.pop();

	// Get the maps
	while (!file.empty() && file.front() != empty) {
		auto line = std::stringstream(file.front());

		uint64_t dst, src, count;
		line >> dst >> src >> count;

		seedmap.emplace_back((mapEntry){src, src + count, count, dst});

		file.pop();
	}

	while (!seeds.empty()) {
		auto seed = seeds.front();
		seeds.pop();

		for (mapEntry entry : seedmap) {
			auto seedStart = seed.first;
			auto seedEnd = seed.first + seed.second;
			auto seedLen = seed.second;
			auto mapStart = entry.start;
			auto mapEnd = entry.end;
			auto mapDest = entry.destination;
			auto mapLen = entry.size;

			// Seed: |-----|
			// Map:    |----|
			if (seedStart < mapStart && seedEnd > mapStart) {
				// mapStart = inside seed range
				seeds.push({seedStart, mapStart - seedStart});
				seed = {mapDest, std::min(mapLen, seedEnd - mapStart)};
				if (seedEnd > mapEnd) {
					seeds.push({mapEnd, seedEnd - mapEnd});
				}
				break;
			}

			// Seed:    |------|
			// Map:  |------|
			if (seedStart >= mapStart && seedStart < mapEnd) {
				seed = {mapDest + (seedStart - mapStart), std::min(seedLen, mapEnd - seedStart)};
				if (seedEnd > mapEnd) {
					seeds.push({mapEnd, seedEnd - mapEnd});
				}
				break;
			}
		}

		seeds_out.push(seed);
	}
	std::swap(seeds, seeds_out);
}

void part2() {
	std::queue<std::pair<uint64_t, uint64_t>> seeds;
	uint64_t smallest = UINT64_MAX;

	// Get the seeds
	auto seedline = std::stringstream(file.front());
	while (!seedline.eof() && seedline.peek() != ':') {
		seedline.get();
	}
	seedline.get();
	// We are after the :
	while (!seedline.eof()) {
		std::pair<uint64_t, uint64_t> c;
		seedline >> c.first >> c.second;
		seeds.push(c);
	}

	file.pop();

	mappart2(seeds);
	mappart2(seeds);
	mappart2(seeds);
	mappart2(seeds);
	mappart2(seeds);
	mappart2(seeds);
	mappart2(seeds);

	while (!seeds.empty()) {
		auto val = seeds.front().first;
		seeds.pop();

		if (val < smallest) {
			smallest = val;
		}
	}

	std::cout << "Day 5 part 2: " << smallest << std::endl;
}

int main() {
	readFile();

	part1();

	readFile();

	part2();

	return 0;
}
