#include <cassert>
#include <cctype>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

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
	uint64_t sum = 0;
	bool a = true;

	unordered_map<uint64_t, vector<uint64_t>> p;
	for (const auto& line : get()) {
		if (a) {
			if (line.empty()) {
				a = false;
				continue;
			}

			auto v = line | views::split('|') | ranges::to<vector<string>>();

			p.try_emplace(stoull(v[0]), vector<uint64_t>());
			p[stoull(v[0])].emplace_back(stoull(v[1]));
		} else {
			auto v = line | views::split(',') |
					 views::transform([](const auto a) { return atoll(a.data()); }) |
					 ranges::to<vector<uint64_t>>();

			vector<uint64_t> done;
			for (const auto i : v) {
				if (p.contains(i)) {
					for (const auto num : p[i]) {
						if (ranges::find(done, num) != done.end()) {
							goto e;
						}
					}
				}

				done.emplace_back(i);
			}

			sum += v[v.size() / 2];

		e:
		}
	}

	cout << "Part 1:" << sum << endl;
}

void part2() {
	uint64_t sum = 0;
	bool a = true;

	unordered_map<uint64_t, vector<uint64_t>> p;
	for (const auto& line : get()) {
		if (a) {
			if (line.empty()) {
				a = false;
				continue;
			}

			auto v = line | views::split('|') | ranges::to<vector<string>>();

			p.try_emplace(stoull(v[0]), vector<uint64_t>());
			p[stoull(v[0])].emplace_back(stoull(v[1]));
		} else {
			auto v = line | views::split(',') |
					 views::transform([](const auto a) { return atoll(a.data()); }) |
					 ranges::to<vector<uint64_t>>();

			vector<uint64_t> done;
			for (const auto i : v) {
				if (p.contains(i)) {
					for (const auto num : p[i]) {
						if (ranges::find(done, num) != done.end()) {
							goto e;
						}
					}
				}

				done.emplace_back(i);
			}

			continue;
		e:
			// To Fix
			list<uint64_t> correct;
			for (const auto i : v) {
				if (!p.contains(i)) {
					correct.emplace_back(i);
					continue;
				}

				auto mit = correct.end();
				bool end = true;

				for (const auto num : p[i]) {
					if (auto it = ranges::find(correct, num); it != correct.end()) {
						if (distance(correct.begin(), it) < distance(correct.begin(), mit)) {
							mit = it;
						}

						end = false;
					}
				}

				if (end) {
					correct.emplace_back(i);
				} else {
					correct.insert(mit, i);
				}
			}

			auto b = correct.begin();
			advance(b, correct.size() / 2);
			sum += *b;
		}
	}

	cout << "Part 2:" << sum << endl;
}

int main() {
	part1();
	part2();
}
