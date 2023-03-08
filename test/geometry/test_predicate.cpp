#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>
#include <vector>

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";
const St dim = 3;

typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef Triangle_<double, 2> Tri2;

Point2 generate_a_point(const Point2& po, double dv, int i, int j){
	return Point2(po[0] + dv * i, po[1] + dv * j);
}

std::vector<Point2> generate_points_shift_x(const Point2& po, double dv, int nx, int ny){
	std::vector<Point2> res;
	for(int i = -nx; i < nx; i++){
		auto p = generate_a_point(po, dv, i, ny);
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
	double tmp = exact::orient2d<Point2::value_type>(pa.data(), pb.data(), pc.data());
    if (tmp > 0) {
        return _POSITIVE_;
    } else if (tmp < 0) {
        return _NEGATIVE_;
    } else {
        return _ZERO_;
    }
}
Trinary _WhichSide3adptive(const Point2& pa, const Point2& pb, const Point2& pc){
	double tmp = adaptive::orient2d<Point2::value_type>(pa.data(), pb.data(), pc.data());
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
	double dv = 1e-17;
	Gnuplot gnu; 
    std::ostringstream sst;
    sst << "output";
    gnu.set_terminal_png(OUTPUTPATH + sst.str());
	int nrow = 20;
	for(int j = -nrow; j <= nrow; j++){
		auto pl = generate_points_shift_x(po, dv, 100, j);
		for(auto p : pl){
			auto t = _WhichSide3fast(p2, p, p1);
			// std::cout << ToString(t) << std::endl;
			if (t == _POSITIVE_){
				auto actor = ToGnuplotActor(p);
				actor.style("with points pt 1 lc 1");
				gnu.add(actor);
			}else if (t == _NEGATIVE_){
				auto actor = ToGnuplotActor(p);
				actor.style("with points pt 1 lc 2");
				gnu.add(actor);
			}else{
				auto actor = ToGnuplotActor(p);
				actor.style("with points pt 1 lc 3");
				gnu.add(actor);
			}
		}
	}
    // gnu.set_xrange(-0.1, 0.1);
    // gnu.set_yrange(-1e-15, 1e-15);
    gnu.set_equal_aspect_ratio();
    gnu.plot();  
}

template<class TYPE>
void add_a_small_number(const std::string& type_name, TYPE dv){
	// small number will be eaten
	TYPE one = 1.0;
	// TYPE dv  = 1e-17;

	Gnuplot gnu; 
    std::ostringstream sst;
    sst << "one_add_small_" + type_name;
    gnu.set_terminal_png(OUTPUTPATH + sst.str());
	int flag_once1 = 0;
	int flag_once2 = 0;
    for(int i = 0; i <= 1e3; i++){
		TYPE small = dv + i * dv;
		bool flag = (one == one + small);
		if(flag){
			Point2 p(small, 1);
			auto actor = ToGnuplotActor(p);
			actor.style("with points pt 2");
			if(flag_once1 == 0){
				actor.title("one == one + small");
				flag_once1 = 1;
			}
			actor.point_size(2);
			actor.line_color_red();
			gnu.add(actor);
		}else{
			Point2 p(small, 0);
			auto actor = ToGnuplotActor(p);
			actor.style("with points pt 4");
			if(flag_once2 == 0){
				actor.title("one != one + small");
				flag_once2++;
			}
			actor.line_color_blue();
			gnu.add(actor);
		}
	}
	gnu.set_xlogscale();
	gnu.set_xlabel("Small Number");
	gnu.set_ylabel("Result");
	gnu.set_label(1, "Number type is " + type_name, 2 * dv, 1.1);
    gnu.set_yrange(-0.2, 1.2);
	gnu.set_grid();
    gnu.plot();  
}

TEST(predicate, small_number){
	add_a_small_number<double>("double", 1e-17);
	add_a_small_number<float>("float", 1e-9);
}
