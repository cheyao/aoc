#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;
	string buf;

	while (getline(file, line)) {
		if (line.empty()) {
			out.emplace_back(std::move(buf));
			buf.clear();
		} else {
			buf += line;
		}
	}

	return out;
}

void part1() {
	uint64_t sum = 0;

	vector<vector<double>> data;
	for (const auto& line : get()) {
		stringstream ss(line);

		vector<double> c;
		uint64_t d = 0;
		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
		ss >> d;
		c.emplace_back(d);

		data.emplace_back(c);
	}

	for (const auto& line : data) {
		double a = (line[4] * line[3] - line[5] * line[2]) / (line[0] * line[3] - line[1] * line[2]);
		double b = (line[5] - a * line[1]) / line[3];

		uint64_t ai = a;
		uint64_t bi = b;

		if (line[4] == ai * line[0] + bi * line[2] && line[5] == ai * line[1] + bi * line[3]) {
			sum += a * 3 + b;
		}
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	vector<vector<double>> data;
	for (const auto& line : get()) {
		stringstream ss(line);

		vector<double> c;
		uint64_t d = 0;
		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '+');
		ss >> d;
		c.emplace_back(d);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
		ss >> d;
		c.emplace_back(d + 10000000000000);

		ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
		ss >> d;
		c.emplace_back(d + 10000000000000);

		data.emplace_back(c);
	}

	for (const auto& line : data) {
		double a = (line[4] * line[3] - line[5] * line[2]) / (line[0] * line[3] - line[1] * line[2]);
		double b = (line[5] - a * line[1]) / line[3];

		uint64_t ai = a;
		uint64_t bi = b;

		if (line[4] == ai * line[0] + bi * line[2] && line[5] == ai * line[1] + bi * line[3]) {
			sum += a * 3 + b;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
