#include <algorithm>
#include <cstdio>
#include <cctype>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "unordered_set.h"
template <typename T>
using set = std::unordered_set<T>;
template <typename Key, typename T>
using map = std::unordered_map<Key, T>;

void tolower_cstr(char* string) {
	while (*string != '\0') {
		*string = std::tolower(*string);
		++string;
	}
}

template <typename It, typename T>
It binary_seacrh(It begin, It end, const T& value) {
	auto item = std::lower_bound(begin, end, value);
	if (item != end && value == *item)
		return item;
	else
		return end;
}

std::vector<std::string> read_file(std::string name) {
	FILE* stream = fopen(name.c_str(), "r");
	const char  sep[] = { "_,./()!&?<>:*" };
	char buffer[20] = { 0 };
	if (stream == NULL)
		throw "no such file";
	std::vector<std::string> result;
	while (!feof(stream)) {
		fscanf(stream, "%19s", buffer);
		tolower_cstr(buffer);

		auto istr = strtok(buffer, sep);
		if (istr != NULL)
			while (istr != NULL) {
				result.push_back(istr);
				istr = strtok(NULL, sep);
			}
		else
			result.push_back(buffer);

		
	}
	fclose(stream);
	return result;
}

template <typename T>
size_t count_duplicate_word(const T& first, const T& second) {
	size_t count = 0;
	set<std::string> tmp(10000);
	for (auto i : first) {
		if (tmp.find(i) == tmp.end())
			if (second.end() != binary_seacrh(second.begin(), second.end(), i)) {
				++count;
				tmp.insert(i);
			}
	}
	return count;
}

template <typename T>
size_t _count_duplicate_word(const T& first, const T& second) {
	size_t count = 0;
	std::vector<std::string> tmp;
	for (auto i : first) {
		std::sort(tmp.begin(), tmp.end());
		if (std::find(tmp.begin(), tmp.end(), i) == tmp.end())
			if (second.end() != binary_seacrh(second.begin(), second.end(), i)) {
				++count;
				tmp.push_back(i);
			}
	}
	return count;
}

template <typename T>
map<std::string, size_t> duplicate_word(const T& first, const T& second) {
	map<std::string, size_t> tmp(3000);
	for (auto i : first) {
		auto item = tmp.find(i);
		if (item == tmp.end()) {
			if (second.end() != binary_seacrh(second.begin(), second.end(), i)) {
				tmp.insert({ i,1 });
			}
		}
		else
			++(item->second);
	}
	return tmp;
}


int main(void) {
	//FILE* stream = fopen("abc.txt", "w");
	auto first = read_file("Sherlock-Holmes-by-Arthur-Conan-Doyle.txt");
	auto second = read_file("Tom-Sawyer-by-Mark-Twain.txt");
	std::sort(first.begin(), first.end());
	std::sort(second.begin(), second.end());
	//auto count_duplicate = _count_duplicate_word(first, second);
	auto tmp = duplicate_word(first, second);
	for (auto& i : tmp) {
		printf("%s %zu\n", i.first.c_str(), i.second);
	}



	/*set<int> v = {1,3,5,76,3,3,3,3,3,8};
	for (auto i : v)
		printf("%d ",i);*/

}