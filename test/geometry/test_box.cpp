/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#include "geometry/geometry.hpp"
#include "utility/tinyformat.hpp"
#include "gtest/gtest.h"

using namespace carpio;

typedef Point_<double, 3>   Point3;
typedef Point_<double, 2>   Point2;
typedef Segment_<double, 2> Seg2;
typedef Box_<double, 2>     Box2;
typedef Line_<double>       Line;

//typedef GGnuplotActor_<double, 2> GA;
typedef GGnuplotActorMaker_<double, 2> GAM;

TEST(box, point_to_box_position){
	Point2 x(1, 0);
	Point2 y(0, 1);
	std::cout << "Initial three points and output" << std::endl;
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;
	Point2 p(0.5, 0.5);

	Box2 box(x, y);
	std::cout << "The box is " << box << std::endl;

	int code = PointToBoxPositionCode(box.min(), box.max(), p);
	ASSERT_EQ(code, 33);
	std::cout << "code = " << code << std::endl;

	ASSERT_EQ(PointToBoxPositionCode(box.min(), box.max(), Point2(-0.1,  0.1)), 31);
	ASSERT_EQ(PointToBoxPositionCode(box.min(), box.max(), Point2(-0.1, -0.1)), 11);
	ASSERT_EQ(PointToBoxPositionCode(box.min(), box.max(), Point2( 0.0,  0.0)), 22);

	ASSERT_EQ(IsInOn(box, Point2( 0.1, 0.1)), true);
	ASSERT_EQ(IsInOn(box, Point2(-0.1, 0.1)), false);
	ASSERT_EQ(IsInOn(box, Point2( 1.0, 1.0)), true);
}

TEST(box, box_box_in_on){
	std::cout << "Initial 2 boxs" << std::endl;
	Point2 min1(1, 0);
	Point2 max1(0, 1);
	Box2 box1(min1, max1);
	Box2 box2(Point2(0.5, 0.5), Point2(1.5, 1,5));
	std::cout << "The box 1 is " << box1 << std::endl;
	std::cout << "The box 2 is " << box2 << std::endl;

	ASSERT_EQ(IsInOn(box1, box2), true);
	Box2 box3(Point2(1., 1.), Point2(1.5, 1.5));
	std::cout << "The box 3 is " << box3 << std::endl;
	ASSERT_EQ(IsInOn(box1, box3), true);
}

TEST(box, box_vs_line){
	std::cout << "Initial box" << std::endl;
	Point2 min1(0.0, 0.0);
	Point2 max1(1, 1);
	Box2 box1(min1, max1);
	std::cout << "The box 1 is " << box1 << std::endl;

	Line line(0.0, 1.0, 1.0);
//	Line line(9.0000001, 1e-11, 1.0);
	std::cout << "Line is " << line << std::endl;

	auto lspp = IntersectLineBox(box1, line);
	std::cout << "Result points : " << lspp.size() << std::endl;
	for(auto& sp : lspp){
		std::cout << "   " << *sp << std::endl;
	}

	Gnuplot gnu;
	GAM     gam;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);
	gnu.set_equal_aspect_ratio();
	gnu.set_terminal_png("./plot/line_box_normal.png");
	auto spbox1 = gam.lines(box1);
	spbox1->style() = "with lines lw 2 lc 8";
	gnu.add(spbox1);
	auto spline = gam.lines(line, -0.5, 1.5);
	spline->style() = "with lines lw 3 lc 6";
	gnu.add(spline);
	// add intersection points
	int numlabel = 30;
	for(auto& sp : lspp){
		auto sppoint = gam.points(*sp);
		sppoint->style() = "with points pt 7 ps 2 lc 7";
		gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", sp->x(), sp->y()),
				     sp->x(), sp->y()+ 0.05, "left font \",16\"");
		gnu.add(sppoint);
		numlabel++;
	}
	gnu.set_label(1,tfm::format("(%.1f, %.1f)",
			      min1.x(), min1.y()),
			      min1.x(), min1.y() + 0.06, "center font \",16\"");
	gnu.set_label(2,tfm::format("(%.1f, %.1f)",
			      max1.x(), max1.y()),
			      max1.x(), max1.y() + 0.06, "center font \",16\"");

	gnu.set_label(4,tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(), line.alpha()),
			        0.1, 1.3, "left font \",16\"");
	gnu.plot();
}


void BoxLinePositiveCase(int num_case,
        double x, double y,
        double a, double b, double alpha) {
    Point2 min1(0.0, 0.0);
    Point2 max1(y, y);
    Box2 box1(min1, max1);

    Line line(a, b, alpha);

    auto lspp = IntersectLineNegativeBox(box1, line);

    Gnuplot gnu;
    GAM gam;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png(tfm::format("./plot/lb_positve_%d.png", num_case));
    auto spbox1 = gam.lines(box1);
    spbox1->style() = "with lines lw 2 lc 8";
    gnu.add(spbox1);
    auto spline = gam.lines(line, -0.5, 1.5);
    spline->style() = "with lines lw 3 lc 6";
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for (auto& sp : lspp) {
        auto sppoint = gam.points(*sp);
        sppoint->style() = "with points pt 7 ps 2 lc 7";
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", sp->x(), sp->y()),
                sp->x(), sp->y() + 0.05, "left font \",16\"");
        gnu.add(sppoint);
        numlabel++;
    }
    gnu.set_object2dp(5, lspp,
            "fillstyle transparent solid 0.3 fc rgb \"blue\"");

//    gnu.set_label(1, tfm::format("(%.1f, %.1f)", min1.x(), min1.y()), min1.x(),
//            min1.y() + 0.06, "center font \",16\"");
//    gnu.set_label(2, tfm::format("(%.1f, %.1f)", max1.x(), max1.y()), max1.x(),
//            max1.y() + 0.06, "center font \",16\"");

    gnu.set_label(4,
            tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(),
                    line.alpha()), 0.1, 1.3, "left font \",16\"");
    gnu.plot();
}


TEST(box, box_vs_line_positive){
    int n = 100;
    Vt dt = 3.1415926 / double(n);
    for(int i = 0; i<n; i++){
        Vt x1 = 0.5, y1 = 0.5;
        Vt r = 1;
        Vt theta = i * dt;
        Vt x2 = x1 + r * std::cos(theta);
        Vt y2 = y1 + r * std::sin(theta);
        Vt a = y1 - y2;
        Vt b = x2 - x1;
        Vt alpha = x2 * y1 - x1 * y2;

        BoxLinePositiveCase(i, 1, 1, a, b, alpha);
    }
}

