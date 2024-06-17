#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

typedef struct Vector2 {
	uint64_t x, y;

	Vector2() : x(0), y(0) {};
	Vector2(uint64_t _x, uint64_t _y) : x(_x), y(_y) {};
	bool operator==(const Vector2& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vector2& rhs) const {
		return x != rhs.x && y != rhs.y;
	}
	bool operator<(const Vector2& rhs) const {
		if (y != rhs.y) {
			return y < rhs.y;
		}

		return x < rhs.x;
	}
	Vector2 operator+(const Vector2& rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	Vector2 operator-(const Vector2& rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}
	Vector2 operator-() const {
		return Vector2(-x, -y);
	}
	Vector2& operator+=(const Vector2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2 operator*(const uint64_t& scalar) {
		return Vector2(x * scalar, y * scalar);
	}
	Vector2& operator*=(const uint64_t& scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
} Vector2;

#define ZERO  Vector2( 0, 0)
#define UP    Vector2( 0,-1)
#define DOWN  Vector2( 0, 1)
#define LEFT  Vector2(-1, 0)
#define RIGHT Vector2( 1, 0)

void part1() {
	ifstream file("18.input");
	uint64_t S1 = 0, S2 = 0;
	uint64_t parim = 0;
	Vector2 last;

	while (1) {
		string s;
		getline(file, s);
		if (file.eof()) {
			break;
		}
		stringstream ss(s);

		Vector2 dir;
		uint64_t blocks;
		char c;
		ss >> c;
		switch (c) {
			case 'U':
				dir = UP;
				break;
			case 'D':
				dir = DOWN;
				break;
			case 'L':
				dir = LEFT;
				break;
			case 'R':
				dir = RIGHT;
				break;
			default:
				exit(1);
		}
		ss >> blocks;
		parim += blocks;
		dir *= blocks;
		dir += last;

		S1 += last.x * dir.y;
		S2 += dir.x * last.y;

		last = dir;
	}

	// Now count the area
	cout << ((S1 - S2) / 2) + (parim  / 2 + 1) << endl;
}

void part2() {
	ifstream file("18.input");
	uint64_t S1 = 0, S2 = 0;
	uint64_t parim = 0;
	Vector2 last;

	while (1) {
		string s;
		getline(file, s);
		if (file.eof()) {
			break;
		}
		stringstream ss(s);

		Vector2 dir;
		uint64_t blocks, discardi;
		char c, discard;
		string color;
		ss >> discard;
		ss >> discardi;

		ss >> color;
		switch (color[color.size() - 2]) {
			case '3':
				dir = UP;
				break;
			case '1':
				dir = DOWN;
				break;
			case '2':
				dir = LEFT;
				break;
			case '0':
				dir = RIGHT;
				break;
			default:
				exit(1);
		}

		color = color.substr(2, color.size() - 4);
		blocks = stoi(color, 0, 16);

		parim += blocks;
		dir *= blocks;
		dir += last;

		S1 += last.x * dir.y;
		S2 += dir.x * last.y;

		last = dir;
	}

	// Now count the area
	cout << ((S1 - S2) / 2) + (parim  / 2 + 1) << endl;
}

int main() {
	part1();
	part2();
}
