#ifndef _TWO_SEGMENTS_HPP_
#define _TWO_SEGMENTS_HPP_

#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>   Point2;
typedef Segment_<double, 2> Segment2;
typedef Intersection_<Segment2, Segment2>  Inter;
typedef GGnuplotActor_<double, 2>      GA;

int a_case(const Point2& p1,
           const Point2& p2,
           const Point2& p3,
           const Point2& p4){
    Segment2 seg1(p1, p2);
    Segment2 seg2(p3, p4);

    Inter inter(seg1, seg2);
    auto strtype = ToString(inter.cal_intersection_type());
    std::cout << "Intersection Type : "<< strtype << std::endl;
    Point2 np = inter.cal_intersection_point();
    std::cout << "new point         : "<< np << std::endl;

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/" + strtype);
    gnu.set_xrange(-5, 5);
    gnu.set_yrange(-5, 5);
    gnu.set_label(1, strtype, -4.5, 4);

    auto a1 = ToGnuplotActor(seg1);
    a1.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");

    auto a2 = ToGnuplotActor(seg2);
    a2.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#F25022\"");

    auto a3 = ToGnuplotActor(np);
    a3.style("with linespoints pointtype 1 pointsize 3 lw 3 lc black");

    gnu.add(a1);
    gnu.add(a2);
    gnu.add(a3);
    gnu.plot();
    return 0;
}

void two_segments_test(){
    Point2 p1(  -1,    0);
    Point2 p2(   3,  0.5);
    Point2 p3( 0.8,  2.0);
    Point2 p4(-0.3, -1.0);
    // intersect
    a_case(p1, p2, p3, p4);
    // NO
    a_case(Point2( -1,   0), Point2(3,    0.5),
           Point2(0.8, 2.0), Point2(-0.3, 1.0));
    // CONNECT
    a_case(Point2( -1,   0), Point2(3,    0.5),
           Point2( -1,   0), Point2(-0.3, 1.0));
    // TOUCH
    a_case(Point2( -1,   0), Point2(3,    0.0),
           Point2(  0,   0), Point2(-0.5, 1.0));
    // OVERLAP
    a_case(Point2(  0, -1), Point2( 4,   3),
           Point2( -1, -2), Point2( 2,   1));
    // SAME
    a_case(Point2( -1, -1), Point2( 2,   2),
           Point2( -1, -1), Point2( 2,   2));
}

#endif