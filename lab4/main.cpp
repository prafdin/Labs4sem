#include <cstdio>
#include <iostream>
#include <cmath>
#include "digestpp/algorithm/sha3.hpp"

using namespace std;

const string symbols = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0','1','2','3','4','5','6','7','8','9' };


string display_set(size_t num) {
	string key;
	auto size_set = symbols.size();
	auto integral_part = num / size_set;
	auto tail = num % size_set;
	auto count_36 = integral_part / size_set;

	if (integral_part > size_set) {
		auto raz = integral_part - size_set;
		auto item = display_set(raz);
		key.append(item.c_str());
		integral_part -= raz;
	}

	if (integral_part != 0)
		key.push_back(symbols[integral_part - 1]);

	key.push_back(symbols[tail]);

	return key;
}

string rec(size_t num) {
	string key;
	auto size_set = symbols.size();
	vector<size_t> abc;
	auto i = num / size_set;
	auto t = num % size_set;
	size_t count = 0;
	if (i > 0) {
		if (i > size_set) {
			count = i / size_set;
			abc.push_back(count);
		}
		i = i - count * size_set;
		if (i != 0)
			abc.push_back(i - 1);
		else
			abc.push_back(i);
	}
	abc.push_back(t);
	return string();
}


size_t count_placements(size_t n, size_t m) {
	return pow(n, m);
}



string brute_force(const string& hex, size_t min_size, size_t max_size) {
	string find_hex;
	string key;
	digestpp::sha3 hasher(256);
	size_t start_num = 0;
	size_t combinations = 0;
	size_t count_symbols = symbols.size();
	for (auto i = min_size - 1; i > 0; --i)
		start_num += count_placements(count_symbols, i);
	for (auto i = max_size; i > 0; --i)
		combinations += count_placements(count_symbols, i);


	//for (size_t i = start_num; i < combinations; ++i) {
	//	key = display_set(i);
	//	hasher.reset();
	//	hasher.absorb(key);
	//	find_hex = hasher.hexdigest();
	//	if (find_hex == hex)
	//		return key;
	//}
	return string();
}


void main(void) {
	string hex("b39c14c8da3b23811f6415b7e0b33526d7e07a46f2cf0484179435767e4a8804");
	//cout << brute_force(hex,2,2);
	rec(47987);
	cout << display_set(1333);


}