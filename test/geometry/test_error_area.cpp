#include "geometry/geometry.hpp"
#include "geometry/boolean/intersection_circle_box.hpp"
#include "geometry/boolean/line_box.hpp"
#include "geometry/fraction/error_area.hpp"
#include "gtest/gtest.h"

#include <cmath>
#include <string>

using namespace carpio;

namespace {

typedef Point_<double, 2> Point2;
typedef Box_<double, 2>   Box2;
typedef Circle_<double>   Circle;
typedef Line_<double>     Line;
typedef GGnuplotActorMaker_<double, 2> GAM;

void PlotErrorAreaCase(
        const std::string& name,
        const double& dx,
        const double& dy,
        const Line& line,
        const Circle& circle) {
    MakeDir("./fig/");

    const Box2 cell(Point2(0.0, 0.0), Point2(dx, dy));
    auto negative_points = NegativeLineBox(dx, dy, line);
    auto positive_points = PositiveLineBox(dx, dy, line);
    auto negative_area = AreaPointChain(negative_points);
    auto positive_area = AreaPointChain(positive_points);
    auto negative_circle_area =
            IntersectSignedAreaCirclePointChain(circle, negative_points);
    auto positive_circle_area =
            IntersectSignedAreaCirclePointChain(circle, positive_points);
    const double error_area = ErrorArea(dx, dy, line, circle);

    std::cout << "\n[error_area] " << name << "\n"
              << "  line              : "
              << line.a() << " x + " << line.b() << " y = "
              << line.alpha() << "\n"
              << "  cell              : dx = " << dx
              << ", dy = " << dy << "\n"
              << "  negative points   : " << negative_points.size() << "\n"
              << "  positive points   : " << positive_points.size() << "\n"
              << "  negative area     : " << negative_area << "\n"
              << "  positive area     : " << positive_area << "\n"
              << "  negative circle   : " << negative_circle_area << "\n"
              << "  positive circle   : " << positive_circle_area << "\n"
              << "  delta neg area    : " << (negative_area - std::abs(negative_circle_area)) << "\n"
              << "  delta pos area    : " << (positive_area - std::abs(positive_circle_area)) << "\n"
              << "  two delta area    : " << (negative_area - std::abs(negative_circle_area)) + std::abs(positive_circle_area) << "\n"
              << "  error area        : " << error_area << "\n";

    Gnuplot gnu;
    GAM gam;
    gnu.set_xrange(-0.2 * dx, 1.2 * dx);
    gnu.set_yrange(-0.2 * dy, 1.2 * dy);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + name);

    auto acell = gam.lines(cell);
    acell->style() = "with lines lw 2 lc 8";
    gnu.add(acell);

    auto acircle = ToGnuplotActor(circle);
    acircle.style() = "with lines lw 2 lc 1";
    gnu.add(acircle);

    auto aline = gam.lines(line, -1.2 * dx, 1.2 * dx);
    aline->style() = "with lines lw 3 lc 6";
    gnu.add(aline);

    if (negative_points.size() >= 2) {
        auto apolygon = gam.lines(
                negative_points.begin(), negative_points.end(), 2, true);
        apolygon->style() = "with lines lw 3 lc 2";
        gnu.add(apolygon);

        auto apoints = gam.points(
                negative_points.begin(), negative_points.end(), 7);
        apoints->style() = "with points pt 7 ps 1.5 lc 7";
        gnu.add(apoints);
    }

    gnu.set_label(1, "error area = " + ToString(error_area),
                  0.05 * dx, dy - 0.1, "left font \",16\"");  
    gnu.set_label(2, "negative area = " + ToString(negative_area),
                  0.05 * dx, dy - 0.2, "left font \",16\"");
    gnu.set_label(3, "negative circle = " + ToString(negative_circle_area),
                  0.05 * dx, dy - 0.3, "left font \",16\"");
    gnu.plot();
}

} // namespace

TEST(error_area, empty_negative_region) {
    const double dx = 1.0;
    const double dy = 1.0;
    const Line line(1.0, 1.0, -0.25);
    const Circle circle(-0.5, -0.5, 0.8);

    PlotErrorAreaCase("error_area_empty_negative_region",
                      dx, dy, line, circle);
    // ASSERT_NEAR(ErrorArea(dx, dy, line, circle), 0.0, 1e-12);
}

TEST(error_area, empty_positive_region) {
    const double dx = 1.0;
    const double dy = 1.0;
    const Line line(1.0, 1.0, 2.25);
    const Circle circle(1.5, 1.5, 0.8);
    const Box2 cell(Point2(0.0, 0.0), Point2(dx, dy));

    ASSERT_NEAR(IntersectionAreaNegative(line, cell), 1.0, 1e-12);
    ASSERT_NEAR(IntersectionAreaPositive(line, cell), 0.0, 1e-12);
    // ASSERT_NEAR(ErrorArea(dx, dy, line, circle), 0.0, 1e-12);

    PlotErrorAreaCase("error_area_empty_positive_region",
                      dx, dy, line, circle);
}

TEST(error_area, full_cell_matches_circle_box_area) {
    const double dx = 1.0;
    const double dy = 1.0;
    const Line line(1.0, 1.0, 1.1);
    const Circle circle(0.0, 0.0, 1.0);
    const Box2 cell(Point2(0.0, 0.0), Point2(dx, dy));

    auto negative = NegativeLineBox(dx, dy, line);
    auto positive = PositiveLineBox(dx, dy, line);
    auto n_area = IntersectionAreaNegative(line, cell);
    auto p_area = IntersectionAreaPositive(line, cell);

    ASSERT_NEAR(n_area + p_area, 1.0, 1e-10);
    // ASSERT_EQ(negative.size(), 3);
    // ASSERT_NEAR(n_area, 0.5, 1e-12);
    // ASSERT_NEAR(p_area, 0.5, 1e-12);
    // ASSERT_NEAR(n_area, p_area, 1e-12);

    // ASSERT_NEAR(
    //         ErrorArea(dx, dy, line, circle),
    //         IntersectAreaCircleBox(circle, cell),
    //         1e-12);
    PlotErrorAreaCase("error_area_full_cell",
                      dx, dy, line, circle);
}

TEST(error_area, error_area_triangle) {
    const double dx = 1.0;
    const double dy = 1.0;
    const Line line(1.0, 1.0, 0.9);
    const Circle circle(-0.3, -0.3, 1.1);
    const Box2 cell(Point2(0.0, 0.0), Point2(dx, dy));

    // ASSERT_NEAR(
    //         std::abs(ErrorArea(dx, dy, line, circle)),
    //         IntersectionAreaNegative(line, cell),
    //         1e-12);
    auto n_area = IntersectionAreaNegative(line, cell);
    auto p_area = IntersectionAreaPositive(line, cell);

    auto e_area = ErrorArea(dx, dy, line, circle);
    std::cout << "Negative area: " << n_area << std::endl;
    std::cout << "Positive area: " << p_area << std::endl;
    std::cout << "Error area   : " << e_area << std::endl;
    PlotErrorAreaCase("error_area_triangle",
                      dx, dy, line, circle);
}
