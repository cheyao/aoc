#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<char>> pipeMap;

struct Vector2 {
       public:
	Vector2() : x(0), y(0){};
	Vector2(int _x, int _y) : x(_x), y(_y){};

	bool operator==(const Vector2& rhs) { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Vector2& rhs) { return !(*this == rhs); }
	bool operator<(const Vector2& rhs) {
		if (y == rhs.y) {
			return x < rhs.x;
		}

		return y < rhs.y;
	}
	Vector2 operator+(const Vector2& rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	Vector2& operator+=(const Vector2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	operator int() { return x + y * pipeMap[0].size(); }

	int x, y;
};

const Vector2 UP{0, -1};
const Vector2 DOWN{0, 1};
const Vector2 LEFT{-1, 0};
const Vector2 RIGHT{1, 0};
const Vector2 ZERO{0, 0};

Vector2 after(Vector2 start, Vector2 pipe) {
	switch (pipeMap[pipe.y][pipe.x]) {
		case '|':
			if (start == pipe + DOWN) {
				return pipe + UP;
			} else if (start == pipe + UP) {
				return pipe + DOWN;
			} else {
				return ZERO;
			}
		case '-':
			if (start == pipe + LEFT) {
				return pipe + RIGHT;
			} else if (start == pipe + RIGHT) {
				return pipe + LEFT;
			} else {
				return ZERO;
			}
		case 'L':
			if (start == pipe + UP) {
				return pipe + RIGHT;
			} else if (start == pipe + RIGHT) {
				return pipe + UP;
			} else {
				return ZERO;
			}
		case 'J':
			if (start == pipe + LEFT) {
				return pipe + UP;
			} else if (start == pipe + UP) {
				return pipe + LEFT;
			} else {
				return ZERO;
			}
		case '7':
			if (start == pipe + LEFT) {
				return pipe + DOWN;
			} else if (start == pipe + DOWN) {
				return pipe + LEFT;
			} else {
				return ZERO;
			}
		case 'F':
			if (start == pipe + RIGHT) {
				return pipe + DOWN;
			} else if (start == pipe + DOWN) {
				return pipe + RIGHT;
			} else {
				return ZERO;
			}
		default:
			return ZERO;
	}
}

vector<Vector2> walked;

void part1() {
	ifstream file("10.input");
	int y = 0;

	Vector2 start = {-1, -1};
	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		pipeMap.emplace_back(vector<char>());

		for (int x = 0; x < line.size(); x++) {
			pipeMap[y].emplace_back(line[x]);
			if (line[x] == 'S') {
				start = {x, y};
			}
		}

		y++;
	}

	if (start == (Vector2{-1, -1})) {
		cerr << "Didn't find the start" << endl;
		exit(1);
	}

	cout << "Found start at " << start.x << ", " << start.y << endl;

	// Find starting pipe
	Vector2 pos = {-1, -1};
	for (Vector2 vec : {UP, DOWN, LEFT, RIGHT}) {
		if (after(start, start + vec) != ZERO) {
			pos = start + vec;
			break;
		}
	}
	if (pos == (Vector2{-1, -1})) {
		cerr << "Didn't find the first pipe" << endl;
		exit(1);
	}

	cout << "First pipe is at: " << pos.x << ":" << pos.y << endl;

	// Yay
	int c = 0;
	Vector2 currentPos = start;
	Vector2 nextPos = pos;
	walked.emplace_back(currentPos);
	walked.emplace_back(nextPos);
	while (pipeMap[nextPos.y][nextPos.x] != 'S') {
		c++;
		Vector2 nextOffset = after(currentPos, nextPos);
		if (nextOffset == ZERO) {
			cout << "ERROR!" << endl;
			exit(1);
		}
		currentPos = nextPos;
		nextPos = nextOffset;
		walked.emplace_back(nextPos);
	}

	cout << "We used " << c/2+1 << " steps" << endl;
}

void part2() {
	ifstream fileS("10.input.small");
	vector<string> file;
	while (1) {
		string s;
		getline(fileS, s);
		if (fileS.eof()) {
			break;
		}
		file.emplace_back(s);
	}

	for (auto vec : walked) {
		char r = 'B';
		switch (file[vec.y][vec.x]) {
			case '|': r = 'A'; break;

			case 'F': r = 'B'; break;
			case 'J': r = 'B'; break;

			case '7': r = 'C'; break;
			case 'L': r = 'C'; break;

			case '-': r = 'D'; break;
		}
		file[vec.y][vec.x] = r;
	}

	int count = 0;
	int l = 0;
	vector<string> out;
	for (auto line : file) {
		out.emplace_back(line);
		int numPipe;
		int numFJ;
		int num7L;
		string cp = line;

		for (int i = 0; i < line.size(); i++) {	
			string s = cp.substr(0, i + 1);
			numPipe = std::count(s.begin(), s.end(), 'A');
			numFJ = std::count(s.begin(), s.end(), 'B');
			num7L = std::count(s.begin(), s.end(), 'C');

			char cur = cp[i];
			if (cur != 'A' && cur !='B' && cur != 'C' && cur != 'D' && 
					(numPipe + numFJ/2 - num7L/2) % 2 != 0) {
				count++;
				out[l][i] = '!';
			}
		}
		l++;
	}

	for (auto str : out) {
		cout << str << endl;
	}

	cout << count << endl;
}

int main() { part1(); part2(); }
