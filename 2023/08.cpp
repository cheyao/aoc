#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#define DEBUG(print)

void part1() {
	std::ifstream file("08.input");
	// AOC-Core v0.1
	std::unordered_map<std::string, std::pair<std::string, std::string>>
	    MEMORY;
	std::string CODE;   // Instructions
	std::string RDX;    // Data address
	std::uint64_t RIP;  // Instruction address
	std::uint64_t RCX;  // Counter of instructions executed

	// Bootloader
	std::getline(file, CODE);  // Load the instructions
	std::getline(file, RDX);   // Purge a line

	while (1) {
		std::getline(file, RDX);

		if (file.eof()) {
			break;
		}

		// Map To MEMORY
		MEMORY[RDX.substr(0, RDX.find(" ="))] =
		    std::pair<std::string, std::string>(
			RDX.substr(RDX.find("(") + 1, 3),
			RDX.substr(RDX.find(", ") + 2, 3));
	}

	// Cpu
	// Initalize registers
	RCX = 0;
	RIP = -1;  // So we start at 0
	RDX = "AAA";
	// Run loop
	while (RDX != "ZZZ") {
		RCX++;	// Increment counter
		RIP++;	// Increment Instruction address
		// Check instruction overflow
		if (RIP == CODE.size()) {
			RIP = 0;
		}

		auto BRANCH = MEMORY[RDX];

		DEBUG("Executing branch condition "
		      << RDX << " with instruction " << CODE[RIP]);
		if (CODE[RIP] == 'L') {
			// LEFT
			RDX = MEMORY[RDX].first;
			DEBUG("Chose left: " << RDX);
		} else {
			RDX = MEMORY[RDX].second;
			DEBUG("Chose right: " << RDX);
		}
	}

	std::cout << "Day 8 part 1: " << RCX << std::endl;
}

#undef DEBUG

#if 0
#define DEBUG(print) std::cerr << "DEBUG: " << print << std::endl
#else
#define DEBUG(print)
#endif

std::string CODE;
std::unordered_map<std::string, std::pair<std::string, std::string>> MEMORY;
// RDX            RCX

int core(std::string START) {
	// AOC Code
	std::string RDX = START;
	std::uint64_t RCX = 0;	// Counter of instructions executed
	std::uint64_t RIP = -1;

	// Run loop
	while (RDX[2] != 'Z') {
		RCX++;	// Increment counter
		RIP++;	// Increment Instruction address
		// Check instruction overflow
		if (RIP == CODE.size()) {
			RIP = 0;
		}

		auto BRANCH = MEMORY[RDX];

		DEBUG("Executing branch condition "
		      << RDX << " with instruction " << CODE[RIP]);
		if (CODE[RIP] == 'L') {
			// LEFT
			RDX = MEMORY[RDX].first;
			DEBUG("Chose left: " << RDX);
		} else if (CODE[RIP] == 'R') {
			RDX = MEMORY[RDX].second;
			DEBUG("Chose right: " << RDX);
		} else {
			DEBUG("ERROR! Invalid instruction");
			exit(1);
		}
	}

	std::cout << "Finished " << START << " with " << RCX << std::endl;
	return RCX;
}

void part2() {
	// AOC Core v1.0: Multi-core
	std::ifstream file("08.input");
	std::string RDX;  // Data address

	std::getline(file, CODE);  // Load the instructions
	std::getline(file, RDX);   // Purge a line

	std::vector<std::string> missions;

	// Bootloader
	while (1) {
		std::getline(file, RDX);

		if (file.eof()) {
			break;
		}

		if (RDX[RDX.find(" =") - 1] == 'A') {
			missions.push_back(RDX.substr(0, RDX.find(" =")));
			DEBUG("Added thread " << missions.back());
		}

		// Map To MEMORY
		MEMORY[RDX.substr(0, RDX.find(" ="))] =
		    std::pair<std::string, std::string>(
			RDX.substr(RDX.find("(") + 1, 3),
			RDX.substr(RDX.find(", ") + 2, 3));
	}

	std::vector<std::uint64_t> results;

	for (auto mission : missions) {
		results.emplace_back(core(mission));
	}

	std::cout << "Part 2: " << std::accumulate(results.begin(), results.end(), static_cast<uint64_t>(1), [] (auto a, auto n) { return std::lcm(a, n); }) << std::endl;
}

int main() {
	part1();
	part2();
}
