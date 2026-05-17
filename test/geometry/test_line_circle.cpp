#include "geometry/geometry.hpp"
#include "geometry/boolean/boolean.hpp"
#include "geometry/boolean/line_circle.hpp"
#include "gtest/gtest.h"

#include <array>
#include <cmath>
#include <list>

using namespace carpio;

namespace {

typedef Point_<double, 2> Point2;
typedef Circle_<double>   Circle;
typedef Line_<double>     Line;

void AssertOnLineAndCircle(
        const std::list<Point2>& points,
        const Line& line,
        const Circle& circle,
        const double tol = 1e-12) {
    for (const auto& p : points) {
        ASSERT_NEAR(line.a() * p.x() + line.b() * p.y(),
                    line.alpha(), tol);
        ASSERT_NEAR((p.x() - circle.xc()) * (p.x() - circle.xc())
                  + (p.y() - circle.yc()) * (p.y() - circle.yc()),
                    circle.r() * circle.r(), tol);
    }
}

void AssertOrderedAlongLineDirection(
        const std::list<Point2>& points,
        const Line& line) {
    ASSERT_EQ(points.size(), 2);
    auto first = points.begin();
    auto second = std::next(first);
    const double dx = second->x() - first->x();
    const double dy = second->y() - first->y();
    ASSERT_GT(Dot(dx, dy, -line.b(), line.a()), 0.0);
}

void AssertOnSegmentAndCircle(
        const std::list<Point2>& points,
        const Point2& start,
        const Point2& end,
        const Circle& circle,
        const double tol = 1e-12) {
    const double dx = end.x() - start.x();
    const double dy = end.y() - start.y();
    const double dd = Dot(dx, dy, dx, dy);
    for (const auto& p : points) {
        ASSERT_NEAR((p.x() - circle.xc()) * (p.x() - circle.xc())
                  + (p.y() - circle.yc()) * (p.y() - circle.yc()),
                    circle.r() * circle.r(), tol);
        ASSERT_NEAR(Cross2(p.x() - start.x(), p.y() - start.y(), dx, dy),
                    0.0, tol);
        ASSERT_GE(Dot(p.x() - start.x(), p.y() - start.y(), dx, dy),
                  -tol);
        ASSERT_LE(Dot(p.x() - start.x(), p.y() - start.y(), dx, dy),
                  dd + tol);
    }
}

} // namespace

TEST(line_circle, horizontal_secant) {
    const Circle circle(0.0, 0.0, 1.0);
    const Line line(0.0, 1.0, 0.0);

    auto points = IntersectLineCircle(line, circle);

    ASSERT_EQ(points.size(), 2);
    AssertOnLineAndCircle(points, line, circle);
    AssertOrderedAlongLineDirection(points, line);

    auto iter = points.begin();
    ASSERT_NEAR(iter->x(), 1.0, 1e-12);
    ASSERT_NEAR(iter->y(), 0.0, 1e-12);
    ++iter;
    ASSERT_NEAR(iter->x(), -1.0, 1e-12);
    ASSERT_NEAR(iter->y(), 0.0, 1e-12);
}

TEST(line_circle, vertical_secant) {
    const Circle circle(0.0, 0.0, 1.0);
    const Line line(1.0, 0.0, 0.0);

    auto points = IntersectLineCircle(line, circle);

    ASSERT_EQ(points.size(), 2);
    AssertOnLineAndCircle(points, line, circle);
    AssertOrderedAlongLineDirection(points, line);

    auto iter = points.begin();
    ASSERT_NEAR(iter->x(), 0.0, 1e-12);
    ASSERT_NEAR(iter->y(), -1.0, 1e-12);
    ++iter;
    ASSERT_NEAR(iter->x(), 0.0, 1e-12);
    ASSERT_NEAR(iter->y(), 1.0, 1e-12);
}

TEST(line_circle, tangent) {
    const Circle circle(0.0, 0.0, 1.0);
    const Line line(1.0, 0.0, 1.0);

    auto points = IntersectLineCircle(line, circle);

    ASSERT_EQ(points.size(), 1);
    ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
    ASSERT_NEAR(points.front().y(), 0.0, 1e-12);
    AssertOnLineAndCircle(points, line, circle);

    MakeDir("./fig/");

    Gnuplot gnu;
    gnu.set_xrange(-1.5, 1.5);
    gnu.set_yrange(-1.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/line_circle_tangent");

    auto acircle = ToGnuplotActor(circle);
    acircle.line_color_blue().line_width(2);
    gnu.add(acircle);

    std::array<double, 2> line_range = {1.5, -1.5};
    auto aline = ToGnuplotActor(line, line_range);
    aline.style() = "with lines lw 2 lc 2";
    gnu.add(aline);

    auto apoints = ToGnuplotActor(points);
    apoints.line_color_black().point_type(7).point_size(2);
    gnu.add(apoints);

    gnu.plot();
}

TEST(line_circle, no_intersection) {
    const Circle circle(0.0, 0.0, 1.0);
    const Line line(1.0, 0.0, 2.0);

    auto points = IntersectLineCircle(line, circle);

    ASSERT_EQ(points.size(), 0);
}

TEST(line_circle, shifted_circle_sloped_line) {
    const Circle circle(1.0, -0.5, 2.0);
    const Line line(-1.0, 1.0, 0.5);

    auto points = IntersectLineCircle(line, circle);

    std::cout << "Intersection points: \n";
    for (const auto& p : points) {
        std::cout << p << std::endl;    
    }

    ASSERT_EQ(points.size(), 2);
    AssertOnLineAndCircle(points, line, circle);
    AssertOrderedAlongLineDirection(points, line);

    MakeDir("./fig/");

    Gnuplot gnu;
    gnu.set_xrange(-2.0, 4.0);
    gnu.set_yrange(-3.0, 2.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/line_circle_sloped_secant");

    auto acircle = ToGnuplotActor(circle);
    acircle.line_color_blue().line_width(2);
    gnu.add(acircle);

    std::array<double, 2> line_range = {3.0, -3.0};
    auto aline = ToGnuplotActor(line, line_range);
    aline.style() = "with lines lw 2 lc 2";
    gnu.add(aline);

    auto apoints = ToGnuplotActor(points);
    apoints.line_color_black().point_type(7).point_size(2);
    gnu.add(apoints);

    gnu.plot();
}

TEST(line_circle, generic_intersect_dispatch) {
    const Circle circle(0.0, 0.0, 1.0);
    const Line line(0.0, 1.0, 0.0);

    auto points_lc = Intersect(line, circle);
    auto points_cl = Intersect(circle, line);

    ASSERT_EQ(points_lc.size(), 2);
    ASSERT_EQ(points_cl.size(), 2);
    AssertOnLineAndCircle(points_lc, line, circle);
    AssertOnLineAndCircle(points_cl, line, circle);
    AssertOrderedAlongLineDirection(points_lc, line);
    AssertOrderedAlongLineDirection(points_cl, line);
}

TEST(line_circle, degenerate_inputs) {
    const Circle circle(0.0, 0.0, 1.0);
    const Line degenerate(0.0, 0.0, 1.0);
    const Circle empty_circle(0.0, 0.0, 0.0);
    const Line line(0.0, 1.0, 0.0);

    ASSERT_EQ(IntersectLineCircle(degenerate, circle).size(), 0);
    ASSERT_EQ(IntersectLineCircle(line, empty_circle).size(), 0);
}

TEST(segment_circle, horizontal_secant) {
    const Circle circle(0.0, 0.0, 1.0);
    const Point2 start(-2.0, 0.0);
    const Point2 end(2.0, 0.0);

    auto points = IntersectSegmentCircle(start, end, circle);

    ASSERT_EQ(points.size(), 2);
    AssertOnSegmentAndCircle(points, start, end, circle);
    auto iter = points.begin();
    ASSERT_NEAR(iter->x(), -1.0, 1e-12);
    ASSERT_NEAR(iter->y(), 0.0, 1e-12);
    ++iter;
    ASSERT_NEAR(iter->x(), 1.0, 1e-12);
    ASSERT_NEAR(iter->y(), 0.0, 1e-12);
}

TEST(segment_circle, tangent) {
    const Circle circle(0.0, 0.0, 1.0);
    const Point2 start(1.0, -2.0);
    const Point2 end(1.0, 2.0);

    auto points = IntersectSegmentCircle(start, end, circle);

    ASSERT_EQ(points.size(), 1);
    ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
    ASSERT_NEAR(points.front().y(), 0.0, 1e-12);
    AssertOnSegmentAndCircle(points, start, end, circle);
}

TEST(segment_circle, intersections_outside_segment) {
    const Circle circle(0.0, 0.0, 1.0);
    const Point2 start(2.0, 0.0);
    const Point2 end(3.0, 0.0);

    auto points = IntersectSegmentCircle(start, end, circle);

    ASSERT_EQ(points.size(), 0);
}

TEST(segment_circle, one_endpoint_on_circle) {
    const Circle circle(0.0, 0.0, 1.0);
    const Point2 start(1.0, 0.0);
    const Point2 end(2.0, 0.0);

    auto points = IntersectSegmentCircle(start, end, circle);

    ASSERT_EQ(points.size(), 1);
    ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
    ASSERT_NEAR(points.front().y(), 0.0, 1e-12);
    AssertOnSegmentAndCircle(points, start, end, circle);
}

TEST(segment_circle, degenerate_segment) {
    const Circle circle(0.0, 0.0, 1.0);

    ASSERT_EQ(IntersectSegmentCircle(Point2(1.0, 0.0),
                                     Point2(1.0, 0.0),
                                     circle).size(), 1);
    ASSERT_EQ(IntersectSegmentCircle(Point2(0.0, 0.0),
                                     Point2(0.0, 0.0),
                                     circle).size(), 0);
}

TEST(circle_tri, shifted_circle_triangle_area) {
    const Circle circle(2.0, -1.0, 3.0);
    const Point2 pa(2.0, -1.0);
    const Point2 pb(3.0, -1.0);
    const Point2 pc(2.0, 0.0);

    ASSERT_NEAR(IntersectSignedAreaCircleTri(pa, pb, pc, circle),
                0.5, 1e-12);
    ASSERT_NEAR(IntersectSignedAreaCircleTri(pa, pc, pb, circle),
                -0.5, 1e-12);
}

TEST(circle_point_chain, triangle_inside_circle) {
    const Circle circle(0.0, 0.0, 3.0);
    PointChain_<double, 2> pc;
    pc.push_back(Point2(0.0, 0.0));
    pc.push_back(Point2(1.0, 0.0));
    pc.push_back(Point2(0.0, 1.0));

    ASSERT_NEAR(IntersectSignedAreaCirclePointChain(circle, pc),
                0.5, 1e-12);
    ASSERT_NEAR(IntersectAreaCirclePointChain(circle, pc),
                0.5, 1e-12);
    ASSERT_NEAR(IntersectArea(circle, pc),
                0.5, 1e-12);
    ASSERT_NEAR(IntersectArea(pc, circle),
                0.5, 1e-12);
}

TEST(circle_point_chain, reversed_triangle_signed_area) {
    const Circle circle(0.0, 0.0, 3.0);
    PointChain_<double, 2> pc;
    pc.push_back(Point2(0.0, 0.0));
    pc.push_back(Point2(0.0, 1.0));
    pc.push_back(Point2(1.0, 0.0));

    ASSERT_NEAR(IntersectSignedAreaCirclePointChain(circle, pc),
                -0.5, 1e-12);
    ASSERT_NEAR(IntersectAreaCirclePointChain(circle, pc),
                0.5, 1e-12);
}

TEST(circle_point_chain, upper_half_circle_rectangle) {
    const Circle circle(0.0, 0.0, 1.0);
    PointChain_<double, 2> pc;
    pc.push_back(Point2(-2.0, 0.0));
    pc.push_back(Point2( 2.0, 0.0));
    pc.push_back(Point2( 2.0, 2.0));
    pc.push_back(Point2(-2.0, 2.0));

    ASSERT_NEAR(IntersectAreaCirclePointChain(circle, pc),
                std::acos(-1.0) * 0.5, 1e-12);
}
