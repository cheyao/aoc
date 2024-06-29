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

	int64_t dot(const Vector3& rhs) {
		return this->x*rhs.x + this->y*rhs.y + this->z*rhs.z;
	}
	Vector3 cross(const Vector3& rhs) {
		return Vector3(this->y*rhs.z - this->z*rhs.y, this->z*rhs.x - this->x*rhs.z, this->x*rhs.y - this->y*rhs.x);
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
inline Vector3 operator*(const Vector3& lhs, const double rhs) {
	Vector3 result = lhs;
	result.x *= rhs;
	result.y *= rhs;
	result.z *= rhs;
	return result;
}
inline Vector3 operator*(const Vector3& lhs, const int64_t rhs) {
	Vector3 result = lhs;
	result.x *= rhs;
	result.y *= rhs;
	result.z *= rhs;
	return result;
}
inline Vector3 operator/(const Vector3& lhs, const int64_t rhs) {
	Vector3 result = lhs;
	result.x /= rhs;
	result.y /= rhs;
	result.z /= rhs;
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

/*
Source: Reddit user 

Stones 1 and 2, relative to stone 0:
p1 = position_1 - position_0
v1 = velocity_1 - velocity_0
p2 = position_2 - position_0
v2 = velocity_2 - velocity_0

Let t1 and t2 be the times that the rock collides with hailstones 1 and 2 respectively.

Viewed from hailstone 0, the two collisions are thus at
p1 + t1 * v1
p2 + t2 * v2

Hailstone 0 is always at the origin, thus its collision is at 0. Since all three collisions must form a straight line, the above two collision vectors must be collinear, and their cross product will be 0:

(p1 + t1 * v1) x (p2 + t2 * v2) = 0

Cross product is distributive with vector addition and compatible with scalar multiplication, so the above can be expanded:

(p1 x p2) + t1 * (v1 x p2) + t2 * (p1 x v2) + t1 * t2 * (v1 x v2) = 0

This is starting to look like a useful linear equation, except for that t1 * t2 term. Let's try to get rid of it. Dot product and cross product interact in a useful way. For arbitrary vectors a and b:

(a x b) * a = (a x b) * b = 0.

We can use this property to get rid of the t1 * t2 term. Let's take the dot product with v2. Note that dot product is also distributive with vector addition and compatible with scalar multiplication. The dot product zeros out both the t2 and t1*t2 terms, leaving a simple linear equation for t1:

(p1 x p2) * v2 + t1 * (v1 x p2) * v2 = 0

t1 = -((p1 x p2) * v2) / ((v1 x p2) * v2)

If we use v1 instead of v2 for the dot product, we get this instead:

(p1 x p2) * v1 + t2 * (p1 x v2) * v1 = 0

t2 = -((p1 x p2) * v1) / ((p1 x v2) * v1)

Once we have t1 and t2 we can compute the locations (in absolute coordinates) of the two collisions and work backwards to find the velocity and initial position of the rock.

c1 = position_1 + t1 * velocity_1
c2 = position_2 + t2 * velocity_2
v = (c2 - c1) / (t2 - t1)
p = c1 - t1 * v
*/

void part2() {
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
	file.close();

	const Hail origin = hails.front();
	hails.erase(hails.begin());

	const Hail a = hails.front();
	hails.erase(hails.begin());
	Vector3 p1 = a.pos - origin.pos;
	Vector3 v1 = a.vel - origin.vel;

	const Hail b = hails.front();
	hails.erase(hails.begin());
	Vector3 p2 = b.pos - origin.pos;
	Vector3 v2 = b.vel - origin.vel;

	double t1 = -((double) (p1.cross(p2)).dot(v2)) / ((v1.cross(p2)).dot(v2));
	double t2 = -((double) (p1.cross(p2)).dot(v1)) / ((p1.cross(v2)).dot(v1));
	cout << t1 << endl;
	Vector3 c1 = a.pos + a.vel * t1;
	Vector3 c2 = b.pos + b.vel * t2;
	Vector3 v = (c2 - c1) / (t2 - t1);
	Vector3 p = c1 - v * t1;

	cout << p << " : " << v << endl;
	cout << "Part 2: " << p.x + p.y + p.z << endl;
}

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
	file.close();

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
	part2();
}
