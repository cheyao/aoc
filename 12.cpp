#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <vector>
#include <ranges>
#include <iostream>

using namespace std;
using std::operator""sv;

bool check(string& s, vector<uint64_t>& dst) {
	vector<uint64_t> src = {};
	bool checked = false;
	int times = 0;

	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == '#') {
			++times;
		} else {
			if (times != 0) {
				src.emplace_back(times);
			}

			times = 0;
		}
	}

	if (times != 0) {
		src.emplace_back(times);
	}

	return src == dst;
}

uint64_t recursion(string s, vector<uint64_t>& rows) {
	if (!s.contains('?')) {
		return check(s, rows);
	}

	uint64_t count = 0;

	auto cur = s.find_first_of('?');
	s[cur] = '#';
	count += recursion(s, rows);
	s[cur] = '.';
	count += recursion(s, rows);

	return count;
}

uint64_t posibility(pair<string, vector<uint64_t>>& row) {
	return recursion(row.first, row.second);
}

void part1() {
	ifstream file("12.input.small");
	vector<pair<string, vector<uint64_t>>> springMap;

	while(1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		string a;
		vector<uint64_t> b;
		a = line.substr(0, line.find(' '));
		line.erase(0, line.find(' ') + 1);
		b = line | views::split(","sv) | views::transform([](auto s)->uint64_t{ return atol(s.data()); }) | ranges::to<vector<uint64_t>>();
		springMap.emplace_back((pair<string, vector<uint64_t>>) {a, b});
	}

	// Brute force our way through
	uint64_t sum = 0;
	for (auto springRow : springMap) {
		sum += posibility(springRow);
	}

	cout << "Day 12 part 1: " << sum << endl;
}

void part2() {
	ifstream file("12.input.small");
	vector<pair<string, vector<uint64_t>>> springMap;

	while(1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		string a;
		vector<uint64_t> b;
		a = line.substr(0, line.find(' '));
		line.erase(0, line.find(' ') + 1);
		b = line | views::split(","sv) | views::transform([](auto s)->uint64_t{ return atol(s.data()); }) | ranges::to<vector<uint64_t>>();
		vector<uint64_t> out;
		out.reserve(b.size() * 5); // preallocate memory
		for (int i = 0; i < 5; i++) {
			out.insert(out.end(), b.begin(), b.end());
		}
		springMap.emplace_back((pair<string, vector<uint64_t>>) {a+a+a+a+a, out});
	}

	// Brute force our way through
	uint64_t sum = 0;
	for (auto springRow : springMap) {
		sum += posibility(springRow);
	}

	cout << "Day 12 part 2: " << sum << endl;
}

int main() { 
	auto begin = chrono::high_resolution_clock::now();
	part1(); 
	auto end = chrono::high_resolution_clock::now();
	std::cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << std::endl;

	auto begin2 = chrono::high_resolution_clock::now();

	auto end2 = chrono::high_resolution_clock::now();
	std::cout << chrono::duration_cast<chrono::nanoseconds>(end2-begin2).count() << "ns" << std::endl;
}
