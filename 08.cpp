#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>

#if 0
#define DEBUG(print) std::cout << "DEBUG: " << print << std::endl
#else
#define DEBUG(print)
#endif

void part1() {
	std::ifstream file("08.input");
	// AOC-Core v0.1
	std::unordered_map<std::string, std::pair<std::string, std::string>> MEMORY;
	std::string   CODE; // Instructions
	std::string   RDX;  // Data address
	std::uint64_t RIP;  // Instruction address
	std::uint64_t RCX;  // Counter of instructions executed

	// Bootloader
	std::getline(file, CODE); // Load the instructions
	std::getline(file, RDX); // Purge a line
	
	while (1) {
		std::getline(file, RDX);

		if (file.eof()) {
			break;
		}

		// Map To MEMORY
		MEMORY[RDX.substr(0, RDX.find(" ="))] = std::pair<std::string, std::string>(RDX.substr(RDX.find("(") + 1, 3), RDX.substr(RDX.find(", ") + 2, 3));
	}

	// Cpu
	// Initalize registers
	RCX = 0;
	RIP = -1; // So we start at 0 
	RDX = "AAA";
	// Run loop
	while (RDX != "ZZZ") {
		RCX++; // Increment counter
		RIP++; // Increment Instruction address
		// Check instruction overflow
		if (RIP == CODE.size()) {
			RIP = 0;
		}

		auto BRANCH = MEMORY[RDX];

		DEBUG("Executing branch condition " << RDX << " with instruction " << CODE[RIP]);
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

int main() {
	part1();
}
