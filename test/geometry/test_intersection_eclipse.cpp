#include "geometry/geometry.hpp"
#include "geometry/boolean/boolean.hpp"
#include "geometry/boolean/intersection_eclipse_tri.hpp"
#include "geometry/boolean/line_eclipse.hpp"
#include "gtest/gtest.h"

#include <array>
#include <cmath>
#include <filesystem>
#include <list>
#include <sstream>

using namespace carpio;

namespace {

typedef Point_<double, 2> Point2;
typedef Segment_<double, 2> Segment2;
typedef Line_<double> Line;
typedef Eclipse_<double> Eclipse;

double Pi() {
    return std::acos(-1.0);
}

void AssertOnLineAndEclipse(
        const std::list<Point2>& points,
        const Line& line,
        const Eclipse& eclipse,
        const double tol = 1e-12) {
    for (const auto& p : points) {
        ASSERT_NEAR(line.a() * p.x() + line.b() * p.y(),
                    line.alpha(), tol);

        const double dx = p.x() - eclipse.xc();
        const double dy = p.y() - eclipse.yc();
        ASSERT_NEAR(dx * dx / (eclipse.a() * eclipse.a())
                  + dy * dy / (eclipse.b() * eclipse.b()),
                    1.0, tol);
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

void AssertOnSegmentAndEclipse(
        const std::list<Point2>& points,
        const Point2& start,
        const Point2& end,
        const Eclipse& eclipse,
        const double tol = 1e-12) {
    const double dx = end.x() - start.x();
    const double dy = end.y() - start.y();
    const double dd = Dot(dx, dy, dx, dy);

    for (const auto& p : points) {
        const double ex = p.x() - eclipse.xc();
        const double ey = p.y() - eclipse.yc();
        ASSERT_NEAR(ex * ex / (eclipse.a() * eclipse.a())
                  + ey * ey / (eclipse.b() * eclipse.b()),
                    1.0, tol);
        ASSERT_NEAR(Cross2(p.x() - start.x(), p.y() - start.y(), dx, dy),
                    0.0, tol);
        ASSERT_GE(Dot(p.x() - start.x(), p.y() - start.y(), dx, dy),
                  -tol);
        ASSERT_LE(Dot(p.x() - start.x(), p.y() - start.y(), dx, dy),
                  dd + tol);
    }
}

GnuplotActor MakeTriangleActor(
        const Point2& pa,
        const Point2& pb,
        const Point2& pc) {
    GnuplotActor actor;
    actor.command() = "using 1:2 title \"\" ";
    actor.style() = "with lines";

    const std::array<Point2, 4> points = {pa, pb, pc, pa};
    for (const auto& p : points) {
        std::ostringstream sst;
        sst << p.x() << " " << p.y();
        actor.data().push_back(sst.str());
    }
    actor.data().push_back("");
    return actor;
}

} // namespace

TEST(line_eclipse, horizontal_secant) {
    const Eclipse eclipse(0.0, 0.0, 2.0, 1.0);
    const Line line(0.0, 1.0, 0.0);

    auto points = IntersectLineEclipse(line, eclipse);

    ASSERT_EQ(points.size(), 2);
    AssertOnLineAndEclipse(points, line, eclipse);
    AssertOrderedAlongLineDirection(points, line);

    auto iter = points.begin();
    // ASSERT_NEAR(iter->x(), 2.0, 1e-12);
    // ASSERT_NEAR(iter->y(), 0.0, 1e-12);
    ++iter;
    // ASSERT_NEAR(iter->x(), -2.0, 1e-12);
    // ASSERT_NEAR(iter->y(), 0.0, 1e-12);

    std::filesystem::create_directories("./fig");

    Gnuplot gnu;
    gnu.set_xrange(-3.0, 3.0);
    gnu.set_yrange(-1.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/line_eclipse_horizontal_secant");

    auto eclipse_actor = ToGnuplotActor(eclipse);
    eclipse_actor.line_color_blue().line_width(2);
    gnu.add(eclipse_actor);

    std::array<double, 2> line_range = {3.0, -3.0};
    auto line_actor = ToGnuplotActor(line, line_range);
    line_actor.line_color_red().line_width(2);
    gnu.add(line_actor);

    auto point_actor = ToGnuplotActor(points);
    point_actor.line_color_black().point_type(7).point_size(2);
    gnu.add(point_actor);

    gnu.plot();
}

TEST(line_eclipse, vertical_secant) {
    const Eclipse eclipse(0.0, 0.0, 2.0, 1.0);
    const Line line(1.0, 0.0, 0.0);

    auto points = IntersectLineEclipse(line, eclipse);

    ASSERT_EQ(points.size(), 2);
    AssertOnLineAndEclipse(points, line, eclipse);
    AssertOrderedAlongLineDirection(points, line);

    auto iter = points.begin();
    ASSERT_NEAR(iter->x(), 0.0, 1e-12);
    ASSERT_NEAR(iter->y(), -1.0, 1e-12);
    ++iter;
    ASSERT_NEAR(iter->x(), 0.0, 1e-12);
    ASSERT_NEAR(iter->y(), 1.0, 1e-12);
}

TEST(line_eclipse, tangent_and_no_intersection) {
    const Eclipse eclipse(0.0, 0.0, 2.0, 1.0);

    {
        const Line tangent(1.0, 0.0, 2.0);
        auto points = IntersectLineEclipse(tangent, eclipse);
        ASSERT_EQ(points.size(), 1);
        ASSERT_NEAR(points.front().x(), 2.0, 1e-12);
        ASSERT_NEAR(points.front().y(), 0.0, 1e-12);
        AssertOnLineAndEclipse(points, tangent, eclipse);
    }
    {
        const Line outside(1.0, 0.0, 3.0);
        ASSERT_EQ(IntersectLineEclipse(outside, eclipse).size(), 0);
    }
}

TEST(line_eclipse, shifted_sloped_line_and_dispatch) {
    const Eclipse eclipse(1.0, -0.5, 3.0, 1.5);
    const Line line(-1.0, 1.0, 0.5);

    auto direct = IntersectLineEclipse(line, eclipse);
    auto dispatched_le = Intersect(line, eclipse);
    auto dispatched_el = Intersect(eclipse, line);

    ASSERT_EQ(direct.size(), 2);
    ASSERT_EQ(dispatched_le.size(), 2);
    ASSERT_EQ(dispatched_el.size(), 2);
    AssertOnLineAndEclipse(direct, line, eclipse);
    AssertOnLineAndEclipse(dispatched_le, line, eclipse);
    AssertOnLineAndEclipse(dispatched_el, line, eclipse);
    AssertOrderedAlongLineDirection(direct, line);
}

TEST(segment_eclipse, horizontal_secant) {
    const Eclipse eclipse(0.0, 0.0, 2.0, 1.0);
    const Point2 start(-3.0, 0.0);
    const Point2 end(3.0, 0.0);

    auto points = IntersectSegmentEclipse(start, end, eclipse);

    ASSERT_EQ(points.size(), 2);
    AssertOnSegmentAndEclipse(points, start, end, eclipse);

    auto iter = points.begin();
    ASSERT_NEAR(iter->x(), -2.0, 1e-12);
    ASSERT_NEAR(iter->y(), 0.0, 1e-12);
    ++iter;
    ASSERT_NEAR(iter->x(), 2.0, 1e-12);
    ASSERT_NEAR(iter->y(), 0.0, 1e-12);
}

TEST(segment_eclipse, tangent_outside_degenerate_and_dispatch) {
    const Eclipse eclipse(0.0, 0.0, 2.0, 1.0);

    {
        const Point2 start(2.0, -2.0);
        const Point2 end(2.0, 2.0);
        auto points = IntersectSegmentEclipse(start, end, eclipse);
        ASSERT_EQ(points.size(), 1);
        ASSERT_NEAR(points.front().x(), 2.0, 1e-12);
        ASSERT_NEAR(points.front().y(), 0.0, 1e-12);
        AssertOnSegmentAndEclipse(points, start, end, eclipse);
    }
    {
        const Point2 start(3.0, 0.0);
        const Point2 end(4.0, 0.0);
        ASSERT_EQ(IntersectSegmentEclipse(start, end, eclipse).size(), 0);
    }
    {
        ASSERT_EQ(IntersectSegmentEclipse(
                Point2(2.0, 0.0), Point2(2.0, 0.0), eclipse).size(), 1);
        ASSERT_EQ(IntersectSegmentEclipse(
                Point2(0.0, 0.0), Point2(0.0, 0.0), eclipse).size(), 0);
    }
    {
        const Segment2 segment(Point2(-3.0, 0.0), Point2(3.0, 0.0));
        ASSERT_EQ(Intersect(segment, eclipse).size(), 2);
        ASSERT_EQ(Intersect(eclipse, segment).size(), 2);
    }
}

TEST(intersection_eclipse_tri, signed_area_origin_eclipse) {
    {
        const double area = IntersectSignedAreaEclipseOriginTri(
                Point2(0.0, 0.0), Point2(2.0, 0.0), Point2(0.0, 1.0),
                2.0, 1.0);
        ASSERT_NEAR(area, 1.0, 1e-12);
    }
    {
        const double area = IntersectSignedAreaEclipseOriginTri(
                Point2(0.0, 0.0), Point2(0.0, 1.0), Point2(2.0, 0.0),
                2.0, 1.0);
        ASSERT_NEAR(area, -1.0, 1e-12);
    }
    {
        const double area = IntersectSignedAreaEclipseOriginTri(
                Point2(-4.0, 0.0), Point2(4.0, 0.0), Point2(0.0, 4.0),
                2.0, 1.0);
        ASSERT_NEAR(area, Pi(), 1e-12);
    }
    {
        const double area = IntersectSignedAreaEclipseOriginTri(
                4.0, 0.0, 0.0, 2.0, 2.0, 1.0);
        ASSERT_NEAR(area, Pi() * 0.5, 1e-12);
    }
}

TEST(intersection_eclipse_tri, shifted_eclipse_triangle_area) {
    const Eclipse eclipse(2.0, -1.0, 2.0, 1.0);
    const Point2 pa(2.0, -1.0);
    const Point2 pb(4.0, -1.0);
    const Point2 pc(2.0, 0.0);

    ASSERT_NEAR(IntersectSignedAreaEclipseTri(pa, pb, pc, eclipse),
                1.0, 1e-12);
    ASSERT_NEAR(IntersectSignedAreaEclipseTri(pa, pc, pb, eclipse),
                -1.0, 1e-12);
}

TEST(intersection_eclipse_tri, circle_equivalent_matches_circle_tri) {
    const Eclipse eclipse(0.0, 0.0, 1.0, 1.0);
    const Circle_<double> circle(0.0, 0.0, 1.0);
    const Point2 pa(-2.0, 0.0);
    const Point2 pb(2.0, 0.0);
    const Point2 pc(0.0, 2.0);

    ASSERT_NEAR(IntersectSignedAreaEclipseTri(pa, pb, pc, eclipse),
                IntersectSignedAreaCircleTri(pa, pb, pc, circle),
                1e-12);
}

TEST(intersection_eclipse, gnuplot_eclipse_line_segment_and_triangle) {
    const Eclipse eclipse(0.0, 0.0, 2.0, 1.0);
    const Line line(-1.0, 1.0, 0.25);
    const Segment2 segment(Point2(-3.0, 0.4), Point2(3.0, 0.4));
    const Point2 pa(-4.0, 0.0);
    const Point2 pb(4.0, 0.0);
    const Point2 pc(0.0, 4.0);

    const auto line_points = IntersectLineEclipse(line, eclipse);
    const auto segment_points = IntersectSegmentEclipse(segment, eclipse);
    const double area = IntersectSignedAreaEclipseOriginTri(
            pa, pb, pc, eclipse.a(), eclipse.b());

    ASSERT_EQ(line_points.size(), 2);
    ASSERT_EQ(segment_points.size(), 2);
    ASSERT_NEAR(area, Pi(), 1e-12);

    std::cout << "Upper half eclipse-triangle intersection area: "
              << area << std::endl;

    std::filesystem::create_directories("./fig");

    Gnuplot gnu;
    gnu.set_xrange(-4.5, 4.5);
    gnu.set_yrange(-1.5, 4.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/intersection_eclipse");

    auto eclipse_actor = ToGnuplotActor(eclipse);
    eclipse_actor.line_color_blue().line_width(2);
    gnu.add(eclipse_actor);

    std::array<double, 2> line_range = {4.0, -4.0};
    auto line_actor = ToGnuplotActor(line, line_range);
    line_actor.line_color_red().line_width(2);
    gnu.add(line_actor);

    auto segment_actor = ToGnuplotActor(segment);
    segment_actor.line_color("#81BC06").line_width(2);
    gnu.add(segment_actor);

    auto triangle_actor = MakeTriangleActor(pa, pb, pc);
    triangle_actor.line_color_black().line_width(2).dash_type(2);
    gnu.add(triangle_actor);

    auto line_point_actor = ToGnuplotActor(line_points);
    line_point_actor.line_color_black().point_type(7).point_size(2);
    gnu.add(line_point_actor);

    auto segment_point_actor = ToGnuplotActor(segment_points);
    segment_point_actor.line_color_red().point_type(5).point_size(2);
    gnu.add(segment_point_actor);

    gnu.plot();
}
