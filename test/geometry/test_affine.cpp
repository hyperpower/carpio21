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
typedef Box_<double, 2> Box;
typedef PointChain_<double, 2> PointChain;

TEST(bezier, mkdir){
    // Do not disable this test
    MakeDir("./test_output/");
}
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
	Seg2   seg(x, y);
	std::list<Vt> trans{2,2,2};
	Translate(seg, trans);

	std::cout << seg[0] << std::endl;
	std::cout << seg[1] << std::endl;
	ASSERT_EQ(seg[0][0], 3);
}

auto MakeFShape(){
	typedef PointChain_<double, 2> PointChain;
	typedef Point_<double, 2> Point;
	PointChain pc;
	pc.set_close();
	pc.push_back(Point(0.3,0.1));
	pc.push_back(Point(0.4,0.1));
	pc.push_back(Point(0.4,0.45));
	pc.push_back(Point(0.7,0.45));
	pc.push_back(Point(0.7,0.55));
	pc.push_back(Point(0.4,0.55));
	pc.push_back(Point(0.4,0.8));
	pc.push_back(Point(0.8,0.8));
	pc.push_back(Point(0.8,0.9));
	pc.push_back(Point(0.3,0.9));
	return pc;
}
auto MakeBox(){
	typedef PointChain_<double, 2> PointChain;
	typedef Point_<double, 2> Point;
	PointChain pc;
	pc.set_close();
	pc.push_back(Point(0.0,0.0));
	pc.push_back(Point(1.0,0.0));
	pc.push_back(Point(1.0,1.0));
	pc.push_back(Point(0.0,1.0));
	return pc;
}

auto AddActorsOriginal(Gnuplot& gnu, const PointChain& box, const PointChain& pc){
	auto actor_pori = ToGnuplotActor(box.get(0));
	actor_pori->style("with points pt 7 ps 2 lc rgb \"#7FBA00\"");
	gnu.add(actor_pori);
	auto actor_px = ToGnuplotActor(box.get(1));
	actor_px->style("with points pt 7 ps 2 lc rgb \"#00A4EF\"");
	gnu.add(actor_px);
	auto actor_py = ToGnuplotActor(box.get(3));
	actor_py->style("with points pt 7 ps 2 lc rgb \"#F25022\"");
	gnu.add(actor_py);
	auto actor_box = ToGnuplotActor(box);
	actor_box->style("with lines lt 1 dashtype 2 lw 2 lc \"black\"");
	gnu.add(actor_box);
	auto actor_pc  = ToGnuplotActor(pc);
	actor_pc->style("with lines lt 1 dashtype 2 lw 2 lc rgb \"#FFB900\"");
	gnu.add(actor_pc);
}
auto AddActorsTransformed(Gnuplot& gnu, const PointChain& box, const PointChain& pc){
	auto actor_pori = ToGnuplotActor(box.get(0));
	actor_pori->style("with points pt 7 ps 2 lc rgb \"#7FBA00\"");
	gnu.add(actor_pori);
	auto actor_px = ToGnuplotActor(box.get(1));
	actor_px->style("with points pt 7 ps 2 lc rgb \"#00A4EF\"");
	gnu.add(actor_px);
	auto actor_py = ToGnuplotActor(box.get(3));
	actor_py->style("with points pt 7 ps 2 lc rgb \"#F25022\"");
	gnu.add(actor_py);
	auto actor_box = ToGnuplotActor(box);
	actor_box->style("with lines lt 1 lw 2 lc \"black\"");
	gnu.add(actor_box);
	auto actor_pc  = ToGnuplotActor(pc);
	actor_pc->style("with lines lt 1 lw 2 lc rgb \"#FFB900\"");
	gnu.add(actor_pc);
}
TEST(box, gnuplot){
	typedef Box_<double, 2> Box;
	typedef Point_<double, 2> Point;
	// Box box(0.0, 1.0);
	Point about(0.5, 0.5);

	auto pc  = MakeFShape();
	auto box = MakeBox();

	Gnuplot gnu;
    gnu.set_terminal_png("./test_output/affine_first");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
	gnu.set_equal_aspect_ratio();
	std::cout << box.size() << std::endl;
	AddActorsOriginal(gnu, box, pc);
	std::vector<double> trans_vec{0.25,0.37};
	Reflect(box, about);
	Reflect(pc , about);
	AddActorsTransformed(gnu, box, pc);
	gnu.plot();
}

TEST(mata, gunpulot){

	std::cout << "Is Geometry Point : " << IsGeometry<Point3>::value << std::endl;
	std::cout << "Is Geometry int   : " << IsGeometry<int>::value << std::endl;

	typedef std::list<Point2> listP2;
	typedef std::vector<Point2> vectorP2;

	ToGnuplotActor(listP2());
	ToGnuplotActor(vectorP2());
	ToGnuplotActor(Point3());

}