#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

constexpr const char* FILE_NAME = "05.input";

vector<string> getFile() {
	vector<string> lines;

	ifstream file(FILE_NAME);
	string str;
	while (getline(file, str)) {
		lines.emplace_back(str);
	}
	file.close();

	return lines;
}

uint64_t part1();
uint64_t part2();

int main(int argc, char** argv) {
	cout << "Day " << FILE_NAME << " part 1 : " << part1() << endl;
	cout << "Day " << FILE_NAME << " part 2 : " << part2() << endl;
}

uint64_t part1() {
	auto lines = getFile();

	vector<stack<char>> stacks((lines[0].size() + 1) / 4, stack<char>());
	uint64_t i = 0;
	for (; lines[i].size() != 0; ++i) {
		for (const auto& [index, c] : views::enumerate(lines[i])) {
			if (!isupper(c)) {
				continue;
			}

			stacks[(index - 1) / 4].emplace(c);
		}
	}
	i++;
	for (auto& s : stacks) {
		stack<char> tmpStack;

		while (!s.empty()) {
			tmpStack.emplace(s.top());
			s.pop();
		}

		s = tmpStack;
	}

	for (; i < lines.size(); ++i) {
		const auto [first, last] =
			ranges::remove_if(lines[i], [](const char& c) { return isalpha(c); });
		lines[i].erase(first, last);
		stringstream ss(lines[i]);
		uint64_t count, src, dst;
		ss >> count >> src >> dst;
		src--;
		dst--;

		while (count--) {
			stacks[dst].emplace(stacks[src].top());
			stacks[src].pop();
		}
	}

	cout << "\"";
	for (const auto& s : stacks) {
		cout << s.top();
	}
	cout << "\"" << endl;

	return 0;
}

uint64_t part2() {
	auto lines = getFile();

	vector<stack<char>> stacks((lines[0].size() + 1) / 4, stack<char>());
	uint64_t i = 0;
	for (; lines[i].size() != 0; ++i) {
		for (const auto& [index, c] : views::enumerate(lines[i])) {
			if (!isupper(c)) {
				continue;
			}

			stacks[(index - 1) / 4].emplace(c);
		}
	}
	i++;
	for (auto& s : stacks) {
		stack<char> tmpStack;

		while (!s.empty()) {
			tmpStack.emplace(s.top());
			s.pop();
		}

		s = tmpStack;
	}

	for (; i < lines.size(); ++i) {
		const auto [first, last] =
			ranges::remove_if(lines[i], [](const char& c) { return isalpha(c); });
		lines[i].erase(first, last);
		stringstream ss(lines[i]);
		uint64_t count, src, dst;
		ss >> count >> src >> dst;
		src--;
		dst--;

		stack<char> tmp;
		while (count--) {
			tmp.emplace(stacks[src].top());
			stacks[src].pop();
		}
		while (!tmp.empty()) {
			stacks[dst].emplace(tmp.top());
			tmp.pop();
		}
	}

	cout << "\"";
	for (const auto& s : stacks) {
		cout << s.top();
	}
	cout << "\"" << endl;

	return 0;
}
