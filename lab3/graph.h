#pragma once 

#include <algorithm>
#include <cstdio>
#include <limits>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>


template<typename TVertex, typename TEdge>
struct link {
	TVertex src;
	TVertex dst;
	TEdge edge;

	link(const TVertex& src, const TVertex& dst, const TEdge& edge) : src(src), dst(dst), edge(edge) { }
};

template
<
	typename TVertex,
	typename TEdge,
	typename TVertexContainer = std::vector<TVertex>,
	typename TEdgeContainer = std::vector<link<TVertex, TEdge>>
>
class graph {
	using map = typename std::unordered_map<TVertex, TEdgeContainer>;
	TVertexContainer _vertices;
	map _edges;

	void dfs(map& edges, TVertex vertex, std::unordered_set<TVertex>& visited) {
		if (visited.find(vertex) == visited.end()) {
			std::cout << vertex;
			visited.insert(vertex);
			auto links = edges[vertex];
			for (auto& i : links) {
				dfs(edges, i.dst, visited);
			}
		}
		
	}

public:
	using VertexIterator = typename TVertexContainer::iterator;
	using AdjacentLinkIterator = typename TEdgeContainer::iterator;
	class EdgeIterator {
		map& _edges;
		typename map::iterator current_vertex_it;
		AdjacentLinkIterator current_link_it;
		friend class graph<TVertex, TEdge, TVertexContainer, TEdgeContainer>;
		EdgeIterator(typename map::iterator current_vertex_it,
			AdjacentLinkIterator current_link_it,
			map& _edges) :
			current_vertex_it(current_vertex_it), current_link_it(current_link_it), _edges(_edges) {}
	public:
		EdgeIterator& operator++() {
			if (current_vertex_it == _edges.end())
				throw std::logic_error("You can't increment end");

			while (current_vertex_it != _edges.end() && current_vertex_it->second.end() == ++current_link_it) {
				++current_vertex_it;
				if (current_vertex_it == _edges.end())
					return *this;
				else {
					current_link_it = current_vertex_it->second.begin();
					if (current_link_it != current_vertex_it->second.end())
						return *this;
					else
						++current_vertex_it;
				}
			}
			return *this;
		}


	

		link< TVertex,TEdge> operator*() {
			return *current_link_it;
		}
		bool operator==(const EdgeIterator& rhs) {
			auto end_vertex_it = _edges.end();
			if (current_vertex_it == end_vertex_it)
				return rhs.current_vertex_it == end_vertex_it;
			if (current_vertex_it == rhs.current_vertex_it && current_link_it == rhs.current_link_it)
				return true;
			else
				return false;
		}
		bool operator != (const EdgeIterator & rhs) {
			return !(*this == rhs);
		}

	};

	void add_vertex(const TVertex& vertex) {
		auto begin = _vertices.begin();
		auto end = _vertices.end();
		if (std::find(begin, end, vertex) != end) {
			throw std::logic_error("Vertex is already inserted!");
		}
		_vertices.insert(_vertices.end(), vertex);
	}

	void add_edge(const TVertex& src, const TVertex& dst, const TEdge& edge) {
		// TODO Проверить src и dst - вершины графа
		link<TVertex, TEdge> new_link(src, dst, edge);
		auto& adjacent_edges = _edges[src];
		adjacent_edges.insert(adjacent_edges.end(), new_link);
	}

	size_t size() const {
		return _vertices.size();
	}

	VertexIterator begin() {
		return _vertices.begin();
	}

	VertexIterator end() {
		return _vertices.end();
	}

	AdjacentLinkIterator begin_adjacent_link(const TVertex& src) {
		return _edges[src].begin();
	}

	AdjacentLinkIterator end_adjacent_link(const TVertex& src) {
		return _edges[src].end();
	}

	EdgeIterator begin_edge() {
		auto map_it = _edges.begin();
		return EdgeIterator(map_it, map_it->second.begin(), _edges);
	}

	EdgeIterator end_edge() {
		return EdgeIterator(_edges.end(), AdjacentLinkIterator(), _edges);
	}

	void dfs() {
		std::unordered_set<TVertex> visited;
		auto begin = _edges.begin();
		dfs(_edges, begin->first, visited);
	}

	void dfs_stack() {
		auto begin = _edges.begin()->first;
		std::unordered_set<TVertex> visited;
		std::stack<TVertex> s;
		s.push(begin);
		while (!s.empty()) {
			auto vertex = s.top();
			s.pop();
			if (visited.find(vertex) != visited.end())
				continue;
			visited.insert(vertex);
			std::cout << vertex;
			auto links = _edges[vertex];
			for (auto& i : links) {
				s.push(i.dst);
			}
		}
	}

	void bfs() {
		auto begin = _edges.begin()->first;
		std::unordered_set<TVertex> visited;
		std::queue<TVertex> q;
		q.push(begin);
		while (!q.empty()) {
			auto vertex = q.front();
			q.pop();
			if (visited.find(vertex) != visited.end())
				continue;
			visited.insert(vertex);
			std::cout << vertex;
			auto links = _edges[vertex];
			for (auto& i : links) {
				q.push(i.dst);
			}
		}
	}

};

struct dijkstra_comparator {
	std::unordered_map<char, double>& d;
	const std::unordered_set<char>& used;

	dijkstra_comparator(std::unordered_map<char, double>& d, const std::unordered_set<char>& used)
		: d(d), used(used) { }

	bool operator()(char src, char dst) {     // src < dst

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