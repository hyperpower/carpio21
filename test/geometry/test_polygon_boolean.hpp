#ifndef GEOMETRY_TEST_POLYGON_BOOLEAN_HPP_
#define GEOMETRY_TEST_POLYGON_BOOLEAN_HPP_

#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef GGnuplotActor_<double, 2> GA;
typedef GFile_<double,2> GF;


inline void two_pointchains(
		const std::string& aname,
		const PointChain_<double, 2>& pcc,
		const PointChain_<double, 2>& pco){

	typedef PolygonBoolean_<double> PB;
	PolygonBoolean_<double> pb(pcc, pco);

	auto inter = pb.output(PB::_INTERSECTION_);
	auto uni   = pb.output(PB::_UNION_);
	auto sub   = pb.output(PB::_SUBSTRACT_);
	pb.show_table();
	Gnuplot gnu;
	gnu.set_terminal_png(aname + ".png", 1200, 1200);
	gnu.set("multiplot layout 2, 2 title \"\" font \",11\"");
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.set_title(aname + "Oringal");
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
	gnu.plot();

	gnu.set_title(aname + "Intersection");
//	gnu.add(pb.actor_clip(gnu));
//	gnu.add(pb.actor_object(gnu));
	for (auto& pc : inter) {
		gnu.add(GA::LinesPoints(pc, 7));
	}
	gnu.add(pb.actor_label(gnu));
	gnu.plot();
	//
	gnu.set_title(aname + "Union");
//	gnu.add(pb.actor_clip(gnu));
//	gnu.add(pb.actor_object(gnu));
	for (auto& pc : uni) {
		gnu.add(GA::LinesPoints(pc, 7));
	}
	gnu.plot();
	//
//	gnu.add(pb.actor_clip(gnu));
//	gnu.add(pb.actor_object(gnu));
	gnu.set_title(aname + "subtract");
	for (auto& pc : sub) {
		gnu.add(GA::LinesPoints(pc, 7));
	}
	gnu.plot();
}

TEST(polygon_boolean, test1) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, 0.5));
	lo.push_back(Point(2.6, 0.5));
	lo.push_back(Point(0.8, 2.5));
	PC pobject(lo);

	two_pointchains("test1", pclip, pobject);
}

TEST(polygon_boolean, test2) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, 0.5));
	lo.push_back(Point(2  , 0.8));
	lo.push_back(Point(0.8, 2.5));
	PC pobject(lo);

	two_pointchains("test2", pclip, pobject);
}

TEST(polygon_boolean, test3) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, 0.5));
	lo.push_back(Point(2.0, 2.0));
	lo.push_back(Point(0.8, 2.5));
	PC pobject(lo);

	two_pointchains("test3", pclip, pobject);
}

TEST(polygon_boolean, test4) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.5, 0.5));
	lo.push_back(Point( 2.0, 2.0));
	lo.push_back(Point(-0.8, 2.0));

	PC pobject(lo);

	two_pointchains("test4", pclip, pobject);
}

TEST(polygon_boolean, test5) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.5, 0.5));
	lo.push_back(Point( 2.0, 2.0));
	lo.push_back(Point( 1.5, 2.5));
	lo.push_back(Point( 0.8, 1.5));
	lo.push_back(Point( 0.2, 2.5));

	PC pobject(lo);

	two_pointchains("test5", pclip, pobject);
}

TEST(polygon_boolean, test6) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 1.0, 1.0));
	lo.push_back(Point( 3.0, 1.0));
	lo.push_back(Point( 1.0, 3.0));
	lo.push_back(Point( 0.5, 1.5));

	PC pobject(lo);

	two_pointchains("test6", pclip, pobject);
}

TEST(polygon_boolean, test7) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.0, -1.0));
	lo.push_back(Point( 2.0, -1.0));
	lo.push_back(Point( 2.0,  3.0));
	lo.push_back(Point( 0.0,  3.0));

	PC pobject(lo);

	two_pointchains("test7", pclip, pobject);
}
TEST(polygon_boolean, test8) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.0, 0.5));
	lo.push_back(Point( 2.0, 0.5));
	lo.push_back(Point( 2.0, 1.5));
	lo.push_back(Point( 0.0, 1.5));

	PC pobject(lo);

	two_pointchains("test8", pclip, pobject);
}

TEST(polygon_boolean, test9) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0, 0));
	lo.push_back(Point(2, 0));
	lo.push_back(Point(2, 2));
	lo.push_back(Point(0, 2));

	PC pobject(lo);

	two_pointchains("test9", pclip, pobject);
}
//
TEST(polygon_boolean, test10) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.0, 0));
	lo.push_back(Point(1.5, 0));
	lo.push_back(Point(0.5, 2));

	PC pobject(lo);

	two_pointchains("test10", pclip, pobject);
}

TEST(polygon_boolean, test11) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(1.5, -1.5));
	lo.push_back(Point(2.5, 0.5));
	lo.push_back(Point(1.5, 1.0));
	PC pobject(lo);

	two_pointchains("test11", pclip, pobject);
}

TEST(polygon_boolean, test12) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, -0.5));
	lo.push_back(Point(2.5, -0.5));
	lo.push_back(Point(2.5, 0.5));
	lo.push_back(Point(2.0, 0.5));
	lo.push_back(Point(2.0, 1.5));
	lo.push_back(Point(1.0, 1.0));
	PC pobject(lo);

	two_pointchains("test12", pclip, pobject);
}


TEST(polygon_boolean, test13) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0,   0));
	lc.push_back(Point(2,   0));
	lc.push_back(Point(2,   0.5));
	lc.push_back(Point(1.0, 1.0));
	lc.push_back(Point(2,   1.5));
	lc.push_back(Point(2,   2));
	lc.push_back(Point(0,   2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, -0.5));
	lo.push_back(Point(2.5, -0.5));
	lo.push_back(Point(2.5, 0.5));
	lo.push_back(Point(2.0, 0.5));
	lo.push_back(Point(2.0, 1.5));
	lo.push_back(Point(1.0, 1.0));
	PC pobject(lo);


//	GF::WritePointChain("test13_clip", pclip);

	two_pointchains("test13", pclip, pobject);
}

TEST(polygon_boolean, test14) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0,   0));
	lc.push_back(Point(2,   0));
	lc.push_back(Point(2,   2));
	lc.push_back(Point(0,   2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(-0.8, -0.5));
	lo.push_back(Point(2.5, -0.5));
	lo.push_back(Point(2.5, 0.5));
	lo.push_back(Point(-0.3, 0.5));
	lo.push_back(Point(-0.3, 1.5));
	lo.push_back(Point(2.3, 1.5));
	lo.push_back(Point(2.3, 1.8));
	lo.push_back(Point(-0.8, 1.8));
	PC pobject(lo);

//	GF::WritePointChain("test14_clip", pclip);

	two_pointchains("test14", pclip, pobject);
}


TEST(polygon_boolean, test15) {
	// box seperate
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0,   0));
	lc.push_back(Point(2,   0));
	lc.push_back(Point(2,   2));
	lc.push_back(Point(0,   2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(2.1, 2.1));
	lo.push_back(Point(2.5, 2.1));
	lo.push_back(Point(3.5, 3.5));
	lo.push_back(Point(2.1,   3.5));
	PC pobject(lo);

	GF::WritePointChain("test15_clip", pclip);

	two_pointchains("test15", pclip, pobject);
}

TEST(polygon_boolean, test16) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(1.5, 0.5));
	lo.push_back(Point(1.5, 1.0));
	lo.push_back(Point(1.0, 1.0));
	lo.push_back(Point(0.3, 0.5));
	PC pobject(lo);

	two_pointchains("test16", pclip, pobject);
}
TEST(polygon_boolean, test17) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(2.0, 0.5));
	lo.push_back(Point(2.0, 1.0));
	lo.push_back(Point(2.5, 1.3));
	lo.push_back(Point(2.3, 0.5));
	PC pobject(lo);

	two_pointchains("test17", pclip, pobject);
}
TEST(polygon_boolean, test18) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, 0.0));
	lo.push_back(Point(2.0, 0.0));
	lo.push_back(Point(2.0, 1.3));
	lo.push_back(Point(1.0, 1.0));
	PC pobject(lo);

	two_pointchains("test18", pclip, pobject);
}

TEST(polygon_boolean, test19) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, 0.5));
	lo.push_back(Point(1.5, 0.4));
	lo.push_back(Point(0.8, 2));

	PC pobject(lo);

	two_pointchains("test19", pclip, pobject);
}
TEST(polygon_boolean, test20) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0, 0));
	lo.push_back(Point(1, -0.5));
	lo.push_back(Point(2, 0));
	lo.push_back(Point(2, 2));
	lo.push_back(Point(1, 2.5));
	lo.push_back(Point(0, 2));

	PC pobject(lo);

	two_pointchains("test20", pclip, pobject);
}




} //end namespace
#endif
