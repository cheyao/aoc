#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
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

unordered_map<string, class Gate*> stog;
unordered_map<string, bool> registers;
unordered_map<string, vector<class Gate*>> dependencies;

class Gate {
  public:
	Gate(string a, string b, string out) : an(a), bn(b), outn(out) {}
	virtual ~Gate() = default;

	void donedep(string d) {
		if (d == an) {
			ad = true;
		} else if (d == bn) {
			bd = true;
		}

		if (ad && bd) {
			flop();
		}
	}

  protected:
	void flop() {
		a = registers[an];
		b = registers[bn];
		registers[outn] = output();

		// Call dependencies
		for (auto* gate : dependencies[outn]) {
			gate->donedep(outn);
		}
	}

	virtual int output() = 0;

	// Non init
	int a = -1, b = -1;
	string an, bn, outn;
	bool ad = false, bd = false;
};

class AND final : public Gate {
  public:
	AND(string a, string b, string out) : Gate(a, b, out) {}
	~AND() override = default;

  private:
	int output() override { return a & b; }
};

class XOR final : public Gate {
  public:
	XOR(string a, string b, string out) : Gate(a, b, out) {}
	~XOR() override = default;

  private:
	int output() override { return a ^ b; }
};

class OR final : public Gate {
  public:
	OR(string a, string b, string out) : Gate(a, b, out) {}
	~OR() override = default;

  private:
	int output() override { return a | b; }
};

void part1() {
	uint64_t sum = 0;
	bool p2 = false;

	vector<string> nondependent;

	for (const auto line : get()) {
		if (line.empty()) {
			p2 = true;

			continue;
		}

		if (!p2) {
			const auto l = line | views::split(": "sv) | ranges::to<vector<string>>();

			registers[l[0]] = stoull(l[1]);

			nondependent.emplace_back(l[0]);
		} else {
			const auto l = line | views::split(' ') | ranges::to<vector<string>>();

			Gate* g;
			switch (l[1][0]) {
				case 'A':
					g = new AND(l[0], l[2], l[4]);
					break;
				case 'X':
					g = new XOR(l[0], l[2], l[4]);
					break;
				case 'O':
					g = new OR(l[0], l[2], l[4]);
					break;
			}

			dependencies[l[0]].emplace_back(g);
			dependencies[l[2]].emplace_back(g);
		}
	}

	for (const auto& reg : nondependent) {
		for (auto* gate : dependencies[reg]) {
			gate->donedep(reg);
		}
	}

	for (const auto& [name, val] : registers) {
		if (name[0] == 'z') {
			sum += static_cast<uint64_t>(val) << atoll(&name[1]);
		}
	}

	cout << "Part 1:" << sum << endl;

	for (const auto& [_, gate] : stog) {
		delete gate;
	}

	stog.clear();
	registers.clear();
	dependencies.clear();
}

void part2() {
	uint64_t sum = 0;
	bool p2 = false;
	ofstream file(__FILE__ ".dot");
	file << "digraph A {\n";

	vector<string> nondependent;

	long long regsize = 0;
	uint64_t i = 0;

	for (const auto line : get()) {
		if (line.empty()) {
			p2 = true;

			continue;
		}

		if (!p2) {
			const auto l = line | views::split(": "sv) | ranges::to<vector<string>>();

			registers[l[0]] = stoull(l[1]);

			nondependent.emplace_back(l[0]);

			if (l[0][0] == 'x') {
				regsize = max(regsize, atoll(&l[0][1]));
			}
		} else {
			const auto l = line | views::split(' ') | ranges::to<vector<string>>();

			Gate* g;
			switch (l[1][0]) {
				case 'A':
					g = new AND(l[0], l[2], l[4]);
					break;
				case 'X':
					g = new XOR(l[0], l[2], l[4]);
					break;
				case 'O':
					g = new OR(l[0], l[2], l[4]);
					break;
			}

			file << l[0] << " -> " << l[1] << i << '\n';
			file << l[2] << " -> " << l[1] << i << '\n';
			file << l[1] << i << " -> " << l[4] << '\n';
			file << l[1] << i << " [label=" << l[1] << "]\n";
			++i;

			dependencies[l[0]].emplace_back(g);
			dependencies[l[2]].emplace_back(g);
		}
	}

	for (const auto& reg : nondependent) {
		for (auto* gate : dependencies[reg]) {
			gate->donedep(reg);
		}
	}

	for (const auto& [name, val] : registers) {
		if (name[0] == 'z') {
			sum += static_cast<uint64_t>(val) << atoll(&name[1]);
		}
	}

	for (const auto& [_, gate] : stog) {
		delete gate;
	}
	file << "}\n";

	// Help me :(
	vector<string> v = {"fgc", "z12", "mtj", "z29", "dtv", "z37", "dgr", "vvm"};
	ranges::sort(v);

	cout << "Part 2:" << join(v, ",") << "\nRun dot -Tsvg " __FILE__ " > " __FILE__ ".svg and look for bad wires" << '\n';
}

int main() {
	part1();
	part2();
}
