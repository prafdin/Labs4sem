#pragma once
#pragma once
#include "cmath"
#include <stdio.h>
const double PI = 3.14159265;
namespace cpp
{
	class Point
	{
	private:
		double m_x;
		double m_y;
	public:

		Point(double x=0, double y=0) : m_x(x), m_y(y) {}
		double get_x() const { return m_x; }
		double get_y() const { return m_y; }
		void print_point() const {
			printf("x=%lf y=%lf\n", m_x, m_y);
		}
	};
	class StraightLine
	{
		double m_a;
		double m_b;
		double m_c;
		double m_k;
	public:
		StraightLine(double a=0, double b=0, double  c=0) : m_a(a),m_b(b), m_c(c) {
			if (b == 0)
				m_k = 0;
			else
				m_k = -a / b;
		}
		double get_a()  const { return m_a; }
		double get_b() const { return m_b; }
		double get_c()  const { return m_c; }
		double get_k()  const { return m_k; }
		double calculate_angle(const StraightLine& tmp) const;
		static double calculate_angle(const StraightLine& first, const StraightLine& second);

	};

	bool points_on_defferent_sides(const StraightLine& line, const Point& a, const Point& b);
	Point decussation_lines(const StraightLine& first_line, const StraightLine& second_line);

	
}
