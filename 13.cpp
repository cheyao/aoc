#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool com(vector<string> ground, int mid) {
	int a = 0;
	int b = ground.size();

	for (int i = min(mid + 1, static_cast<int>(ground.size() - (mid + 1))); i > 0; i--) {
		if (ground[mid + 1 - i] != ground[mid + i]) {
			return false;
		}
	}
	
	return true;
}

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

int count(vector<string> ground) {
	for (int i = 0; i < ground.size() - 1; i++) {
		if (com(ground, i)) {
			return (i + 1) * 100;
		}
	}

	ground = rotate(ground);

	for (int i = 0; i < ground.size() - 1; i++) {
		if (com(ground, i)) {
			return i + 1;
		}
	}

	cout << "Unfound center for matrix: " << endl;
	for (auto l : ground) {
		cout << l << endl;
	}

	cout << "Uncached thing" << endl;
	return 0;
}

int find(vector<string> ground, int last = -1) {
	for (int i = 0; i < ground.size() - 1; i++) {
		if (((i + 1) * 100) != last && com(ground, i)) {
			return (i + 1) * 100;
		}
	}

	ground = rotate(ground);

	for (int i = 0; i < ground.size() - 1; i++) {
		if ((i + 1) != last && com(ground, i)) {
			return i + 1;
		}
	}

	cout << "Uncached thing" << endl;
	return 0;
}

int smudge(vector<string> ground) {
	int last = find(ground);
	if (last == 0) {
		return 0;
	}

	int t = 0;
	for (auto& line : ground) {
		for (auto& c : line) {
			char tmp = c;
			c = (c == '#' ? '.' : '#');
			t = find(ground, last);
			c = tmp;
			if (t != 0 && t != last) {
				goto end;
			}
		}
	}

end: 
	return t;
}

void part2() {
	ifstream file("13.input");

	bool run = true; 
	int sum = 0;
	while (run) {
		vector<string> ground;
		while (1) {
			string line;
			getline(file, line);

			if (file.eof()) {
				run = false;
			}

			if (file.eof() || line.empty()) {
				break;
			}

			ground.push_back(line);
		}

		sum += smudge(ground);
	}

	cout << "Part 2: " << sum << endl;
}

void part1() {
	ifstream file("13.input");
	bool run = true; 
	int sum = 0;
	while (run) {
		vector<string> ground;
		while (1) {
			string line;
			getline(file, line);

			if (file.eof()) {
				run = false;
			}

			if (file.eof() || line.empty()) {
				break;
			}

			ground.push_back(line);
		}

		sum += count(ground);
	}

	cout << "Part 1: " << sum << endl;
}

int main() { 
	// part1();
	part2();
}
