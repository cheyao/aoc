// Day 2 of AoC
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int Part1(std::string line) {
	int gameNumber = 0;
	int iter = 0;

	while (!std::isdigit(line[iter]) && iter < line.size()) {
		iter++;
	}

	while (std::isdigit(line[iter]) && iter < line.size()) {
		gameNumber *= 10;
		gameNumber += line[iter] - '0';
		iter++;
	}

	iter += 2;

	std::string games = line.substr(iter, line.size());

	std::vector<std::string> gameList;

	size_t pos;
	while ((pos = games.find("; ")) != std::string::npos) {
		gameList.push_back(games.substr(0, pos));
		games.erase(0, pos + 2);
	}
	gameList.push_back(games);

	for (auto i : gameList) {
		std::vector<std::string> tmp;
		size_t pos;
		while ((pos = i.find(", ")) != std::string::npos) {
			tmp.push_back(i.substr(0, pos));
			i.erase(0, pos + 2);
		}
		tmp.push_back(i);

		for (auto j : tmp) {
			int numCubes = 0;
			int it = 0;
			while (std::isdigit(j[it])) {
				numCubes *= 10;
				numCubes += j[it] - '0';
				it++;
			}

			// 12 red cubes, 13 green cubes, and 14 blue cubes
			if (j[it + 1] == 'r') {
				if (numCubes > 12) return 0;
			} else if (j[it + 1] == 'g') {
				if (numCubes > 13) return 0;
			} else if (j[it + 1] == 'b') {
				if (numCubes > 14) return 0;
			}
		}
	}

	return gameNumber;
}

int Part2(std::string line) {
	int i = 0;
	int tmpNumber = 0;
	int r = 0;
	int g = 0;
	int b = 0;

	// Advance to first number
	while (line[i] != ':') {
		++i;
	}

	for (; i < line.size(); ++i) {
		if (std::isdigit(line[i])) {
			// Add to temp number
			tmpNumber *= 10;
			tmpNumber += line[i] - '0';
		} else {
			++i;  // Skip over the space

			// Set number if bigger
			if (line[i] == 'r' && tmpNumber > r) {
				r = tmpNumber;
			} else if (line[i] == 'g' && tmpNumber > g) {
				g = tmpNumber;
			} else if (line[i] == 'b' && tmpNumber > b) {
				b = tmpNumber;
			}

			tmpNumber = 0;

			// Advance to next number
			while (!std::isdigit(line[i]) && i < line.size()) {
				++i;
			}
			--i;
		}
	}

	return r * g * b;
}

int main(int argc, char** argv) {
	int sum1 = 0;
	int sum2 = 0;
	if (argc == 2) {
		sum1 = Part1(argv[1]);
		sum2 = Part2(argv[1]);
	} else {
		std::ifstream input("02.input", std::ios::in);
		while (1) {
			std::string line;
			std::getline(input, line);

			if (input.eof()) {
				break;
			}

			sum1 += Part1(line);
			sum2 += Part2(line);
		}
	}
	std::cout << "[Part 1] Sum of games possible: " << sum1 << std::endl;
	std::cout << "[Part 2] Sum of power of games: " << sum2 << std::endl;

	return 0;
}
