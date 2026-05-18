
#ifndef _INTERSECTION_BOX_BOX_HPP_
#define _INTERSECTION_BOX_BOX_HPP_

#include "algebra/algebra_define.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/box.hpp"
#include "geometry/objects/basic/point.hpp"

#include <algorithm>
#include <cmath>
#include <list>
#include <vector>

namespace carpio {

template<class POINT>
void _BoxBoxAddPoint(
        std::list<POINT>& res,
        const POINT& p,
        const typename POINT::coord_value_type& tol) {
    if (!std::isfinite(double(p.x())) || !std::isfinite(double(p.y()))) {
        return;
    }
    for (const auto& op : res) {
        if (IsSame(op, p)) {
            return;
        }
    }
    res.emplace_back(p);
}

template<class NUM>
std::list<Point_<NUM, 2> >
IntersectBoxBox(
        const NUM& box1_point_ax, const NUM& box1_point_ay,
        const NUM& box1_point_bx, const NUM& box1_point_by,
        const NUM& box2_point_ax, const NUM& box2_point_ay,
        const NUM& box2_point_bx, const NUM& box2_point_by,
        const NUM& tol = DefaultTolerance<NUM>()) {
    typedef Point_<NUM, 2> Point;
    typedef std::list<Point> ListPoint;
    ListPoint res;

    NUM b1_xmin, b1_ymin, b1_xmax, b1_ymax;
    MinMaxPointsForBox(
            box1_point_ax, box1_point_ay,
            box1_point_bx, box1_point_by,
            b1_xmin, b1_ymin, b1_xmax, b1_ymax);

    NUM b2_xmin, b2_ymin, b2_xmax, b2_ymax;
    MinMaxPointsForBox(
            box2_point_ax, box2_point_ay,
            box2_point_bx, box2_point_by,
            b2_xmin, b2_ymin, b2_xmax, b2_ymax);

    if (b1_xmax < b1_xmin || b1_ymax < b1_ymin
            || b2_xmax < b2_xmin || b2_ymax < b2_ymin) {
        return res;
    }

    const NUM xmin = (std::max)(b1_xmin, b2_xmin);
    const NUM ymin = (std::max)(b1_ymin, b2_ymin);
    const NUM xmax = (std::min)(b1_xmax, b2_xmax);
    const NUM ymax = (std::min)(b1_ymax, b2_ymax);

    if (xmax < xmin - tol || ymax < ymin - tol) {
        return res;
    }

    // Counter-clockwise order from the lower-left corner.
    _BoxBoxAddPoint(res, Point(xmin, ymin), tol);
    _BoxBoxAddPoint(res, Point(xmax, ymin), tol);
    _BoxBoxAddPoint(res, Point(xmax, ymax), tol);
    _BoxBoxAddPoint(res, Point(xmin, ymax), tol);

    return res;
}

template<class TYPE>
std::list<Point_<TYPE, 2> >
IntersectBoxBox(
        const Box_<TYPE, 2>& box1,
        const Box_<TYPE, 2>& box2) {
    return IntersectBoxBox<TYPE>(
            box1.min().x(), box1.min().y(),
            box1.max().x(), box1.max().y(),
            box2.min().x(), box2.min().y(),
            box2.max().x(), box2.max().y(),
            DefaultTolerance<TYPE>());
}

template<class NUM>
NUM IntersectAreaBoxBox(
        const NUM& box1_point_ax, const NUM& box1_point_ay,
        const NUM& box1_point_bx, const NUM& box1_point_by,
        const NUM& box2_point_ax, const NUM& box2_point_ay,
        const NUM& box2_point_bx, const NUM& box2_point_by,
        const NUM& tol = DefaultTolerance<NUM>()) {
    NUM b1_xmin, b1_ymin, b1_xmax, b1_ymax;
    MinMaxPointsForBox(
            box1_point_ax, box1_point_ay,
            box1_point_bx, box1_point_by,
            b1_xmin, b1_ymin, b1_xmax, b1_ymax);

    NUM b2_xmin, b2_ymin, b2_xmax, b2_ymax;
    MinMaxPointsForBox(
            box2_point_ax, box2_point_ay,
            box2_point_bx, box2_point_by,
            b2_xmin, b2_ymin, b2_xmax, b2_ymax);

    const NUM xmin = (std::max)(b1_xmin, b2_xmin);
    const NUM ymin = (std::max)(b1_ymin, b2_ymin);
    const NUM xmax = (std::min)(b1_xmax, b2_xmax);
    const NUM ymax = (std::min)(b1_ymax, b2_ymax);

    if (xmax <= xmin + tol || ymax <= ymin + tol) {
        return NUM(0);
    }
    return (xmax - xmin) * (ymax - ymin);
}

template<class TYPE>
TYPE IntersectAreaBoxBox(
        const Box_<TYPE, 2>& box1,
        const Box_<TYPE, 2>& box2) {
    return IntersectAreaBoxBox<TYPE>(
            box1.min().x(), box1.min().y(),
            box1.max().x(), box1.max().y(),
            box2.min().x(), box2.min().y(),
            box2.max().x(), box2.max().y(),
            DefaultTolerance<TYPE>());
}

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               BoxTag, BoxTag){
    (void)method;
    return IntersectBoxBox(g1, g2);
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               BoxTag, BoxTag){
    (void)method;
    return IntersectAreaBoxBox(g1, g2);
}

}

#endif
