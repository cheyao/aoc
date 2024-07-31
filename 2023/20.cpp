#include <algorithm>
#include <bits/ranges_algo.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

using namespace std;

// Flip flop
// Default : Low
// High : NONE
// Low  : Flip
#define FLIPFLOP '%'
#define CONJONCTION '&'

#define LOW 0
#define HIGH 1

struct Gate {
	string id;
	char type;
	bool state;

	vector<reference_wrapper<Gate>> mOuts;
	vector<string> mOutIDs;
	vector<reference_wrapper<Gate>> mIns;
	vector<string> mInIDs;
};

using Circuit = unordered_map<string, Gate>;

struct Pulse {
	explicit Pulse(class Gate& _src, bool _level, class Gate& _dst)
	    : src(_src), level(_level), dst(_dst){};

	class Gate& src;
	bool level;
	class Gate& dst;
};

Circuit phrase() {
	ifstream file("20.input");

	Circuit circuit;

	while (1) {
		string line;
		getline(file, line);
		if (file.eof()) {
			break;
		}

		auto split = line | views::split(" -> "sv) |
			     ranges::to<vector<string>>();
		string name = split[0];
		vector<string> destionations = split[1] | views::split(", "sv) |
					       ranges::to<vector<string>>();

		Gate gate;

		if (name == "broadcaster") {
			gate.id = name;
		} else if (name[0] == FLIPFLOP || name[0] == CONJONCTION) {
			gate.id = name.substr(1);
		} else {
			// cerr << "Uhh" << name << endl;
		}

		gate.type = name[0];
		gate.state = LOW;
		gate.mOutIDs = destionations;

		circuit[gate.id] = gate;
	}

	// Initalize the outputs
	for (const auto& [name, gate] : circuit) {
		for (const auto out : gate.mOutIDs) {
			[[unlikely]] if (!circuit.contains(out)) {
				Gate g;
				g.id = out;
				g.type = ' ';
				g.mInIDs = {};
				g.mOutIDs = {};
				circuit[out] = g;
			}
			circuit[out].mInIDs.emplace_back(name);

			circuit[name].mOuts.emplace_back(circuit[out]);
			circuit[out].mIns.emplace_back(circuit[name]);
		}
	}

	return circuit;
}

bool process(const Pulse& p) {
	auto& src = p.src;
	auto& dst = p.dst;

	bool switched = true;

	if (dst.type == FLIPFLOP) {
		if (src.state == LOW) {
			switched = true;
			dst.state = !dst.state;
		} else {
			switched = false;
		}
	} else if (dst.type == CONJONCTION) {
		bool init = true;
		for (auto g : dst.mIns) {
			init = init & g.get().state;
		}
		dst.state = !init;
	} else {
		// cout << "Uncached " << dst.type << endl;
		dst.state = src.state;
	}

	return switched;
}

vector<Pulse> button(Circuit& cir) {
	queue<reference_wrapper<Gate>> front({cir.at("broadcaster")});
	vector<Pulse> pulses;

	while (!front.empty()) {
		Gate& gate = front.front().get();
		// cout << gate.id << endl;
		front.pop();

		for (auto dest : gate.mOuts) {
			Pulse pulse(gate, gate.state, dest);
			pulses.emplace_back(pulse);
			auto switched = process(pulse);
			if (switched) {
				front.emplace(dest);
			}
		}
	}

	return pulses;
}

void part1() {
	auto circuit = phrase();
	uint64_t h = 0, l = 0;
	for (auto i = 0; i < 1000; i++) {
		l++;
		auto pulses = button(circuit);
		for (const auto& pulse : pulses) {
			h += pulse.level;
			l += !pulse.level;
		}
	}
	cout << "Part 1: " << l * h << endl;
}

void part2() {
	Circuit circuit = phrase();

	if (!circuit.contains("rx")) {
		exit(1);
	}
	unordered_map<string, uint64_t> o;
	for (const auto& id : circuit[circuit["rx"].mInIDs.front()].mInIDs) {
		o[id] = 0;
	}

	bool fin = false;
	uint64_t p = 0;
	while (!fin) {
		p++;
		auto pulses = button(circuit);
		for (const auto& pulse : pulses) {
			if (pulse.level == HIGH && o.contains(pulse.src.id) && o[pulse.src.id] == 0) {
				o[pulse.src.id] = p;
			}
		}

		fin = all_of(o.begin(), o.end(), [](auto c) {return c.second != 0;});
	}
end:
	cout << "Part 2: "  << ranges::fold_left(o | views::values, (uint64_t) 1, lcm<uint64_t, uint64_t>) << endl;
}

int main() {
	part1();
	part2();
}

