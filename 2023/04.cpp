#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
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
				}

				current_number = 0;
			}

			if (i != line.size() && std::isdigit(line[i])) {
				current_number *= 10;
				current_number += line[i] - '0';
			}

			++i;
		}

		total_points += card_points;
	}

	std::cout << "Day 4 part 1: " << total_points << std::endl;
}

void part2() {
	std::map<int, int> cards;

	for (int i = 0; i < file.size(); i++) {
		cards[i] = 1;
	}

	int l = 0;
	for (auto line : file) {
		int i = line.find(':') + 1;  // Skip over card number
		int current_number = 0;
		int card_points = 0;
		int wins = 0;
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
					// Winning number
					wins++;
					cards[l + wins] += 1 * cards[l];
				}

				current_number = 0;
			}

			if (i != line.size() && std::isdigit(line[i])) {
				current_number *= 10;
				current_number += line[i] - '0';
			}

			++i;
		}

		++l;
	}

	int sum = 0;
	for (const auto& [num, count] : cards) {
		sum += count;
	}
	std::cout << "Day 4 part 2: " << sum << std::endl;
}

int main() {
	readFile();

	part1();

	readFile();

	part2();

	return 0;
}
