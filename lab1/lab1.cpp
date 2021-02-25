#include <iostream>
#include "cmath"
#include <cstring>
#include "c_straight_lines.h"
#include "cpp_straight_lines.h"

const int size_mass = 100;
const int code_sign_minus = 45;
const int code_dot = 46;
const int code_digit_0 = 48;
const int code_digit_9 = 57;

//INPUT
double determinate_num(const char* ptr) {
	int length_str = strlen(ptr) - 1;
	bool dot_flag = false;
	for (int i = 0; i < length_str; i++)
	{
		if (ptr[i] > code_digit_9 || ptr[i] < code_digit_0 )
			if(ptr[i] != code_sign_minus && ptr[i] != code_dot)
				return nan("1");
	}
	return strtod(ptr, NULL);
}
double input_num() {
	char mass[size_mass] = "0";
	double num = 0;
	do {
		memset(mass, 0, sizeof(char) * size_mass);
		fgets(mass, size_mass, stdin);
		num = determinate_num(mass);
		if (num != num)
			printf("Number is uncorrect. Try again: ");
		else
			break;
	} while (true);
	return num;

}
//CREATE OBJECT
c::StraightLine create_c_line() {
	printf("Enter data line :\nEnter A1 : ");
	double A = input_num();
	printf("Enter B1 : ");
	double B = input_num();
	printf("Enter C1 : ");
	double C = input_num();
	return c::StraightLine{ A, B, C };
}
cpp::StraightLine create_cpp_line() {
	printf("Enter data line :\nEnter A1 : ");
	double A = input_num();
	printf("Enter B1 : ");
	double B = input_num();
	printf("Enter C1 : ");
	double C = input_num();
	return cpp::StraightLine(A, B, C);
}
c::Point create_c_point() {
	printf("Enter data point :\nEnter x1 : ");
	double x = input_num();
	printf("Enter y1 : ");
	double y = input_num();
	
	return c::Point{ x, y };
}
cpp::Point create_cpp_point() {
	printf("Enter data point :\nEnter x1 : ");
	double x = input_num();
	printf("Enter y1 : ");
	double y = input_num();
	return cpp::Point(x, y);
}

int main()
{
	printf("Enter data first line :\nEnter A1 : ");
	double A1 = input_num();
	printf("Enter B1 : ");
	double B1 = input_num();
	printf("Enter C1 : ");
	double C1 = input_num();
	printf("Enter data second  line :\nEnter A1 : ");
	double A2 = input_num();
	printf("Enter B1 : ");
	double B2 = input_num();
	printf("Enter C1 : ");
	double C2 = input_num();
	printf("Enter data first point :\nEnter x1 : ");
	double x1 = input_num();
	printf("Enter y1 : ");
	double y1 = input_num();
	printf("Enter data second point :\nEnter x1 : ");
	double x2 = input_num();
	printf("Enter y1 : ");
	double y2 = input_num();
	printf("C realization:\n");
	{
		using namespace c;
		StraightLine first_line{ A1, B1, C1 };
		StraightLine second_line{ A2, B2, C2 };
		StraightLine abscissa{ 0,0,0 };
		Point first_point{ x1, y1 };
		Point second_point{ x2, y2 };
		printf("Angle between abscissa and first line :%lf\n", calculate_angle(abscissa, first_line));
		printf("Angle between abscissa and second line :%lf\n", calculate_angle(abscissa, second_line));
		printf("Angle between lines :%lf\n", calculate_angle(first_line, second_line));
		if (points_on_different_sides(first_line, first_point, second_point) == true)
			printf("Points on defferent sides\n");
		else
			printf("Points on one sides\n");
		printf("Lines decussation in point ");
		print_point(decussation_lines(first_line, second_line));

	}
	printf("CPP realization:\n");
	{
		using namespace cpp;
		StraightLine first_line(A1, B1, C1);
		StraightLine  second_line(A2, B2, C2);
		StraightLine  abscissa(0, 0, 0);
		Point first_point(x1, y1);
		Point second_point(x2, y2);
		printf("Angle between abscissa and first line :%lf\n", abscissa.calculate_angle(first_line));
		printf("Angle between abscissa and second line :%lf\n", abscissa.calculate_angle(second_line));
		printf("Angle between lines :%lf\n", first_line.calculate_angle(second_line));
		if (points_on_defferent_sides(first_line, first_point, second_point) == true)
			printf("Points on defferent sides\n");
		else
			printf("Points on one sides\n");
		printf("Lines decussation in point ");
		decussation_lines(first_line, second_line).print_point();
	}
  
}

