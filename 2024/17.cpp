#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;
	string buf;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

void part1() {
	uint64_t sum = 0;

	auto input = get();

	uint64_t A = stoull((input[0] | views::split(' ') | ranges::to<vector<string>>())[2]);
	uint64_t B = stoull((input[1] | views::split(' ') | ranges::to<vector<string>>())[2]);
	uint64_t C = stoull((input[2] | views::split(' ') | ranges::to<vector<string>>())[2]);
	uint64_t IP = 0;

	const vector<uint64_t> program =
		(input[4] | views::split(' ') | ranges::to<vector<string>>())[1] | views::split(',') |
		views::transform([](const auto& s) { return atoll(s.data()); }) |
		ranges::to<remove_cv_t<decltype(program)>>();

	vector<uint64_t> out;
	while (IP < program.size()) {
		const auto inst = program.at(IP);
		const auto op = program.at(IP + 1);
		const auto combo = op == 4 ? A : op == 5 ? B : op == 6 ? C : op;

		switch (inst) {
			case 0:
				A = A / (1 << combo);
				break;
			case 1:
				B = B ^ op;
				break;
			case 2:
				B = combo % 8;
				break;
			case 3:
				if (A != 0) {
					IP = op;
					continue;
				}
				break;
			case 4:
				B = B ^ C;
				break;
			case 5:
				out.emplace_back(combo % 8);
				break;
			case 6:
				B = A / (1 << combo);
				break;
			case 7:
				C = A / (1 << combo);
				break;
			default:
				cout << "ERR!";
				break;
		}
		++IP;
		++IP;
	}

	cout << "Part 1:";
	std::ranges::copy(out, std::ostream_iterator<int>(std::cout, ","));
	cout << '\n';
}

// Program: 2,4,1,3,7,5,0,3,4,1,1,5,5,5,3,0

void calc(uint64_t A, uint64_t B, uint64_t C, const vector<uint64_t>& program) {
	uint64_t IP = 0;
	size_t i = 0;

	while (IP < program.size()) {
		const auto inst = program.at(IP);
		const auto op = program.at(IP + 1);
		const auto combo = op == 4 ? A : op == 5 ? B : op == 6 ? C : op;

		switch (inst) {
			case 0:
				A = A / (1 << combo);
				break;
			case 1:
				B = B ^ op;
				break;
			case 2:
				B = combo % 8;
				break;
			case 3:
				if (A != 0) {
					IP = op;
					continue;
				}
				break;
			case 4:
				B = B ^ C;
				break;
			case 5:
				return;

				break;
			case 6:
				B = A / (1 << combo);
				break;
			case 7:
				C = A / (1 << combo);
				break;
			default:
				break;
		}
		IP += 2;
	}
}

uint64_t step(uint64_t A) {
	uint64_t B = A % 8;		   // A < 7 // 0b111
	B = B ^ 3;				   // B ^ 0b11
	uint64_t C = A / (1 << B); // C Depends on bit 3 of a (4): 128 or 8
	A = A / 8;				   // 8
	B = B ^ C;
	B = B ^ 5; // 0b101 B depends on bit 2 of C and B
	return B % 8;
}

vector<uint64_t> program;
vector<uint64_t> As;

// Backtrack
void find(uint64_t A, uint64_t col = 0) {
	if (step(A) != program[program.size() - (col + 1)]) {
		return;
	}

	if (col == program.size() - 1) {
		As.emplace_back(A);
	} else {
		for (size_t B = 0; B < 8; ++B) {
			find(A * 8 + B, col + 1);
		}
	}
}

void part2() {
	auto input = get();

	program = (input[4] | views::split(' ') | ranges::to<vector<string>>())[1] | views::split(',') |
			  views::transform([](const auto& s) { return atoll(s.data()); }) |
			  ranges::to<remove_cv_t<decltype(program)>>();

	uint64_t A = 1;
	uint64_t B = stoull((input[1] | views::split(' ') | ranges::to<vector<string>>())[2]);
	uint64_t C = stoull((input[2] | views::split(' ') | ranges::to<vector<string>>())[2]);

	for (auto i = 0; i < 8; ++i) {
		find(i);
	}

	cout << "Part 2:" << As[0] << endl;
}

int main(int argc, char** argv) {
	part1();
	part2();
}
