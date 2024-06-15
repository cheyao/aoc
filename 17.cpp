#include <cstdint>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
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
} Vector2;

vector<string> maze;

#define ZERO  Vector2( 0, 0)
#define UP    Vector2( 0,-1)
#define DOWN  Vector2( 0, 1)
#define LEFT  Vector2(-1, 0)
#define RIGHT Vector2( 1, 0)

typedef struct Node {
	Vector2 position;
	vector<Node*> neighbors;

	Vector2 direction;
	int turns = 0;

	Node() : position(ZERO), neighbors(), direction(RIGHT) {};
	Node(Vector2 pos) : position(pos), neighbors(), direction(RIGHT) {};

	bool inside(vector<string>& maze, Vector2 offset = ZERO) {
		return (
				position.x + offset.x >= 0 &&
				position.x + offset.x < maze[0].size() &&
				position.y + offset.y >= 0 &&
				position.y + offset.y < maze.size()
		       );
	}

	bool operator==(const Node& other) const {
		return position == other.position;
	}
	bool operator!=(const Node& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(ostream& os, Node& node) {
		os << "Node: (" << node.position.x << ", " << node.position.y << ")";
		return os;
	}
} Node;

typedef struct PriorityQueue {
	typedef std::pair<uint64_t, Node> Element;
	std::priority_queue<Element, std::vector<Element>, decltype([] (const Element& a, const Element& b) { return a.first > b.first; })> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(Node item, uint64_t priority) {
		elements.emplace(priority, item);
	}

	Node get() {
		Node best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
} PriorityQueue;

namespace std {
template <> struct hash<Vector2> {
	std::size_t operator()(const Vector2& id) const noexcept {
		return std::hash<int>()(id.x ^ (id.y << 16));
	}
};
template <> struct hash<Node> {
	std::size_t operator()(const Node& id) const noexcept {
		return std::hash<int>()(id.position.x ^ (id.position.y << 16));
	}
};
}

static auto& get(vector<string>& vec, Vector2 pos) {
	return vec[pos.y][pos.x];
}

static uint64_t tileCost(const Vector2 pos) {
	return get(maze, pos) - '0';
}

static Vector2 dir(const Node& neighbor, const Node& b) {
	return (neighbor.position - b.position);
}

bool valid(const Node& neighbor, const Node& node) {
	if (node.turns == 3 && node.direction == dir(neighbor, node)) {
		return false;
	}

	if (dir(neighbor, node) == -node.direction) {
		return false;
	}

	return true;
}

unordered_map<Node, Node> Dijkstra(vector<Node>& nodes, Node& start, Node& end) {
	PriorityQueue frontier;
	frontier.put(start, 1);
	unordered_map<Node, Node> outPath;
	outPath[start] = start;
	unordered_map<Node, uint64_t> costs;
	costs[start] = tileCost(start.position);

	while (!frontier.empty()) {
		auto node = frontier.get();

		if (node == end) {
			break;
		}

		for (Node* neighbor : node.neighbors) {
			const uint64_t newCost = costs[node] + tileCost(neighbor->position);

			if ((!outPath.contains(*neighbor) || newCost < costs[*neighbor]) && valid(*neighbor, node)) {
				cout << node << " to " << *neighbor << " with cost " << newCost << endl;
				costs[*neighbor] = newCost;
				outPath[*neighbor] = node;

				neighbor->direction = dir(*neighbor, node);

				if (neighbor->direction == node.direction) {
					neighbor->turns = node.turns + 1;
				} else {
					neighbor->turns = 1;
				}
				frontier.put(*neighbor, newCost);
			}
		}
	}

	return outPath;
}

Node& findNode(vector<Node>& nodes, const Vector2& position) {
	for (Node& node : nodes) {
		if (node.position == position) {
			return node;
		}
	}

	return nodes[0];
}

void populate(Node& node, vector<Node>& nodes) {
	for (const Vector2& vec : {RIGHT, UP, DOWN, LEFT}) {
		if (!node.inside(maze, vec)) {
			continue;
		}

		node.neighbors.push_back(&findNode(nodes, node.position + vec));
	}
}

const char direction(const Node& node, const Node& direction) {
	 static unordered_map<Vector2, char> dirMap = {
		{UP, '^'}, 
		{DOWN, 'v'},
		{LEFT, '<'},
		{RIGHT, '>'}
	};
	return dirMap[direction.position - node.position];
}

const string beutify(const char& c) {
	 static unordered_map<char, string> charMap = {
		 {'>', "→"},
		 {'<', "←"},
		 {'v', "↓"},
		 {'^', "↑"},
		 {'S', "\x1B[33m⛌\033[0m"},
		 {'D', "\x1B[33m★\033[0m"},
		 {' ', " "},
		 {'.', " "}
	};
	return charMap.contains(c) ? charMap[c] : "\033[0m" + string(1, c) + "\033[0m";
}

void getMap() {
	ifstream file("17.input.small");
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

	vector<Node> nodes;
	Node start;
	Node end;
	for (int y = 0; y < maze.size(); ++y) {
		for (int x = 0; x < maze[0].size(); ++x) {
			Node node;
			node.position = Vector2(x, y);
			node.neighbors.clear();
			node.turns = 0;
			node.direction = ZERO;

			nodes.push_back(node);

			 if (y == 0 && x == 0) {
				 start = node;
			 }
			 if (y == maze.size() - 1 && x == maze[0].size() - 1) {
				 end = node;
			 }
		}
	}

	for (auto& node : nodes) {
		populate(node, nodes);
	}
	populate(start, nodes);
	start.direction = ZERO;
	start.turns = 0;

	auto timeStart = std::chrono::high_resolution_clock::now();
	auto outPath = Dijkstra(nodes, start, end);
	auto timeEnd = std::chrono::high_resolution_clock::now();

#if 1
	vector<string> maze1 = maze;
	// Print out the results
	cout << "Output:" << endl;
	Node cur = end;
	while (cur != start) {
		Node next = outPath[cur];
		get(maze1, cur.position) = '.';
		cur = next;
	}
	maze1[0][0] = 'S';
	for (auto& line : maze1) {
		cout << line << endl;
	}

	cout << "Path flow:" << endl;
	for (auto [node, neighbor] : outPath) {
		if (maze1[node.position.y][node.position.x] != '.') {
			continue;
		}
		if (node.position == start.position) {
			maze1[node.position.y][node.position.x] = 'S';
		} else if (node.position == end.position) {
			maze1[node.position.y][node.position.x] = 'D';
		} else {
			maze1[node.position.y][node.position.x] = direction(node, neighbor);
		}
	}
	for (const auto& line : maze1) {
		for (const auto& c : line) {
			cout << "\x1B[32m" << beutify(c) << "\033[0m";
		}
		cout << endl;
	}

	Node cur2 = end;
	while (cur2 != start) {
		Node next = outPath[cur];
		get(maze, cur.position) = '.';
		cur2 = next;
	}
	cout << "Flow:" << endl;
	for (auto [node, neighbor] : outPath) {
		if (node.position == start.position) {
			maze[node.position.y][node.position.x] = 'S';
		} else if (node.position == end.position) {
			maze[node.position.y][node.position.x] = 'D';
		} else {
			maze[node.position.y][node.position.x] = direction(node, neighbor);
		}
	}
	for (const auto& line : maze) {
		for (const auto& c : line) {
			cout << "\x1B[32m" << beutify(c) << "\033[0m";
		}
		cout << endl;
	}
#endif

	cout << "Part 1 took " << timeEnd-timeStart << "ns" << endl;
}

int main() {
	part1();
}
