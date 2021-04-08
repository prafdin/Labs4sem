#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include "median_filter_set.h"

int DEBAG = 0;

using namespace std;

template <typename T>
bool can_capture(const vector<T>& v, int index, int size_window) {
	size_t directions = 2;
	int count_capturing = size_window - 1;
	int count_capturing_left = count_capturing / directions;
	int count_capturing_right = count_capturing - count_capturing_left;
	if ((index - count_capturing_left) < 0)
		return false;
	else if ((index + count_capturing_right) > (int)v.size() - 1) {
		swap(count_capturing_left, count_capturing_right);
		if ((index + count_capturing_right) > (int)v.size() - 1)
			return false;
	}
	return true;
}

template <typename T, typename Comparator >
void capture(vector<T>& v, int index, vector<reference_wrapper<T>>& window, int size_window) {
	if (window.size() == 0) { 
		size_t directions = 2;
		int count_capturing = size_window - 1;
		int count_capturing_left = count_capturing / directions;
		int count_capturing_right = count_capturing - count_capturing_left;
		auto current_it = v.begin() + index;
		window.insert(window.begin(), current_it - count_capturing_left, current_it + count_capturing_right + 1);
		sort(window.begin(), window.end(), Comparator());
	}
	else {

		if (window.size() > size_window) {
			int  count_capturing_right = (int)(((double)window.size()) / 2 + 0.5);
			while (window.size() > size_window) {
				del_item(window, v[(size_t)index - 1 - count_capturing_right]);
				--count_capturing_right;
			}
			return;
		}

		//size_t index_current = window.size() / 2;
		int  count_capturing_right = (int)(((double)size_window - 1.0) / 2 + 0.5);
		int need_add = fabs((int)window.size() - size_window);
		if (index + count_capturing_right > (int)v.size() - 1)
			count_capturing_right = -count_capturing_right;
		while (need_add > 0) {
			insert_sorted<T>(window, v[(size_t)index + count_capturing_right]);
			--need_add;
			count_capturing_right > 0 ? --count_capturing_right : ++count_capturing_right;
		}
	}
}

template <typename T>
bool del_item(vector<reference_wrapper<T>>& window, T& item) {
	size_t size = window.size();
	for (size_t i = 0; i < size; ++i) {
		if (&window[i].get() == &item) { // if we find del item
			if (DEBAG == 1) cout << "Del :" << *(window.begin() + i) << endl;
			window.erase(window.begin() + i);
			return true;
		}
	}
	return false;;
}

template< typename T>
void insert_sorted(std::vector<reference_wrapper<T>>& vec, T& item) {
	auto tmp = upper_bound<vector<reference_wrapper<T>>::iterator>(vec.begin(), vec.end(), item);
	vec.insert(tmp, ref<T>(item));
}

template <typename T, typename Comparator>
vector<T> median_filter(vector<T> data, size_t size_window) {
	size_t size = data.size();
	vector<T> result;
	vector<reference_wrapper<T>> window;

	for (size_t i = 0; i < size; ++i) {
		size_t current_size_w = size_window;
		while (!can_capture(data, i, current_size_w)) {
			--current_size_w;
		}

		capture<int, Comparator>(data, i, window, current_size_w);

		if (DEBAG == 1) {
			cout << "[" << window.size() << "] ";
			for (auto& i : window) {
				cout << i << " ";
			}
			cout << endl;
		}
		
		result.push_back(window[window.size() / 2 ]);

		int count_capturing_left = (current_size_w - 1) / 2; 
		int  count_capturing_right = (int)(((double)current_size_w - 1.0) / 2 + 0.5);

		if (i + count_capturing_right > data.size() - 1)
			count_capturing_left = count_capturing_right;

		if (current_size_w == size_window)
			del_item(window, data[i - count_capturing_left]);
	}
	return result;
}

int main(void) {

	vector<int> a = { 1,2,4,7,1,4,2,4,7,7,4,2,4,7,7,1,4,2,4,7,7,4,2,4,7,7,1,2,2,4,2,4,7,7,4,2,4,7,7,1 };

	
	auto r = median_filter<int, less<int>>(a, 3);
	auto r1 = median_filter_set<int, less<int>>()(a, 3);
	for (auto& i : a) {
		cout << i << " ";
	}	
	cout << endl;
	for (auto& i : r) {
		cout << i << " ";
	}
	cout << endl;
	for (auto& i : r1) {
		cout << i << " ";
	}
}