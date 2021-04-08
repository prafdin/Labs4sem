#pragma once 
#include <iostream>
#include <vector>
#include <set>
using namespace std;

template <typename T, typename Comparator>
class median_filter_set {
	using ref_T = reference_wrapper<const T>;

	multiset<ref_T> window;
	typename multiset<ref_T>::iterator mid;

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
	void del_item(const T& item) {
		auto first_occur = window.find(cref(item));
		while (*first_occur == item) {
			if (&((*first_occur).get()) == &item) {
				if (window.size() % 2 == 0) {
					if (*first_occur >= *mid)
						--mid;
				}
				else {
					if (*first_occur <= *mid)
						++mid;
				}
				window.erase(first_occur);
				return;
			}
			++first_occur;
		}
	}
	void capture(const vector<T>& v, int index, int size_window) {
		if (window.size() == 0) {
			size_t directions = 2;
			int count_capturing = size_window - 1;
			int count_capturing_left = count_capturing / directions;
			int count_capturing_right = count_capturing - count_capturing_left;
			auto current_it = v.begin() + index;
			window.insert(current_it - count_capturing_left, current_it + count_capturing_right + 1);
			mid = window.begin();
			size_t size = window.size() / 2;
			for (size_t i = 0; i < size; ++i)
				++mid;
		}
		else {
			if (window.size() > size_window) {
				int  count_capturing_right = (int)(((double)window.size()) / 2 + 0.5);
				while (window.size() > size_window) {
					del_item(v[(size_t)index - 1 - count_capturing_right]);
					--count_capturing_right;
				}
				return;
			}

			int  count_capturing_right = (int)(((double)size_window - 1.0) / 2 + 0.5);
			int need_add = fabs((int)window.size() - size_window);
			if (index + count_capturing_right > (int)v.size() - 1)
				count_capturing_right = -count_capturing_right;
			while (need_add > 0) {
				window.insert(v[(size_t)index + count_capturing_right]);
				
				if ((window.size() - 1) % 2 == 0) {
					if (*mid > v[(size_t)index + count_capturing_right])
						--mid;
				}
				else {
					if (*mid < v[(size_t)index + count_capturing_right])
						++mid;
				}


				--need_add;
				count_capturing_right > 0 ? --count_capturing_right : ++count_capturing_right;
			}
		}
	}
public:
	median_filter_set() : window(multiset<ref_T>()), mid(window.end()) {}

	vector<T> operator()(const vector<T>& data, size_t size_window) {
		size_t size = data.size();
		vector<T> result;

		for (size_t i = 0; i < size; ++i) {
			size_t current_size_w = size_window;
			while (!can_capture(data, i, current_size_w)) {
				--current_size_w;
			}

			capture(data, i, current_size_w);

			if (DEBAG == 1) {
				cout << " Mid is " << &((*mid).get()) << endl;
				cout << "[" << window.size() << "] ";
				for (auto& i : window) {
					cout << i <<" [" << &(i.get()) << "] ";
				}
				cout << endl;
			}

			result.push_back(*mid);

			int count_capturing_left = (current_size_w - 1) / 2;
			int  count_capturing_right = (int)(((double)current_size_w - 1.0) / 2 + 0.5);

			if (i + count_capturing_right > data.size() - 1)
				count_capturing_left = count_capturing_right;

			if (current_size_w == size_window)
				del_item(data[i - count_capturing_left]);
		}
		return result;

	}

};













