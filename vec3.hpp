#pragma once

#include <cstdint>
#include <istream>

struct vec3 {
	std::int64_t x, y, z;

	vec3& operator+=(const vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	friend vec3 operator+(vec3 lhs, const vec3& rhs) {
		lhs += rhs;
		return lhs;
	}

	vec3& operator-=(const vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	friend vec3 operator-(vec3 lhs, const vec3& rhs) {
		lhs -= rhs;
		return lhs;
	}

	std::int64_t operator*(const vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	vec3& operator*=(const vec3& other) = delete;
};

std::ostream& operator<<(std::ostream& os, const vec3& obj) {
	os << obj.x << ',' << obj.y << ',' << obj.z;
	return os;
}

