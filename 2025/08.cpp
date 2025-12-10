#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <ratio>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

#include "../vec3.hpp"

using namespace std;

static vector<string> get() {
	static vector<string> out;

	if (out.empty()) {
		ifstream file(__FILE__ ".txt");
		string line;

		while (getline(file, line)) {
			if (!line.empty() && ranges::count(line, '.') != line.length()) {
				out.emplace_back(std::move(line));
			}
		}
	}

	return out;
}

struct connection {
	uint64_t dist;
	uintptr_t a;
	uintptr_t b;
};

auto part1() {
	uint64_t sum = 0;

	vector<vec3> junctions;
	vector<connection> connections;

	for (const auto& l : get()) {
		const auto split = l | views::split(',') |
						   views::transform([](const auto& s) { return atoll(s.data()); }) |
						   ranges::to<vector<uint64_t>>();
		junctions.emplace_back(split[0], split[1], split[2]);
	}

	for (uintptr_t i = 0; i < junctions.size(); ++i) {
		for (uintptr_t j = 0; j < i; ++j) {
			assert(i != j);

			const vec3 vec = junctions[i] - junctions[j];

			connections.emplace_back(vec * vec, i, j);
		}
	}

	ranges::sort(connections, {}, &connection::dist);

	// Is connected? If yes to connections[connection]
	vector<uint64_t> connected(junctions.size(), 0);
	// Counting table
	vector<uint64_t> count;

	count.emplace_back(0); // Reserve 0th element

	uint64_t num = 0;
	for (const auto& conn : connections) {
		if (num == 1000) {
			break;
		}

		++num;

		if (connected[conn.a] == 0 && connected[conn.b] == 0) {
			connected[conn.a] = count.size();
			connected[conn.b] = count.size();

			count.emplace_back(2);

			continue;
		}

		if (connected[conn.a] == 0 || connected[conn.b] == 0) {
			const auto m = max(connected[conn.a], connected[conn.b]);

			connected[conn.a] = m;
			connected[conn.b] = m;

			++count[m];
			continue;
		}

		if (connected[conn.a] != connected[conn.b]) {
			// Tie two nets together...
			count[connected[conn.a]] += count[connected[conn.b]];
			count[connected[conn.b]] = 0;
			const auto change = connected[conn.b];
			for (auto& c : connected) {
				if (c == change) {
					c = connected[conn.a];
				}
			}
			continue;
		}
	}

	ranges::sort(count, ranges::greater());

	return count[0] * count[1] * count[2];
}

auto part2() {
	uint64_t sum = 0;

	vector<vec3> junctions;
	vector<connection> connections;

	for (const auto& l : get()) {
		const auto split = l | views::split(',') |
						   views::transform([](const auto& s) { return atoll(s.data()); }) |
						   ranges::to<vector<uint64_t>>();
		junctions.emplace_back(split[0], split[1], split[2]);
	}

	for (uintptr_t i = 0; i < junctions.size(); ++i) {
		for (uintptr_t j = 0; j < i; ++j) {
			assert(i != j);

			const vec3 vec = junctions[i] - junctions[j];

			connections.emplace_back(vec * vec, i, j);
		}
	}

	ranges::sort(connections, {}, &connection::dist);

	// Is connected? If yes to connections[connection]
	vector<uint64_t> connected(junctions.size(), 0);
	// Counting table
	vector<uint64_t> count;

	count.emplace_back(0); // Reserve 0th element

	for (const auto& conn : connections) {
		if (connected[conn.a] == 0 && connected[conn.b] == 0) {
			connected[conn.a] = count.size();
			connected[conn.b] = count.size();

			count.emplace_back(2);

			continue;
		}

		if (connected[conn.a] == 0 || connected[conn.b] == 0) {
			const auto m = max(connected[conn.a], connected[conn.b]);

			connected[conn.a] = m;
			connected[conn.b] = m;

			++count[m];

            if (count[m] == junctions.size()) {
                sum = junctions[conn.a].x * junctions[conn.b].x;
                break;
            }

			continue;
		}

		if (connected[conn.a] != connected[conn.b]) {
			// Tie two nets together...
			count[connected[conn.a]] += count[connected[conn.b]];
			count[connected[conn.b]] = 0;

            if (count[connected[conn.a]] == junctions.size()) {
                sum = junctions[conn.a].x * junctions[conn.b].x;
                break;
            }

			const auto change = connected[conn.b];
			for (auto& c : connected) {
				if (c == change) {
					c = connected[conn.a];
				}
			}
			continue;
		}
	}

	return sum;
}

int main() {
	auto tries = 1;
    auto ignore = 0;

	double time = 0;
	uint64_t result = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part1();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= ignore) {
			time += ms_double.count();
		}
	}
	cout << "Part 1: " << result << endl;
	std::cout << time / (tries - ignore) << "us\n";

	time = 0;
	for (auto i = 0; i < tries; ++i) {
		auto t1 = chrono::high_resolution_clock::now();
		result = part2();
		auto t2 = chrono::high_resolution_clock::now();

		chrono::duration<double, std::micro> ms_double = t2 - t1;
		if (i >= ignore) {
			time += ms_double.count();
		}
	}
	cout << "Part 2: " << result << endl;
	std::cout << time / (tries - ignore) << "us\n";
}
