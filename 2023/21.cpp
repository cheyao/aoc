#include <algorithm>
#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ranges>

using namespace std;

typedef struct Vector2 {
	int x, y;

	Vector2() : x(0), y(0){};
	Vector2(int _x, int _y) : x(_x), y(_y){};
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

static char& get(Vector2 pos) { return maze[pos.y][pos.x]; }

bool inside(Vector2 position) {
	return (position.x >= 0 && position.x < maze[0].size() &&
		position.y >= 0 && position.y < maze.size());
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

			for (const auto& dir :
			     (vector<Vector2>){UP, DOWN, LEFT, RIGHT}) {
				auto next = node + dir;
				if (!inside(next) ||
				    get(next) == '#' ||
				    find(visited.begin(), visited.end(),
					 next) != visited.end()) {
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

	return a;
}

namespace std {
template <> struct hash<Vector2> {
	std::size_t operator()(const Vector2& id) const noexcept {
		return std::hash<int>()(id.x ^ (id.y << 16));
	}
};
}

void parts() {
	ifstream file("21.input");
	maze.clear();
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
	// BFD
	deque<Vector2> frontier;
	uint64_t steps = 0;
	unordered_map<Vector2, uint64_t> ma;

	frontier.emplace_back(start);
	ma[start] = 0;

	while (!frontier.empty()) {
		auto node = frontier.front();
		frontier.pop_front();

		for (const auto& dir :
		     (vector<Vector2>){UP, DOWN, LEFT, RIGHT}) {
			auto next = node + dir;
			if (!inside(next) ||
			    get(next) == '#' ||
			    ma.contains(next)) {
				continue;
			}

			frontier.emplace_back(next);
			ma[next] = ma[node] + 1;
		}
	}

	constexpr uint64_t n = 202300;
	uint64_t smalleven = ranges::count_if(ma, [] (auto thing) -> bool { return thing.second > 65 && thing.second % 2 == 0; });
	uint64_t smallodd = ranges::count_if(ma, [] (auto thing) -> bool { return thing.second > 65 && thing.second % 2 == 1; });
	uint64_t alleven = ranges::count_if(ma, [] (auto thing) -> bool { return thing.second % 2 == 0; });
	uint64_t allodd = ranges::count_if(ma, [] (auto thing) -> bool { return thing.second % 2 == 1; });

	cout << "Part 1: " << smalleven << endl;
	cout << "Part 2: " << ((n+1)*(n+1)) * allodd + (n*n) * alleven - (n+1) * smallodd + n * smalleven << endl;
}

int main() {
	parts();
}
