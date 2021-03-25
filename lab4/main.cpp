#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include "brute_force.h"
#include "collection_interface.h"

void main(void) {
	/*
	size_t min =6 ;
	size_t max = 6;
	std::string hex = "2bc335bf6103d225bddd33d29df13ec5959b24f369c236c1db577641ae8c10e1";
	brute tmp(hex, min, max);
	std::cout << tmp.result();*/


	ICollection<int>* collection = new vector<int>;
	auto &begin = collection->begin();
	collection->insert(begin,1);
	for (auto &i = collection->begin(); i != collection->end();++i) {
		std::cout << *i << std::endl;
	}
}