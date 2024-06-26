#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef struct Vector2 {
	int x, y;

	Vector2() : x(0), y(0) {};
	Vector2(uint64_t _x, uint64_t _y) : x(_x), y(_y) {};
	bool operator==(const Vector2& rhs) const {
		return x == rhs.x && y == rhs.y;
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
	friend ostream& operator<<(ostream& out, const Vector2& vec) {
		out << '(' << vec.x << ", " << vec.y << ')';
		return out;
	}
} Vector2;

namespace std {
template <> struct hash<Vector2> {
	std::size_t operator()(const Vector2& id) const noexcept {
		return std::hash<int64_t>()(id.x ^ (id.y << 16));
	}
};
}

#define ZERO  Vector2( 0, 0)
#define UP    Vector2( 0,-1)
#define DOWN  Vector2( 0, 1)
#define LEFT  Vector2(-1, 0)
#define RIGHT Vector2( 1, 0)

vector<string> maze;

template<typename T> 
static auto& get(vector<T>& vec, Vector2 pos) {
	return vec[pos.y][pos.x];
}

bool inside(const Vector2& position) {
	return (position.x >= 0 && position.x < maze[0].size() &&
		position.y >= 0 && position.y < maze.size() &&
		get(maze, position) != '#');
}

const static vector<Vector2> direction(const char& dir) {
	 static unordered_map<char, vector<Vector2>> dirMap = {
		{'^', {UP}}, 
		{'v', {DOWN}},
		{'<', {LEFT}},
		{'>', {RIGHT}},
		{'.', {UP, DOWN, LEFT, RIGHT}}
	};

	return dirMap[dir];
}

uint64_t search(Vector2 pos, const Vector2& end, unordered_set<Vector2> searched) {
	if (pos == end) {
		return 1;
	}

	searched.emplace(pos);

	uint64_t len = 0;

	for (const Vector2& vec : direction(get(maze, pos))) {
		if (!inside(pos + vec) || searched.contains(pos + vec)) {
			continue;
		}

		len = max(search(pos + vec, end, searched), len);
	}

	return len + 1;
}

void readMap() {
	maze.clear();
	ifstream file("23.input");
	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		maze.emplace_back(line);
	}
}

// TODO: In and out
void part1() {
	readMap();

	Vector2 start;
	Vector2 end;
	for (int x = 0; x < maze[0].size(); ++x) {
		if (maze[0][x] == '.') {
			start = Vector2(x, 0);
		}

		if (maze[maze.size() - 1][x] == '.') {
			end = Vector2(x, maze.size() - 1);
		}
	}

	auto s = chrono::high_resolution_clock::now();
	auto outDist = search(start, end, {});
	auto e = chrono::high_resolution_clock::now();

	cout << "Part 1: " << outDist - 1 << ", Took " << (e-s) << endl;
}

int main() {
	part1();
}

