#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> file;

void readFile() {
	file.clear();
	std::ifstream input("03.input");
	std::string line;

	if (!input.is_open()) {
		std::cerr << "Failed to open input" << std::endl;
		exit(1);
	}

	while (!input.eof()) {
		std::getline(input, line);
		file.emplace_back(line);
	}

	// There will be a blank line at the end
	file.pop_back();
}

void part1() {
	const int line_size = file[0].size();
	std::unordered_map<int, int*> number_map;

	int* tmp = new int;
	*tmp = 0;
	for (int y = 0; y < file.size(); ++y) {
		for (int x = 0; x < line_size; ++x) {
			char c = file[y][x];

			if (std::isdigit(c)) {
				*tmp *= 10;
				*tmp += c - '0';
				number_map[y * line_size + x] = tmp;
			}

			if (!std::isdigit(c) && *tmp != 0) {
				tmp = new int;
				*tmp = 0;
			}
		}

		if (*tmp != 0) {
			tmp = new int;
			*tmp = 0;
		}
	}
	delete tmp;

	std::vector<int*> found;
	int sum = 0;
	// Now check for gears
	for (int y = 0; y < file.size(); ++y) {
		for (int x = 0; x < line_size; ++x) {
			char c = file[y][x];

			if (std::isdigit(c) || c == '.') {
				// Ins't the symbol
				continue;
			}

			std::vector<int> checks;

			// Ahh we need to map the numbers to check
			if (y != 0 && x != 0) 
				checks.emplace_back((y - 1) * line_size + x - 1);
			if (y != file.size() - 1 && x != line_size - 1) 
				checks.emplace_back((y + 1) * line_size + x + 1);
			if (x != line_size - 1) 
				checks.emplace_back((y + 0) * line_size + x + 1);
			if (y != 0) 
				checks.emplace_back((y - 1) * line_size + x + 0);
			if (y != 0 && x != line_size - 1) 
				checks.emplace_back((y - 1) * line_size + x + 1);
			if (x != 0) 
				checks.emplace_back((y - 0) * line_size + x - 1);
			if (y != file.size() - 1 && x != 0) 
				checks.emplace_back((y + 1) * line_size + x - 1);
			if (y != file.size() - 1) 
				checks.emplace_back((y + 1) * line_size + x - 0);

			// Check all boxes around
			for (auto i : checks) {
				if (number_map[i] == nullptr ||
				    std::find(found.begin(), found.end(),
					      number_map[i]) != found.end()) {
					// Aleady added or isn't valid
					continue;
				}

				sum += *number_map[i];
				found.push_back(number_map[i]);
			}
		}
	}

	std::cout << "The sum of part 1 is: " << sum << std::endl;

	// be nice and free memory UwU
	std::vector<int*> freed;
	for (auto& [key, var] : number_map) {
		if (std::find(freed.begin(), freed.end(), var) != freed.end()) {
			continue;
		}

		freed.push_back(var);
		delete var;
	}
}

void part2() {
	const int line_size = file[0].size();
	std::unordered_map<int, int*> number_map;

	int* tmp = new int;
	*tmp = 0;
	for (int y = 0; y < file.size(); ++y) {
		for (int x = 0; x < line_size; ++x) {
			char c = file[y][x];

			if (std::isdigit(c)) {
				*tmp *= 10;
				*tmp += c - '0';
				number_map[y * line_size + x] = tmp;
			}

			if (!std::isdigit(c) && *tmp != 0) {
				tmp = new int;
				*tmp = 0;
			}
		}

		if (*tmp != 0) {
			tmp = new int;
			*tmp = 0;
		}
	}
	delete tmp;

	int sum = 0;
	// Now check for gears
	for (int y = 0; y < file.size(); ++y) {
		for (int x = 0; x < line_size; ++x) {
			std::vector<int*> found;
			char c = file[y][x];

			if (c != '*') {
				// Isn't a symbol
				continue;
			}

			std::vector<int> checks;

			// Ahh we need to map the numbers to check
			if (y != 0 && x != 0) 
				checks.emplace_back((y - 1) * line_size + x - 1);
			if (y != file.size() - 1 && x != line_size - 1) 
				checks.emplace_back((y + 1) * line_size + x + 1);
			if (x != line_size - 1) 
				checks.emplace_back((y + 0) * line_size + x + 1);
			if (y != 0) 
				checks.emplace_back((y - 1) * line_size + x + 0);
			if (y != 0 && x != line_size - 1) 
				checks.emplace_back((y - 1) * line_size + x + 1);
			if (x != 0) 
				checks.emplace_back((y - 0) * line_size + x - 1);
			if (y != file.size() - 1 && x != 0) 
				checks.emplace_back((y + 1) * line_size + x - 1);
			if (y != file.size() - 1) 
				checks.emplace_back((y + 1) * line_size + x - 0);

			// Check all boxes around
			for (auto i : checks) {
				if (number_map[i] == nullptr ||
				    std::find(found.begin(), found.end(),
					      number_map[i]) != found.end()) {
					// Aleady added or isn't valid
					continue;
				}

				found.push_back(number_map[i]);
			}

			// Check if it's two
			if (found.size() == 2) {
				sum = sum + (*found[0]) * (*found[1]);
			}
		}
	}

	std::cout << "The sum of part 2 is: " << sum << std::endl;

	// be nice and free memory UwU
	std::vector<int*> freed;
	for (auto& [key, var] : number_map) {
		if (std::find(freed.begin(), freed.end(), var) != freed.end()) {
			continue;
		}

		freed.push_back(var);
		delete var;
	}
}

int main() {
	std::cout << "Day 3:" << std::endl;

	readFile();

	part1();

	readFile();

	part2();

	return 0;
}
