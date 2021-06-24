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


void plot_triangle_by_code(Gnuplot& gnu, const Tri2& t, int code0, int code1){
	// input location code
	GAM gam;
	for(int i = 0; i < 3; i++){
		GAM::spActor actor = gam.points(t[i], -1);
		if((code0 == i) || (code1 == i)){
			actor->style() = "with points pt 5 ps 1 lc variable";
		}else{
			actor->style() = "with points pt 2 ps 1 lc variable";
		}
		gnu.add(actor);
	}
	// plot edge
	std::array<std::array<int, 2>, 3> arridx = {{
		{{0, 1}},{{0,2}},{{1, 2}}
	}}; 
	for(int i = 0; i < 3; i++){
		auto& pair = arridx[i];    
		auto& p0 = t[pair[0]];
		auto& p1 = t[pair[1]];
		auto actor = gam.arrows(p0,p1);
		if((code0 == i + 3) || (code1 == i + 3)){
			std::cout << "T edge = " << i + 3 << "intersect" << std::endl;
			actor->style() = "with vectors lw 2 lc variable";
		}else{
			actor->style() = "with vectors lc variable";
		}
		gnu.add(actor);
	}


}


TEST(segtri, initial){
	typedef IntersectionSegTri_<double, 2> Inter;
	Point2 x(0.1, 0.0);
	Point2 y(0.3, 0.9);
	Seg2 seg(x, y);
	Point2 t0(0,   0);
	Point2 t1(1.,  0.0);
	Point2 t2(0.0, 1.0);
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
    gseg->style()   = "with linespoints pt 6 ps 2 lc variable";
	int ct0 = inter.triangle_intersection_code(0);
	int ct1 = inter.triangle_intersection_code(1);
	plot_triangle_by_code(gnu, tri, ct0, ct1);

	// auto gat = gam.lines_points(tri.begin(), tri.end(), -1, true);

	gnu.add(gseg);
	// gnu.add(gat);

	gnu.set_equal_aspect_ratio();
	gnu.plot();

}