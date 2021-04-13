#include <utility>
#include <vector>

template <typename T>
void select_sort(std::vector<T>& data) {
	size_t size = data.size();
	for (size_t i = 0; i < size-1; ++i) {
		auto index_min =  i+1 ;
		for (size_t j = i + 1; j < size; ++j) {
			if (data[index_min] > data[j])
				index_min = j;
		}
		if (data[index_min] < data[i])
			std::swap(data[index_min], data[i]);
	}

}