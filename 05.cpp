#include <sys/types.h>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::queue<std::string> file;

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

void map_stuff(std::vector<uint64_t>& input) {
	std::vector<uint64_t> output;
	std::vector<conv> convmap;

	file.pop();
	file.pop();

	// Seed to soil map
	while (file.front() != "") {
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
idk: null();
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

int main() {
	readFile();

	part1();

	readFile();

	return 0;
}
