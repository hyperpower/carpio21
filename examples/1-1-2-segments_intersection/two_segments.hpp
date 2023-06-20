#ifndef _TWO_SEGMENTS_HPP_
#define _TWO_SEGMENTS_HPP_

#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "segment_with_name.hpp"
// #include "algebra/number/rational.hpp"

using namespace carpio;

typedef Point_<double, 2>   Point;
typedef SegmentWithName_<double, 2> Segment;
typedef Intersection_<Segment, Segment>  Inter;
typedef typename Inter::Result Result;
// typedef GGnuplotActor_<double, 2>      GA;

int plot_a_case(const std::string& case_name,
                const Segment& seg1,
                const Segment& seg2,
                const Point&   interp){
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/" + case_name);
    gnu.set_xrange(-5, 5);
    gnu.set_yrange(-5, 5);
    gnu.set_label(1, case_name, -4.5, 4);

    auto a1 = ToGnuplotActor(seg1);
    a1.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
    auto nv1 = seg1.normal_unit_vector();
    double ratio = 0.2;
    std::ostringstream sst;
    sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
        << nv1.x() * ratio << ", " << nv1.y() * ratio; 
    gnu.set_label(1, seg1.get_name(), seg1.pc().x(), seg1.pc().y(),  sst.str());
    auto a11 = ToGnuplotActorAsVector(seg1);
    a11.title("Segment " + seg1.get_name());
    a11.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");

    auto a2 = ToGnuplotActor(seg2);
    a2.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#F25022\"");
    nv1 = seg2.normal_unit_vector();
    sst.str("");
    sst << "front font \", 18\" textcolor rgb \"#F25022\" offset first " 
        << nv1.x() * ratio << ", " << nv1.y() * ratio; 
    gnu.set_label(2, seg2.get_name(), seg2.pc().x(), seg2.pc().y(),  sst.str());
    auto a21 = ToGnuplotActorAsVector(seg2);
    a21.title("Segment " + seg2.get_name());
    a21.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#F25022\"");

    auto a3 = ToGnuplotActor(interp);
    a3.style("with points pointtype 1 pointsize 3 lw 3 lc black");
    a3.title("Intersection point " + ToString(interp));

    gnu.set_grid();

    gnu.add(a1);
    gnu.add(a11);
    gnu.add(a2);
    gnu.add(a21);
    gnu.add(a3);

    gnu.plot();
    return 0;

}

int a_case(const Point& p1,
           const Point& p2,
           const Point& p3,
           const Point& p4){
    Segment seg1(p1, p2, "a");
    Segment seg2(p3, p4, "b");

    Inter inter(seg1, seg2);
    auto res = inter.execute();
    std::cout << "Intersection Type : "<< res.type  << std::endl;
    std::cout << "new point         : "<< res.point << std::endl;

    plot_a_case(ToString(res.type), seg1, seg2, res.point);
    return 0;
}

void two_segments_test(){
    Point p1(  -3,  0.2);
    Point p2(   3,  0.5);
    Point p3( 2.1,  3.0);
    Point p4(-0.8, -1.5);
    // intersect
    a_case(p1, p2, p3, p4);
    // NO
    a_case(Point( -1,   0), Point(3,    0.5),
           Point(0.8, 2.0), Point(-0.3, 1.0));
    // CONNECT
    a_case(Point( -1,   0), Point(3,    0.5),
           Point( -1,   0), Point(-0.3, 1.0));
    // TOUCH
    a_case(Point( -1,   0), Point(3,    0.0),
           Point(  2,   0), Point(-0.5, 1.0));
    // OVERLAP
    a_case(Point(  0, -1), Point( 4,   3),
           Point( -1, -2), Point( 2,   1));
    // SAME
    a_case(Point( -1, -1), Point( 2,   2),
           Point( -1, -1), Point( 2,   2));
}


void intersection_on_segment(){
    typedef Rational_<int> Cvt;
    // typedef float Cvt;
    typedef Point_<Cvt, 2> Poi;
    typedef SegmentWithName_<Cvt, 2> Seg;
    typedef Intersection_<Seg, Seg>  Inter;

    Poi p1(  -30, -30);
    Poi p2(   30,  30);

    int dx = 1;
    Poi p3(-25 + dx , -25);
    Poi p4( 25 - dx,   25);
    std::cout << p3 << std::endl;

    Seg seg1(p1, p2, "a");
    Seg seg2(p3, p4, "b");

    // Inter inter(seg1, seg2);
    // auto strtype = ToString(inter.cal_intersection_type());
    // std::cout << "Intersection Type : "<< strtype << std::endl;
    // Poi np = inter.cal_intersection_point();
    // std::cout << "new point         : "<< np << std::endl;

	// auto res     = OnWhichSide7(p1, p2, np);
	// auto str_res = ToString(res);
    // std::cout << "Intersection point position : " << str_res << std::endl;

    // plot_a_case("inter", seg1, seg2, np);
}

#endif