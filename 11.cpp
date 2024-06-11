#include <cstdint>
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
						sum += abs(xn - x) + (yn - y);
					}
				}

				xn = 0;
			}
		}
	}

	cout << "The sum for part 1 is " << sum << endl;
}

void part2() {
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
			for (auto& c : *iter) {
				c = '-';
			}
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
				if (line[i] == '-') {
					line[i] = '+';
				} else {
					line[i] = '|';
				}
			}
		}
	}

	// Calculate paths
	uint64_t sum = 0;
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
						// Now we need to count the distance
						uint64_t expansion = 1000000;
						// sum += abs(xn - x) + (yn - y);
						for (int xc = min(xn, x); xc < max(xn, x); xc++) {
							if (galaxy[y][xc] == '|') {
								sum += expansion;
							} else {
								sum += 1;
							}
						}

						for (int yc = y; yc < yn; yc++) {
							if (galaxy[yc][x] == '-') {
								sum += expansion;
							} else {
								sum += 1;
							}
						}
					}
				}

				xn = 0;
			}
		}
	}

	cout << "The sum for part 2 is " << sum << endl;
}

int main() { 
	part1();
	part2();
}
