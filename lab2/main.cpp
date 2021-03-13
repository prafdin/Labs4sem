#include <algorithm>
#include "set.h"
#include <set>




void main(void) {
	set<int> a;
	for (auto i = 0; i < 500; ++i) {
		a.insert(i);
	}
	
	for (auto& i : a) {
		printf("%d ",i);
	}


}