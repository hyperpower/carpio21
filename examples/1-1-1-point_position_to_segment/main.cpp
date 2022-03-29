#include <iostream>
#include <memory> 
#include <string>
#include <algorithm>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>         Point;
typedef Segment_<double, 2>       Seg;
typedef GGnuplotActor_<double, 2> GA;


std::string a_case(const Point& p1, const Point& p2, const Point& p3){
	auto res     = OnWhichSide7(p1, p2, p3);
	auto str_res = ToString(res);

	Seg  seg(p1, p2);
	// Plot
	Gnuplot gnu;
	gnu.set_terminal_png(
			"./fig/" + str_res, // const std::string& filename,
			800,                // double  x         = 800,
			600,                // double  y         = 600,
			"Sans",             // const std::string& font     = "Helvetica",
			13                  // int  fontsize  = 12);
			);
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);

	std::replace( str_res.begin(), str_res.end(), '_', ' ');
	str_res = "Location status = " + str_res;
	gnu.set_label(1, str_res, -0.3, 1.2);

	auto a1 = ToGnuplotActor(seg);
	a1->style() = "with linespoints pointtype 7 pointsize 3 lw 2 lc black";

	auto a2 = ToGnuplotActor(p3);
	a2->style() = "with points pointtype 5 pointsize 3 lc rgb \"#00A4EF\"";
	
	gnu.add(a1);
	gnu.add(a2);
	gnu.plot();
	return str_res;
}

int main(int argc, char** argv) {
    Point p1( 0.0, 0.0);
    Point p2( 1.0, 1.0);
    
    // left
    a_case(p1, p2, Point(0.5, 0.7));
    // right
    a_case(p1, p2, Point(0.8, 0.3));
    // on start end
    a_case(p1, p2, p1);
    // on end
    a_case(p1, p2, p2);
    // inside
    a_case(p1, p2, Point(0.5, 0.5));
    // end extension
    a_case(p1, p2, Point(1.2, 1.2));
    // start extension
    a_case(p1, p2, Point(-0.1, -0.1));
}