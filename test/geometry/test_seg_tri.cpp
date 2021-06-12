#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";
const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActor_<double, 2> GA;
typedef Triangle_<double, 2> Tri2;
typedef Segment_<double, 2> Seg2;



TEST(segtri, initial){
	typedef IntersectionSegTri_<double, 2> Inter;
	Point2 x(1.1, 0.2);
	Point2 y(0.2, 1.5);
	Seg2 seg(x, y);
	Point2 t1(1.1, 0);
	Point2 t2(0,  1.5);
	Point2 t3(0,  0);
	Tri2 tri(t1, t2, t3);
	Inter inter(tri, seg);

    
}