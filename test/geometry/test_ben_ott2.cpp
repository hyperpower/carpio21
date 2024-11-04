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

    std::cout << "slope 1 = " << slope1 << std::endl;
    std::cout << "slope 2 = " << slope2 << std::endl;
    std::cout << "slope up = " << slopeup << std::endl;
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
    Point p1(0, 0);
    Point p2(5, 0);
    EXPECT_TRUE(IsLess(p1, p2)); //p1 < p2
    Point p3(0, 0.0);
    Point p4(0, -1.0);
    EXPECT_TRUE(IsLess(p4, p3)); //p3 < p4
}