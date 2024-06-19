#include <cstdint>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <utility>
#include <vector>

using namespace std;
using std::operator""sv;

typedef tuple<uint64_t, uint64_t, uint64_t, uint64_t> Rating;

string process(Rating rating, string code) {
	vector<string> instructions = code | views::split(","sv) | ranges::to<vector<string>>();
	string def = instructions.back();
	instructions.pop_back();

	for (auto inst : instructions) {
		// Ugh
		// a<2006:qkq
		char let = inst[0];
		char comp = inst[1];
		inst.erase(0, 2);
		uint64_t num = stoi(inst.substr(0, inst.find(':')));
		inst.erase(0, inst.find(':') + 1);

		uint64_t toComp;
		switch (let) { //{x=787,m=2655,a=1222,s=2876}
			case 'x':
				toComp = get<0>(rating);
				break;
			case 'm':
				toComp = get<1>(rating);
				break;
			case 'a':
				toComp = get<2>(rating);
				break;
			case 's':
				toComp = get<3>(rating);
				break;
		}
		if (comp == '<') {
			if (toComp < num) {
				def = inst;
				break;
			}
		} else if (comp == '>') {
			if (toComp > num) {
				def = inst;
				break;
			}
		} else {
			cerr << "Oh no" << endl;
		}
	}

	return def;
}

void part1() {
	ifstream file("19.input");
	unordered_map<string, string> functions;
	// Read functions
	while (1) {
		string line;
		getline(file, line);
		if (line.empty()) {
			break;
		}
		string title = line.substr(0, line.find('{'));
		string func = line.substr(line.find('{') + 1, (line.find('}') - line.find('{') - 1));

		functions[title] = func;
	}

	uint64_t sum = 0;

	// Now the data
	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}

		regex ratingRegex("\\{\\w=(\\d+),\\w=(\\d+),\\w=(\\d+),\\w=(\\d+)\\}"); //TODO ATTention
		line = regex_replace(line, ratingRegex, "$1 $2 $3 $4");

		stringstream lineStream(line);
		Rating rating;
		lineStream >> get<0>(rating) >> get<1>(rating) >> get<2>(rating) >> get<3>(rating);

		// Process
		string func = "in";
		while (func != "R" && func != "A") {
			func = process(rating, functions[func]);
		}

		if (func == "A") {
			// cout << "Accepted";
			sum += get<0>(rating);
			sum += get<1>(rating);
			sum += get<2>(rating);
			sum += get<3>(rating);
		}
		else if (func == "R") {
			// cout << "Rej";
		} else {
			cout << "Wut";
		}
	}

	cout << "Part 1: " << sum << endl;

	file.close();
}

typedef struct Function {
	char gear;
	char op;
	uint64_t val;
	string to;
} Function;

using Functions = unordered_map<string, vector<Function>>;
using Gear = unordered_map<char, pair<uint64_t, uint64_t>>;

[[nodiscard]] pair<Gear, Gear> split(Gear g, Function f) {
	Gear out = g;
	if (f.op == '<') {
		g[f.gear].second = min(g[f.gear].second, f.val);
		out[f.gear].first = max(out[f.gear].first, f.val);
		return (pair<Gear, Gear>) {g, out};
	}
	if (f.op == '>') {
		g[f.gear].first = max(g[f.gear].first, f.val + 1);
		out[f.gear].second = min(out[f.gear].second, f.val + 1);
		return (pair<Gear, Gear>) {g, out};
	}

	unreachable();
}

uint64_t rate(const Functions& functions){
	uint64_t sum = 0;
	vector<pair<string, Gear>> ratings = {{"in", {{'x', {1, 4001}},
	 						  {'m', {1, 4001}},
							  {'a', {1, 4001}},
							  {'s', {1, 4001}}}}};

	while (!ratings.empty()) {
		auto [func, gear] = ratings.back();
		ratings.pop_back();
		
		[[unlikely]] if (func == "R") {
			continue; // Discard
		}

		[[unlikely]] if (func == "A") {
			const auto vals = gear | views::values;
			sum += ranges::fold_left(vals.begin(), vals.end(), 1, [] (const auto& last, const auto& num) { return last * (num.second - num.first); });
			continue;
		}

		for (const auto& rule : functions.at(func)) {
			[[unlikely]] if (rule.op == ' ') {
				ratings.push_back({rule.to, gear});
				break;
			}

			auto [thing, other] = split(gear, rule);
			ratings.push_back({rule.to, thing});
			gear = other;
		}
	}

	return sum;
}

void part2() {
	ifstream file("19.input");
	Functions functions;
	// Read functions
	while (1) {
		string line;
		getline(file, line);
		if (line.empty()) {
			break;
		}
		const string title = line.substr(0, line.find('{'));
		string func = line.substr(line.find('{') + 1, (line.find('}') - line.find('{') - 1));

		vector<string> instructions = func | views::split(","sv) | ranges::to<vector<string>>();
		string def = instructions.back();
		instructions.pop_back();

		for (auto inst : instructions) {
			// Process the data
			Function func;
			func.gear = inst[0];
			func.op = inst[1];
			inst.erase(0, 2);
			func.val = stoi(inst.substr(0, inst.find(':')));
			inst.erase(0, inst.find(':') + 1);
			func.to = inst;

			functions[title].push_back(func);
		}
		
		functions[title].push_back({.gear = ' ', .op = ' ', .val = 0, .to = def});
	}

	cout << "Part 2: " << rate(functions) << endl;

	file.close();
}

int main() {
	part1();
	part2();
}
