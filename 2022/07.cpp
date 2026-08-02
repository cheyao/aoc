#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define INPUT "07.cpp.txt"

std::vector<std::string> getInput(void) {
	std::vector<std::string> lines;

	std::ifstream file(INPUT);
	std::string line;
	while (std::getline(file, line)) {
		lines.emplace_back(line);
	}
	file.close();

	return lines;
}

typedef struct file {
	bool dir;
	uint64_t size;
	std::vector<struct file> children;
} file;
std::vector<file*> stack;
std::vector<uint64_t> sizes;

uint64_t sum_size(file* f) {
	uint64_t s = 0;

	for (int i = 0; i < f->children.size(); ++i) {
		if (f->children[i].dir) {
			// Dir
			s += sum_size(&f->children[i]);
			sizes.emplace_back(f->children[i].size);
		}
	}

	return s + ((f->size <= 100000) ? f->size : 0);
}

void part1() {
	const auto input = getInput();

	file root{.dir = 1, .size = 0};
	stack.emplace_back(&root);

	// Lets parse the tree!
	// Line 1 is always cd /
	for (int i = 1; i < input.size(); ++i) {
		// Here we should always be on a cmd line
		assert(input[i][0] == '$');

		// cd up
		if (input[i][2] == 'c' && input[i][5] == '.') {
			stack.pop_back();
			continue;
		}

		// We go into a new directory
		if (input[i][2] == 'c') {
			stack.back()->children.emplace_back(file{.dir = 1, .size = 0});
			stack.emplace_back(&stack.back()->children.back());
			continue;
		}

		// We add files!
		assert(input[i][2] == 'l');
		while (input[++i][0] != '$') {
			if (input[i][0] == 'd') {
				continue;
			}

			// This is a new file
			uint64_t size = std::stoull(input[i]);
			// stack.back()->children.emplace_back(file{.dir = 0, .size = size});
			// Propagate?
			for (auto f : stack) {
				f->size += size;
			}

			if (i == input.size() - 1) {
				++i;
				break;
			}
		}
		--i;
	}

	std::cout << "Part 1: " << sum_size(&root) << std::endl;
	sizes.emplace_back(root.size);
}

void part2() {
	std::ranges::sort(sizes);

	const uint64_t threashold = 30000000 - (70000000 - sizes.back());
	for (const auto size : sizes) {
		if (size >= threashold) {
			std::cout << "Part 2: " << size << '\n';
			break;
		}
	}
}

int main() {
	part1();
	part2();
}
