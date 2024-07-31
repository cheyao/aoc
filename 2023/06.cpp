#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

std::queue<std::pair<uint64_t, uint64_t>> input;

void getInput() {
	std::ifstream file("06.input");
	if (!file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		exit(1);
	}

	std::string time;
	std::string distance;

	std::getline(file, time);
	std::getline(file, distance);

	time.erase(0, time.find(':') + 1);
	distance.erase(0, distance.find(':') + 1);

	auto timeStream = std::stringstream(time);
	auto distanceStream = std::stringstream(distance);

	while (!timeStream.eof() && !distanceStream.eof()) {
		uint64_t t, d;

		timeStream >> t;
		distanceStream >> d;

		input.push({t, d});
	}
}


void part1() {
	uint64_t solutionProduct = 1;

	while (!input.empty()) {
		uint64_t solutions = 0;
		uint64_t time = input.front().first;
		uint64_t distance = input.front().second;

		input.pop();

		for (uint64_t i = 0; i < time; i++) {
			uint64_t speed = i;
			uint64_t timeLeft = time - i;
			uint64_t distanceTraveled = speed * timeLeft;

			if (distanceTraveled > distance) {
				solutions++;
			}
		}

		solutionProduct *= solutions;
	}

	std::cout << "There is " << solutionProduct << " solutions" << std::endl;
}

void part2() {
	std::ifstream file("06.input");
	if (!file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		exit(1);
	}

	std::string timeString;
	std::string distanceString;

	std::getline(file, timeString);
	std::getline(file, distanceString);

	timeString.erase(0, timeString.find(':') + 1);
	distanceString.erase(0, distanceString.find(':') + 1);

	timeString.erase(std::remove_if(timeString.begin(), timeString.end(), isspace), timeString.end());
	distanceString.erase(std::remove_if(distanceString.begin(), distanceString.end(), isspace), distanceString.end());

	auto time = std::stol(timeString);
	auto distance = std::stol(distanceString);

	uint64_t min;
	uint64_t max;

	for (int i = 0; i < distance; i++) {
		if (i * (time - i) > distance) {
			min = i;

			break;
		}
	}

	for (int i = distance; i > 0; i--) {
		if (i * (time - i) > distance) {
			max = i;

			break;
		}
	}

	std::cout << "Part 2: " << max - min + 1 << std::endl;
}

int main() {
	getInput();
	part1();
	part2();

	return 0;
}

