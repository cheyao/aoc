// Day 1
// Get a calibration number by combining the first digit and the last digit (in
// that order) to form a single two-digit number. Print the sum of all numbers
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
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

	std::cout << "The sum of all of the calibration values is: " << sum
		  << std::endl;
	return 0;
}
