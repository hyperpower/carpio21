/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#include <type_traits>
#include "geometry/geometry.hpp"
#include "utility/rational.hpp"
#include "gtest/gtest.h"

using namespace carpio;

const St dim = 3;

typedef Rational_<int> Rational;
typedef double NumT;

typedef Point_<NumT, dim> Point;
typedef Point_<NumT, 3> Point3;
typedef Point_<NumT, 2> Point2;
typedef GGnuplotActor_<NumT, 2> GA;

TEST(numtype, int_relation){
    using value_type = decltype(int() + long());
    std::cout << std::numeric_limits<int>::max() << std::endl;
    std::cout << std::numeric_limits<long>::max() << std::endl;
    std::cout << std::numeric_limits<long long>::max() << std::endl;
    std::cout << std::numeric_limits<value_type>::max() << std::endl;
}


TEST(point, point_initial){
    Point3 x(1, 0, 0);
    Point3 y(0, 1, 0);
    Point3 z(0, 0, 1);
    std::cout << "[==========] Initial three points and output" << std::endl;
    std::cout << "Point x = " << x << std::endl;
    std::cout << "Point y = " << y << std::endl;
    std::cout << "Point z = " << z << std::endl;
    ASSERT_EQ(x(0), 1);
    ASSERT_EQ(y(1), 1);

    Translate(x, y);
    ASSERT_EQ(x[1], 1);

    Vt v = 1;
    x = {3, 0, 0};
    Translate(x, v);
    ASSERT_EQ(x[0], 4);

    x = {3, 0, 0};
    std::list<Vt> arr{1 , 1};
    Translate(x, arr);
    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[1], 1);
    ASSERT_EQ(x[2], 0);

}

TEST(point, distance){
    Point2 x(0, 0);
    Point2 y(1, 1);
    Point2 z(0, 1);
    std::cout << "Point x = " << x << std::endl;
    std::cout << "Point y = " << y << std::endl;
    std::cout << "Square Distance from x to y = " << Distance2(x, y) << std::endl;
    std::cout << "       Distance from x to y = " << Distance(x, y) << std::endl;
}

TEST(point, max_and_min){
    Point3 x(1, 0, 0);
    Point3 y(0, 1, 0);
    Point3 z(0, 0, 1);
    std::cout << "Initial three points and output/n";
    std::cout << "Point x = " << x << std::endl;
    std::cout << "Point y = " << y << std::endl;
    std::cout << "Point z = " << z << std::endl;

    std::cout << "Max of x and y " << Max(x, y) << std::endl;
    std::cout << "Min of x and z " << Min(x, z) << std::endl;

    ASSERT_EQ(Min(x,z), Point3(0,0,0));
    ASSERT_EQ(Max(x,y), Point3(1,1,0));

}


TEST(point, distance_to_line){
    typedef Line_<Vt> Line;
    Line l(1.0, 1.0, 1.0);
    Point2 p(0.5, 0.3);
    auto dis = Distance(p, l);
    std::cout<< "Line  : " << l   << std::endl;
    std::cout<< "Point : " << p   << std::endl;
    std::cout<< "Dis   : " << dis << std::endl;


    Gnuplot gnu;
    auto ap = GA::Points(p, 0);
    gnu.set_terminal_png("./fig/distance_to_line");
    ap->style() = "with points pointtype 7 pointsize 3 lc variable";
    gnu.add(ap);
    auto al = GA::Lines(l);
    gnu.add(al);
    gnu.plot();
}
