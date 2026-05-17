#ifndef _INTERSECTION_CIRCLE_POINT_CHAIN_HPP_
#define _INTERSECTION_CIRCLE_POINT_CHAIN_HPP_

#include "algebra/algebra_define.hpp"
#include "geometry/boolean/intersection_circle_tri.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/basic/point_chain.hpp"

#include <cmath>
#include <iterator>

namespace carpio {

template<class NUM, class POINT_CHAIN>
NUM IntersectSignedAreaCirclePointChain(
        const Circle_<NUM>& circle,
        const POINT_CHAIN& point_chain,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    if (circle.r() <= NUM(0) || point_chain.size() < 3) {
        return NUM(0);
    }

    const auto center = circle.center();
    const NUM r = circle.r();
    NUM area = NUM(0);

    auto prev = std::prev(point_chain.end());
    for (auto iter = point_chain.begin(); iter != point_chain.end(); ++iter) {
        area += IntersectSignedAreaCircleOriginTri(
                prev->x() - center.x(), prev->y() - center.y(),
                iter->x() - center.x(), iter->y() - center.y(),
                r, tol);
        prev = iter;
    }
    return area;
}

template<class NUM, class POINT_CHAIN>
NUM IntersectAreaCirclePointChain(
        const Circle_<NUM>& circle,
        const POINT_CHAIN& point_chain,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    return std::abs(IntersectSignedAreaCirclePointChain(
            circle, point_chain, tol));
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               CircleTag, PointChainTag) {
    (void)method;
    return IntersectAreaCirclePointChain(g1, g2);
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               PointChainTag, CircleTag) {
    (void)method;
    return IntersectAreaCirclePointChain(g2, g1);
}

}

#endif
