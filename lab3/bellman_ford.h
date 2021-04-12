#pragma once
#include "graph.h"

template < typename TVertex, typename TEdge>
double bellman_ford(graph<TVertex, TEdge>& g, TVertex src, TVertex dst) {
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
			auto edge = (*begin);
			auto _src = edge.dst();
			auto  _dst = edge.src();
			auto w = edge();
			if (d[_src] > d[_dst] + w)
				d[_src] = d[_dst] + w;
			++begin;
		}
	}
	return d[dst];
}