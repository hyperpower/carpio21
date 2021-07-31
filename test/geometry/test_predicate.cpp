#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>
#include <vector>

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";
const St dim = 3;

typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActorMaker_<double, 2> GAM;
typedef Triangle_<double, 2> Tri2;

Point2 generate_a_point(const Point2& po, double dv, int i, int j){
	return Point2(po[0] + dv * i, po[1] + dv * j);
}

std::vector<Point2> generate_points_shift_x(const Point2& po, double dv, int nl, int nx){
	std::vector<Point2> res;
	for(int i = 0; i < nl; i++){
		auto p = generate_a_point(po, dv, i + nx, i);
		res.push_back(p);
	}
	return res;
}

double orient2dfast(const Point2& pa, const Point2& pb, const Point2& pc)
{
  double acx, bcx, acy, bcy;

  acx = pa[0] - pc[0];
  bcx = pb[0] - pc[0];
  acy = pa[1] - pc[1];
  bcy = pb[1] - pc[1];
  return acx * bcy - acy * bcx;
}

Trinary _WhichSide3fast(const Point2& pa, const Point2& pb, const Point2& pc){
	double tmp = orient2dfast(pa, pb, pc);
    if (tmp > 0) {
        return _POSITIVE_;
    } else if (tmp < 0) {
        return _NEGATIVE_;
    } else {
        return _ZERO_;
    }
}
Trinary _WhichSide3exact(const Point2& pa, const Point2& pb, const Point2& pc){
	double tmp = exact::orient2d(pa.data(), pb.data(), pc.data());
    if (tmp > 0) {
        return _POSITIVE_;
    } else if (tmp < 0) {
        return _NEGATIVE_;
    } else {
        return _ZERO_;
    }
}
TEST(predicate, just_one){
	Point2 p1(-10, -10);
	Point2 p2( 10,  10);

    Point2 po(0.0, 0.0);
	double dv = 1e-16;
	Gnuplot gnu; 
	GAM gam;
    std::ostringstream sst;
    sst << "fig";
    gnu.set_terminal_png(OUTPUTPATH + sst.str());
	int nrow = 20;
	for(int j = -nrow; j <= nrow; j++){
		auto pl = generate_points_shift_x(po, dv, 100, j);
		for(auto p : pl){
			auto t = _WhichSide3exact(p2, p, p1);
			// std::cout << ToString(t) << std::endl;
			if (t == _POSITIVE_){
				auto actor = gam.points(p);
				actor->style()  = "with points pt 1 lc 1";
				gnu.add(actor);
			}else if (t == _NEGATIVE_){
				auto actor = gam.points(p);
				actor->style()  = "with points pt 1 lc 2";
				gnu.add(actor);
			}else{
				auto actor = gam.points(p);
				actor->style()  = "with points pt 1 lc 3";
				gnu.add(actor);
			}
		}
	}
    // gnu.set_xrange(-0.1, 0.1);
    // gnu.set_yrange(-0.1, 0.1);
    gnu.set_equal_aspect_ratio();
    gnu.plot();  
}