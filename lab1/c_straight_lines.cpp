#include<stdio.h>
#include "c_straight_lines.h"
#include "math.h"
using namespace c;

float radian_to_degrees(double radin)
{
	const float PI = 3.14159f;
	const float RightAngle = 90.0f;
	return (float)(radin * 2 * RightAngle / PI);
}
double c::calculate_angle(StraightLine first, StraightLine second)
{
	double k1 = 0, k2 = 0;
	if (first.b != 0)
		k1 = -((first.a) / (first.b));
	if (second.b != 0)
		k2 = -((second.a) / (second.b));
	double angle_in_radian = atan((k2 - k1) / (1 + k1 * k2));
	double angle_in_degrees = radian_to_degrees(angle_in_radian);
	return (angle_in_degrees);
}
StraightLine create_line(Point a)
{
	StraightLine tmp;
	tmp.a = a.x;
	tmp.b = a.y;
	tmp.c = 0;
	return  tmp;
}
bool c::points_on_different_sides(StraightLine line, Point a, Point b)
{
	double k1 = (-a.x/ a.y);
	double k2 = (-b.x / b.y);
	double k_line = -line.a / line.b;
	if (((k1 > k_line) && (k2 < k_line)) || ((k1 < k_line) && (k2 > k_line)))
		return true;
	else
		return false;
}
Point find_solution_system(StraightLine vector_1, StraightLine vector_2)
{
	Point a;
	double dx = vector_1.c * vector_2.b - vector_2.c * vector_1.b;
	double dy = vector_1.a * vector_2.c - vector_2.a * vector_1.c;
	double d = vector_1.a * vector_2.b - vector_1.b * vector_2.a;
	a.x = dx / d;
	a.y = dy / d;
	return a;
}
Point c::decussation_lines(StraightLine first, StraightLine second)
{

	Point tmp;
	tmp.x = NULL;
	tmp.y = NULL;
	if (first.a == second.a&& first.b == second.b&& first.c == second.c) {
		return tmp;
	}
	if (calculate_angle(first, second) == 0 )
		return tmp;
	else
		return find_solution_system(first, second);
}
void c::print_point(Point point) {
	if (point.x == NULL&& point.y == NULL)
		printf("x=NULL;Y=NULL\n");
	else
	printf("x=%lf y=%lf\n", point.x, point.y);
}


