/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */

#ifndef GEOMETRY_TEST_SEGMENT_HPP_
#define GEOMETRY_TEST_SEGMENT_HPP_
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef Segment_<double, 2> Seg2;
typedef GGnuplotActor_<double, 2> GA;

TEST(segment, segment_initial){
	Point2 x(1, 0);
	Point2 y(0, 1);
	std::cout << "Initial three points and output/n";
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;

	Seg2 seg(x, y);
	std::cout << "The segemnt is " << seg << std::endl;
	ASSERT_EQ(seg.ps(), x);
	ASSERT_EQ(seg.pe(), y);
}


TEST(segment, segment_plot){
	Point2 x(1, 0);
	Point2 y(0, 1);
	std::cout << "Initial three points and output/n";
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;

	Seg2 seg(x, y);
Gnuplot gnu;
	gnu.add(GA::LinesPoints(seg));
//	gnu.plot();
	std::cout << "The segemnt is " << seg << std::endl;
	ASSERT_EQ(seg.ps(), x);
	ASSERT_EQ(seg.pe(), y);
}

TEST(segment, segment_point_location){
	Point2 x(0, 0);
	Point2 y(0, 1);
	Seg2 seg(x, y);

	Point2 p(0.0, 1.50);

	Gnuplot gnu;
	gnu.add(GA::Points(p));
	gnu.add(GA::LinesPoints(seg));
//	gnu.plot();
	std::cout << "The segemnt is :" << seg << std::endl;
	std::cout << "The point is   :" << p << std::endl;
	auto side = OnWhichSide7(seg, p);
	std::cout << "Position       :" << ToString(side) << std::endl;
//	ASSERT_EQ(side, _PS_RIGHT_);
}

TEST(segment, intersection){
	typedef IntersectionPairSS_<double, 2> Inter;
	Point2 p1(  -1,    0);
	Point2 p2(   3,  0.5);
	Point2 p3( 0.8,  2.0);
	Point2 p4(-0.3, -1.0);
	Seg2 seg1(p1, p2);
	Seg2 seg2(p3, p4);

	Inter inter(seg1, seg2);
	auto strtype = ToString(inter.cal_intersection_type());
	std::cout << "Intersection Type : "<< strtype << std::endl;
	Point2 np = inter.cal_intersection_point();
	std::cout << "new point         : "<< np << std::endl;

	Gnuplot gnu;
    gnu.set_terminal_png("./plot/line_segment_1.png");
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.set_label(1, strtype, -4.5, 4);
	gnu.add(GA::LinesPoints(seg1, 0));
	gnu.add(GA::LinesPoints(seg2, 1));
	gnu.add(GA::Points(np, 3));
	gnu.plot();
}

TEST(segment, intersection2){
	typedef IntersectionPairSS_<double, 2> Inter;
	Point2 p1(  2.0,    0);
	Point2 p2(  2.0,  2.0);
	Point2 p3(  3.0,  1.0);
	Point2 p4(  1.0,  3.0);
	Seg2 seg1(p1, p2);
	Seg2 seg2(p3, p4);

	Inter inter(seg1, seg2);
	auto strtype = ToString(inter.cal_intersection_type());
	std::cout << "Intersection Type : "<< strtype << std::endl;
	Point2 np = inter.cal_intersection_point();
	std::cout << "new point         : "<< np << std::endl;

	Gnuplot gnu;
    gnu.set_terminal_png("./plot/line_segment_2.png");
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.set_label(1, strtype, -4.5, 4);
	gnu.add(GA::LinesPoints(seg1, 0));
	gnu.add(GA::LinesPoints(seg2, 1));
	gnu.add(GA::Points(np, 3));
	gnu.plot();
}
TEST(segment, intersection3){
	typedef IntersectionPairSS_<double, 2> Inter;
	Point2 p1(  0.0,    0);
	Point2 p2(  1.0,  0.0);
	Point2 p3( -3.0,  0.0);
	Point2 p4(  0.5,  0.0);
	Seg2 seg1(p1, p2);
	Seg2 seg2(p3, p4);

	Inter inter(seg1, seg2);
	auto strtype = ToString(inter.cal_intersection_type());
	std::cout << "Intersection Type : "<< strtype << std::endl;
	Point2 np = inter.cal_intersection_point();
	std::cout << "new point         : "<< np << std::endl;

	Gnuplot gnu;
    gnu.set_terminal_png("./plot/line_segment_3.png");
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.set_label(1, strtype, -4.5, 4);
	gnu.add(GA::LinesPoints(seg1, 0));
	gnu.add(GA::LinesPoints(seg2, 1));
	gnu.add(GA::Points(np, 3));
	gnu.plot();
}







} //end namespace
#endif
