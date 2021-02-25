#pragma once

namespace c
{
	struct StraightLine
	{
		double a, b, c;
	};


	struct Point
	{
		double x;
		double y;
	};
	double calculate_angle(StraightLine first, StraightLine second);
	bool points_on_different_sides(StraightLine main_line, Point a, Point b);
	Point decussation_lines(StraightLine first, StraightLine second);
	void print_point(Point point);
}
