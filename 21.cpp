#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef struct Vector2 {
	int x, y;

	Vector2() : x(0), y(0){};
	Vector2(uint64_t _x, uint64_t _y) : x(_x), y(_y){};
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
	Vector2 operator-() const { return Vector2(-x, -y); }
	Vector2& operator+=(const Vector2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
} Vector2;

#define ZERO Vector2(0, 0)
#define UP Vector2(0, -1)
#define DOWN Vector2(0, 1)
#define LEFT Vector2(-1, 0)
#define RIGHT Vector2(1, 0)

vector<string> maze;

typedef struct Node {
	Vector2 position;
	vector<const Node*> neighbors;
	Node() : position(ZERO), neighbors(){};
	Node(Vector2 pos) : position(pos), neighbors(){};

	bool inside(vector<string>& maze, Vector2 offset = ZERO) {
		return (position.x + offset.x >= 0 &&
			position.x + offset.x < maze[0].size() &&
			position.y + offset.y >= 0 &&
			position.y + offset.y < maze.size());
	}

	bool operator==(const Node& other) const {
		return position == other.position;
	}
	bool operator!=(const Node& other) const { return !(*this == other); }

	friend std::ostream& operator<<(ostream& os, Node& node) {
		os << "Node: (" << node.position.x << ", " << node.position.y
		   << ")";
		return os;
	}
} Node;

namespace std {
template <>
struct hash<Vector2> {
	std::size_t operator()(const Vector2& id) const noexcept {
		return std::hash<int>()(id.x ^ (id.y << 16));
	}
};
template <>
struct hash<Node> {
	std::size_t operator()(const Node& id) const noexcept {
		return std::hash<int>()(id.position.x ^ (id.position.y << 16));
	}
};
}  // namespace std

static char& get(Vector2 pos) {
	return maze[pos.y][pos.x];
}

bool inside(Vector2 position) {
	return (position.x >= 0 &&
		position.x < maze[0].size() &&
		position.y >= 0 &&
		position.y < maze.size());
}

uint64_t BFD(Vector2& start, const uint64_t& steps = 64) {
	deque<Vector2> frontier;
	uint64_t a = 1;
	deque<Vector2> frontierNext;
	uint64_t b = 0;
	vector<Vector2> visited;
	frontier.emplace_back(start);
	visited.emplace_back(start);

	for (int i = 0; i < steps; i++) {
		while (!frontier.empty()) {
			auto node = frontier.front();
			frontier.pop_front();

			for (const auto& dir : (vector<Vector2>) {
					UP,
					DOWN,
					LEFT,
					RIGHT
					}) {
				auto next = node + dir;
				if (!inside(next) || get(next) == '#' || find(visited.begin(), visited.end(), next) != visited.end()) {
					continue;
				}

				frontierNext.emplace_back(next);
				visited.emplace_back(next);
				++b;
			}
		}

		swap(frontier, frontierNext);
		swap(a, b);
	}

	/*
	// Print out the results
	while (!frontier.empty()) {
		const Vector2 v = frontier.front();
		get(v) = 'O';
		tiles.pop_front();
	}
	for (auto& line : maze) {
		cout << line << endl;
	}
	*/

	return a;
}

void part1() {
	ifstream file("21.input");
	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		maze.emplace_back(line);
	}

	Vector2 start;
	for (int y = 0; y < maze.size(); ++y) {
		for (int x = 0; x < maze[0].size(); ++x) {
			if (maze[y][x] == 'S') {
				start = Vector2(x, y);
				get(start) = '.';
				goto fin;
			}
		}
	}
	exit(1);  // No reach

fin:
	auto tiles = BFD(start, 64);

	cout << "Part 1: " << tiles << endl;
}

int main() { part1(); }
