#ifndef GEOMETRY_TEST_VOFTOOL_HPP_
#define GEOMETRY_TEST_VOFTOOL_HPP_
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef GGnuplotActor_<double, 2> GA;

typedef CuboidTool_<Vt, 2> Tool;
typedef CuboidToolPL_<Vt>  ToolPL;
typedef Line_<Vt> Line;
typedef Point_<Vt, 2> Point;
typedef Box_<Vt, 2>   Box;

TEST(GEOVOF, DISABLED_case1){
	Vt m1    =  0.8;
	Vt m2    =  1.0;
	Vt alpha = 2.1;
	Vt c1    = 1.5;
	Vt c2    = 1.0;
	std::vector<Point> box{
			Point(0, 0),
			Point(c1,0),
			Point(c1,c2),
			Point(0, c2),
			Point(0, 0)
		};
	Line l1(m1, m2, alpha);
	std::cout << "l1    : " << l1 << std::endl;
	std::cout << "alpha =  " << l1.alpha() << std::endl;
	std::cout << "m1    =  " << l1.a() << std::endl;
	std::cout << "m2    =  " << l1.b() << std::endl;
	std::cout << "c1    =  " << c1 << std::endl;
	std::cout << "c2    =  " << c2 << std::endl;
	std::cout << "c2 / c1 =  " << c2 / c1 << std::endl;
	std::cout << "m1 / m2 =  " << m1 / m2 << std::endl;
	std::shared_ptr<Tool> sptool(new ToolPL());
	auto area = sptool->cal_area(l1, c1, c2);
	std::cout << "Area  = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;

	auto spl  = sptool->construct_interface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}

	auto sps = sptool->cal_segment((*spl), c1, c2);
	Gnuplot gnu;
//	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	gnu.add(GA::LinesPoints((*sps), 3));
//	gnu.plot();
}

TEST(GEOVOF, DISABLED_case2){
	Vt m1 = -0.5;
	Vt m2 =  0.9;
	Vt alpha = 1.0;
	Vt c1 = 1.0;
	Vt c2 = 1.5;
	std::vector<Point> box{
			Point(0,0),
			Point(c1,0),
			Point(c1,c2),
			Point(0,c2),
			Point(0,0)
		};
	Line l1(m1, m2, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(-m1, m2, alpha - m1 * c1);
	std::cout<< "l2 : " << l2 << std::endl;

	std::shared_ptr<Tool> sptool(new ToolPL());
	auto area = sptool->cal_area(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;
	auto spl  = sptool->construct_interface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}
	auto sps = sptool->cal_segment((*spl), c1, c2);
	Gnuplot gnu;
//	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 1));
	ASSERT_LE(l1.alpha() - spl->alpha(), 1e-8);
	gnu.add(GA::Arrows((*sps), 3));
//	gnu.add(GA::Lines(l2, -1, 5, 2));
	gnu.plot();
}

TEST(GEOVOF, DISABLED_case3){
	Vt m1 = -0.5;
	Vt m2 = -0.9;
	Vt alpha = -1.4;
	Vt c1 = 1.5;
	Vt c2 = 1.0;
	std::vector<Point> box{
		Point(0,0),
		Point(c1,0),
		Point(c1,c2),
		Point(0,c2),
		Point(0,0)
	};
	Line l1(m1, m2, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(-m2, -m1, alpha -m1*c1 - m2*c2 ); // important
	std::cout<< "l2 : " << l2 << std::endl;
	std::shared_ptr<Tool> sptool(new ToolPL());
	auto area = sptool->cal_area(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;
	auto spl  = sptool->construct_interface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}

	Gnuplot gnu;
	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	ASSERT_LE(l1.alpha() - spl->alpha(), 1e-8);
//	gnu.plot();
}

TEST(GEOVOF, DISABLED_case4){
	Vt m1     =  0.5;
	Vt m2     = -0.9;
	Vt alpha = -0.2;
	Vt c1    = 1.5;
	Vt c2    = 1.0;
	std::vector<Point> box{
		Point(0,0),
		Point(c1,0),
		Point(c1,c2),
		Point(0,c2),
		Point(0,0)
	};
	Line l1(m1, m2, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(m1, -m2, alpha - m2*c2 ); // important
	std::cout<< "l2 : " << l2 << std::endl;


	std::shared_ptr<Tool> sptool(new ToolPL());
	auto area = sptool->cal_area(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;
	auto spl  = sptool->construct_interface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}

	Gnuplot gnu;
	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	ASSERT_LE(l1.alpha() - spl->alpha(), 1e-8);
//	gnu.plot();
}
TEST(GEOVOF, case5){
	Vt m1     =  0.5;
	Vt m2     = -0.9;
	Vt alpha = -0.2;
	Line l(m1, m2, alpha);
	Point pmin(1.0, 1.0);
	Point pmax(2.0, 2.5);
	std::cout << "pmin = " << pmin << std::endl;
	std::cout << "pmin = " << pmax << std::endl;

	std::shared_ptr<Tool> sptool(new ToolPL());
	Vt area = sptool->cal_area(l, pmin, pmax);
	std::cout << "Area = " << area << std::endl;

	Box box(pmin, pmax);
	Box boxm(Point(0,0), pmax - pmin);

	Vt c1 = pmax.x() - pmin.x();
	Vt c2 = pmax.y() - pmin.y();
	// pmin becomes the original point
	Vt a = l.a();
	Vt b = l.b();
	Line ln(a, b, - a * pmin.x() - b * pmin.y() + l.alpha());
	std::cout << "l  : " << l 	<< std::endl;
	std::cout << "ln : " << ln	<< std::endl;
	Gnuplot gnu;
	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box));
	gnu.add(GA::Lines(boxm, 2));

	gnu.add(GA::Lines(l, -1, 5));
	gnu.add(GA::Lines(ln, -1, 4, 2));
//	gnu.plot();
}

TEST(CuboidTool, case5){
	Tool tool;
	std::cout << "Tool dim = " << tool.Dim << std::endl;
	std::cout << "Ori = " << ToString(_M_) << ToString(_P_)
			  << " Order = " << tool.vertex_order(_M_,_P_) << std::endl;
	std::cout << "Face order ----------------------\n";
	std::cout << "a = " << ToString(_X_) << " o = " << ToString(_P_);
	std::cout << " Order = " << tool.face_order(_X_, _P_) <<std::endl;
}

TEST(CuboidTool, case6){
	ToolPL tool;
	typename ToolPL::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt t){
		return x * x + y * y - 0.25;
	};
	typename ToolPL::Point po(-1.0, -1.0);
	Vt dx = 1.0;
	Vt dy = 1.0;
	auto aa = tool.cal_cell_aperture_ratios(
			          po.x(),po.y(), dx, dy, 0.0, 0.0, fun, 1e-4);
	std::cout << "a 0 = " << aa[0] << std::endl;
	std::cout << "a 1 = " << aa[1] << std::endl;
	std::cout << "a 2 = " << aa[2] << std::endl;
	std::cout << "a 3 = " << aa[3] << std::endl;
}


}

#endif
