#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> file;

void readFile() {
	file.clear();
	std::ifstream input("04.input");
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
	int total_points = 0;

	for (auto line : file) {
		int i = line.find(':') + 1;  // Skip over card number
		int current_number = 0;
		int card_points = 0;
		std::vector<int> winning_numbers;

		// Get winning numbers
		while (line[i] != '|') {
			if (line[i] == ' ' && current_number != 0) {
				winning_numbers.emplace_back(current_number);
				current_number = 0;
			}

			if (std::isdigit(line[i])) {
				current_number *= 10;
				current_number += line[i] - '0';
			}

			++i;
		}

		++i;
		current_number = 0;

		while (i <= line.size()) {
			if (i == line.size() ||
			    (line[i] == ' ' && current_number != 0)) {
				if (std::find(winning_numbers.begin(),
					      winning_numbers.end(),
					      current_number) !=
				    winning_numbers.end()) {
					// Digit is winning number
					if (card_points == 0) {
						card_points = 1;
					} else {
						card_points *= 2;
					}

					current_number = 0;
				} else {
					current_number = 0;
				}
			}

			if (i != line.size() && std::isdigit(line[i])) {
				current_number *= 10;
				current_number += line[i] - '0';
			}

			++i;
		}

		total_points += card_points;
	}

	std::cout << total_points << std::endl;
}

int main() {
	readFile();

	part1();

	return 0;
}
