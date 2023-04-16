/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#include <type_traits>
#include "geometry/geometry.hpp"
#include "algebra/number/rational.hpp"
#include "gtest/gtest.h"

using namespace carpio;

const St dim = 3;

typedef Rational_<int> Rational;
typedef Rational NumT;
// typedef double NumT;

typedef Point_<NumT, dim> Point;
typedef Point_<NumT, 3> Point3;
typedef Point_<NumT, 2> Point2;
typedef GGnuplotActor_<NumT, 2> GA;

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
    std::list<int> arr{1 , 1};
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
    Point2 p(0.4, 0.2);
    auto dis = Distance(p, l);
    std::cout<< "Line  : " << l   << std::endl;
    std::cout<< "Point : " << p   << std::endl;
    std::cout<< "Dis   : " << dis << std::endl;

    Line ln = NormalLinePassPoint(l, p);
    std::cout<< "normal Line  : " << ln   << std::endl;
    
    

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/distance_to_line");
    auto ap = ToGnuplotActor(p);
    ap.style("with points pointtype 7 pointsize 3");
    gnu.add(ap);
    //intersect
    auto inp = Line::Intersect(l, ln);
    auto ainp = ToGnuplotActor(*inp);
    ainp.style("with points pointtype 7 pointsize 3");
    gnu.add(ainp);
    // auto al = GA::Lines(l);
    auto al = ToGnuplotActor(l, std::vector<double>{-0.0, 1.0});
    gnu.add(al);
    auto a2 = ToGnuplotActor(ln, std::vector<double>{-0.0, 1.0});
    gnu.add(a2);
    gnu.set_equal_aspect_ratio();
    gnu.plot();
}
