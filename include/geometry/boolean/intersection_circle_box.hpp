#ifndef _INTERSECTION_CIRCLE_BOX_HPP_
#define _INTERSECTION_CIRCLE_BOX_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/box.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/analytic/circle.hpp"

#include <algorithm>
#include <cmath>
#include <list>
#include <vector>

namespace carpio {

template<class NUM>
inline NUM _CircleSegmentIntegral(const NUM& x, const NUM& r) {
    if (r <= NUM(0)) {
        return NUM(0);
    }
    const NUM cx = (std::max)(-r, (std::min)(r, x));
    const NUM v  = (std::max)(NUM(0), r * r - cx * cx);
    return NUM(0.5) * (cx * std::sqrt(v) + r * r * std::asin(cx / r));
}

template<class POINT>
void _CircleBoxAddPoint(
        std::list<POINT>& res,
        const POINT& p,
        const double& tol) {
    if (!std::isfinite(double(p.x())) || !std::isfinite(double(p.y()))) {
        return;
    }
    for (const auto& op : res) {
        if (IsSame(op, p, tol)) {
            return;
        }
    }
    res.emplace_back(p);
}

template<class NUM>
inline void _CircleBoxAddYBreaks(
        std::vector<NUM>& breaks,
        const NUM& y,
        const NUM& left,
        const NUM& right,
        const NUM& r,
        const NUM& tol) {
    const NUM ay = std::abs(y);
    if (ay < r - tol) {
        const auto arrx = CalculateCircleXInRange(
                NUM(0), NUM(0), r, y, left, right, _oo_, tol);
        if (std::isfinite(double(arrx[0]))) {
            breaks.emplace_back(arrx[0]);
        }
        if (std::isfinite(double(arrx[1]))) {
            breaks.emplace_back(arrx[1]);
        }
    }
}

template<class NUM>
inline NUM _CircleBoxAreaCentered(
        const NUM& x0, const NUM& x1,
        const NUM& y0, const NUM& y1,
        const NUM& r,
        const NUM& tol) {

    const NUM left  = (std::max)(x0, -r);
    const NUM right = (std::min)(x1,  r);
    if (right <= left) {
        return NUM(0);
    }

    std::vector<NUM> breaks;
    breaks.reserve(8);
    breaks.emplace_back(left);
    breaks.emplace_back(right);

    _CircleBoxAddYBreaks(breaks, y0, left, right, r, tol);
    _CircleBoxAddYBreaks(breaks, y1, left, right, r, tol);

    std::sort(breaks.begin(), breaks.end());
    std::vector<NUM> unique_breaks;
    unique_breaks.reserve(breaks.size());
    for (auto b : breaks) {
        if (unique_breaks.empty() || std::abs(b - unique_breaks.back()) > tol) {
            unique_breaks.emplace_back(b);
        }
    }

    NUM area = NUM(0);
    for (St i = 0; i + 1 < unique_breaks.size(); ++i) {
        const NUM a = unique_breaks[i];
        const NUM b = unique_breaks[i + 1];

        const NUM xm = NUM(0.5) * (a + b);
        const NUM sm = std::sqrt((std::max)(NUM(0), r * r - xm * xm));
        const bool upper_is_arc = sm <= y1;
        const bool lower_is_arc = -sm >= y0;
        const NUM upper = upper_is_arc ? sm : y1;
        const NUM lower = lower_is_arc ? -sm : y0;
        if (upper <= lower) {
            continue;
        }

        const NUM dx = b - a;
        const NUM arc = _CircleSegmentIntegral(b, r)
                      - _CircleSegmentIntegral(a, r);
        if (upper_is_arc && lower_is_arc) {
            area += NUM(2) * arc;
        } else if (upper_is_arc) {
            area += arc - y0 * dx;
        } else if (lower_is_arc) {
            area += y1 * dx + arc;
        } else {
            area += (y1 - y0) * dx;
        }
    }

    return (std::max)(NUM(0), area);
}



template<class NUM>
std::list<Point_<NUM, 2> >
IntersectCircleBox(
        const NUM& point_ax, const NUM& point_ay,
        const NUM& point_bx, const NUM& point_by,
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    typedef Point_<NUM, 2> Point;
    typedef std::list<Point> ListPoint;
    ListPoint res;
    if (r <= NUM(0)) {
        return res;
    }

    NUM xmin, ymin, xmax, ymax;
    MinMaxPointsForBox(
            point_ax, point_ay, point_bx, point_by,
            xmin, ymin, xmax, ymax);
    if (xmax <= xmin || ymax <= ymin) {
        return res;
    }

    const auto bottom = CalculateCircleXInRange(
            xc, yc, r, ymin, xmin, xmax, _cc_, tol);
    _CircleBoxAddPoint(res, Point(bottom[0], ymin), tol);
    _CircleBoxAddPoint(res, Point(bottom[1], ymin), tol);

    const auto right = CalculateCircleYInRange(
            xc, yc, r, xmax, ymin, ymax, _cc_, tol);
    _CircleBoxAddPoint(res, Point(xmax, right[0]), tol);
    _CircleBoxAddPoint(res, Point(xmax, right[1]), tol);

    const auto top = CalculateCircleXInRange(
            xc, yc, r, ymax, xmin, xmax, _cc_, tol);
    _CircleBoxAddPoint(res, Point(top[1], ymax), tol);
    _CircleBoxAddPoint(res, Point(top[0], ymax), tol);

    const auto left = CalculateCircleYInRange(
            xc, yc, r, xmin, ymin, ymax, _cc_, tol);
    _CircleBoxAddPoint(res, Point(xmin, left[1]), tol);
    _CircleBoxAddPoint(res, Point(xmin, left[0]), tol);

    return res;
}



template<class NUM>
NUM IntersectAreaCircleBox(
        const NUM& point_ax, const NUM& point_ay,
        const NUM& point_bx, const NUM& point_by,
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    if (r <= NUM(0)) {
        return NUM(0);
    }
    // get box min max
    NUM xmin, ymin, xmax, ymax;
    MinMaxPointsForBox(
            point_ax, point_ay, point_bx, point_by,
            xmin, ymin, xmax, ymax);
    if (xmax <= xmin || ymax <= ymin) {
        return NUM(0);
    }
    // move to origin
    xmin -= xc; xmax -= xc;
    ymin -= yc; ymax -= yc;

    // box is outside of circle
    if (xmax < -r - tol || xmin > r + tol
            || ymax < -r - tol || ymin > r + tol) {
        return NUM(0);
    }
    // circle is completely inside box
    const NUM r2 = r * r;
    if (xmin <= -r - tol && xmax >= r + tol
            && ymin <= -r - tol && ymax >= r + tol) {
        return NUM(_PI_) * r2;
    }
    // box is completely inside circle
    if (Distance2_OriginToPoint2d(xmin, ymin) <= r2 + tol
            && Distance2_OriginToPoint2d(xmin, ymax) <= r2 + tol
            && Distance2_OriginToPoint2d(xmax, ymin) <= r2 + tol
            && Distance2_OriginToPoint2d(xmax, ymax) <= r2 + tol) {
        return (xmax - xmin) * (ymax - ymin);
    }

    return _CircleBoxAreaCentered(
            xmin, xmax, ymin, ymax, r, tol);
}

template<class TYPE>
std::list<Point_<TYPE, 2> >
IntersectCircleBox(const Circle_<TYPE>& circle,
                   const Box_<TYPE, 2>& box) {
    return IntersectCircleBox<TYPE>(
            box.min().x(), box.min().y(),
            box.max().x(), box.max().y(),
            circle.xc(), circle.yc(), circle.r(), DefaultFloatTolerance<TYPE>());
}

template<class TYPE>
double IntersectAreaCircleBox(
        const Circle_<TYPE>& circle,
        const Box_<TYPE, 2>& box) {
    return IntersectAreaCircleBox<TYPE>(
            box.min().x(), box.min().y(),
            box.max().x(), box.max().y(),
            circle.xc(), circle.yc(), circle.r(), DefaultFloatTolerance<TYPE>());
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               CircleTag, BoxTag){
    (void)method;
    return IntersectCircleBox(g1, g2);
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               BoxTag, CircleTag){
    (void)method;
    return IntersectCircleBox(g2, g1);
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               CircleTag, BoxTag){
    (void)method;
    return IntersectAreaCircleBox(g1, g2);
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               BoxTag, CircleTag){
    (void)method;
    return IntersectAreaCircleBox(g2, g1);
}


}

#endif
