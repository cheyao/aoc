#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <ranges>

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

int main() {
	part1();
}
