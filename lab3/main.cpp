#include <iostream>
#include <vector>
#include <functional>
#include "dijkstra.h"
#include "dijkstra_metro.h"
#include "bellman_ford.h"
#include "metro.h"
#include "Windows.h"
#include <memory>

using vertex = std::shared_ptr<station>;
using edge = std::shared_ptr<route>;

class station_predicat {
	size_t id_station;
public:
	station_predicat(size_t id) :id_station(id) {}
	bool operator()(const vertex& station) const {
		return station->id == id_station;
	}
};

void add_stations(std::vector<vertex>& stations, graph<vertex, edge>& graph) {
	for (auto& i : stations) {
		graph.add_vertex(i);
	}
}

void add_routes(std::vector<vertex>& station, std::vector<edge>& routes, graph<vertex, edge>& graph) {
	for (auto& i : routes) {
		auto& src = std::find_if(station.begin(), station.end(), station_predicat(i->id_src));
		auto& dst = std::find_if(station.begin(), station.end(), station_predicat(i->id_dst));
		graph.add_edge(*src, *dst, i);
	}
}

void print_path(std::ostream& stream, graph<vertex, edge>& a, vertex src, vertex dst) {

	auto path = dijkstra<vertex, edge, std::vector<graph<vertex, edge>::AdjacentLinkIterator>>(a, src, dst);
	size_t time = 0;
	stream << "Путь со станции " << src->name << " до станции " << dst->name <<":"<<std::endl;
	for (auto& i : path) {
		if (i->edge->type == Connection::change) {
			stream << "Пересадка на станции " << i->src->name << ": ";
			stream << i->src->line_name  << " -> " << i->dst->line_name << " [" << i->edge->time << "]" << std::endl;
			time += i->edge->time;
		}
		else {
			stream << "Проезд на линии " << i->src->line_name << ": ";
			stream << i->src->name  << " -> " << i->dst->name << " [" << i->edge->time << "]" << std::endl;
			time += i->edge->time;
		}
	}
	stream << "Время в пути: ~" << time/60 <<" минут" << std::endl;
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	auto routes = read_routes("E:/repos/Labs4sem/moscow-subway-routes.txt");
	auto stations = read_stations("E:/repos/Labs4sem/moscow-subway-stations.txt");
	graph<vertex, edge> a;

	add_stations(stations, a);
	add_routes(stations, routes, a);

	print_path(std::cout, a, stations[20], stations[96]);


}