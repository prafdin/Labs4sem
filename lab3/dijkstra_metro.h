#pragma once
#include "graph.h"
#include "metro.h"
#include <unordered_map>
#include <memory>

using route_ptr = std::shared_ptr<route>;
using station_ptr = std::shared_ptr<station>;
using AdjacentLinkIterator = typename graph<station_ptr, route_ptr>::AdjacentLinkIterator;
template <>
struct dijkstra_comparator<station_ptr, size_t > {
    std::unordered_map<station_ptr, size_t>& d;
    const std::unordered_set<station_ptr>& used;
    dijkstra_comparator(std::unordered_map<station_ptr, size_t>& d, const std::unordered_set<station_ptr>& used)
        : d(d), used(used) { }
    bool operator()(station_ptr src, station_ptr dst) {     // src < dst

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


template <>
std::vector<AdjacentLinkIterator> dijkstra(graph<station_ptr, route_ptr>& g, station_ptr src,
    station_ptr dst) {
    std::unordered_map<station_ptr, size_t > d;
    std::unordered_set<station_ptr> used;
    std::unordered_map<station_ptr,std::pair<station_ptr,AdjacentLinkIterator>> p;

    const auto max = std::numeric_limits<size_t>::max();;

    for (const auto& v : g) {
        if (v != src)
            d[v] = max;
        else
            d[v] = 0;
    }

   dijkstra_comparator<station_ptr, size_t> comparator(d, used);
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
            auto& link = *begin;
            auto q = link.dst;
            auto w = link.edge->time;
            if (d[v] + w < d[q]) {
                d[q] = d[v] + w;
                p[q] = std::make_pair(v, begin);
            }
            ++begin;
        }
    }

    std::vector<AdjacentLinkIterator> path;
    auto prev = dst;
  
    while (prev != src) {
        path.push_back(p[prev].second);
        prev = p[prev].first;
    }
    std::reverse(path.begin(), path.end());

    return path;//d[dst];
}


