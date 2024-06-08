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

	return;
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

int main() {
	getInput();
	part1();

	return 0;
}

