#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

enum class Connection {
	drive,
	change,
};

enum class StationType {
	deadend,
	cross,
	pass,
};

std::istream& operator>> (std::istream& in, Connection& point)
{
	std::string tmp;
	in >> tmp;
	if (tmp == std::string("drive")) {
		point = Connection::drive;
	}
	if (tmp == std::string("change")) {
		point = Connection::change;
	}

	return in;
}

std::istream& operator>> (std::istream& in, StationType& point)
{
	std::string tmp;
	in >> tmp;
	if (tmp == std::string("deadend")) {
		point = StationType::deadend;
	}
	if (tmp == std::string("cross")) {
		point = StationType::cross;
	}
	if (tmp == std::string("pass")) {
		point = StationType::pass;
	}

	return in;
}

struct route {
	size_t id_src;
	size_t id_dst;
	Connection type;
	size_t time;

	route(size_t id_src, size_t id_dst, Connection type, size_t time) :id_src(id_src), id_dst(id_dst), time(time), type(type) {}
	size_t src() {
		return id_src;
	}
	size_t dst() {
		return id_src;
	}
	size_t operator()() {
		return time;
	}
	bool operator==(const route& rhs) {
		return (id_src == rhs.id_src && id_dst == rhs.id_dst && type == rhs.type && time == rhs.time);
	}
};

struct station {
	size_t id;
	std::string line_name;
	std::string name;
	StationType type;
	size_t interval;
	double x;
	double y;
	station(size_t id, const std::string& line_name, const std::string& name,
		StationType type, size_t interval, double x, double y) :id(id), line_name(line_name), name(name),
		type(type), interval(interval), x(x), y(y) {}
	bool operator==(const station& rhs) {
		if (name == rhs.name && id == rhs.id)
			return true;
		else
			return false;
	}
};

void parse_route_string(std::vector<std::shared_ptr<route>>& vector, const  std::string& sring) {
	size_t id_src;
	size_t id_dst;
	Connection type;
	size_t time;
	std::istringstream buf(sring);

	buf >> id_src;
	buf >> id_dst;
	buf >> type;
	buf >> time;

	vector.emplace_back(std::make_shared<route>(id_src, id_dst, type, time));
}

void parse_station_string(std::vector< std::shared_ptr<station>>& vector, const  std::string& sring) {
	size_t id;
	std::string line_name;
	std::string name;
	StationType type;
	size_t interval;
	double x;
	double y;
	std::istringstream buf(sring);
	buf >> id;
	std::getline(buf, std::string(), '\t');
	std::getline(buf, line_name, '\t');
	std::getline(buf, name, '\t');
	buf >> type;
	buf >> interval;
	buf >> x;
	buf >> y;

	vector.emplace_back(std::make_shared<station>(id, line_name, name, type, interval, x, y));
}

std::vector<std::shared_ptr<route>> read_routes(const std::string& path) {
	std::ifstream fin(path);
	std::string line;
	std::vector< std::shared_ptr<route>> vector;
	if (fin.is_open()) {
		while (std::getline(fin, line))
		{
			parse_route_string(vector, line);
		}
	}
	return std::move(vector);
}

std::vector<std::shared_ptr<station>> read_stations(const std::string& path) {
	std::ifstream fin(path);
	std::string line;
	std::vector< std::shared_ptr<station>> vector;
	if (fin.is_open()) {
		while (std::getline(fin, line))
		{
			parse_station_string(vector, line);
		}
	}
	return std::move(vector);

}