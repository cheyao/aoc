// Day 1
// Get a calibration number by combining the first digit and the last digit (in
// that order) to form a single two-digit number. Print the sum of all numbers
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

void part1() {
	int sum = 0;
	std::ifstream input("01.input", std::ios::in);

	if (!input.is_open()) {
		std::cerr << "Failed to open 01.input";
	}

	while (!input.eof()) {
		std::string line;
		std::getline(input, line);

		// Get first number
		for (int i = 0; i < line.length(); ++i) {
			if (std::isdigit(line[i])) {
				sum += (line[i] - '0') * 10;
				break;
			}
		}

		// Get second number
		for (int i = line.size() - 1; i >= 0; --i) {
			if (std::isdigit(line[i])) {
				sum += line[i] - '0';
				break;
			}
		}
	}

	input.close();

	std::cout << "[Part 1] The sum of all of the calibration values is: " << sum << std::endl;
}

const std::string numbers[] = {"", "one", "two", "three", "four",
			                   "five", "six", "seven", "eight", "nine"};

int getDigit(std::string line) {
	int sum = 0;

	// Get first number
	// Letter
	int fPos = line.length();
	int fNumber = 0;
	for (int i = 1; i <= 9; ++i) {
		int pos = line.find(numbers[i]);
		if (pos != std::string::npos && pos < fPos) {
			fPos = pos;
			fNumber = i;
		}
	}
	// Number
	for (int i = 0; i < fPos; ++i) {
		if (std::isdigit(line[i])) {
			fNumber = (line[i] - '0');
			break;
		}
	}
	sum += fNumber * 10;

	// Get second number
	// Letter
	int lPos = 0;
	int lNumber = 0;
	for (int i = 1; i <= 9; ++i) {
		int pos = 0;
		while (pos != std::string::npos) {
			pos = line.find(numbers[i], pos + 1);
			if (pos != std::string::npos && pos > lPos) {
				lPos = pos;
				lNumber = i;
			}
		}
	}
	// Number
	for (int i = line.length(); i >= lPos; --i) {
		if (std::isdigit(line[i])) {
			lNumber = (line[i] - '0');
			break;
		}
	}
	sum += lNumber;

	return sum;
}
void part2() {
	int sum = 0;
	std::ifstream input("01.input", std::ios::in);

	if (!input.is_open()) {
		std::cerr << "Failed to open 01.input";
	}

	while (!input.eof()) {
		std::string line;
		std::getline(input, line);

		if (input.eof()) break;

		sum += getDigit(line);
	}

	input.close();

	std::cout << "[Part 2] The sum of all of the calibration values is: " << sum << std::endl;
}

int main(int argc, char** argv) {
	part1();
	part2();

	return 0;
}

