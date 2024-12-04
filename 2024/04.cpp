#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;

	while (getline(file, line)) {
		if (!line.empty()) {
			out.emplace_back(std::move(line));
		}
	}

	return out;
}

int c(const std::string& s, const std::string& target) {
	int occurrences = 0;
	std::string::size_type pos = 0;
	while ((pos = s.find(target, pos)) != std::string::npos) {
		++occurrences;
		++pos;
	}

	return occurrences;
}

void part1() {
	uint64_t sum = 0;

	const auto lines = get();
	std::vector<string> cols(lines[0].size(), string());
	std::vector<string> diags(lines[0].size() + lines.size() - 1, string());
	std::vector<string> ldiags(lines[0].size() + lines.size() - 1, string());
	size_t off = 0;
	for (const auto& line : lines) {
		sum += c(line, "XMAS");
		sum += c(line, "SAMX");

		for (size_t i = 0; i < line.size(); ++i) {
			cols[i] += line[i];
			diags[i + off] += line[line.size() - i - 1];
			ldiags[i + off] += line[i];
		}
		++off;
	}

	for (const auto& line : cols) {
		sum += c(line, "XMAS");
		sum += c(line, "SAMX");
	}

	for (const auto& line : diags) {
		sum += c(line, "XMAS");
		sum += c(line, "SAMX");
	}

	for (const auto& line : ldiags) {
		sum += c(line, "XMAS");
		sum += c(line, "SAMX");
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	const auto lines = get();
	for (size_t y = 1; y < lines.size() - 1; ++y) {
		for (size_t x = 1; x < lines[0].size() - 1; ++x) {
			if (lines[y][x] == 'A') {
				if ((lines[y - 1][x - 1] == 'M')
				 && (lines[y + 1][x - 1] == 'M')
				 && (lines[y - 1][x + 1] == 'S')
				 && (lines[y + 1][x + 1] == 'S')) {
					++sum;
				}

				if ((lines[y - 1][x - 1] == 'M')
				 && (lines[y + 1][x - 1] == 'S')
				 && (lines[y - 1][x + 1] == 'M')
				 && (lines[y + 1][x + 1] == 'S')) {
					++sum;
				}

				if ((lines[y - 1][x - 1] == 'S')
				 && (lines[y + 1][x - 1] == 'S')
				 && (lines[y - 1][x + 1] == 'M')
				 && (lines[y + 1][x + 1] == 'M')) {
					++sum;
				}

				if ((lines[y - 1][x - 1] == 'S')
				 && (lines[y + 1][x - 1] == 'M')
				 && (lines[y - 1][x + 1] == 'S')
				 && (lines[y + 1][x + 1] == 'M')) {
					++sum;
				}
			}
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
