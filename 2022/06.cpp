#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <string>

using namespace std;

uint64_t part1(string s);
uint64_t part2(string s);

int main(int argc, char** argv) {
	cout << "Day 06 part 1 : " << part1(argv[1]) << endl;
	cout << "Day 06 part 2 : " << part2(argv[1]) << endl;
}

uint64_t part1(string s) {
	auto slides = s | views::slide(4);

	uint64_t c = 0;
	for (auto slide : slides) {
		string sl = slide | ranges::to<string>();
		ranges::sort(sl);
		const auto ret = ranges::unique(sl);

		if (ret.size() == 0) {
			break;
		}

		c++;
	}

	return c + 4;
}

uint64_t part2(string s) {
	auto slides = s | views::slide(14);

	uint64_t c = 0;
	for (auto slide : slides) {
		string sl = slide | ranges::to<string>();
		ranges::sort(sl);
		const auto ret = ranges::unique(sl);

		if (ret.size() == 0) {
			break;
		}

		c++;
	}

	return c + 14;
}
