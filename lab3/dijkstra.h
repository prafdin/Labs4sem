#pragma once
#include "graph.h"

double dijkstra(graph<char, double>& g, char src, char dst) {

    std::unordered_map<char, double> d;
    std::unordered_set<char> used;
    std::unordered_map<char, char> p;

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

    dijkstra_comparator comparator(d, used);
    const auto n = g.size();
    for (size_t i = 0; i < n; ++i) {
        // иру 1
        auto v = *std::min_element(g.begin(), g.end(), comparator);
        if (d[v] == max) {
            break;
        }

        // иру 2
        used.insert(v);

        // иру 3
        auto begin = g.begin_adjacent_link(v);
        auto end = g.end_adjacent_link(v);
        while (begin != end) {
            auto link = *begin;
            auto q = link.dst;
            auto w = link.edge;
            if (d[v] + w < d[q]) {
                d[q] = d[v] + w;
                p[q] = v;
            }
            ++begin;
        }
    }

    return d[dst];
}

