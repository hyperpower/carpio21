#include "geometry/geometry.hpp"
#include "geometry/boolean/boolean.hpp"
#include "geometry/boolean/line_box.hpp"
#include "gtest/gtest.h"

#include <cmath>
#include <filesystem>

using namespace carpio;

typedef Point_<double, 2> Point2;
typedef Point_<double, 3> Point3;
typedef Box_<double, 2>   Box2;
typedef Circle_<double>   Circle;
typedef Line_<double>     Line;

namespace {

double Pi() {
    return std::acos(-1.0);
}

double SectorArea(
        const double ax, const double ay,
        const double bx, const double by,
        const double r) {
    return SignedSectorAreaOrigin(ax, ay, bx, by, r);
}

} // namespace

TEST(circle_box, intersection_points) {
    {
        Circle circle(0.0, 0.0, 0.5);
        Box2 box(Point2(-1.0, -1.0), Point2(1.0, 1.0));
        auto points = IntersectCircleBox(circle, box);
        ASSERT_EQ(points.size(), 0);
    }
    {
        Circle circle(0.0, 0.0, 2.0);
        Box2 box(Point2(-0.5, -0.5), Point2(0.5, 0.5));
        auto points = IntersectCircleBox(circle, box);
        ASSERT_EQ(points.size(), 0);
    }
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(-2.0, -0.5), Point2(2.0, 0.5));
        auto points = IntersectCircleBox(circle, box);
        ASSERT_EQ(points.size(), 4);
        auto iter = points.begin();
        ASSERT_NEAR(iter->x(), -std::sqrt(0.75), 1e-12);
        ASSERT_NEAR(iter->y(), -0.5, 1e-12);
        ++iter;
        ASSERT_NEAR(iter->x(), std::sqrt(0.75), 1e-12);
        ASSERT_NEAR(iter->y(), -0.5, 1e-12);
        ++iter;
        ASSERT_NEAR(iter->x(), std::sqrt(0.75), 1e-12);
        ASSERT_NEAR(iter->y(), 0.5, 1e-12);
        ++iter;
        ASSERT_NEAR(iter->x(), -std::sqrt(0.75), 1e-12);
        ASSERT_NEAR(iter->y(), 0.5, 1e-12);
    }
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(1.0, -0.5), Point2(2.0, 0.5));
        auto points = IntersectCircleBox(circle, box);
        ASSERT_EQ(points.size(), 1);
        ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
        ASSERT_NEAR(points.front().y(), 0.0, 1e-12);
    }
    {
        Circle circle(0.0, 0.0, std::sqrt(2.0));
        Box2 box(Point2(1.0, 1.0), Point2(2.0, 2.0));
        auto points = IntersectCircleBox(circle, box);
        ASSERT_EQ(points.size(), 1);
        ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
        ASSERT_NEAR(points.front().y(), 1.0, 1e-12);
    }
}

TEST(circle_box, circle_cal_x_y) {
    Circle circle(0.0, 0.0, 1.0);

    auto ys = circle.cal_y(0.0);
    ASSERT_NEAR(ys[0], -1.0, 1e-12);
    ASSERT_NEAR(ys[1],  1.0, 1e-12);

    auto xs = circle.cal_x(0.0);
    ASSERT_NEAR(xs[0], -1.0, 1e-12);
    ASSERT_NEAR(xs[1],  1.0, 1e-12);

    auto xs_in_range = CalculateCircleXInRange(0.0, 0.0, 1.0, 0.0, 0.0, 2.0);
    ASSERT_TRUE(std::isnan(xs_in_range[0]));
    ASSERT_NEAR(xs_in_range[1], 1.0, 1e-12);

    auto ys_in_range = CalculateCircleYInRange(0.0, 0.0, 1.0, 0.0, -2.0, 0.0);
    ASSERT_NEAR(ys_in_range[0], -1.0, 1e-12);
    ASSERT_TRUE(std::isnan(ys_in_range[1]));
}

TEST(circle_box, intersection_area) {
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(2.0, 2.0), Point2(3.0, 3.0));
        ASSERT_NEAR(IntersectAreaCircleBox(circle, box), 0.0, 1e-12);
    }
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(-2.0, -2.0), Point2(2.0, 2.0));
        ASSERT_NEAR(IntersectAreaCircleBox(circle, box), Pi(), 1e-12);
    }
    {
        Circle circle(0.0, 0.0, 2.0);
        Box2 box(Point2(-0.5, -0.5), Point2(0.5, 0.5));
        ASSERT_NEAR(IntersectAreaCircleBox(circle, box), 1.0, 1e-12);
    }
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(0.0, 0.0), Point2(1.0, 1.0));
        ASSERT_NEAR(IntersectAreaCircleBox(circle, box), Pi() * 0.25, 1e-12);
        ASSERT_NEAR(IntersectArea(circle, box), Pi() * 0.25, 1e-12);
        ASSERT_NEAR(IntersectArea(box, circle), Pi() * 0.25, 1e-12);
    }
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(-1.0, 0.0), Point2(1.0, 1.0));
        ASSERT_NEAR(IntersectAreaCircleBox(circle, box), Pi() * 0.5, 1e-12);
    }
}

TEST(circle_tri, signed_area_origin_circle) {
    {
        const double area = IntersectSignedAreaCircleOriginTri(
                1.0, 0.0, 0.0, 1.0, 2.0);
        ASSERT_NEAR(area, 0.5, 1e-12);
    }
    {
        const double area = IntersectSignedAreaCircleOriginTri(
                0.0, 1.0, 1.0, 0.0, 2.0);
        ASSERT_NEAR(area, -0.5, 1e-12);
    }
    {
        const double area = IntersectSignedAreaCircleOriginTri(
                2.0, 0.0, 0.0, 2.0, 1.0);
        ASSERT_NEAR(area, Pi() * 0.25, 1e-12);
    }
    {
        const double h = std::sqrt(0.75);
        const double expected =
                SectorArea(-2.0, 0.5, -h, 0.5, 1.0)
              + SignedAreaOriginTri2d(-h, 0.5, h, 0.5)
              + SectorArea(h, 0.5, 2.0, 0.5, 1.0);
        const double area = IntersectSignedAreaCircleOriginTri(
                -2.0, 0.5, 2.0, 0.5, 1.0);
        ASSERT_NEAR(area, expected, 1e-12);
    }
    {
        const double h = std::sqrt(0.75);
        const double expected =
                SignedAreaOriginTri2d(0.0, 0.5, h, 0.5)
              + SectorArea(h, 0.5, 2.0, 0.5, 1.0);
        const double area = IntersectSignedAreaCircleOriginTri(
                0.0, 0.5, 2.0, 0.5, 1.0);
        ASSERT_NEAR(area, expected, 1e-12);
    }
    {
        const double area = IntersectSignedAreaCircleOriginTri(
                1.0, 0.0, 0.0, 1.0, 1.0);
        ASSERT_NEAR(area, 0.5, 1e-12);
    }
    {
        ASSERT_NEAR(IntersectSignedAreaCircleOriginTri(
                1.0, 0.0, 1.0, 0.0, 1.0), 0.0, 1e-12);
        ASSERT_NEAR(IntersectSignedAreaCircleOriginTri(
                1.0, 0.0, 0.0, 1.0, 0.0), 0.0, 1e-12);
    }
}

TEST(box_box, intersection_points) {
    {
        Box2 box1(Point2(0.0, 0.0), Point2(2.0, 2.0));
        Box2 box2(Point2(1.0, 1.0), Point2(3.0, 3.0));
        auto points = IntersectBoxBox(box1, box2);
        ASSERT_EQ(points.size(), 4);
        auto iter = points.begin();
        ASSERT_NEAR(iter->x(), 1.0, 1e-12);
        ASSERT_NEAR(iter->y(), 1.0, 1e-12);
        ++iter;
        ASSERT_NEAR(iter->x(), 2.0, 1e-12);
        ASSERT_NEAR(iter->y(), 1.0, 1e-12);
        ++iter;
        ASSERT_NEAR(iter->x(), 2.0, 1e-12);
        ASSERT_NEAR(iter->y(), 2.0, 1e-12);
        ++iter;
        ASSERT_NEAR(iter->x(), 1.0, 1e-12);
        ASSERT_NEAR(iter->y(), 2.0, 1e-12);
    }
    {
        Box2 box1(Point2(0.0, 0.0), Point2(1.0, 1.0));
        Box2 box2(Point2(1.0, 0.25), Point2(2.0, 0.75));
        auto points = IntersectBoxBox(box1, box2);
        ASSERT_EQ(points.size(), 2);
        ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
        ASSERT_NEAR(points.front().y(), 0.25, 1e-12);
        ASSERT_NEAR(points.back().x(), 1.0, 1e-12);
        ASSERT_NEAR(points.back().y(), 0.75, 1e-12);
    }
    {
        Box2 box1(Point2(0.0, 0.0), Point2(1.0, 1.0));
        Box2 box2(Point2(1.0, 1.0), Point2(2.0, 2.0));
        auto points = Intersect(box1, box2);
        ASSERT_EQ(points.size(), 1);
        ASSERT_NEAR(points.front().x(), 1.0, 1e-12);
        ASSERT_NEAR(points.front().y(), 1.0, 1e-12);
    }
}

TEST(box_box, intersection_area) {
    {
        Box2 box1(Point2(0.0, 0.0), Point2(2.0, 2.0));
        Box2 box2(Point2(1.0, 1.0), Point2(3.0, 3.0));
        ASSERT_NEAR(IntersectAreaBoxBox(box1, box2), 1.0, 1e-12);
        ASSERT_NEAR(IntersectArea(box1, box2), 1.0, 1e-12);
    }
    {
        Box2 box1(Point2(0.0, 0.0), Point2(1.0, 1.0));
        Box2 box2(Point2(1.0, 0.25), Point2(2.0, 0.75));
        ASSERT_NEAR(IntersectAreaBoxBox(box1, box2), 0.0, 1e-12);
    }
}

TEST(line_box, intersection_area_unit_box_outside_line) {
    Point2 min(0.0, 0.0);
    Point2 max(1.0, 1.0);
    Box2 box(min, max);

    {
        Line line(1.0, 1.0, 2.0);
        auto negative = NegativeLineBox(min, max, line.a(), line.b(), line.alpha());
        ASSERT_EQ(negative.size(), 4);
        ASSERT_NEAR(IntersectionAreaNegative(line, box), 1.0, 1e-12);
        ASSERT_NEAR(IntersectionAreaPositive(line, box), 0.0, 1e-12);
    }
    {
        Line line(1.0, 1.0, -1.0);
        auto positive = PositiveLineBox(min, max, line.a(), line.b(), line.alpha());
        ASSERT_EQ(positive.size(), 4);
        ASSERT_NEAR(IntersectionAreaNegative(line, box), 0.0, 1e-12);
        ASSERT_NEAR(IntersectionAreaPositive(line, box), 1.0, 1e-12);
    }
    {
        Line line(1.0, 1.0, 1.0);
        ASSERT_NEAR(IntersectionAreaNegative(line, box), 0.5, 1e-12);
        ASSERT_NEAR(IntersectionAreaPositive(line, box), 0.5, 1e-12);
    }
    {
        Line line(0.0, 1.0, 0.0);
        ASSERT_NEAR(IntersectionAreaNegative(line, box), 0.0, 1e-12);
        ASSERT_NEAR(IntersectionAreaPositive(line, box), 1.0, 1e-12);
    }
    {
        Line line(0.0, 1.0, 1.0);
        ASSERT_NEAR(IntersectionAreaNegative(line, box), 1.0, 1e-12);
        ASSERT_NEAR(IntersectionAreaPositive(line, box), 0.0, 1e-12);
    }
}

TEST(plane_box, intersection_points_scaled_box) {
    Point3 min(0.0, 0.0, 0.0);
    Point3 max(2.0, 4.0, 8.0);

    auto points = IntersectPlaneBox(min, max, 0.0, 0.0, 1.0, 4.0);
    ASSERT_EQ(points.size(), 4);
    for (const auto& point : points) {
        ASSERT_NEAR(point.z(), 4.0, 1e-12);
    }
}

TEST(circle_box, gnuplot_circle) {
    Circle circle(-0.1, -0.2, 1.0);
    Box2 box(Point2(-0.3, -0.4), Point2(0.8, 0.7));

    auto circle_actor = ToGnuplotActor(circle);
    auto box_actor = ToGnuplotActor(box);
   

    auto points = IntersectCircleBox(circle, box);
    // ASSERT_FALSE(points.empty());

    auto point_actor = ToGnuplotActor(points);
    // ASSERT_FALSE(point_actor.empty());

    auto area = IntersectAreaCircleBox(circle, box);
    std::cout << "Circle: " << circle.area() << std::endl;\
    std::cout << "Box: " << box.area() << std::endl;
    std::cout << "Intersection area: " << area << std::endl;

    std::filesystem::create_directories("./fig");

    Gnuplot gnu;
    gnu.set_xrange(-1.2, 1.2);
    gnu.set_yrange(-1.2, 1.2);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/circle_box");

    circle_actor.line_color_blue().line_width(2);
    box_actor.line_color_red().line_width(2);
    point_actor.line_color_black().point_type(7).point_size(2);
    gnu.add(circle_actor);
    gnu.add(box_actor);
    gnu.add(point_actor);
    gnu.plot();
}
