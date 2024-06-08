#include <functional>

#define _DEBUG_MODE_

#include "utility/random.hpp"
#include "utility/profile.hpp"
#include "gtest/gtest.h"

#include "io/io_define.hpp"
#include "geometry/ben-ott/segment_slope.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"




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

    SegmentSlope_<Segment> slope1(s1);
    SegmentSlope_<Segment> slope2(s2);

    std::cout << "slope 1 = " << slope1.value << std::endl;
    std::cout << "slope 2 = " << slope2.value << std::endl;

    EXPECT_EQ(slope1.value , 0);
    EXPECT_EQ(slope2.value , 1);

    EXPECT_TRUE(slope1 < slope2);


}