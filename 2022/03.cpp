#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

constexpr const char* FILE_NAME = "03.input";

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

	uint64_t score = 0;
	for (const std::string& line : lines) {
		string a = line.substr(0, line.length() / 2);
		string b = line.substr(line.length() / 2);
		ranges::sort(a);
		ranges::sort(b);

		vector<char> r;

		ranges::set_intersection(a, b, std::back_inserter(r));

		if (r.size() == 0) {
			continue;
		}

		char c = r[0];
		if (isupper(c)) {
			score += 27 + (c - 'A');
		} else {
			score += 1 + (c - 'a');
		}
	}

	return score;
}

uint64_t part2() {
	auto lines = getFile();

	uint64_t score = 0;
	auto const chunks = lines | views::chunk(3); // three at a time
	for (auto chunk : chunks) {
		ranges::sort(chunk[0]);
		ranges::sort(chunk[1]);
		ranges::sort(chunk[2]);

		vector<char> s1;
		ranges::set_intersection(chunk[0], chunk[1], back_inserter(s1));
		ranges::sort(s1);

		vector<char> s2;
		ranges::set_intersection(s1, chunk[2], back_inserter(s2));
		ranges::sort(s2);

		char c = s2[0];
		if (isupper(c)) {
			score += 27 + (c - 'A');
		} else {
			score += 1 + (c - 'a');
		}
	}

	return score;
}
