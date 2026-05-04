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

constexpr double _CircleBoxPi() {
    return 3.141592653589793238462643383279502884;
}

inline double _CircleSegmentIntegral(const double& x, const double& r) {
    if (r <= 0.0) {
        return 0.0;
    }
    const double cx = (std::max)(-r, (std::min)(r, x));
    const double v  = (std::max)(0.0, r * r - cx * cx);
    return 0.5 * (cx * std::sqrt(v) + r * r * std::asin(cx / r));
}

inline void _CircleBoxAddUniqueBreak(
        std::vector<double>& breaks,
        const double& value,
        const double& left,
        const double& right,
        const double& tol) {
    if (value > left + tol && value < right - tol) {
        breaks.emplace_back(value);
    }
}

inline double _CircleBoxAreaCentered(
        const double& x0, const double& x1,
        const double& y0, const double& y1,
        const double& r,
        const double& tol) {
    if (r <= 0.0 || x1 <= x0 || y1 <= y0) {
        return 0.0;
    }

    const double left  = (std::max)(x0, -r);
    const double right = (std::min)(x1,  r);
    if (right <= left) {
        return 0.0;
    }

    std::vector<double> breaks;
    breaks.reserve(10);
    breaks.emplace_back(left);
    breaks.emplace_back(right);

    const auto add_y_breaks = [&](const double& y) {
        const double ay = std::abs(y);
        if (ay < r - tol) {
            const double x = std::sqrt((std::max)(0.0, r * r - ay * ay));
            _CircleBoxAddUniqueBreak(breaks, -x, left, right, tol);
            _CircleBoxAddUniqueBreak(breaks,  x, left, right, tol);
        }
    };
    add_y_breaks(y0);
    add_y_breaks(y1);

    std::sort(breaks.begin(), breaks.end());
    std::vector<double> unique_breaks;
    unique_breaks.reserve(breaks.size());
    for (auto b : breaks) {
        if (unique_breaks.empty() || std::abs(b - unique_breaks.back()) > tol) {
            unique_breaks.emplace_back(b);
        }
    }

    double area = 0.0;
    for (St i = 0; i + 1 < unique_breaks.size(); ++i) {
        const double a = unique_breaks[i];
        const double b = unique_breaks[i + 1];
        if (b <= a) {
            continue;
        }

        const double xm = 0.5 * (a + b);
        const double sm = std::sqrt((std::max)(0.0, r * r - xm * xm));
        const bool upper_is_arc = sm <= y1;
        const bool lower_is_arc = -sm >= y0;
        const double upper = upper_is_arc ? sm : y1;
        const double lower = lower_is_arc ? -sm : y0;
        if (upper <= lower) {
            continue;
        }

        const double dx = b - a;
        const double arc = _CircleSegmentIntegral(b, r)
                         - _CircleSegmentIntegral(a, r);
        if (upper_is_arc && lower_is_arc) {
            area += 2.0 * arc;
        } else if (upper_is_arc) {
            area += arc - y0 * dx;
        } else if (lower_is_arc) {
            area += y1 * dx + arc;
        } else {
            area += (y1 - y0) * dx;
        }
    }

    return (std::max)(0.0, area);
}

template<class POINT>
void _CircleBoxAddPoint(
        std::list<POINT>& res,
        const POINT& p,
        const double& tol) {
    for (const auto& op : res) {
        if (IsSame(op, p, tol)) {
            return;
        }
    }
    res.emplace_back(p);
}

template<class POINT>
void _CircleBoxAddFinitePoint(
        std::list<POINT>& res,
        const POINT& p,
        const double& tol) {
    if (std::isfinite(double(p.x())) && std::isfinite(double(p.y()))) {
        _CircleBoxAddPoint(res, p, tol);
    }
}

template<class POINT>
std::list<POINT>
IntersectCircleBox(const POINT& min, const POINT& max,
                   const double& xc, const double& yc, const double& r,
                   const double& tol = 1e-14) {
    static_assert(POINT::Dim == 2, "IntersectCircleBox only supports 2D points.");
    typedef std::list<POINT> ListPoint;
    ListPoint res;
    if (r <= 0.0) {
        return res;
    }

    const double xmin = (std::min)(double(min.x()), double(max.x()));
    const double xmax = (std::max)(double(min.x()), double(max.x()));
    const double ymin = (std::min)(double(min.y()), double(max.y()));
    const double ymax = (std::max)(double(min.y()), double(max.y()));
    if (xmax <= xmin || ymax <= ymin) {
        return res;
    }

    const auto bottom = CalculateCircleXInRange(
            xc, yc, r, ymin, xmin, xmax, _cc_, tol);
    _CircleBoxAddFinitePoint(res, POINT(bottom[0], ymin), tol);
    _CircleBoxAddFinitePoint(res, POINT(bottom[1], ymin), tol);

    const auto right = CalculateCircleYInRange(
            xc, yc, r, xmax, ymin, ymax, _cc_, tol);
    _CircleBoxAddFinitePoint(res, POINT(xmax, right[0]), tol);
    _CircleBoxAddFinitePoint(res, POINT(xmax, right[1]), tol);

    const auto top = CalculateCircleXInRange(
            xc, yc, r, ymax, xmin, xmax, _cc_, tol);
    _CircleBoxAddFinitePoint(res, POINT(top[1], ymax), tol);
    _CircleBoxAddFinitePoint(res, POINT(top[0], ymax), tol);

    const auto left = CalculateCircleYInRange(
            xc, yc, r, xmin, ymin, ymax, _cc_, tol);
    _CircleBoxAddFinitePoint(res, POINT(xmin, left[1]), tol);
    _CircleBoxAddFinitePoint(res, POINT(xmin, left[0]), tol);

    return res;
}

template<class TYPE>
std::list<Point_<TYPE, 2> >
IntersectCircleBox(const Circle_<TYPE>& circle,
                   const Box_<TYPE, 2>& box,
                   const double& tol = 1e-14) {
    typedef Point_<TYPE, 2> Point;
    return IntersectCircleBox<Point>(
            box.min(), box.max(),
            circle.xc(), circle.yc(), circle.r(), tol);
}

template<class POINT>
double IntersectAreaCircleBox(
        const POINT& min, const POINT& max,
        const double& xc, const double& yc, const double& r,
        const double& tol = 1e-14) {
    static_assert(POINT::Dim == 2, "IntersectAreaCircleBox only supports 2D points.");
    if (r <= 0.0) {
        return 0.0;
    }

    const double xmin = (std::min)(double(min.x()), double(max.x()));
    const double xmax = (std::max)(double(min.x()), double(max.x()));
    const double ymin = (std::min)(double(min.y()), double(max.y()));
    const double ymax = (std::max)(double(min.y()), double(max.y()));
    if (xmax <= xmin || ymax <= ymin) {
        return 0.0;
    }

    const double r2 = r * r;
    const double closest_x = (std::max)(xmin, (std::min)(xmax, xc));
    const double closest_y = (std::max)(ymin, (std::min)(ymax, yc));
    const double dx = closest_x - xc;
    const double dy = closest_y - yc;
    if (dx * dx + dy * dy > r2 + tol) {
        return 0.0;
    }

    if (xmin <= xc - r && xmax >= xc + r
            && ymin <= yc - r && ymax >= yc + r) {
        return _CircleBoxPi() * r2;
    }

    const bool cover_box =
            Distance2_PointToPoint2d(xc, yc, xmin, ymin) <= r2 + tol
         && Distance2_PointToPoint2d(xc, yc, xmin, ymax) <= r2 + tol
         && Distance2_PointToPoint2d(xc, yc, xmax, ymin) <= r2 + tol
         && Distance2_PointToPoint2d(xc, yc, xmax, ymax) <= r2 + tol;
    if (cover_box) {
        return (xmax - xmin) * (ymax - ymin);
    }

    return _CircleBoxAreaCentered(
            xmin - xc, xmax - xc,
            ymin - yc, ymax - yc,
            r, tol);
}

template<class TYPE>
double IntersectAreaCircleBox(
        const Circle_<TYPE>& circle,
        const Box_<TYPE, 2>& box,
        const double& tol = 1e-14) {
    return IntersectAreaCircleBox(
            box.min(), box.max(),
            circle.xc(), circle.yc(), circle.r(), tol);
}

}

#endif
