#include <iostream>
#include <cstdio>
#include <vector>
#include <exception>
#include <ctime>
#include <chrono>
#include <algorithm>
class Timer
{
private:
	// ѕсевдонимы типов используютс€ дл€ удобного доступа к вложенным типам
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

template <typename It,typename T>
It foo(It begin, It end, T WhatFind) {
	auto  x = begin; //“екуща€ позици€ массива, с которым сравниваетс€ искомое
	auto a = begin; //Ћева€ граница области, где ведетс€ поиск
	auto  b = end - 1; //ѕрава€ граница области, где ведетс€ поиск


	bool found; 

	for (found = false; (*a < WhatFind) && (*b > WhatFind) && !found; )
	{
		//¬ычисление интерпол€цией следующего элемента, который будет сравниватьс€ с искомым
		x = a + ((WhatFind - *a) * (b - a)) / (*b - *a);
		//ѕолучение новых границ области, если искомое не найдено
		if (*x < WhatFind)
			a = x + 1;
		else if (*x > WhatFind)
			b = x - 1;
		else
			found = true;
	}

	if (*a == WhatFind)
		return a;
	else if (*b == WhatFind)
		return b;
	else
		throw std::exception("not found");
}

template <typename It, typename T>
It binary_search(It begin, It end, const T& value) {
	if (*begin == value)
		return begin;
	else {
		size_t lenght = end - begin;
		if (*(begin + lenght / 2) <= value) {
			return binary_search(begin + lenght / 2, end, value);
		}
		else {
			return binary_search(begin, begin + lenght / 2, value);
		}
	}
}

template <typename It, typename T>
It interpolation_search(It begin, It end, const T& value) {

	while (*begin <= value && *(end - 1) >= value) {
		auto physical_lenght = end - begin;
		auto real_lenght = *(end-1) - *begin;
		auto real_pos = value - *begin;
		auto physica_pos = physical_lenght * real_pos / real_lenght;
		if (*(begin + physica_pos) == value)
			return begin + physica_pos;
		else if (*(begin + physica_pos) > value)
			end = begin + (physica_pos ? physica_pos : 1);

		else
			begin = begin + (physica_pos ? physica_pos : 1);
	}
	throw std::exception("Not found");
}

void main(void) {
	Timer t;
	
	std::vector<int> v = { 1,2,3,4,5,7,9,15,55,135,999,123423};
	
	interpolation_search(v.begin(), v.end(), 15);
	
	std::cout << "Time elapsed: " << t.elapsed() << '\n';

}