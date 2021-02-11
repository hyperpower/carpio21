#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include "utility/random.hpp"

using namespace carpio;

typedef Point_<double, 2> Point;
typedef PointChain_<double, 2> PC;
typedef GGnuplotActor_<double, 2> GA;

TEST(point_chain, initial){
	std::list<Point> l;
	l.push_back(Point(0,0));
	l.push_back(Point(1,0));
	l.push_back(Point(1,1));
	l.push_back(Point(0,1));

	PC pc(l);
	Gnuplot gnu;
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Arrows(pc, 0));
//	gnu.plot();
}

TEST(point_chain, winding_number){
	std::list<Point> l =
	{ Point(0,   0),
	  Point(1,   0),
	  Point(-0.1 ,0.5),
	  Point(1,   1),
	  Point(0,   1)};

	PC pc(l);

	std::list<Point> ltp;

	ltp.push_back(Point(Random::nextDouble(-0.5, 1.5), Random::nextDouble(-0.5, 1.5)));
	ltp.push_back(Point(0.6, 0.8));
	ltp.push_back(Point(0.8, 0.5));
	ltp.push_back(Point(0.3, 1.0));
	ltp.push_back(Point(0,   1.0));
	ltp.push_back(Point(0,   0.5));
	ltp.push_back(Point(0.8, 0.3));
	ltp.push_back(Point(1.0, 0.0));

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);
	int count = 1;
	for(auto& p : ltp){
		bool wn = IsInOn(p, pc);
		auto actor = GA::Points(p, wn? 3 : 4);
		actor->style() = "with points ps 2 pt 7 lc variable";
		gnu.add(actor);
		gnu.set_label(count, wn? "  IN ":"  OUT ", p.x(), p.y(), "left" );
		count++;
	}
	auto a_arrows = GA::Arrows(pc, 2);
	a_arrows->style() = "with vectors lw 2 lc variable";
	gnu.add(a_arrows);
//	gnu.plot();
}

TEST(point_chain, is_simple){
	std::list<Point> l =
		{ Point(0,   0),
		  Point(1,   0),
		  Point(-0.1 ,0.5),
		  Point(1,   1),
		  Point(0,   1)};
	PC pc(l);
	// std::cout << "Is Simple = " << pc.is_simple() << std::endl;
	Gnuplot gnu;
	gnu.set_xrange(-0.5, 2);
	gnu.set_yrange(-0.5, 2);
	gnu.add(GA::Arrows(pc, 0));
    gnu.set_terminal_png("./test_output/point_chain_is_simple");
	gnu.plot();
}


//TEST(point_chain, winding_number_2){
//	typedef Point_<double, 2> Point;
//	typedef PointChain_<double, 2> PC;
//
//	std::list<Point> l;
//	l.push_back(Point(0,   0));
//	l.push_back(Point(0,   1));
//	l.push_back(Point(1,   1));
//	l.push_back(Point(0.5 ,0.5));
//	l.push_back(Point(1,   0));
//
//	PC pc(l);
//
//	std::list<Point> ltp;
//	ltp.push_back(Point(0.6, 1.8));
//	ltp.push_back(Point(0.6, 0.8));
//	ltp.push_back(Point(0.8, 0.5));
//	ltp.push_back(Point(0.3, 1.0));
//	ltp.push_back(Point(0,   1.0));
//	ltp.push_back(Point(0,   0.3));
//	ltp.push_back(Point(0.8, 0.3));
//	ltp.push_back(Point(0.8, 0.0));
//
//
//	Gnuplot gnu;
//	gnu.set_xrange(-1, 2);
//	gnu.set_yrange(-1, 2);
//	int count = 1;
//	for(auto& p : ltp){
//		int wn = IsInOn(p, pc);
//		gnu.add(GnuplotActor::Points(p, 3));
//		gnu.set_label(count, "" + ToString(wn), p.x(), p.y(), "left" );
//		count++;
//	}
//	gnu.add(GnuplotActor::Arrows(pc, 0));
//	gnu.plot();
//
//}

