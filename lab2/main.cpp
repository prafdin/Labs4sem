#include <cstdio>
#include <unordered_set>
#include "unordered_set.h"
template <typename T> 
using set = unordered_set<T>;//std::unordered_set<T>;

int main(void) {
	set<int> v = {1,3,5,76,3,3,3,3,3,8};	
	for (auto i : v)
		printf("%d ",i);
}