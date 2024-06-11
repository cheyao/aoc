#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void part1() {
	ifstream file("11.input");
	vector<string> galaxy;

	while (1) {
		string s;
		getline(file, s);
		if (file.eof()) {
			break;
		}
		galaxy.push_back(s);
	}

	// Get horizontal pairs
	vector<decltype(galaxy.begin())> toAdd;
	for (auto iter = galaxy.begin(); iter != galaxy.end(); iter++) {
		if (find(iter->begin(), iter->end(), '#') == iter->end()) {
			toAdd.emplace_back(iter);
		}
	}
	reverse(toAdd.begin(), toAdd.end());
	for (auto iter : toAdd) {
		galaxy.insert(iter, *iter);
	}

	// Vertical pairs
	for (int i = 0; i < galaxy[0].size(); i++) {
		bool present = false;
		for (auto line : galaxy) {
			if (line[i] == '#') {
				present = true;
				break;
			}
		}

		if (!present) {
			for (auto& line : galaxy) {
				line.insert(i, ".");
			}

			i++;
		}
	}

	// Calculate paths
	int sum = 0;
	for (int y = 0; y < galaxy.size(); y++) {
		for (int x = 0; x < galaxy[0].size(); x++) {
			if (galaxy[y][x] != '#') {
				continue;
			}

			// We need to match all pairs
			int yn;
			int xn;

			if (x == galaxy[0].size()) {
				yn = y + 1;
				xn = 0;
			} else {
				yn = y;
				xn = x + 1;
			}

			for (; yn < galaxy.size(); yn++) {
				for (; xn < galaxy[0].size(); xn++) {
					if (galaxy[yn][xn] == '#') {
						// printf("Found match: (%d, %d) and (%d, %d) with lenth %d\n", x, y, xn, yn, abs(xn - x) + (yn - y));
						sum += abs(xn - x) + (yn - y);
					}
				}

				xn = 0;
			}
		}
	}

	for (auto line : galaxy) {
		cout << line << endl;
	}

	cout << "The sum is " << sum << endl;

	return;
}

int main() { part1(); }
