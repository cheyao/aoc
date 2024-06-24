#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <fstream>
#include <ostream>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>

using namespace std;

typedef struct Vector3 {
	uint64_t x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(uint64_t _x, uint64_t _y, uint64_t _z) : x(_x), y(_y), z(_z) {};
	bool operator==(const Vector3& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	// Vector3 operator-() const { return Vector3(-x, -y, -z); }
	Vector3& operator+=(const Vector3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	friend ostream& operator<<(ostream& out, const Vector3& vec) {
		out << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
		return out;
	}
} Vector3;

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
	Vector3 result = lhs;
	result += rhs;
	return result;
}
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
	Vector3 result = lhs;
	result -= rhs;
	return result;
}

typedef struct Brick {
	Vector3 a, b;

	Brick() : a(), b() {};
        Brick(Vector3 _a, Vector3 _b) : a(_a), b(_b) {};
	Brick(uint64_t xa, uint64_t ya, uint64_t za, uint64_t xb, uint64_t yb, uint64_t zb) : a(xa, ya, za), b(xb, yb, zb) {};

	static bool operator()(const Brick& a, const Brick& b) {
		return max(a.a.z, a.b.z) > max(b.a.z, b.b.z);
	}
	friend ostream& operator<<(ostream& out, const Brick& vec) {
		out << "Brick: " << vec.a << "~" << vec.b;
		return out;
	}
} Brick;

uint64_t ids = 0;

// Returns highest block in range
const Brick highest(const Brick& brick, const vector<vector<uint64_t>>& bmap, unordered_map<uint64_t, Brick>& idtob) {
        Brick high = idtob[0]; // Highest default: ground
        //
        for (int y = brick.a.y; y <= brick.b.y; y++) {
                for (int x = brick.a.x; x <= brick.b.x; x++) {
                        if (idtob[bmap[y][x]].b.z > high.b.z) {
                                high = idtob[bmap[y][x]];
                        }
                }
        }

        return high;
}

void fall(Brick& brick, vector<vector<uint64_t>>& bmap, unordered_map<uint64_t, Brick>& idtob) {
        // Fall
        const auto highestBlock = highest(brick, bmap, idtob);

        uint64_t hight = brick.b.z - brick.a.z;
        brick.a.z = highestBlock.b.z + 1;
        brick.b.z = brick.a.z + hight;

        // Register the brick
        for (int y = brick.a.y; y <= brick.b.y; y++) {
                for (int x = brick.a.x; x <= brick.b.x; x++) {
                        bmap[y][x] = ids;
                }
        }
        idtob[ids] = brick;
        ids++;
}

void part1() {
	ifstream file("22.input.small");
	priority_queue<Brick, vector<Brick>, Brick> bricksToFall;
	// Data structs
	// Map
	vector<vector<uint64_t>> bmap;
	unordered_map<uint64_t, Brick> idtob;
        uint64_t maxX = 0, maxY = 0;

	// Read bricks
        string line;
        while (getline(file, line)) {
                const vector<Vector3> vec = line | views::split('~') | views::transform([] (const auto& str) { 
                        auto vec = str | views::split(',') | views::transform([] (const auto& str) {
                                        return stoi(str.data());
                        }) | ranges::to<vector<uint64_t>>();

                        return Vector3(vec[0], vec[1], vec[2]);
                }) | ranges::to<vector<Vector3>>();

                Vector3 a(min(vec[0].x, vec[1].x), min(vec[0].y, vec[1].y), min(vec[0].z, vec[1].z));
                Vector3 b(max(vec[0].x, vec[1].x), max(vec[0].y, vec[1].y), max(vec[0].z, vec[1].z));
                maxX = max(maxX, b.x);
                maxY = max(maxY, b.y);

                bricksToFall.emplace(Brick(a, b));
        }
        file.close();

	Brick ground(0, 0, 0, maxX, maxY, 0);

        // Init map and idtob
        bmap.reserve(maxY + 1);
        for (uint64_t y = 0; y <= maxY; y++) {
                bmap.emplace_back((vector<uint64_t>) {});
                bmap.back().reserve(maxX + 1);

                for (uint64_t x = 0; x <= maxX; x++) {
                        bmap[y].emplace_back(0);
                }
        }
	idtob[0] = ground;
        ids++;

        // Fall bricks
        vector<Brick> bricks;
	while (!bricksToFall.empty()) {
		auto brick = bricksToFall.top();
		bricksToFall.pop();

                fall(brick, bmap, idtob);

                bricks.emplace_back(brick);
	}

        for (auto thing : bricks) {
                cout << thing << endl;
        }
}

int main() {
	part1();
}
