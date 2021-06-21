#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";
const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActorMaker_<double, 2> GAM;
typedef Triangle_<double, 2> Tri2;
typedef Segment_<double, 2> Seg2;



TEST(segtri, initial){
	typedef IntersectionSegTri_<double, 2> Inter;
	Point2 x(0.0, 0.0);
	Point2 y(1.1, 0.4);
	Seg2 seg(x, y);
	Point2 t0(0,  0);
	Point2 t1(1.1, 0.3);
	Point2 t2(0.1, 1.2);
	Tri2 tri(t0, t1, t2);
	Inter inter(tri, seg);
	inter.is_intersect();

    Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);
	GAM     gam;
	auto gax = gam.points(x);
	auto gay = gam.points(y);
	auto gseg = gam.lines_points(seg.begin(),seg.end());

	auto gat = gam.lines_points(tri.begin(), tri.end(), -1, true);

	gnu.add(gseg);
	gnu.add(gat);

	gnu.set_equal_aspect_ratio();
	gnu.plot();

}