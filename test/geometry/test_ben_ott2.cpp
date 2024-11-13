#include <functional>

#define _DEBUG_MODE_

#include "utility/random.hpp"
#include "utility/profile.hpp"
#include "gtest/gtest.h"

#include "io/io_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"

#include "geometry/ben-ott/segment_slope.hpp"
#include "geometry/ben-ott/segment_with_slope.hpp"
#include "geometry/ben-ott/sweep_event.hpp"
#include "geometry/ben-ott/sweep_event_queue.hpp"
#include "geometry/ben-ott/comp_seg.hpp"

using namespace carpio;

typedef double NumType;
// typedef Rational_<long> NumType;
typedef Point_<NumType, 2> Point;
typedef Segment_<NumType, 2> Segment;

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

int compare_seg(const Segment& s1, const Segment& s2){
    typedef CompareSeg_<Segment> CompSeg;
    Point p(0.5, 0.5);
    CompSeg compare(&p, nullptr, "compare_seg_1");
    return compare(&s1, &s2);
}


TEST(ben_ott, comp_seg){
    Segment s1(Point(0.0, 0.0), Point(1.0, 1.0));
    Segment s2(Point(0.2, 0.0), Point(0.8, 1.0));
    std::cout << "s1 = " << s1 << std::endl;       
    std::cout << "s2 = " << s2 << std::endl;       
    auto res = compare_seg(s1, s2);
    std::cout << "res= " << res << std::endl;
    
}