#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../algorithms.hpp"

using namespace std;

vector<string> get() {
	ifstream file(__FILE__ ".txt");
	string line;
	vector<string> out;

	while (getline(file, line)) {
		out.emplace_back(std::move(line));
	}

	return out;
}

void part1() {
	unordered_map<string, vector<string>> map;
	unordered_set<string> chef;
	for (const auto line : get()) {
		const auto nodes = line | views::split('-') | ranges::to<vector<string>>();

		map[nodes[0]].emplace_back(nodes[1]);
		map[nodes[1]].emplace_back(nodes[0]);

		if (nodes[0][0] == 't') {
			chef.emplace(nodes[0]);
		}

		if (nodes[1][0] == 't') {
			chef.emplace(nodes[1]);
		}
	}

	// Now
	vector<string> pairs;
	for (const auto comp : chef) {
		const auto& connections = map[comp];
		for (const auto con : connections) {
			const auto& connections2 = map[con];
			for (const auto con2 : connections2) {
				if (ranges::find(map[con2], comp) != map[con2].end()) {
					vector v = {comp, con, con2};
					ranges::sort(v);
					pairs.emplace_back(v[0] + ',' + v[1] + ',' + v[2]);
				}
			}
		}
	}

	ranges::sort(pairs);
	const auto [f, l] = ranges::unique(pairs);
	pairs.erase(f, l);

	cout << "Part 1:" << pairs.size() << endl;
}

void part2() {
	unordered_map<string, vector<string>> map;
	for (const auto line : get()) {
		const auto nodes = line | views::split('-') | ranges::to<vector<string>>();

		map[nodes[0]].emplace_back(nodes[1]);
		map[nodes[1]].emplace_back(nodes[0]);
	}

	vector<string> P;
	for (auto& [node, neighbors] : map) {
		ranges::sort(neighbors);
		P.emplace_back(node);
	}

	ranges::sort(P);

	vector<vector<string>> cliques;
	bronKerbosch(map, {}, P, {}, cliques);
	
	auto max = *ranges::max_element(cliques, [](const auto& a, const auto& b) { return a.size() < b.size(); });
	ranges::sort(max);

	cout << "Part 2:" << join(max, ",") << '\n';
}

int main() {
	part1();
	part2();
}
