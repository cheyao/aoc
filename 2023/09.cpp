#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <vector>

using namespace std;
using std::operator""sv;

int64_t next(vector<int64_t> numbers) {
	vector<vector<int64_t>> values;
	values.emplace_back(numbers);
	int64_t out = 0;
	
	while (!ranges::all_of(values.back(), [](auto i){return i == 0;})) {
		auto diff = values.back();
		out += diff.back();
		values.push_back(vector<int64_t>());
		for (auto i = 0; i < diff.size() - 1; i++) {
			values.back().push_back(diff[i+1] - diff[i]);
		}
	}

	return out;
}

void part1() {
	ifstream file("09.input");
	string line;
	int64_t sum = 0;
	while (1) {
		getline(file, line);
		if (file.eof()) {
			break;
		}
		
		sum += next(line | std::views::split( " "sv) | views::transform([](auto str)->int64_t{return atol(str.data());}) | ranges::to<vector<int64_t>>());
	}

	cout << "Part1: " << sum << endl;
}

void part2() {
	ifstream file("09.input");
	string line;
	int64_t sum = 0;
	while (1) {
		getline(file, line);
		if (file.eof()) {
			break;
		}
		auto i = line | std::views::split( " "sv) | views::transform([](auto str)->int64_t{return atol(str.data());}) | ranges::to<vector<int64_t>>();
		sum += next(vector(i.rbegin(), i.rend()));
	}

	cout << "Part2: " << sum << endl;
}

int main() {
	auto t1 = chrono::high_resolution_clock::now();
	part1();
	auto t2 = chrono::high_resolution_clock::now();
	auto ms =
	    (double)duration_cast<chrono::microseconds>(t2 - t1).count() / 1000;
	cout << ms << endl;
	part2();
}
