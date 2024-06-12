#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <ranges>
#include <iostream>

using namespace std;
using std::operator""sv;

map<pair<string, vector<uint64_t>>, uint64_t> cache;

// Call: ???.### 1,1,3--\
//    1: .??.### 1,1,3  #??.### 1,1,3
//    2:                 ??.### 1,3
uint64_t recursion(string s, vector<uint64_t> rows, uint64_t ammount = 0) {
	if (s.empty()) {
		if (!rows.empty() && ammount == rows.front()) {
			rows.erase(rows.begin());
		}

		if (rows.empty()) {
			return 1;
		} else {
			return 0;
		}
	}

	if (cache.contains({s, rows})) {
		return cache[{s, rows}];
	} 

	// Isn't in cache ^n^
	if (s[0] == '?') {
		// Can be # and .
		uint64_t sum = 0;
		s[0] = '#';
		sum += recursion(s, rows, ammount);
		s[0] = '.';
		sum += recursion(s, rows, ammount);
		cache[{s, rows}] = sum;
		return sum;
	} 

	if (s[0] == '#') {
		ammount++;
		if (rows.empty() || ammount > rows.front()) {
			return 0;
		}

		s = s.substr(1);
		return recursion(s, rows, ammount);
	}

	if (s[0] == '.') {
		if (ammount != 0) {
			if (ammount == rows.front()) {
				rows.erase(rows.begin());
			} else {
				return 0;
			}
		} 
		
		ammount = 0;
		s = s.substr(1);
		return recursion(s, rows, ammount);
	}

	exit(1); // This is not supposed to happed
	return 0;
}

uint64_t posibility(pair<string, vector<uint64_t>> row) {
	return recursion(row.first, row.second);
}

void part1() {
	ifstream file("12.input");
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
		cache.clear();
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
		springMap.emplace_back((pair<string, vector<uint64_t>>) {a+"?"+a+"?"+a+"?"+a+"?"+a, out});
	}

	// Brute force our way through
	uint64_t sum = 0;
	for (auto springRow : springMap) {
		cache.clear();
		// sum += posibility(springRow);
	}

	cout << "Day 12 part 2: " << sum << endl;
}

void test() {
	if (posibility((pair<string, vector<uint64_t>>) {".#?", {1}}) == 1) {
		cout << " Passed" << endl;
	} else {
		cout << " Failed" << endl;
	}
}

int main() { 
	// test();

	auto begin = chrono::high_resolution_clock::now();
	part1(); 
	auto end = chrono::high_resolution_clock::now();
	std::cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << std::endl;

	auto begin2 = chrono::high_resolution_clock::now();
	// part2();
	auto end2 = chrono::high_resolution_clock::now();
	std::cout << chrono::duration_cast<chrono::nanoseconds>(end2-begin2).count() << "ns" << std::endl;
}
