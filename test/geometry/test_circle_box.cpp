#include "geometry/geometry.hpp"
#include "geometry/boolean/boolean.hpp"
#include "gtest/gtest.h"

#include <cmath>
#include <filesystem>

using namespace carpio;

typedef Point_<double, 2> Point2;
typedef Box_<double, 2>   Box2;
typedef Circle_<double>   Circle;

namespace {

double Pi() {
    return std::acos(-1.0);
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
    }
    {
        Circle circle(0.0, 0.0, 1.0);
        Box2 box(Point2(-1.0, 0.0), Point2(1.0, 1.0));
        ASSERT_NEAR(IntersectAreaCircleBox(circle, box), Pi() * 0.5, 1e-12);
    }
}

TEST(circle_box, gnuplot_circle) {
    Circle circle(0.0, 0.0, 1.0);
    Box2 box(Point2(-0.3, -0.4), Point2(0.8, 0.7));

    auto circle_actor = ToGnuplotActor(circle);
    ASSERT_FALSE(circle_actor.empty());
    ASSERT_GT(circle_actor.data().size(), 10);

    auto box_actor = ToGnuplotActor(box);
    ASSERT_FALSE(box_actor.empty());
    ASSERT_GT(box_actor.data().size(), 4);

    auto points = IntersectCircleBox(circle, box);
    ASSERT_FALSE(points.empty());

    auto point_actor = ToGnuplotActor(points);
    ASSERT_FALSE(point_actor.empty());

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
