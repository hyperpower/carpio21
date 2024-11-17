#include <functional>

#define _DEBUG_MODE_

#include "utility/random.hpp"
#include "utility/profile.hpp"
#include "gtest/gtest.h"

#include "io/io_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/operation/boolean/intersection_base.hpp"
#include "geometry/operation/boolean/segment_segment.hpp"
// #include "geometry/operation/boolean/multi_segments.hpp"

#include "geometry/ben-ott/segment_slope.hpp"
#include "geometry/ben-ott/segment_with_slope.hpp"
#include "geometry/ben-ott/sweep_event.hpp"
#include "geometry/ben-ott/sweep_event_queue.hpp"
#include "geometry/ben-ott/comp_seg.hpp"
#include "geometry/ben-ott/ben_ott_intersection.hpp"

using namespace carpio;

typedef double NumType;
// typedef Rational_<long> NumType;
typedef Point_<NumType, 2> Point;
typedef Segment_<NumType, 2> Segment;
typedef IntersectionBenOtt_<Segment>  Inter;

TEST(ben_ott, mkdir){
    // check current working dir
    auto curdir = GetWorkingPath();
    std::cout << "Current Working dir = " << curdir << std::endl;
    auto binclude = FileAccessCheck(curdir + "/include", 0);
    if (! binclude) {
        GTEST_FATAL_FAILURE_("Current working directroy may be not in Project Folder");
    }
    // Do not disable this test
    MakeDir("./fig/");

    #ifdef _DEBUG_MODE_
    std::cout << "[ IN DEBUG MODE ]" << std::endl;
    #endif
}

TEST(ben_ott, slope){
    Segment s1(Point(0, 0), Point(5, 0));
    Segment s2(Point(0, 0), Point(5, 5));
    Segment sup(Point(0, 0), Point(0, 5));
    Segment sdown(Point(0, 0), Point(0, -5));

    SegmentSlope_<Segment> slope1(s1);
    SegmentSlope_<Segment> slope2(s2);
    SegmentSlope_<Segment> slopeup(sup);
    SegmentSlope_<Segment> slopedown(sdown);

    std::cout << "slope 1    = " << slope1 << std::endl;
    std::cout << "slope 2    = " << slope2 << std::endl;
    std::cout << "slope up   = " << slopeup << std::endl;
    std::cout << "slope down = " << slopedown << std::endl;

    EXPECT_EQ(slope1.value , 0);
    EXPECT_EQ(slope2.value , 1);

    EXPECT_TRUE(slope1 < slopeup);
    EXPECT_TRUE(slope2 < slopeup);
    EXPECT_TRUE(slopedown < slopeup);
    EXPECT_TRUE(slopedown < slope1);
    EXPECT_TRUE(slopedown < slope2);
    EXPECT_TRUE(slopedown < slopeup);
}

TEST(ben_ott, is_less){
    typedef typename Point::coord_value_type Cvt;
    std::cout<< "epsilon = " << std::numeric_limits<Cvt>::epsilon() << std::endl;
    Point p1(0, 0);
    Point p2(5, 0);
    EXPECT_TRUE(IsLess(p1, p2)); //p1 < p2
    Point p3(0, 0.0);
    Point p4(0, -1.0);
    EXPECT_TRUE(IsLess(p4, p3)); //p3 < p4
    p3 = p4;
    EXPECT_FALSE(IsLess(p4, p3)); //p3 = p4
    p3.x(p3.x() + 1e-16);
    p3.x(p3.x() + 1e-16);
    p3.x(p3.x() + 1e-16);
    std::cout << "p3 = " << p3 << std::endl;
    std::cout << "p4 = " << p4 << std::endl;
    EXPECT_TRUE(IsLess(p4, p3)); //p4 < p3
}

bool compare_seg(const std::string& name, const Segment& s1, const Segment& s2){
    typedef SegWithSlope_<Segment> SegWithSlope;
    typedef CompareSeg_<SegWithSlope> CompSeg;
    SegWithSlope ss1(s1);
    SegWithSlope ss2(s2);
    Point p(0.6, 0.5);
    CompSeg comp(&p, nullptr, name, true);
    return comp(&ss1, &ss2);
}


TEST(ben_ott, comp_seg){
    Segment s1(Point(0.0, 0.0), Point(1.0, 1.0));
    Segment s2(Point(0.2, 0.0), Point(0.8, 1.0));
    std::cout << "s1 = " << s1 << std::endl;       
    std::cout << "s2 = " << s2 << std::endl;       
    auto res = compare_seg("normal", s1, s2);
    std::cout << "res = " << ToString(res) << std::endl;
    EXPECT_TRUE(res);

    s1.ps(Point(0.0, 0.0));
    s1.pe(Point(1.0, 1.0));
    s2.ps(Point(0.2, 0.2));
    s2.pe(Point(0.8, 0.8));
    std::cout << "s1 = " << s1 << std::endl;       
    std::cout << "s2 = " << s2 << std::endl;       
    res = compare_seg("overlap", s1, s2);
    std::cout << "res = " << ToString(res) << std::endl;
    EXPECT_FALSE(res);

    s1.ps(Point(0.6, 0.0));
    s1.pe(Point(0.6, 1.0));
    s2.ps(Point(0.2, 0.2));
    s2.pe(Point(0.8, 0.8));
    std::cout << "s1 = " << s1 << std::endl;       
    std::cout << "s2 = " << s2 << std::endl;       
    res = compare_seg("vertial", s1, s2);
    std::cout << "res = " << ToString(res) << std::endl;
    EXPECT_TRUE(res);
}


template<class SEG_TYPE>
auto GenerateSegmentsCase1(){ // mulit right
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(0, 10), Point(15, 26)));
    lseg.push_back(Seg(Point(15, 26), Point(10, 40)));
    lseg.push_back(Seg(Point(15, 26), Point(2,  20))); //
    lseg.push_back(Seg(Point(15, 26), Point(10,  30)));
    lseg.push_back(Seg(Point(6, 18),  Point(20,  35))); //
    
    return lseg;
}

TEST(ben_ott, case1){
    auto sl = GenerateSegmentsCase1<Segment>();
    std::cout << "Input segmnets = " << sl.size() << std::endl;
    Inter inter(sl);
    auto lres = inter.execute();
    std::cout << "Find  inter    = " << lres.size() << std::endl;

    // PlotIntersection("case1", sl, lres);
    // auto lresn2 = Intersect(sl, "n2");
    // std::cout << "Find  inter n2 = " << lresn2.size() << std::endl;
    
    // EXPECT_EQ(lres.size(),   4);
    // EXPECT_EQ(lresn2.size(), 9);
}