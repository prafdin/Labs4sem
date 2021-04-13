#include <utility>
#include <vector>

template <typename T>
void insert_sort(std::vector<T>& data) {
	size_t 	size = data.size();
	for (size_t i = 0; i < size; ++i) {
		size_t new_index = i;
		while (new_index != 0 && data[i] < data[new_index - 1]) {
			--new_index;
		}
		for (size_t j = i; j> new_index; --j) {
			std::swap(data[j], data[j - 1]);
		}
	}
}