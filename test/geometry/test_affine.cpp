#include "geometry/geometry.hpp"
#include "io/io_define.hpp"
#include "gtest/gtest.h"

using namespace carpio;

const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActor_<double, 2> GA;
typedef Segment_<double, 2> Seg2;


TEST(affine, point_initial){
	Point3 x(1, 0, 0);
	Point3 y(0, 1, 0);
	Point3 z(0, 0, 1);
	std::cout << "[==========] Initial three points and output" << std::endl;
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;
	std::cout << "Point z = " << z << std::endl;
	ASSERT_EQ(x(0), 1);
	ASSERT_EQ(y(1), 1);

	Translate(x, y);
	ASSERT_EQ(x[1], 1);

	// Vt v = 1;
	// x = {3, 0, 0};
	// Translate(x, v);
	// ASSERT_EQ(x[0], 4);

	x = {3, 0, 0};
	std::list<Vt> arr{1 , 1};
	Translate(x, arr);
	ASSERT_EQ(x[0], 4);
	ASSERT_EQ(x[1], 1);
	ASSERT_EQ(x[2], 0);

	x = {3, 0, 0};
	arr = {1.5, 1.5};
	Scale(x, arr);
	ASSERT_EQ(x[0], 4.5);

	std::cout << ToString(x, " ") << std::endl;
	
	Gnuplot gnu;
	auto actor = ToGnuplotActor(x);
	actor->show_command();
	gnu.add(ToGnuplotActor(x));
	gnu.add(ToGnuplotActor(y));
}

TEST(affine, segment){
	Point2 x(1, 0);
	Point2 y(0, 1);
	Seg2 seg(x, y);
	std::list<Vt> trans{2,2,2};
	Translate(seg, trans);

	std::cout << seg[0] << std::endl;
	std::cout << seg[1] << std::endl;
	ASSERT_EQ(seg[0][0], 3);

	Gnuplot gnu;
	auto actor = ToGnuplotActor(x);
	actor->show_command();
	gnu.add(ToGnuplotActor(x));
	gnu.add(ToGnuplotActor(y));

	// std::string atest = "abchenb  bd aa";
	// std::list<std::string> res;
	// Tokenize(atest, res, "bh");
	// for(auto& r : res){
		// std::cout << r << std::endl;
	// }
	gnu.plot();


}