#include "geometry/fraction/piecewise_linear.hpp"
#include "geometry/boolean/line_box.hpp"
#include "gtest/gtest.h"

using namespace carpio;

namespace {

typedef Point_<double, 2> Point2;
typedef Box_<double, 2>   Box2;
typedef Line_<double>     Line;

void AssertSameArea(
        const double& a, const double& b, const double& alpha,
        const double& dx, const double& dy){
    const auto fraction_area = FractionVolume(a, b, alpha, dx, dy);
    const Line line(a, b, alpha);
    const Box2 box(Point2(0.0, 0.0), Point2(dx, dy));
    const auto intersection_area = IntersectionAreaNegative(line, box);
    ASSERT_NEAR(fraction_area, intersection_area, 1e-12);
}

void AssertAlphaRoundTrip(
        const double& fraction,
        const double& a, const double& b,
        const double& dx, const double& dy){
    const auto alpha = CalAlpha(fraction, a, b, dx, dy);
    const auto area = FractionVolume(a, b, alpha, dx, dy);
    ASSERT_NEAR(area / (dx * dy), fraction, 1e-12);
}

void AssertAlphaRoundTrip(
        const double& fraction,
        const double& a, const double& b, const double& c,
        const double& dx, const double& dy, const double& dz){
    const auto alpha = CalAlpha(fraction, a, b, c, dx, dy, dz);
    const auto volume = FractionVolume(a, b, c, alpha, dx, dy, dz);
    ASSERT_NEAR(volume / (dx * dy * dz), fraction, 1e-12);
}

} // namespace

TEST(piecewise_linear, fraction_volume_2d) {
    ASSERT_NEAR(FractionVolume(1.0, 1.0, 1.0, 1.0, 1.0), 0.5, 1e-12);
    AssertSameArea(1.0, 1.0, 1.0, 1.0, 1.0);
    ASSERT_NEAR(FractionVolume(1.0, 1.0, 1.0, 2.0, 1.0), 0.5, 1e-12);
    AssertSameArea(1.0, 1.0, 1.0, 2.0, 1.0);
    ASSERT_NEAR(FractionVolume(-1.0, 1.0, 0.0, 1.0, 1.0), 0.5, 1e-12);
    AssertSameArea(-1.0, 1.0, 0.0, 1.0, 1.0);

    ASSERT_NEAR(FractionVolume(1.0, 1.0, -1.0, 2.0, 3.0), 0.0, 1e-12);
    AssertSameArea(1.0, 1.0, -1.0, 2.0, 3.0);
    ASSERT_NEAR(FractionVolume(1.0, 1.0, 5.0, 2.0, 3.0), 6.0, 1e-12);
    AssertSameArea(1.0, 1.0, 5.0, 2.0, 3.0);

    ASSERT_NEAR(FractionVolume(1.0, 0.0, 0.5, 2.0, 3.0), 1.5, 1e-10);
    AssertSameArea(1.0, 0.0, 0.5, 2.0, 3.0);
    ASSERT_NEAR(FractionVolume(1e-16, 1.0, 0.5, 1.0, 1.0), 0.5, 1e-12);
}

TEST(piecewise_linear, fraction_volume_3d) {
    ASSERT_NEAR(
            FractionVolume(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0),
            1.0 / 6.0, 1e-12);
    ASSERT_NEAR(
            FractionVolume(1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0),
            1.0 / 6.0, 1e-12);
    ASSERT_NEAR(
            FractionVolume(-1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0),
            1.0 / 6.0, 1e-12);
    ASSERT_NEAR(
            FractionVolume(-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0),
            5.0 / 6.0, 1e-12);
    ASSERT_NEAR(
            FractionVolume(1.0, 1.0, 1.0, 1.5, 1.0, 1.0, 1.0),
            0.5, 1e-12);

    ASSERT_NEAR(
            FractionVolume(1.0, 1.0, 1.0, -1.0, 2.0, 3.0, 4.0),
            0.0, 1e-12);
    ASSERT_NEAR(
            FractionVolume(1.0, 1.0, 1.0, 9.0, 2.0, 3.0, 4.0),
            24.0, 1e-12);

    ASSERT_NEAR(
            FractionVolume(1.0, 0.0, 0.0, 0.5, 2.0, 3.0, 4.0),
            6.0, 1e-10);
    ASSERT_NEAR(
            FractionVolume(1e-16, 1.0, 0.0, 0.5, 1.0, 1.0, 1.0),
            0.5, 1e-12);
}

TEST(piecewise_linear, cal_alpha_2d) {
    ASSERT_NEAR(CalAlpha(0.5, 1.0, 1.0, 1.0, 1.0), 1.0, 1e-12);
    ASSERT_NEAR(CalAlpha(0.25, 1.0, 1.0, 2.0, 1.0), 1.0, 1e-12);
    ASSERT_NEAR(CalAlpha(0.5, -1.0, 1.0, 1.0, 1.0), 0.0, 1e-12);

    AssertAlphaRoundTrip(0.25, 1.0, 1.0, 2.0, 1.0);
    AssertAlphaRoundTrip(0.75, 1.0, 1.0, 2.0, 1.0);
    AssertAlphaRoundTrip(0.5, -1.0, 1.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.25, 1.0, 0.0, 2.0, 3.0);
    AssertAlphaRoundTrip(0.5, 1e-16, 1.0, 1.0, 1.0);
}

TEST(piecewise_linear, cal_alpha_3d) {
    ASSERT_NEAR(CalAlpha(1.0 / 6.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0), 1.0, 1e-12);
    ASSERT_NEAR(CalAlpha(0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0), 1.5, 1e-12);
    ASSERT_NEAR(CalAlpha(1.0 / 6.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0), 0.0, 1e-12);

    AssertAlphaRoundTrip(0.25, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.75, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.20, 0.3, 0.7, 1.4, 1.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.40, 0.3, 0.7, 1.4, 1.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.75, 0.3, 0.7, 1.4, 1.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.5, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    AssertAlphaRoundTrip(0.25, 1.0, 0.0, 0.0, 2.0, 3.0, 4.0);
    AssertAlphaRoundTrip(0.5, 1e-16, 1.0, 0.0, 1.0, 1.0, 1.0);
}
