#include <fstream>
#include <iostream>
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

void part1() {
	ifstream file("10.input.small");
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
	while (pipeMap[nextPos.y][nextPos.x] != 'S') {
		cout << nextPos.x << ":" << nextPos.y << endl;
		c++;
		Vector2 nextOffset = after(currentPos, nextPos);
		if (nextOffset == ZERO) {
			cout << "ERROR!" << endl;
			exit(1);
		}
		currentPos = nextPos;
		nextPos = nextOffset;
	}

	cout << "We used " << c/2+1 << " steps" << endl;
}

int main() { part1(); }
