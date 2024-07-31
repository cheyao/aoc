#include <cstdint>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ostream>
#include <queue>
#include <string>
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
} Vector2;

vector<string> maze;

#define ZERO  Vector2( 0, 0)
#define UP    Vector2( 0,-1)
#define DOWN  Vector2( 0, 1)
#define LEFT  Vector2(-1, 0)
#define RIGHT Vector2( 1, 0)

typedef struct Node {
	Vector2 position;
	Vector2 direction = ZERO;

	uint64_t turns = 0;
	uint64_t loss = 0;

	Node() : position(ZERO) {};
	Node(Vector2 pos) : position(pos) {};
	Node(Vector2 pos, Vector2 dir) : position(pos), direction(dir) {};

	bool inside(vector<string>& maze, Vector2 offset = ZERO) const {
		return (
				position.x + offset.x >= 0 &&
				position.x + offset.x < maze[0].size() &&
				position.y + offset.y >= 0 &&
				position.y + offset.y < maze.size()
		       );
	}

	bool operator==(const Node& other) const {
		return position == other.position && direction == other.direction;
	}
	bool operator!=(const Node& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(ostream& os, Node& node) {
		os << "Node: (" << node.position.x << ", " << node.position.y << ")";
		return os;
	}
} Node;

typedef struct NodeHash {
	uint64_t operator()(const Node& node) const {
		auto cantor = [](size_t a, size_t b){ return (a + b + 1) * (a + b) / 2 + b; };
		return cantor(node.position.x, cantor(node.position.y, cantor(node.direction.x, cantor(node.direction.y, node.turns))));
	}
} NodeHash;

static auto& get(vector<string>& vec, const Vector2& pos) {
	return vec[pos.y][pos.x];
}

const uint64_t Dijkstra(const Node& end) {
	uint64_t sum;
	priority_queue<Node, vector<Node>, decltype([](const Node& a, const Node& b) { return a.loss > b.loss; })> q;
	unordered_set<Node, NodeHash> visited;

	Node start;
	start.turns = 0;
	start.loss = 0;
	start.direction = DOWN;
	q.push(start);
	start.direction = RIGHT;
	q.push(start);

	while (!q.empty()) {
		auto cur = q.top();
		q.pop();

		// Already visited
		if (visited.contains(cur)) {
			continue;
		}
		visited.insert(cur);

		if (cur.position == end.position) {
			return cur.loss;
		}

		for (auto vec : (vector<Vector2>) {
				UP,
				DOWN,
				LEFT,
				RIGHT,
				}) {
			Node next;
			next.direction = vec;
			next.position = cur.position + vec;

			if (next.direction == cur.direction) {
				next.turns = cur.turns + 1;
			} else {
				next.turns = 1;
			}

			if (!next.inside(maze)) {
				continue;
			}

			// No more then 3
			if (next.turns > 3 && next.direction == cur.direction) {
				continue;
			}

			// No turning back
			if (next.direction == -cur.direction) {
				continue;
			}

			next.loss = cur.loss + (get(maze, next.position) - '0');

			q.push(next);
		}
	}

	return 0;
}

void getMap() {
	maze.clear();
	ifstream file("17.input");
	while (1) {
		string s;
		getline(file, s);
		if (file.eof()) {
			break;
		}

		maze.emplace_back(s);
	}
	file.close();
}

void part1() {
	getMap();

	const auto timeStart = std::chrono::high_resolution_clock::now();
	const auto sum = Dijkstra(Vector2(maze[0].size() - 1, maze.size() - 1));
	const auto timeEnd = std::chrono::high_resolution_clock::now();

	cout << "Part 1 took " << timeEnd-timeStart << endl;
	cout << "Sum of part 1: " << sum << endl;
}

void part2() {
	getMap();

	const Node end(Vector2(maze[0].size() - 1, maze.size() - 1));

	priority_queue<Node, vector<Node>, decltype([](const Node& a, const Node& b) { return a.loss > b.loss; })> q;
	unordered_set<Node, NodeHash> visited;

	Node start;
	start.turns = 0;
	start.loss = 0;
	start.direction = DOWN;
	q.push(start);
	start.direction = RIGHT;
	q.push(start);

	while (!q.empty()) {
		auto cur = q.top();
		q.pop();

		// Already visited
		if (visited.contains(cur)) {
			continue;
		}
		visited.insert(cur);

		if (cur.position == end.position) {
			if (cur.turns < 4) {
				continue;
			}

			cout << "Part 2: " << cur.loss << endl;
			return;
		}

		for (auto vec : (vector<Vector2>) {
				UP,
				DOWN,
				LEFT,
				RIGHT,
				}) {
			Node next;
			next.direction = vec;
			next.position = cur.position + vec;

			if (next.direction == cur.direction) {
				if (cur.turns >= 10) {
					continue;
				}

				next.turns = cur.turns + 1;
			} else {
				if (cur.turns < 4) {
					continue;
				}

				next.turns = 1;
			}

			if (!next.inside(maze)) {
				continue;
			}

			// No turning back
			if (next.direction == -cur.direction) {
				continue;
			}

			next.loss = cur.loss + (get(maze, next.position) - '0');

			q.push(next);
		}
	}

	cout << "Error" << endl;
}

int main() {
	part1();
	part2();
}
