#pragma once
#include "digestpp/algorithm/sha3.hpp"


class characters {
	std::vector<int> indexes;
	static std::string symbols;
	int number_digits; // разряды		
public:
	characters(long long num) : number_digits(0) {
		long i;
		int t;
		auto count_symbols = symbols.size();
		do {
			i = (int) (num /(long) count_symbols);
			t = (int)(num % (long)count_symbols);
			num = i;
			indexes.push_back(t);
			++number_digits;
		} while (i > count_symbols);
		if (i != 0) {
			indexes.push_back(i - 1);
			++number_digits;
		}
		std::reverse(indexes.begin(), indexes.end());
	}

	void operator=(long long num) {
		number_digits = 0;
		indexes.clear();
		int i;
		int t;
		auto count_symbols = symbols.size();
		do {
			i = (int)(num / (long)count_symbols);
			t = (int)(num % (long)count_symbols);
			num = i;
			indexes.push_back(t);
			++number_digits;
		} while (i > count_symbols);
		if (i != 0) {
			indexes.push_back(i - 1);
			++number_digits;
		}
		std::reverse(indexes.begin(), indexes.end());
	}

	characters() {
		indexes.push_back(0);
		number_digits = 1;
	}

	characters& operator++() {
		bool carry = false;
		auto index_last_char = symbols.size() - 1;
		if (indexes.back() + 1 > index_last_char) { // if need carry
			indexes.back() = 0;
			carry = true;
		}
		else
			++indexes.back();

		for (auto i = (number_digits - 2); i >= 0 && carry == true; --i) {
			if (indexes[i] + 1 > index_last_char) { // if need carry
				indexes[i] = 0;
			}
			else {
				++indexes[i];
				carry = false;
			}
		}
		if (carry == true) {
			indexes.push_back(0);
			++number_digits;
		}
		return *this;
	}

	std::string operator()() {
		std::string tmp;
		for (auto i = 0; i < number_digits; ++i)
			tmp.push_back(symbols[indexes[i]]);
		return tmp;
	}

	static size_t size() {
		return symbols.size();
	}
};
long long count_placements(size_t n, size_t m) {
	return (long long)pow(n, m);
}

std::string characters::symbols = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0','1','2','3','4','5','6','7','8','9' };


std::string brute_force(const std::string& hex, size_t min_size, size_t max_size) {
	std::string find_hex;
	std::string key;
	digestpp::sha3 hasher(256);
	size_t start_num = 0;
	size_t combinations = 0;
	size_t number_digits = characters::size();
	for (auto i = min_size - 1; i > 0; --i)
		start_num += count_placements(number_digits, i);
	for (auto i = max_size; i > 0; --i)
		combinations += count_placements(number_digits, i);
	characters display_num(start_num);
	for (size_t i = start_num; i < combinations; ++i) {
		key = display_num();
		++display_num;
		hasher.reset();
		hasher.absorb(key);
		find_hex = hasher.hexdigest();
		if (find_hex == hex)
			return key;
	}
	return std::string();
}


class brute {
	std::mutex _mutex;
	characters _key;
	std::string _result;
	long long num_current_key;
	long long num_last_key;
	const std::string& hex;

	void check_hex() {
		digestpp::sha3 hasher(256);

		while (true) {
			std::string key;
			_mutex.lock();
			if ((num_current_key == num_last_key) || _result.size() != 0) {
				_mutex.unlock();
				return;
			}
			key = _key();
			++_key;
			++num_current_key;
			_mutex.unlock();
			hasher.reset();
			hasher.absorb(key);
			if (hasher.hexdigest() == hex) {
				_result = key;
				return;
			}
		}
	}


public:
	brute(const std::string& hex, size_t min_size, size_t max_size) : hex(hex), num_current_key(0), num_last_key(0) {
		size_t count_symbols = characters::size();
		for (auto i = min_size - 1; i > 0; --i)
			num_current_key += count_placements(count_symbols, i);
		for (auto i = max_size; i > 0; --i)
			num_last_key += count_placements(count_symbols, i);
		_key = num_current_key;
	}


	std::string result(size_t thread_count = 0) {
		if (thread_count == 0) {
			thread_count = std::thread::hardware_concurrency();
			if (thread_count == 0) {
				thread_count = 1;
			}
		}

		std::vector<std::thread> threads;
		for (size_t i = 0; i < thread_count; ++i) {
			threads.emplace_back(&brute::check_hex, this);
		}

		for (auto& thread : threads) {
			thread.join();
		}


		return _result;
	}


};
