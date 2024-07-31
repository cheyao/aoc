#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<string> rotate(vector<string>& matrix) {
	vector<string> out;

	for (auto& i : matrix[0]) {
		out.emplace_back("");
	}

	for (auto line = matrix.rbegin(); line != matrix.rend(); ++line) {
		for (int i = 0; i < out.size(); i++) {
			out[i] += (*line)[i];
		}
	}

	return out;
}

int count(vector<string> platform) {
	int sum = 0;
	for (int i = 0; i < platform.size(); ++i) {
		sum += count(platform[i].begin(), platform[i].end(), 'O') * (platform.size() - i);
	}
	return sum;
}

void tilt(vector<string>& platform) {
	for (int i = 0; i < platform[0].size(); ++i) {
		int line = 0;
		while (1) {
			if (line == platform.size()) {
				break;
			}

			if (line > 0 && platform[line][i] == 'O' && platform[line - 1][i] == '.') {
				platform[line][i] = '.';
				platform[line - 1][i] = 'O';

				line--;
			} else {
				line++;
			}
		}
	}
}

map<vector<string>, vector<string>> cache;

void cycle(vector<string>& platform) {
	if (cache.contains(platform)) {
		platform = cache[platform];
	}

	vector<string> old = platform;
	tilt(platform);
	platform = rotate(platform);
	tilt(platform);
	platform = rotate(platform);
	tilt(platform);
	platform = rotate(platform);
	tilt(platform);
	platform = rotate(platform);
	cache[old] = platform;
}

void part2() {
	ifstream file("14.input");
	vector<string> platform;

	while (1) {
		string s;
		getline(file, s);
		if (file.eof()) {
			break;
		}
		platform.emplace_back(s);
	}


	for (uint64_t i = 0; i < 1000; ++i) {
		cycle(platform);
	}

	int load = count(platform);

	cout << "Part 1: " << load << endl;
}

void part1() {
	ifstream file("14.input");
	vector<string> platform;

	while (1) {
		string s;
		getline(file, s);
		if (file.eof()) {
			break;
		}
		platform.emplace_back(s);
	}

	tilt(platform);

	int load = count(platform);

	cout << "Part 1: " << load << endl;
}

int main() {
	part1();
	part2();
}
