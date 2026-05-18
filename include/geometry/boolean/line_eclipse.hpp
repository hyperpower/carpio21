#ifndef _BOOLEAN_LINE_ECLIPSE_HPP_
#define _BOOLEAN_LINE_ECLIPSE_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/solver/direct.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/analytic/eclipse.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <list>

namespace carpio {

template<class NUM>
inline void _LineEclipseOrderAlongLineDirection(
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
IntersectLineEclipse(
        const NUM& la, const NUM& lb, const NUM& alpha,
        const NUM& xc, const NUM& yc,
        const NUM& ea, const NUM& eb,
        const NUM& tol = DefaultTolerance<NUM>()) {
    typedef Point_<NUM, 2> Point;
    typedef std::list<Point> ListPoint;
    ListPoint res;

    if (ea <= NUM(0) || eb <= NUM(0)) {
        return res;
    }
    if (std::abs(la) <= tol && std::abs(lb) <= tol) {
        return res;
    }

    const NUM ea2 = ea * ea;
    const NUM eb2 = eb * eb;

    if (std::abs(lb) <= tol) {
        const NUM x = alpha / la;
        const NUM dx = x - xc;
        const NUM d = eb2 * (NUM(1) - dx * dx / ea2);
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
        _LineEclipseOrderAlongLineDirection(res, la, lb);
        return res;
    }

    const NUM k = -la / lb;
    const NUM c = alpha / lb;
    const NUM cb = c - yc;
    const NUM coef_a = NUM(1) / ea2 + k * k / eb2;
    const NUM coef_b = NUM(-2) * xc / ea2 + NUM(2) * k * cb / eb2;
    const NUM coef_c = xc * xc / ea2 + cb * cb / eb2 - NUM(1);

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
    _LineEclipseOrderAlongLineDirection(res, la, lb);
    return res;
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectLineEclipse(
        const Line_<NUM>& line,
        const Eclipse_<NUM>& eclipse) {
    return IntersectLineEclipse(
            line.a(), line.b(), line.alpha(),
            eclipse.xc(), eclipse.yc(), eclipse.a(), eclipse.b(),
            DefaultTolerance<NUM>());
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentEclipseOrigin(
        const NUM& start_x, const NUM& start_y,
        const NUM& end_x,   const NUM& end_y,
        const NUM& ea, const NUM& eb,
        const NUM& tol = DefaultTolerance<NUM>()) {
    typedef Point_<NUM, 2> Point;
    typedef std::list<Point> ListPoint;
    ListPoint res;

    if (ea <= NUM(0) || eb <= NUM(0)) {
        return res;
    }

    const NUM dx = end_x - start_x;
    const NUM dy = end_y - start_y;
    const NUM ea2 = ea * ea;
    const NUM eb2 = eb * eb;

    const NUM coef_a = dx * dx / ea2 + dy * dy / eb2;
    if (coef_a <= tol * tol) {
        const NUM value = start_x * start_x / ea2
                        + start_y * start_y / eb2;
        if (std::abs(value - NUM(1)) <= tol) {
            res.emplace_back(start_x, start_y);
        }
        return res;
    }

    const NUM coef_b = NUM(2) * (start_x * dx / ea2
                               + start_y * dy / eb2);
    const NUM coef_c = start_x * start_x / ea2
                     + start_y * start_y / eb2
                     - NUM(1);
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
                               Point(start_x + nt2 * dx, start_y + nt2 * dy)))) {
            res.emplace_back(start_x + nt2 * dx, start_y + nt2 * dy);
        }
    }

    return res;
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentEclipse(
        const NUM& start_x, const NUM& start_y,
        const NUM& end_x,   const NUM& end_y,
        const NUM& xc, const NUM& yc,
        const NUM& ea, const NUM& eb,
        const NUM& tol = DefaultTolerance<NUM>()) {
    auto res = IntersectSegmentEclipseOrigin(
            start_x - xc, start_y - yc,
            end_x - xc, end_y - yc,
            ea, eb, tol);
    for (auto& p : res) {
        p.x() += xc;
        p.y() += yc;
    }
    return res;
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentEclipse(
        const Point_<NUM, 2>& start, const Point_<NUM, 2>& end,
        const Eclipse_<NUM>& eclipse) {
    return IntersectSegmentEclipse(
            start.x(), start.y(),
            end.x(), end.y(),
            eclipse.xc(), eclipse.yc(), eclipse.a(), eclipse.b(),
            DefaultTolerance<NUM>());
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectSegmentEclipse(
        const Segment_<NUM, 2>& segment,
        const Eclipse_<NUM>& eclipse) {
    return IntersectSegmentEclipse(
            segment.ps(), segment.pe(),
            eclipse);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               LineTag, EclipseTag) {
    (void)method;
    return IntersectLineEclipse(g1, g2);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               EclipseTag, LineTag) {
    (void)method;
    return IntersectLineEclipse(g2, g1);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               SegmentTag, EclipseTag) {
    (void)method;
    return IntersectSegmentEclipse(g1, g2);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               EclipseTag, SegmentTag) {
    (void)method;
    return IntersectSegmentEclipse(g2, g1);
}

}

#endif
