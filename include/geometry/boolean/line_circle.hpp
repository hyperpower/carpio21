#ifndef _BOOLEAN_LINE_CIRCLE_HPP_
#define _BOOLEAN_LINE_CIRCLE_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/solver/direct.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <list>

namespace carpio {

template<class NUM>
inline void _LineCircleOrderAlongLineDirection(
        std::list<Point_<NUM, 2> >& points,
        const NUM& a,
        const NUM& b) {
    if (points.size() != 2) {
        return;
    }

    auto first = points.begin();
    auto second = std::next(first);
    const NUM dx = second->x() - first->x();
    const NUM dy = second->y() - first->y();
    const NUM dot = Dot(dx, dy, -b, a);
    if (dot < NUM(0)) {
        std::swap(*first, *second);
    }
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectLineCircle(
        const NUM& a, const NUM& b, const NUM& alpha,
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    typedef Point_<NUM, 2> Point;
    typedef std::list<Point> ListPoint;
    ListPoint res;

    if (r <= NUM(0)) {
        return res;
    }
    if (std::abs(a) <= tol && std::abs(b) <= tol) {
        return res;
    }

    if (std::abs(b) <= tol) {
        const NUM x = alpha / a;
        const NUM d = r * r - (x - xc) * (x - xc);
        if (d < -tol) {
            return res;
        }
        if (std::abs(d) <= tol) {
            res.emplace_back(x, yc);
            return res;
        }
        const NUM sd = std::sqrt(d);
        res.emplace_back(x, yc - sd);
        res.emplace_back(x, yc + sd);
        _LineCircleOrderAlongLineDirection(res, a, b);
        return res;
    }

    const NUM k = -a / b;
    const NUM c = alpha / b;
    const NUM cb = c - yc;
    const NUM coef_a = NUM(1) + k * k;
    const NUM coef_b = NUM(2) * (k * cb - xc);
    const NUM coef_c = xc * xc + cb * cb - r * r;

    const NUM discri = coef_b * coef_b - NUM(4) * coef_a * coef_c;
    if (discri < -tol) {
        return res;
    }
    if (std::abs(discri) <= tol) {
        const NUM x = -coef_b / (NUM(2) * coef_a);
        res.emplace_back(x, k * x + c);
        return res;
    }

    double x1 = 0.0;
    double x2 = 0.0;
    const int num_roots = SolveQuadraticEquation(
            coef_a, coef_b, coef_c, x1, x2);
    if (num_roots == 0) {
        return res;
    }
    if (num_roots == 1) {
        const NUM x = NUM(x1);
        res.emplace_back(x, k * x + c);
        return res;
    }

    const NUM nx1 = NUM(x1);
    const NUM nx2 = NUM(x2);
    res.emplace_back(nx1, k * nx1 + c);
    res.emplace_back(nx2, k * nx2 + c);
    _LineCircleOrderAlongLineDirection(res, a, b);
    return res;
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectLineCircle(
        const Line_<NUM>& line,
        const Circle_<NUM>& circle) {
    return IntersectLineCircle(
            line.a(), line.b(), line.alpha(),
            circle.xc(), circle.yc(), circle.r(),
            DefaultFloatTolerance<NUM>());
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentCircleOrigin(
        const NUM& start_x, const NUM& start_y,
        const NUM& end_x,   const NUM& end_y,
        const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    typedef Point_<NUM, 2> Point;
    typedef std::list<Point> ListPoint;
    ListPoint res;

    if (r <= NUM(0)) {
        return res;
    }

    const NUM dx = end_x - start_x;
    const NUM dy = end_y - start_y;

    const NUM coef_a = Dot(dx, dy, dx, dy);
    if (coef_a <= tol * tol) {
        const NUM dist2 = Dot(start_x, start_y, start_x, start_y);
        if (std::abs(dist2 - r * r) <= tol) {
            res.emplace_back(start_x, start_y);
        }
        return res;
    }

    const NUM coef_b = NUM(2) * Dot(start_x, start_y, dx, dy);
    const NUM coef_c = Dot(start_x, start_y, start_x, start_y) - r * r;
    const NUM discri = coef_b * coef_b - NUM(4) * coef_a * coef_c;

    if (discri < -tol) {
        return res;
    }

    if (std::abs(discri) <= tol) {
        const NUM t = -coef_b / (NUM(2) * coef_a);
        if (IsInRange(NUM(0), t, NUM(1), _cc_, tol)) {
            res.emplace_back(start_x + t * dx, start_y + t * dy);
        }
        return res;
    }

    double t1 = 0.0;
    double t2 = 0.0;
    const int num_roots = SolveQuadraticEquation(
            coef_a, coef_b, coef_c, t1, t2);
    if (num_roots == 0) {
        return res;
    }

    const NUM nt1 = NUM(t1);
    if (IsInRange(NUM(0), nt1, NUM(1), _cc_, tol)) {
        res.emplace_back(start_x + nt1 * dx, start_y + nt1 * dy);
    }

    if (num_roots == 2) {
        const NUM nt2 = NUM(t2);
        if (IsInRange(NUM(0), nt2, NUM(1), _cc_, tol)
                && (res.empty()
                    || !IsSame(res.back(),
                               Point(start_x + nt2 * dx, start_y + nt2 * dy),
                               tol))) {
            res.emplace_back(start_x + nt2 * dx, start_y + nt2 * dy);
        }
    }

    return res;
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentCircle(
        const NUM& start_x, const NUM& start_y,
        const NUM& end_x,   const NUM& end_y,
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    auto res = IntersectSegmentCircleOrigin(
            start_x - xc, start_y - yc,
            end_x - xc, end_y - yc,
            r, tol);
    for (auto& p : res) {
        p.x() += xc;
        p.y() += yc;
    }
    return res;
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentCircle(
        const Point_<NUM,2 >& start, const Point_<NUM,2>& end,
        const Circle_<NUM>& circle) {
    return IntersectSegmentCircle(
            start.x(), start.y(),
            end.x(), end.y(),
            circle.xc(), circle.yc(), circle.r(),
            DefaultFloatTolerance<NUM>());
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentCircle(
        const Segment_<NUM,2>& segment,
        const Circle_<NUM>& circle) {
    return IntersectSegmentCircle(
            segment.start(), segment.end(),
            circle);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               LineTag, CircleTag) {
    (void)method;
    return IntersectLineCircle(g1, g2);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               CircleTag, LineTag) {
    (void)method;
    return IntersectLineCircle(g2, g1);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               SegmentTag, CircleTag) {
    (void)method;
    return IntersectSegmentCircle(g1, g2);
}
template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               CircleTag, SegmentTag) {
    (void)method;
    return IntersectSegmentCircle(g2, g1);
}
}

#endif
