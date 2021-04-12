#pragma once
#include "graph.h"

template <typename TVertex, typename TEdge, typename Weight >
Weight dijkstra(graph<TVertex, TEdge>& g, TVertex src, TVertex dst) {
    using AdjacentLinkIterator = typename graph<TVertex, TEdge>::AdjacentLinkIterator;
    std::unordered_map<TVertex, TEdge> d;
    std::unordered_set<TVertex> used;
    std::unordered_map<TVertex,std::pair<TVertex, AdjacentLinkIterator>> p;

    const auto max =
        std::numeric_limits<TEdge>::has_infinity
        ? std::numeric_limits<TEdge>::infinity()
        : std::numeric_limits<TEdge>::max();
    for (const auto& v : g) {
        if (v != src)
            d[v] = max;
        else
            d[v] = 0;
    }

    dijkstra_comparator comparator(d, used);
    const auto n = g.size();
    for (size_t i = 0; i < n; ++i) {
        // step 1
        auto v = *std::min_element(g.begin(), g.end(), comparator);
        if (d[v] == max) {
            break;
        }

        // step 2
        used.insert(v);

        // step 3
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

template <typename TVertex, typename TEdge>
struct dijkstra_comparator {
    std::unordered_map<TVertex, TEdge>& d;
    const std::unordered_set<char>& used;
    dijkstra_comparator(std::unordered_map<TVertex, TEdge>& d, const std::unordered_set<TVertex>& used)
        : d(d), used(used) { }
    bool operator()(TVertex src, TVertex dst) {     // src < dst

        auto end = used.end();
        auto src_used = used.find(src) != end;
        auto dst_used = used.find(dst) != end;
        if (src_used && dst_used) {
            return d[src] < d[dst];
        }
        else if (src_used) {
            return false;
        }
        else if (dst_used) {
            return true;
        }
        else {
            return d[src] < d[dst];
        }
    }
};