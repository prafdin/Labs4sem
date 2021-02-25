#include "cpp_straight_lines.h"
#include "math.h"
using namespace cpp;
bool cpp::points_on_defferent_sides(const StraightLine& line, const Point& a, const Point& b)
{
	double k1 = (-a.get_x() / a.get_y());
	double k2 = (-b.get_x() / b.get_y());
	if (((k1 > line.get_k()) && (k2 < line.get_k())) || ((k1 < line.get_k()) && (k2 > line.get_k())))
		return true;
	else
		return false;
}
Point generate_nan_point() {
	Point tmp(nan("1"), nan("1"));
	return tmp;
}
double radian_to_degrees(double radin)
{
	const double PI = 3.14159f;
	const double RightAngle = 90.0f;
	return (radin * 2 * RightAngle / PI);
}
double StraightLine::calculate_angle(const StraightLine& tmp)const {
	double angle_in_radian = atan((tmp.get_k() - m_k) / (1 + m_k * tmp.get_k()));
	return radian_to_degrees(angle_in_radian);

}
Point find_solution_system(const StraightLine& vector_1, const StraightLine& vector_2)
{
	double dx = vector_1.get_c() * vector_2.get_b() - vector_2.get_c() * vector_1.get_b();
	double dy = vector_1.get_a() * vector_2.get_c() - vector_2.get_a() * vector_1.get_c();
	double d = vector_1.get_a() * vector_2.get_b() - vector_1.get_b() * vector_2.get_a();
	Point a((dx / d), (dy / d));
	return a;

}
Point cpp::decussation_lines(const StraightLine& first_line, const StraightLine& second_line)
{
	if (first_line.get_a() == second_line.get_a() && first_line.get_b() == second_line.get_b() && first_line.get_c() == second_line.get_c()) {
		return generate_nan_point();
	}
	if (StraightLine::calculate_angle(first_line,second_line) == 0)
		return generate_nan_point();
	else
		return find_solution_system(first_line, second_line);
}
double StraightLine::calculate_angle(const StraightLine& first, const StraightLine& second) {
	double angle_in_radian = atan((second.get_k() - first.get_k()) / (1 + first.get_k() * second.get_k()));
	return radian_to_degrees(angle_in_radian);
}