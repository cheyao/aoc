#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <ratio>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

using namespace std;

static vector<string> get() {
	static vector<string> out;

	if (out.empty()) {
		ifstream file(__FILE__ ".txt");
		string line;

		while (getline(file, line)) {
			if (!line.empty()) {
				out.emplace_back(std::move(line));
			}
		}
	}

	return out;
}

auto part1() {
	uint64_t sum = 0;

	vector<vector<uint64_t>> columns;
	vector<char> operators;
	for (const auto& l : get()) {
		if (ispunct(l[0])) {
			for (const auto& c : l) {
				if (isspace(c)) {
					continue;
				}

				operators.push_back(c);
			}

			break;
		}

		istringstream ss(l);
		vector<uint64_t> data;

		for (uint64_t number; ss >> number;) {
			data.emplace_back(number);
		}

		columns.emplace_back(std::move(data));
	}

	for (uint64_t i = 0; i < operators.size(); ++i) {
		uint64_t s = columns[0][i];
		for (uint64_t j = 1; j < columns.size(); ++j) {
			switch (operators[i]) {
				case '+':
					s += columns[j][i];
					break;
				case '-':
					s -= columns[j][i];
					break;
				case '*':
					s *= columns[j][i];
					break;
			}
		}
		sum += s;
	}

	return sum;
}

auto part2() {
	uint64_t sum = 0;

	const auto data = get();
	vector<string> s(data[0].size(), "");
	vector<char> operators;
	for (const auto& l : data) {
		if (ispunct(l[0])) {
			for (const auto& c : l) {
				if (isspace(c)) {
					continue;
				}

				operators.push_back(c);
			}

			break;
		}

		for (uint64_t i = 0; i < l.size(); ++i) {
			s[i] += l[i];
		}
	}

	uint64_t i = 0;
	uint64_t colsum = 0;
	for (const auto& n : s) {
		const uint64_t digit = atoll(n.data());

		if (digit == 0) {
			++i;
			sum += colsum;
            colsum = 0;
			continue;
		}

        if (colsum == 0) {
            colsum = digit;
            continue;
        }

		switch (operators[i]) {
			case '+':
				colsum += digit;
				break;
			case '-':
				colsum -= digit;
				break;
			case '*':
				colsum *= digit;
				break;
		}
	}
	sum += colsum;

	return sum;
}

int main() {
	auto tries = 250;

	double time = 0;
	uint64_t result = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part1();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 1: " << result << endl;
	std::cout << time / (tries - 3) << "us\n";

	time = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part2();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= 3) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - 3) << "us\n";
}
