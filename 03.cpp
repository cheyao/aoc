#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int Part1() {
	std::ifstream input("03.input", std::ios::in);
	int sum = 0;

	std::vector<std::string> schematic;

	while (1) {
		std::string line;
		std::getline(input, line);

		if (input.eof()) {
			break;
		}

		schematic.emplace_back(line);
	}

	for (int y = 0; y < schematic.size(); ++y) {
		int number = 0;
		bool part = false;

		for (int x = 0; x < schematic[0].size(); ++x) {
			// Check surrounding symbols
			// Befor & after
			if (x != 0 && schematic[y][x - 1] != '.' &&
			    !std::isdigit(schematic[y][x - 1])) {
				part = true;
			}
			if (x != schematic[0].size() - 1 &&
			    schematic[y][x + 1] != '.' &&
			    !std::isdigit(schematic[y][x + 1])) {
				part = true;
			}

			// Top and bottom
			for (int i = (x == 0 ? 0 : -1);
			     i <= (x == schematic[0].size() - 1 ? 0 : 1); ++i) {
				if (y != 0 && schematic[y - 1][x + i] != '.' &&
				    !std::isdigit(schematic[y - 1][x + i])) {
					part = true;
				}

				if (y != schematic.size() - 1 &&
				    schematic[y + 1][x + i] != '.' &&
				    !std::isdigit(schematic[y + 1][x + i])) {
					part = true;
				}
			}

			if (std::isdigit(schematic[y][x])) {
				number *= 10;
				number += schematic[y][x] - '0';

				if ((x == schematic[0].size() - 1) && part) {
					sum += number;
					if (number != 0)
						std::cout << number
							  << std::endl;
					number = 0;
					part = false;
				}
			} else if (part) {
				sum += number;
				if (number != 0)
					std::cout << number << std::endl;
				number = 0;
				part = false;
			} else {
				number = 0;
			}
		}
	}

	return sum;
}

int main(int argc, char** argv) {
	std::cout << Part1() << std::endl;

	return 0;
}
