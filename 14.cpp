#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// O
// O
// .
// O
// .
// O
// .
// .
// #
// #
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

	/*
	for (auto l : platform) {
		cout << l << endl;
	}
	*/

	int load = count(platform);

	cout << "Part 1: " << load << endl;
}

int main() {
	part1();
}
