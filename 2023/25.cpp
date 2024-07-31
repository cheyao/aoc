#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using std::operator""sv;

void part1() {
	ifstream file("25.input");
	unordered_map<string, unordered_set<string>> componenents;

	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}

		const vector<string> parts = line | views::split(": "sv) | ranges::to<vector<string>>();

		const unordered_set<string> dsts = parts[1] | views::split(" "sv) | ranges::to<unordered_set<string>>();

		componenents[parts[0]].insert(dsts.begin(), dsts.end());

		for (const string& dst : dsts) {
			componenents[dst].insert(parts[0]);
		}
	}

	ofstream out("25.dot");
	out << "strict graph {" << endl;

	for (const auto& [name, tos] : componenents) {
		for (const auto& to : tos) {
			out << name << " -- " << to << endl;
		}
	}
	out << "}" << endl;

	cout << "Run dot -Tsvg 25.dot -Kneato and get the names" << endl;

	const vector<pair<string, string>> todel = {
		{"kbr", "bbg"},
		{"tdk", "czs"},
		{"fht", "vtt"}
	};
	for (const auto& p : todel) {
		componenents[p.first].erase(p.second);
		componenents[p.second].erase(p.first);
	}

	unordered_set<string> viewn;
	queue<string> front;

	front.emplace(todel[0].first);
	viewn.insert(todel[0].first);

	uint64_t sum1 = 0;
	while (!front.empty()) {
		++sum1;
		for (const auto& str : componenents[front.front()]) {
			if (!viewn.contains(str)) {
				front.emplace(str);
				viewn.insert(str);
			}
		}
		front.pop();
	}

	assert(front.empty());
	viewn.clear();

	front.emplace(todel[0].second);
	viewn.insert(todel[0].second);

	uint64_t sum2 = 0;
	while (!front.empty()) {
		++sum2;

		for (const auto& str : componenents[front.front()]) {
			if (!viewn.contains(str)) {
				front.emplace(str);
				viewn.insert(str);
			}
		}

		front.pop();
	}

	cout << "Part 1: " << sum1 * sum2 << endl;
}

int main (int argc, char *argv[]) {
	part1();
	return 0;
}
