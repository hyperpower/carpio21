#ifndef _INTERSECTION_ECLIPSE_TRI_HPP_
#define _INTERSECTION_ECLIPSE_TRI_HPP_

#include "algebra/algebra_define.hpp"
#include "geometry/boolean/line_eclipse.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/analytic/eclipse.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/triangle.hpp"

#include <cmath>

namespace carpio {

template<class NUM>
NUM SignedSectorAreaEclipseOrigin(
        const NUM& ax, const NUM& ay,
        const NUM& bx, const NUM& by,
        const NUM& ea, const NUM& eb) {
    if (ea <= NUM(0) || eb <= NUM(0)) {
        return NUM(0);
    }

    const NUM sax = ax / ea;
    const NUM say = ay / eb;
    const NUM sbx = bx / ea;
    const NUM sby = by / eb;
    const NUM cross = Cross2(sax, say, sbx, sby);
    const NUM dot   = Dot(sax, say, sbx, sby);
    return NUM(0.5) * ea * eb * std::atan2(cross, dot);
}

template<class NUM>
NUM IntersectSignedAreaEclipseOriginTri(
        const NUM& point_ax, const NUM& point_ay,
        const NUM& point_bx, const NUM& point_by,
        const NUM& ea, const NUM& eb,
        const NUM& tol = DefaultTolerance<NUM>()) {

    typedef Point_<NUM, 2> Point;
    Point a(point_ax, point_ay);
    Point b(point_bx, point_by);

    if (ea <= NUM(0) || eb <= NUM(0)) {
        return NUM(0);
    }

    const NUM ea2 = ea * ea;
    const NUM eb2 = eb * eb;
    const NUM value_a = a.x() * a.x() / ea2 + a.y() * a.y() / eb2;
    const NUM value_b = b.x() * b.x() / ea2 + b.y() * b.y() / eb2;
    const bool a_inside = value_a <= NUM(1) + tol;
    const bool b_inside = value_b <= NUM(1) + tol;

    if (a_inside && b_inside) {
        return SignedAreaOriginTri2d(a.x(), a.y(), b.x(), b.y());
    }

    auto points = IntersectSegmentEclipseOrigin(
            a.x(), a.y(), b.x(), b.y(), ea, eb, tol);
    const St count = points.size();

    if (count == 0 || (!a_inside && !b_inside && count < 2)) {
        return SignedSectorAreaEclipseOrigin(
                a.x(), a.y(), b.x(), b.y(), ea, eb);
    }

    if (!a_inside && !b_inside) {
        const Point k1 = points.front();
        const Point k2 = points.back();

        return SignedSectorAreaEclipseOrigin(
                    a.x(), a.y(), k1.x(), k1.y(), ea, eb)
             + SignedAreaOriginTri2d(k1.x(), k1.y(), k2.x(), k2.y())
             + SignedSectorAreaEclipseOrigin(
                    k2.x(), k2.y(), b.x(), b.y(), ea, eb);
    }

    const Point k = a_inside ? points.back() : points.front();

    if (a_inside) {
        return SignedAreaOriginTri2d(a.x(), a.y(), k.x(), k.y())
             + SignedSectorAreaEclipseOrigin(
                    k.x(), k.y(), b.x(), b.y(), ea, eb);
    }else{
        return SignedSectorAreaEclipseOrigin(
                    a.x(), a.y(), k.x(), k.y(), ea, eb)
             + SignedAreaOriginTri2d(k.x(), k.y(), b.x(), b.y());
    }
}

template<class NUM>
NUM IntersectSignedAreaEclipseOriginTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const NUM& ea, const NUM& eb) {
    return IntersectSignedAreaEclipseOriginTri(
            pa.x(), pa.y(), pb.x(), pb.y(), ea, eb);
}

template<class NUM>
NUM IntersectSignedAreaEclipseOriginTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const Point_<NUM, 2>& pc,
        const NUM& ea, const NUM& eb) {
    return IntersectSignedAreaEclipseOriginTri(
                pa.x(), pa.y(), pb.x(), pb.y(), ea, eb)
            + IntersectSignedAreaEclipseOriginTri(
                pb.x(), pb.y(), pc.x(), pc.y(), ea, eb)
            + IntersectSignedAreaEclipseOriginTri(
                pc.x(), pc.y(), pa.x(), pa.y(), ea, eb);
}

template<class NUM>
NUM IntersectSignedAreaEclipseTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const Point_<NUM, 2>& pc,
        const Eclipse_<NUM>& eclipse) {
    const auto center = eclipse.center();
    return IntersectSignedAreaEclipseOriginTri(
            pa - center,
            pb - center,
            pc - center,
            eclipse.a(), eclipse.b());
}

template<class NUM>
NUM IntersectSignedAreaEclipseOutOriginTri(
        const NUM& point_ax, const NUM& point_ay,
        const NUM& point_bx, const NUM& point_by,
        const NUM& ea, const NUM& eb,
        const NUM& tol = DefaultTolerance<NUM>()) {
    return SignedAreaOriginTri2d(point_ax, point_ay, point_bx, point_by)
         - IntersectSignedAreaEclipseOriginTri(
                point_ax, point_ay, point_bx, point_by, ea, eb, tol);
}

template<class NUM>
NUM IntersectSignedAreaEclipseOutOriginTri(
        const Point_<NUM, 2>& pa,
        const Point_<NUM, 2>& pb,
        const NUM& ea, const NUM& eb) {
    return IntersectSignedAreaEclipseOutOriginTri(
            pa.x(), pa.y(), pb.x(), pb.y(), ea, eb);
}

}

#endif
