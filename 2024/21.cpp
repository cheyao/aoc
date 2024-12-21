#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "../vec2.hpp"

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

/*
KPD
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
	| 0 | A |
	+---+---+

KP0
	+---+---+
	| ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+

KP1
	+---+---+
	| ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+

KPM
	+---+---+
	| ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+

Hypothese:
Only shotest length key combination be one of the shortest length key press combinations for the
next keypad.

Demonstration:
For all key sequence wanted S, we have the same number of A keys to press as the length of S.
Between all key in S and the key's corresponding home key, there exists a optimal key squence O with
the length of the manhattan distance between the desired key and the home key. Thus for all
additional key press, the next keypad would optimally need 2 more keys to suppliment this additional
key press, thus rendering the solution non-optimal.

Hypothese:
For all key combination,

Yes since for all additional non-needed key presses made, one more is needed to suppliment it, thus
the length of the next key sequence will be increased of at least 2 even in the best case.

029A
<A^A^^>AvvvA
<A^A^>^AvvvA
<A^A>^^AvvvA

For all sequence, there may only be A+B and B+A
*/

// Returns all shortest path from a to b
vector<string> getSM(char ac, char bc) {
	static unordered_map<char, vec2> ctop = {
		{'A', {2, 3}}, {'0', {1, 3}}, {'1', {0, 2}}, {'2', {1, 2}}, {'3', {2, 2}}, {'4', {0, 1}},
		{'5', {1, 1}}, {'6', {2, 1}}, {'7', {0, 0}}, {'8', {1, 0}}, {'9', {2, 0}}};
	vec2 a = ctop[ac];
	vec2 b = ctop[bc];

	string A;
	string B;

	int64_t xdiff = b.first - a.first;
	int64_t ydiff = b.second - a.second;

	if (xdiff >= 0) {
		A = string(xdiff, '>');
	} else {
		A = string(llabs(xdiff), '<');
	}

	if (ydiff >= 0) {
		B = string(ydiff, 'v');
	} else {
		B = string(llabs(ydiff), '^');
	}

	if (xdiff == 0) {
		return {B + "A"};
	}
	if (ydiff == 0) {
		return {A + "A"};
	}

	return {A + B + "A", B + A + "A"};
}

void part1() {
	uint64_t sum = 0;

	for (const auto code : get()) {
		// Need to start from the end
		for (const auto v : ("A" + code) | views::slide(2)) {
			auto sets = getSM(v[0], v[1]);
			for (const auto s : sets)
				cout << s << '\n';
			cout << '\n';
		}

		sum += stoull(code);
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
