#pragma once
#include "graph.h"

double bellman_ford(graph<char, double>& g, char src, char dst) {
	std::unordered_map<char, double> d;
	auto size = g.size();

	const auto max =
		std::numeric_limits<double>::has_infinity
		? std::numeric_limits<double>::infinity()
		: std::numeric_limits<double>::max();
	for (const auto& v : g) {
		if (v != src)
			d[v] = max;
		else
			d[v] = 0;
	}

	
	auto end = g.end_edge();

	for (size_t i = 0; i < size - 1; ++i) {
		auto begin = g.begin_edge();
		while (begin != end) {
			auto _src = (*begin).dst;
			auto  _dst = (*begin).src;
			auto edge = (*begin).edge;
			if (d[_src] > d[_dst] + edge)
				d[_src] = d[_dst] + edge;
			++begin;
		}
	}
	return d[dst];
}