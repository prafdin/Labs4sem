#include <algorithm>
#include "set.h"
#include <set>




void main(void) {
	set<int> a;
	for (auto i = 0; i <40; ++i) {
		a.insert(i);
	}
	
	for (auto& i : a) {
		printf("%d ",i);
	}
	a.erase(15);
	printf("\n");
	//a.debug();
	for (auto& i : a) {
		printf("%d ", i);
	}
}