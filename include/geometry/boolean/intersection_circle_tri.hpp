#ifndef _INTERSECTION_CIRCLE_TRI_HPP_
#define _INTERSECTION_CIRCLE_TRI_HPP_

#include "algebra/algebra_define.hpp"
#include "geometry/boolean/line_circle.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/triangle.hpp"

#include <cmath>

namespace carpio {

template<class NUM>
NUM IntersectSignedAreaCircleOriginTri(
        const NUM& point_ax, const NUM& point_ay,
        const NUM& point_bx, const NUM& point_by,
        const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {

    typedef Point_<NUM, 2> Point;
    Point a(point_ax, point_ay);
    Point b(point_bx, point_by);

    const NUM r2 = r * r;
    const NUM dist_a2 = Distance2_OriginToPoint(a);
    const NUM dist_b2 = Distance2_OriginToPoint(b);
    const bool a_inside = dist_a2 <= r2 + tol;
    const bool b_inside = dist_b2 <= r2 + tol;

    if (a_inside && b_inside) {
        return SignedAreaOriginTri2d(a.x(), a.y(), b.x(), b.y());
    }

    auto points = IntersectSegmentCircleOrigin(
            a.x(), a.y(), b.x(), b.y(), r, tol);
    const St count = points.size();

    if (count == 0 || (!a_inside && !b_inside && count < 2)) {
        return SignedSectorAreaOrigin(a.x(), a.y(), b.x(), b.y(), r);
    }

    if (!a_inside && !b_inside) {
        const Point k1 = points.front();
        const Point k2 = points.back();

        return SignedSectorAreaOrigin(
                    a.x(), a.y(), k1.x(), k1.y(), r)
             + SignedAreaOriginTri2d(k1.x(), k1.y(), k2.x(), k2.y())
             + SignedSectorAreaOrigin(k2.x(), k2.y(), b.x(), b.y(), r);
    }

    const Point k = a_inside ? points.back() : points.front();

    if (a_inside) {
        return SignedAreaOriginTri2d(a.x(), a.y(), k.x(), k.y())
             + SignedSectorAreaOrigin(k.x(), k.y(), b.x(), b.y(), r);
    }else{
        return SignedSectorAreaOrigin(a.x(), a.y(), k.x(), k.y(), r)
            + SignedAreaOriginTri2d(k.x(), k.y(), b.x(), b.y());
    }
}

template<class NUM>
NUM IntersectSignedAreaCircleOriginTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const NUM& r) {
    return IntersectSignedAreaCircleOriginTri(
            pa.x(), pa.y(), pb.x(), pb.y(), r);
}

template<class NUM>
NUM IntersectSignedAreaCircleOriginTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const Point_<NUM, 2>& pc,
        const NUM& r) {
    return IntersectSignedAreaCircleOriginTri(
                pa.x(), pa.y(), pb.x(), pb.y(), r)
            + IntersectSignedAreaCircleOriginTri(
                pb.x(), pb.y(), pc.x(), pc.y(), r)
            + IntersectSignedAreaCircleOriginTri(
                pc.x(), pc.y(), pa.x(), pa.y(), r);
}

template<class NUM>
NUM IntersectSignedAreaCircleTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const Point_<NUM, 2>& pc,
        const Circle_<NUM>& circle) {
    const auto center = circle.center();
    return IntersectSignedAreaCircleOriginTri(
            pa - center,
            pb - center,
            pc - center,
            circle.r());
}

template<class NUM>
NUM IntersectSignedAreaCircleOutOriginTri(
        const NUM& point_ax, const NUM& point_ay,
        const NUM& point_bx, const NUM& point_by,
        const NUM& r,
        const NUM& tol = DefaultFloatTolerance<NUM>()) {
    return SignedAreaOriginTri2d(point_ax, point_ay, point_bx, point_by)
         - IntersectSignedAreaCircleOriginTri(
                point_ax, point_ay, point_bx, point_by, r, tol);
}

template<class NUM>
NUM IntersectSignedAreaCircleOutOriginTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const NUM& r) {
    return IntersectSignedAreaCircleOutOriginTri(
            pa.x(), pa.y(), pb.x(), pb.y(), r);
}

}

#endif
