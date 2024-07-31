#include <Eigen/Dense>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
using Eigen::Vector2i;

typedef Vector2i Vector2;

const static Vector2 ZERO(0, 0);
const static Vector2 NORTH(0, -1);
const static Vector2 EAST(1, 0);
const static Vector2 SOUTH(0, 1);
const static Vector2 WEST(-1, 0);

typedef struct Laser {
	Laser(Vector2 pos = ZERO, Vector2 vel = ZERO) : position(pos), velocity(vel) {};

	Vector2 position;
	Vector2 velocity;

	bool zero() {
		return velocity == ZERO;
	}

	bool operator==(const Laser& laser) {
		return (position == laser.position) && (velocity == laser.velocity);
	}

	void advance() {
		this->position += this->velocity;
	}
	
	bool inside(const vector<string>& layout) {
		return (
				this->position.x() >= 0 &&
				this->position.x() < layout[0].size() &&
				this->position.y() >= 0 &&
				this->position.y() < layout.size()
		       );
	}
} Laser;

// +-----> x
// |
// |
// |
//\/ y

typedef struct FootprintS {
	uint8_t N : 1; // ^
	uint8_t E : 1; // ->
	uint8_t S : 1; // \/
	uint8_t W : 1; // <-
} FootprintS;

typedef union Footprint {
	uint8_t val = 0;
	FootprintS bits;
} Footprint;

// Only change direction
// Need to .advance()
Laser bounce(Laser& laser, const char& tile) {
	switch(tile) {
		case '\\': {
			int x = laser.velocity.x();
			laser.velocity.x() = laser.velocity.y();
			laser.velocity.y() = x;
			break;
		}

		case '/': {
			int x = laser.velocity.x();
			laser.velocity.x() = -laser.velocity.y();
			laser.velocity.y() = -x;
			break;
		}

		case '-': {
			if (laser.velocity.y() != 0) {
				laser.velocity = EAST;

				return Laser(laser.position, WEST); 
			}
			
			break;
		}

		case '|': {
			if (laser.velocity.x() != 0) {
				laser.velocity = NORTH;

				return Laser(laser.position, SOUTH); 
			}
			break;
		}

		case '.': {
			break;
		}

		default: {
			break;
		}
	}

	return Laser(); // NULL
}

Footprint toBits(Laser& l) {
	Footprint foot;

	if (l.velocity == NORTH) { foot.bits.N = 1; }
	if (l.velocity == EAST)  { foot.bits.E = 1; }
	if (l.velocity == SOUTH) { foot.bits.S = 1; }
	if (l.velocity == WEST)  { foot.bits.W = 1; }

	return foot;
}

void shoot(queue<Laser>& lasers, const vector<string>& layout, vector<vector<Footprint>>& footprints) {
	while (!lasers.empty()) {
		Laser laser = lasers.front();
		lasers.pop();

		if (!laser.inside(layout)) {
			continue;
		}

		int in = footprints[laser.position.y()][laser.position.x()].val & toBits(laser).val;
		if (in) {
			continue;
		}
		footprints[laser.position.y()][laser.position.x()].val |= toBits(laser).val;

		const char& tile = layout[laser.position.y()][laser.position.x()];

		Laser newLaser = bounce(laser, tile);

		if (!newLaser.zero()) {
			newLaser.advance();
			lasers.push(newLaser);
		}

		laser.advance();
		lasers.push(laser);
	}
}

uint64_t calc(vector<string>& layout, Laser laser) {
		vector<vector<Footprint>> footprints;
		// Initialize footprints
		for (int i = 0; i < layout.size(); ++i) {
			footprints.emplace_back(vector<Footprint>());
			for (int j = 0; j < layout[0].size(); j++) {
				footprints.back().emplace_back(Footprint());
			}
		}

		// Walked paths
		queue<Laser> lasers;
		lasers.push(laser);

		shoot(lasers, layout, footprints);

		int energized = 0;
		for (auto line : footprints) {
			for (auto footprint : line) {
				if (footprint.val != 0) {
					energized++;
				}
			}
		}
		return energized;
}

void part1() {
	fstream file("16.input");
	vector<string> layout;

	// Read the layout
	while (1) {
		string s;
		getline(file, s);

		if (file.eof()) {
			break;
		}

		layout.emplace_back(s);
	}
	
	vector<vector<Footprint>> footprints;
	// Initialize footprints
	for (int i = 0; i < layout.size(); ++i) {
		footprints.emplace_back(vector<Footprint>());
		for (int j = 0; j < layout[0].size(); j++) {
			footprints.back().emplace_back(Footprint());
		}
	}
	// footprints[0][0].bits.E = 1;

	// Walked paths
	queue<Laser> lasers;
	lasers.push({Laser(ZERO, EAST)});

	shoot(lasers, layout, footprints);

	int energized = 0;
	for (auto line : footprints) {
		for (auto footprint : line) {
			if (footprint.val != 0) {
				energized++;
			}
		}
	}

	cout << "Part 1: " << energized << endl;
}

void part2() {
	fstream file("16.input");
	vector<string> layout;

	// Read the layout
	while (1) {
		string s;
		getline(file, s);

		if (file.eof()) {
			break;
		}

		layout.emplace_back(s);
	}

	uint64_t m = 0;
	// Horizontal
	for (int i = 0; i < layout[0].size(); ++i) {
		m = max(calc(layout, {{i, 0}, SOUTH}), m);
		m = max(calc(layout, {{i, layout.size() - 1}, NORTH}), m);
	}
	// Vert
	for (int i = 0; i < layout.size(); ++i) {
		m = max(calc(layout, {{0, i}, EAST}), m);
		m = max(calc(layout, {{layout[0].size() - 1, i}, WEST}), m);
	}

	cout << "Part 2: " << m << endl;
}

int main() {
	auto begin = chrono::high_resolution_clock::now();
	part1(); 
	auto end = chrono::high_resolution_clock::now();
	std::cout << "Part 1 took " << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << " ns" << std::endl;

	auto begin2 = chrono::high_resolution_clock::now();
	part2();
	auto end2 = chrono::high_resolution_clock::now();
	std::cout << "Part 2 took " << chrono::duration_cast<chrono::nanoseconds>(end2-begin2).count() << " ns" << std::endl;
}
