#pragma once

#include <cstdint>
#include <cstdlib>
#include <span>
#include <string>
#include <utility>
#include <vector>

using vec2 = std::pair<int64_t, int64_t>;

template <typename T, typename U>
std::pair<T, U> operator+=(std::pair<T, U>& l, const std::pair<T, U>& r) noexcept {
	l.first += r.first;
	l.second += r.second;
	return l;
}

template <typename T, typename U>
std::pair<T, U> operator+(std::pair<T, U> l, const std::pair<T, U>& r) noexcept {
	l += r;
	return l;
}

template <typename T, typename U>
std::pair<T, U> operator-=(std::pair<T, U>& l, const std::pair<T, U>& r) noexcept {
	l.first -= r.first;
	l.second -= r.second;
	return l;
}

template <typename T, typename U>
std::pair<T, U> operator-(std::pair<T, U> l, const std::pair<T, U>& r) noexcept {
	l -= r;
	return l;
}

template <typename T, typename U>
std::pair<T, T> operator*(const std::pair<T, T>& l, const U r) noexcept {
	return {l.first * r, l.second * r};
}

template <typename U> char& at(std::vector<std::string>& map, const std::pair<U, U>& l) noexcept {
	return map[l.second][l.first];
}

template <typename U>
const char& at(const std::span<std::string> map, const std::pair<U, U>& l) noexcept {
	return map[l.second][l.first];
}

template <typename T, typename U>
inline T& at(std::vector<std::vector<T>>& map, const std::pair<U, U>& l) noexcept {
	return map[l.second][l.first];
}

template <typename U>
inline bool valid(const std::span<std::string> map, const std::pair<U, U>& pos) noexcept {
	return (pos.first < map[0].size() && pos.first >= 0 && pos.second < map.size() &&
			pos.second >= 0);
}

template <> struct std::hash<vec2> {
	std::size_t operator()(const vec2& p) const noexcept {
		return static_cast<uint64_t>(p.first) | (static_cast<uint64_t>(p.second) << 32);
	}
};

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& m) {
	return os << m.first << ", " << m.second;
}

template <typename T, typename U>
std::uint64_t dist(const std::pair<T, U>& a, const std::pair<T, U>& b) {
	return llabs(static_cast<std::int64_t>(a.first) - static_cast<std::int64_t>(b.first)) +
		   llabs(static_cast<std::int64_t>(a.second) - static_cast<std::int64_t>(b.second));
}

constexpr const vec2 UP = std::make_pair(0, -1);
constexpr const vec2 DOWN = std::make_pair(0, 1);
constexpr const vec2 LEFT = std::make_pair(-1, 0);
constexpr const vec2 RIGHT = std::make_pair(1, 0);
