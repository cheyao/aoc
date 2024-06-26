#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/types.h>
#include <vector>

using namespace std;

struct Vector3 {
	int64_t x, y, z;

	Vector3(const int64_t& _x = 0, const int64_t& _y = 0, const int64_t& _z = 0) : x(_x), y(_y), z(_z) {};
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
		out << vec.x << ", " << vec.y << ", " << vec.z;
		return out;
	}
	friend istream& operator>>(istream& in, Vector3& vec) {
		in >> vec.x >> vec.y >> vec.z;
		return in;
	}
};

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

#define ZERO Vector2(0, 0)

struct Hail {
	Vector3 pos;
	Vector3 vel;

	explicit Hail(string s) {
		for (char& c : s) {
			if (c == ',') {
				c = ' ';
			}
		}
		stringstream ss(s);
		ss >> *this;
	}

	friend ostream& operator<<(ostream& out, const Hail& hail) {
		out << hail.pos << " @ " << hail.vel;
		return out;
	}
	friend istream& operator>>(istream& in, Hail& hail) {
		char c;
		in >> hail.pos >> c >> hail.vel;

		assert(c == '@');

		return in;
	}
};

void part1() {
	ifstream file("24.input");
	vector<Hail> hails;
	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}
		hails.emplace_back(Hail(line));
	}

	// Maths corner
	// https://stackoverflow.com/questions/16524096/how-to-calculate-the-point-of-intersection-between-two-lines
	// f(*) = s1(x - xp) + yp
	// = (s1) * x (- s1 * xp1 + yp1)
	// = (s2) * x (- s2 * xp2 + yp2)
	// y = m2 * x + c2
	// intersectionX = (c2 - c1) / (m1 - m2)
	// ((-s2 * xp2 + yp2) - (-s1 * xp1 + yp1)) / (s1 - s2)
	// (-s2 * xp2 + yp2 + s1 * xp1 - yp1) / (s1 - s2)
	// (yp2 - yp1 - s2 * xp2 - s1 * xp1) / (s1 - s2)
	// (m1*c1 - c2*m2) / m1-m2
	// m1 * intersectionX + c1
	// s1 * x - s1 * xp1 + yp1
	constexpr uint64_t m = 200000000000000;
	constexpr uint64_t M = 400000000000000;

	uint64_t sum = 0;

	for (uint64_t i = 0; i < hails.size(); i++) {
		const Hail& src = hails[i];
		const double srcs = static_cast<double>(src.vel.y) / static_cast<double>(src.vel.x);

		for (uint64_t j = i + 1; j < hails.size(); j++) {
			const Hail& dst = hails[j];
			const double dsts = static_cast<double>(dst.vel.y) / static_cast<double>(dst.vel.x);

			if (srcs == dsts) {
				continue;
			}

			// Complex maths
			auto x = (dst.pos.y - src.pos.y - dsts * dst.pos.x + srcs * src.pos.x) / (srcs - dsts);
			// auto y = srcs * x - srcs * src.pos.x + src.pos.y;
			
			auto t1 = (x - src.pos.x) / src.vel.x, t2 = (x - dst.pos.x) / dst.vel.x;
			if (t1 < 0 || t2 < 0) continue;
			auto y = src.pos.y + src.vel.y * t1;

			sum += (x >= m && x <= M && y >= m && y <= M);
		}
	}

	cout << "Part 1: " << sum << endl;
}

int main() {
	part1();
}
