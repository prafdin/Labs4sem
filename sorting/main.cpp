#include <iostream>
#include "bubble_sort.h"
#include "shake_sort.h"
#include "insert_sort.h"
#include "select_sort.h"

int main() {
	std::vector<int> tmp = { 6,1,89,3,2,9 };

	select_sort <int>(tmp);

	for (auto& i : tmp) {
		std::cout << i << " ";
	}
}