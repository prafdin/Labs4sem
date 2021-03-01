#include <cstdio>
#include <vector>
#include  <utility>

template <typename It>
It merge(It bg1, It end1, It bg2, It end2, It dst, bool preorder) { //return past  end
	while (true) {
		if (bg1 != end1 && bg2 != end2) {
			if (*bg1 < *bg2) {
				*dst = *bg1;
				++bg1;
			}
			else {
				*dst = *bg2;
				--bg2;
			}
		}
		else if (bg1 == end1 && bg2 != end2) {
			while (bg2 != end2) {
				*dst = *bg2;
				--bg2;
				preorder ? ++dst : --dst;
			}
			return dst;
		}
		else if (bg1 != end1 && bg2 == end2) {
			while (bg1 != end1) {
				*dst = *bg1;
				++bg1;
				preorder ? ++dst : --dst;
			}
			return dst;
		}
		preorder ? ++dst : --dst;
	}
}

template <typename It>
It max_in_series(It begin, It end, bool preorder) {
	It max_item = preorder ? begin : end;
	while (begin != end) {
		if (preorder ? *(++begin) >= *max_item : *(--end) > *max_item)
			preorder ? ++max_item : --max_item;
		else
			break;
	}
	if (begin == end && preorder ? *end >= *max_item: *begin > *max_item)
		max_item = preorder ? end : begin;
	return max_item;
}

template <typename It>
void insert_interval(It begin, It end, It dst) {
	while (begin != end) {
		*dst++ = *begin++;
	}
	*dst = *begin;
}

template <typename T>
void merge_sort(T& collection) {
	bool all_merged = false;
	bool even = true;
	bool sorted = false;
	T v(collection.size());
	auto dst_begin = v.begin();
	auto dst_end = v.end();
	auto src_begin = collection.begin();
	auto src_end = --collection.end();
	while (!sorted) {
		even = !even;
		all_merged = false;
		for (int i = 0; i < 2; ++i) {

			auto begin_left = src_begin;
			auto past_end_left = max_in_series(src_begin, src_end, true);
			auto begin_right = src_end;
			auto past_end_right = max_in_series(src_begin, src_end, false);
			if (begin_left == begin_right) {
				*dst_end = *begin_right;
				all_merged = true;
				break;
			}
			else if (past_end_left != past_end_right) {
				--past_end_right;
				++past_end_left;
			}
			else {
				all_merged = true;
				++past_end_left;
			}
			if (i == 0) {
				dst_begin = merge(begin_left, past_end_left, begin_right, past_end_right, dst_begin, true);
				if (all_merged) {
					sorted = true;
					break;
				}
			}
			else
				dst_end = merge(begin_left, past_end_left, begin_right, past_end_right, --dst_end, false);
			src_begin = past_end_left;
			src_end = past_end_right;
		}
		if (!all_merged)
			insert_interval(src_begin, src_end, dst_begin);
		dst_begin = even ? v.begin() : collection.begin();
		dst_end = even ? --v.end() : --collection.end();
		src_begin = even ? collection.begin() : v.begin();
		src_end = even ? --collection.end() : --v.end();
	}
	if (!even)
		collection = v;
}



int main(void) {
	std::vector<int> v1 = { 0,0,1,1,3,6,0,5,1,3,6,8,2,4,7,4,1,3,5,6,8,4,1,3,4,4,3};
	for (auto& item : v1) {
		printf("%d ", item);
	}
	merge_sort(v1);
	printf("\n");
	for (auto& item : v1) {
		printf("%d ",item);
	}
}