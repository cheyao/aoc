#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <stack>
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
		return std::hash<int64_t>()(id.x ^ ((int64_t) id.y << 32));
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

const static bool inside(const Vector2& position) {
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

	return len == 0 ? 0 : len + 1;
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

// Part 2:
struct Node {
	Vector2 pos;
	uint64_t len;
	vector<Node*> Neighbors;

	Node() : pos(ZERO), len(0) {};
	Node(Vector2 p) : pos(p), len(0) {};

	bool inside(vector<string>& maze, Vector2 offset = ZERO) {
		return (
				pos.x + offset.x >= 0 &&
				pos.x + offset.x < maze[0].size() &&
				pos.y + offset.y >= 0 &&
				pos.y + offset.y < maze.size()
		       );
	}

	bool operator==(const Node& rhs) const {
		return pos == rhs.pos;
	}
};

namespace std {
template <> struct hash<Node> {
	std::size_t operator()(const Node& id) const noexcept {
		return std::hash<int64_t>()(id.pos.x ^ ((int64_t) id.pos.y << 32));
	}
};
}

struct Edge {
	Node a;
	Node b;
	uint64_t len;

	Edge(Node _a, Node _b, uint64_t l) : a(_a), b(_b), len(l) {};
};

void readNodes(unordered_set<Node>& dst, unordered_map<Node, vector<Edge>>& nte) {
	for (uint64_t y = 1; y < maze.size() - 1; ++y) {
		for (uint64_t x = 1; x < maze[0].size() - 1; ++x) {
			if (maze[y][x] == '#') {
				continue;
			}

			Vector2 pos = Vector2(x, y);
			uint64_t count = 0;
			for (const auto& vec : (const vector<Vector2>) {UP, DOWN, LEFT, RIGHT}) {
				if (inside(pos + vec) && get(maze, pos + vec) != '#') {
					count++;
				}
			}
			if (count > 2) {
				dst.insert(Node(pos));
			}
		}
	}


	// Now populate edges
	for (const Node& node : dst) {
		stack<Node> s;
		s.emplace(node.pos);

		vector<vector<bool>> seen(maze.size(), vector<bool>(maze[0].size(), false));

		while (!s.empty()) {
			const Node cur = s.top();
			s.pop();

			seen[cur.pos.y][cur.pos.x] = true;

			for (const auto& vec : (const vector<Vector2>) {UP, DOWN, LEFT, RIGHT}) {
				Node next = Node(cur.pos + vec);
				next.len = cur.len + 1;

				if (!inside(next.pos) || get(maze, next.pos) == '#' || seen[next.pos.y][next.pos.x]) {
					continue;
				}

				if (dst.find(Node(next)) == dst.end()) {
					s.emplace(next);
				} else {
					nte[node].emplace_back(Edge(node, next, next.len));
				}
			}
		}
	}
}

void scan(stack<Node>& s, unordered_map<Node, vector<Edge>>& nte, const Node& end, uint64_t& max, vector<vector<bool>>& seen) {
	if (s.empty()) {
		return;
	}

	const Node cur = s.top();

	seen[cur.pos.y][cur.pos.x] = true;

	if (cur == end) {
		max = std::max(cur.len, max);
	} else {
		for (const Edge& edge : nte[cur]) {
			Node next = edge.b;
			next.len = cur.len + edge.len;

			if (!seen[next.pos.y][next.pos.x]) {
				s.push(next);
				scan(s, nte, end, max, seen);
				s.pop();
			}
		}
	}

	seen[cur.pos.y][cur.pos.x] = false;
}

void part2() {
	readMap();

	Node start;
	Node end;

	for (int x = 0; x < maze[0].size(); ++x) {
		if (maze[0][x] == '.') {
			start = Node(Vector2(x, 0));
		}

		if (maze[maze.size() - 1][x] == '.') {
			end = Node(Vector2(x, maze.size() - 1));
		}
	}

	unordered_set<Node> nodes{start, end};
	unordered_map<Node, vector<Edge>> nte;
	vector<vector<bool>> seen(maze.size(), vector<bool>(maze[0].size(), false));

	readNodes(nodes, nte);

	uint64_t max = 0;

	stack<Node> stack;
	stack.push(start);

	auto s = chrono::high_resolution_clock::now();
	scan(stack, nte, end, max, seen);
	auto e = chrono::high_resolution_clock::now();

	cout << "Part 2: " << max << ", took " << (e-s) << endl;
}

int main() {
	part1();
	part2();
}

