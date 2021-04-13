#include <iostream>
#include <vector>
#include <utility>

template <typename T>
void bubble_sort(std::vector<T> &data) {
	auto size = data.size();
	for (size_t i = 1; i < size; ++i) {
		for (size_t j = 0; j < size - i; ++j) {
			if (data[j] > data[j+1]) {
				std::swap(data[j], data[j + 1]);
			}
		}
	}
}