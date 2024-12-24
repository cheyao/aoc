#pragma once

#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "vec2.hpp"

template <typename Range, typename Value = typename Range::value_type>
std::string join(Range const& elements, const char* const delimiter) {
	std::ostringstream os;
	auto b = begin(elements), e = end(elements);

	if (b != e) {
		std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
		b = prev(e);
	}
	if (b != e) {
		os << *b;
	}

	return os.str();
}

template <typename T>
void bronKerbosch(const std::unordered_map<T, std::vector<T>>& edges, std::vector<T> R,
				  std::vector<T> P, std::vector<T> X, std::vector<std::vector<T>>& cliques) {
	if (P.empty() && X.empty()) {
		cliques.emplace_back(R);
		return;
	}

	while (!P.empty()) {
		auto Ri = R;

		auto v = P.back();
		Ri.emplace_back(v);

		decltype(P) Pi;
		decltype(P) Xi;

		std::ranges::set_intersection(edges.at(v), P, back_inserter(Pi));
		std::ranges::set_intersection(edges.at(v), X, back_inserter(Xi));

		bronKerbosch(edges, Ri, Pi, Xi, cliques);

		P.pop_back();
		X.emplace_back(v);
	}
}

template <typename T>
std::unordered_map<vec2, vec2> BFS(const vec2 start, const vec2 end,
								   const std::function<std::vector<vec2>(vec2)> neighbors) {
	std::deque<vec2> frontiere;
	std::unordered_map<vec2, vec2> came_from;

	while (!frontiere.empty()) {
		const auto current = frontiere.front();
		frontiere.pop_front();

		for (const auto& next : neighbors(current)) {
			if (!came_from.contains(next)) {
				frontiere.emplace_back(next);

				came_from[next] = current;
			}
		}
	}

	return came_from;
}
