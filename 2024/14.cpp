#include <cassert>
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

int64_t W = 101;
int64_t H = 103;
int64_t MIDDLE_X = W / 2;
int64_t MIDDLE_Y = H / 2;

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;
	string buf;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

using vec2 = pair<int64_t, int64_t>;

template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U>& l, const std::pair<T, U>& r) {
	return {l.first + r.first, l.second + r.second};
}

template <typename T, typename U> std::pair<T, T> operator*(const std::pair<T, T>& l, const U r) {
	return {l.first * r, l.second * r};
}

void part1() {
	uint64_t sum = 0;

	uint64_t a = 0;
	uint64_t b = 0;
	uint64_t c = 0;
	uint64_t d = 0;

	for (const auto& line : get()) {
		auto _line =
			line | views::split('=') |
			ranges::views::transform([](auto i) { return i | ranges::views::split(' '); }) |
			views::join |
			ranges::views::transform([](auto i) { return i | ranges::views::split(','); }) |
			views::join | ranges::to<vector<string>>();

		const vec2 pos = make_pair(stoll(_line[1]), stoll(_line[2]));
		const vec2 speed = make_pair(stoll(_line[4]), stoll(_line[5]));

		vec2 dst = pos + speed * 100;

		dst.first %= W;
		dst.second %= H;

		if (dst.first < 0) {
			dst.first += W;
		}
		if (dst.second < 0) {
			dst.second += H;
		}

		// TL
		if (dst.first < MIDDLE_X && dst.second < MIDDLE_Y) {
			cout << dst.first << ':' << dst.second << '\n';
			++a;
		}

		// TR
		if (dst.first > MIDDLE_X && dst.second < MIDDLE_Y) {
			++b;
		}

		if (dst.first > MIDDLE_X && dst.second > MIDDLE_Y) {
			++c;
		}

		if (dst.first < MIDDLE_X && dst.second > MIDDLE_Y) {
			++d;
		}
	}

	sum = a * b * c * d;

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;

	vector<pair<vec2, vec2>> robots;
	for (const auto& line : get()) {
		auto _line =
			line | views::split('=') |
			ranges::views::transform([](auto i) { return i | ranges::views::split(' '); }) |
			views::join |
			ranges::views::transform([](auto i) { return i | ranges::views::split(','); }) |
			views::join | ranges::to<vector<string>>();

		robots.emplace_back(make_pair(make_pair(stoll(_line[1]), stoll(_line[2])),
									  make_pair(stoll(_line[4]), stoll(_line[5]))));
	}

	while (1) {
		++sum;
		vector<string> map(H, string(W, '.'));

		for (const auto& [pos, speed] : robots) {
			vec2 dst = pos + speed * sum;

			dst.first %= W;
			dst.second %= H;

			if (dst.first < 0) {
				dst.first += W;
			}
			if (dst.second < 0) {
				dst.second += H;
			}

			map[dst.second][dst.first] = '*';
		}

		bool y = false;
		for (const auto& l : map) {
			if (l.contains("**********")) {
				y = true;

				break;
			}
		}

		if (y) {
			for (const auto& l : map) {
				cout << l << '\n';
			}

			break;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main(int argc, char** argv) {
	if (argc == 3) {
		W = atoll(argv[1]);
		H = atoll(argv[2]);
		MIDDLE_X = W / 2;
		MIDDLE_Y = H / 2;
		cout << W << ',' << H << endl;
	}

	part1();
	part2();
}
