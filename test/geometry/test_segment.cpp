/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#include <functional>

#include "geometry/geometry.hpp"
#include "utility/random.hpp"
#include "utility/profile.hpp"
#include "gtest/gtest.h"
#include "algebra/number/rational.hpp"

using namespace carpio;

typedef double NumType;
// typedef Rational_<long> NumType;
typedef Point_<NumType, 3> Point3;
typedef Point_<NumType, 2> Point2;
typedef Segment_<NumType, 2> Seg2;
typedef GGnuplotActor_<NumType, 2> GA;


TEST(segment, mkdir){
    // check current working dir
    auto curdir = GetWorkingPath();
    std::cout << "Current Working dir = " << curdir << std::endl;
    auto binclude = FileAccessCheck(curdir + "/include", 0);
    std::cout << "Is include Ok = " << binclude << std::endl;
    if (! binclude) {
        GTEST_FATAL_FAILURE_("Current working directroy may be not in Project Folder");
    }
    // Do not disable this test
    MakeDir("./fig/");
}
TEST(segment, rational){
    typedef Rational_<int> Rat;
    std::cout << "Is Number (int)     = " << IsNumber<int>::value << std::endl;
    std::cout << "Is Number (Rationl) = " << IsNumber<Rat>::value << std::endl;
    std::cout << "Is Number (segment) = " << IsNumber<Seg2>::value << std::endl;
}
TEST(segment, segment_initial){
    Point2 x(1, 0);
    Point2 y(0, 1);
    std::cout << "Initial three points and output/n";
    std::cout << "Point x = " << x << std::endl;
    std::cout << "Point y = " << y << std::endl;

    Seg2 seg(x, y);
    std::cout << "The segemnt is " << seg << std::endl;
    ASSERT_EQ(seg.ps(), x);
    ASSERT_EQ(seg.pe(), y);
}


TEST(segment, segment_plot){
    Point2 x(1, 0);
    Point2 y(0, 1);
    std::cout << "Initial three points and output" << std::endl;
    std::cout << "Point x = " << x << std::endl;
    std::cout << "Point y = " << y << std::endl;

    Seg2 seg(x, y);
    Gnuplot gnu;
    gnu.add(ToGnuplotActor(seg));
//	gnu.plot();
    std::cout << "The segemnt is " << seg << std::endl;
    ASSERT_EQ(seg.ps(), x);
    ASSERT_EQ(seg.pe(), y);
}

TEST(segment, segment_point_location){
    Point2 x(0, 0);
    Point2 y(0, 1);
    Seg2 seg(x, y);

    Point2 p(0.0, 1.50);

    std::cout << "The segemnt is :" << seg << std::endl;
    std::cout << "The point is   :" << p << std::endl;
    auto side = OnWhichSide7(seg, p);
    std::cout << "Position       :" << ToString(side) << std::endl;
    ASSERT_EQ(side, _PS_OUT_END_);
}

TEST(segment, intersection_intersect){
    Point2 p1(  -1,    1);
    Point2 p2(   2,  1.5);
    Point2 p3( 0.8,  2.0);
    Point2 p4(-0.3, -1.0);
    Seg2 seg1(p1, p2);
    Seg2 seg2(p3, p4);

    typedef Intersection_<Seg2, Seg2> Intersection;

    Intersection inter(seg1, seg2);
    auto strtype = ToString(inter.cal_intersection_type());
    std::cout << "Intersection Type : "<< strtype << std::endl;
    ASSERT_EQ(strtype, "INTERSECT");
    Point2 np = inter.cal_intersection_point();
    std::cout << "new point         : "<< np << std::endl;

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/line_segment_1");
    gnu.set_xrange(-3, 3);
    gnu.set_yrange(-3, 3);
    gnu.set_label(1, strtype, -2.5, 2);
    gnu.add(ToGnuplotActor(seg1).line_color(2).line_width(2));
    gnu.add(ToGnuplotActor(seg2).line_color(1).line_width(2));
    gnu.add(ToGnuplotActor(np));
    gnu.plot();
}

TEST(segment, intersection){
    typedef Intersection_<Seg2, Seg2> Inter;
    Point2 p1(  2.0,  0.5);
    Point2 p2(  2.0,  2.0);
    Point2 p3(  3.0,  1.0);
    Point2 p4(  1.0,  3.0);
    Seg2 seg1(p1, p2);
    Seg2 seg2(p3, p4);

    Inter inter(seg1, seg2);
    auto strtype = ToString(inter.cal_intersection_type());
    std::cout << "Intersection Type : "<< strtype << std::endl;
    ASSERT_EQ(strtype, "TOUCH");
    Point2 np = inter.cal_intersection_point();
    std::cout << "new point         : "<< np << std::endl;
    ASSERT_EQ(np, Point2(2.0, 2.0));

    auto s11 = inter.point_position(0);
    std::cout <<"ps1 to seg2: " << ToString(s11) << std::endl; 
    s11 = inter.point_position(1);
    ASSERT(s11 == _PS_IN_);
    std::cout <<"pe1 to seg2: " << ToString(s11) << std::endl; 
    s11 = inter.point_position(2);
    std::cout <<"ps2 to seg1: " << ToString(s11) << std::endl; 
    s11 = inter.point_position(3);
    std::cout <<"pe2 to seg1: " << ToString(s11) << std::endl; 
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/line_segment_2");
    gnu.set_xrange(-0, 5);
    gnu.set_yrange(-0, 5);
    gnu.set_label(1, strtype, 1.0, 4);
    gnu.add(ToGnuplotActor(seg1).line_color(0).line_width(2));
    gnu.add(ToGnuplotActor(seg2).line_color(1).line_width(2));
    gnu.add(ToGnuplotActor(np));
    gnu.plot();
}
TEST(segment, intersection3){
    typedef Intersection_<Seg2, Seg2> Inter;
    Point2 p1(  0.0,    0);
    Point2 p2(  1.0,  0.0);
    Point2 p3( -3.0,  0.0);
    Point2 p4(  0.5,  0.0);
    Seg2 seg1(p1, p2);
    Seg2 seg2(p3, p4);

    Inter inter(seg1, seg2);
    auto strtype = ToString(inter.cal_intersection_type());
    std::cout << "Intersection Type : "<< strtype << std::endl;
    Point2 np = inter.cal_intersection_point();
    std::cout << "new point         : "<< np << std::endl;

    auto res = inter.execute();
    res.show();

    auto s11 = inter.point_position(0);
    std::cout <<"ps1 to seg2: " << ToString(s11) << std::endl; 
    s11 = inter.point_position(1);
    std::cout <<"pe1 to seg2: " << ToString(s11) << std::endl; 
    s11 = inter.point_position(2);
    std::cout <<"ps2 to seg1: " << ToString(s11) << std::endl; 
    s11 = inter.point_position(3);
    std::cout <<"pe2 to seg1: " << ToString(s11) << std::endl; 

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/line_segment_3");
    gnu.set_xrange(-5, 5);
    gnu.set_yrange(-5, 5);
    gnu.set_label(1, strtype, -4.5, 4);
    gnu.add(ToGnuplotActor(seg1));
    gnu.add(ToGnuplotActor(seg2).line_color(1));
    gnu.add(ToGnuplotActor(np));
    gnu.plot();
}

auto GenerateRandomSegments(int num,
                            const double& xmin, const double& xmax,
                            const double& ymin, const double& ymax){
    typedef Segment_<double, 2> Segment;
    typedef std::list<Segment> ListSegment;
    ListSegment lseg;
    // Random::seed(std::time(0));
    for (int i = 0; i< num ; i++){
        double x1 = Random::nextDouble(xmin, xmax);
        double y1 = Random::nextDouble(ymin, ymax);
        double x2 = Random::nextDouble(xmin, xmax);
        double y2 = Random::nextDouble(ymin, ymax);
        lseg.push_back(Segment(x1, x2, y1, y2));
    }
    return lseg;
}


TEST(segment, multi){
    ProfileStart("N2");
    ProfileStart("GenerateSegments");
    auto lseg = GenerateRandomSegments(100, 0, 1000, 0, 1);
    ProfileEnd();
    // intersection =========
    ProfileStart("Intersect");
    auto res = Intersect(lseg);
    ProfileEnd();
    ProfileEnd();
    ProfileListShow();
    ProfileClean();
    
    // show =================
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/line_segment_4");
    gnu.set_xrange(-0.2, 1.2);
    gnu.set_yrange(-0.2, 1.2);
    gnu.add(ToGnuplotActor(lseg));

    int count = 1;
    for(auto& r : res){
        // std::cout << count << std::endl;
        // r.show();
        auto actor = ToGnuplotActor(r.point);
        gnu.add(actor);
        count++;
    }
    gnu.plot();
}

TEST(segment, sweep_line){
    ProfileStart("SweepLine");
    ProfileStart("SGenerateSegments");
    auto lseg = GenerateRandomSegments(100, 0, 1000, 0, 1);
    ProfileEnd();
    // intersection =========
    typedef Segment_<double, 2> Segment;
    ProfileStart("SSortSegments");
    IntersectionSweepLineSimple_<Segment> inter(lseg);
    ProfileEnd();

    ProfileStart("SExecute");
    auto res = inter.execute();
    ProfileEnd();
    // inter.print_event_queue();
    ProfileEnd();
    ProfileListShow();
    ProfileClean();
    // IntersectionRet<Segment, Segment> ret;
    // ret.geo1;
    // show =================
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/line_segment_5");
    gnu.set_xrange(-0.2, 1.2);
    gnu.set_yrange(-0.2, 1.2);
    gnu.add(ToGnuplotActor(lseg));

    int count = 1;
    for(auto& r : res){
        // std::cout << count << std::endl;
        // r.show();
        auto actor = ToGnuplotActor(r.point);
        gnu.add(actor);
        count++;
    }
    gnu.plot();
}

typedef GeoComponent_<Seg2> GeoComponent;
typedef GeoStatus_<GeoComponent> GeoStatus;

void visit_output(GeoStatus::cpNode pn){
    std::cout << "x = " << pn->value.sweep_x << " y = " << pn->value.value_y << std::endl;
}

TEST(segment, geo_status_b){

    typedef std::function<bool(GeoComponent&, GeoComponent&)> CompareFun;

    double cur_x =1.2;

    CompareFun cfun = [&cur_x](GeoComponent& a, GeoComponent& b){
        a.sweep_x = cur_x;
        return a.value_y < b.value_y;
    };

    GeoStatus status;

    GeoComponent gc1(nullptr, 1, 2);
    status.insert(gc1, cfun);
    GeoComponent gc2(nullptr, 1, 3);
    status.insert(gc2, cfun);
    GeoComponent gc3(nullptr, 1, 1.5);
    status.insert(gc3, cfun);

    cur_x = 0.1;
    GeoComponent gc4(nullptr, 1, 1.3);
    status.insert(gc4, cfun);
    status.in_order(visit_output);
}


