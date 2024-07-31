#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::tuple<std::string, std::string, std::uint64_t>> hands;

int main() {
	// Ooo one liner
	std::ifstream file("07.input");
	while (1) {
		std::string line;
		std::getline(file, line);
		if (file.eof()) {
			break;
		}
		std::string s = line.substr(0, line.find(' '));
		hands.push_back(std::tuple<std::string, std::string, std::uint64_t>(s, s, std::stol(line.substr(line.find(' ')))));
	}

	uint64_t total = 0, n = 0;
	std::cout << [&total, &n](auto sorted) { while (!sorted.empty()) { n++; total += n * std::get<2>(sorted.top()); sorted.pop(); } return total; } (std::priority_queue< std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t>, std::vector<std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t>>, auto (*)(std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t>&, std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t>&) -> bool> {[](std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t>& first, std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t>& second) -> bool { return std::get<0>(first) == std::get<0>(second) ? std::accumulate(std::get<1>(first).begin(), std::get<1>(first).end(), static_cast<std::uint64_t>(0), [](std::uint64_t acc, std::uint64_t obj){ return acc * 100 + obj; }) > std::accumulate(std::get<1>(second).begin(), std::get<1>(second).end(), static_cast<std::uint64_t>(0), [](std::uint64_t acc, std::uint64_t obj){ return acc * 100 + obj; }) : std::get<0>(first) > std::get<0>(second); }, hands | std::views::transform([](std::tuple<std::string, std::string, std::uint64_t>& hands) -> std::tuple<std::uint64_t, std::vector<std::uint64_t>, std::uint64_t> { return { /* Tansform the string! */ [](std::vector<std::uint64_t> vec)-> std::uint64_t { return std::accumulate(vec.begin(), vec.end(), static_cast<std::uint64_t>(0)); }(std::get<0>(hands) | std::views::transform([&hands](char c) -> std::uint64_t { return std::count(std::get<0>(hands).begin(), std::get<0>(hands).end(), c); }) | std::ranges::to<std::vector>()), std::get<1>(hands) | std::views::transform([valueMap = std::unordered_map<char, char>{{'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}}](char character) -> std::uint64_t { return valueMap.contains(character) ? valueMap.at(character) : character - '1'; }) | std::ranges::to<std::vector>(), std::get<2>(hands)};}) | std::ranges::to<std::vector>()});

	return 0;
}

