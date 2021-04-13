#include <utility>
#include <vector>

template <typename T>
void shake_sort(std::vector<T>& data) {
	auto begin = 0;
	auto end = data.size()-1;
	bool even = true;
	auto size = data.size();
	while (begin != end) {
		if (even) {
			for (size_t i = begin; i < end; ++i) {
				if (data[i] > data[i + 1])
					std::swap(data[i], data[i + 1]);
			}
			++begin;
		}
		else {
			for (size_t i = end; i > begin; --i) {
				if (data[i-1] > data[i])
					std::swap(data[i-1], data[i]);
			}
			--end;
		}
		even = !even;
	}

}