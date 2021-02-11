#ifndef _INTERSECTION_LS_HPP_
#define _INTERSECTION_LS_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "algebra/algebra.hpp"

#include <memory>
#include <array>

namespace carpio {

template<class TYPE>
std::shared_ptr<Point_<TYPE, 2> >
IntersectLineSegment(
		const TYPE& sx,
		const TYPE& sy,
		const TYPE& ex,
		const TYPE& ey,
		const TYPE& a,
		const TYPE& b,
		const TYPE& c){
	typedef Line_<TYPE> Line;
	typedef Point_<TYPE, 2> Point;
	std::array<TYPE, 3> l1 = Line::Construct(sx, sy, ex, ey);
	std::array<TYPE, 3> l2 = {a, b, c};
	auto res = Line::Intersect(l1[0], l1[1], l1[2],
			                   l2[0], l2[1], l2[2]);
	if (res != nullptr) {
		Point ps(sx, sy);
		Point pe(ex, ey);
		Point max = Max(ps, pe);
		Point min = Min(ps, pe);
		if (IsInRange(min.x(), res->x(), max.x(), _cc_)
				&& IsInRange(min.y(), res->y(), max.y(), _cc_)) {
			return res;
		} else {
			return std::shared_ptr<Point_<TYPE, 2> >(nullptr);
		}
	}
}

template<class TYPE>
std::shared_ptr<Point_<TYPE, 2> >
IntersectLineExtSegment(
		const TYPE& sx,
		const TYPE& sy,
		const TYPE& ex,
		const TYPE& ey,
		const TYPE& a,
		const TYPE& b,
		const TYPE& c){
	typedef Line_<TYPE> Line;
	typedef Point_<TYPE, 2> Point;
	std::array<TYPE, 3> l1 = Construct(sx, sy, ex, ey);
	std::array<TYPE, 3> l2 = {a, b, c};
	return  Line::Intersect(l1[0], l1[1], l1[2],
			                l2[0], l2[1], l2[2]);
}



template<class TYPE, St DIM>
std::shared_ptr<Point_<TYPE, DIM> >
IntersectLineSegment(
		const Point_<TYPE, DIM>& pstart,
		const Point_<TYPE, DIM>& pend,
		const Line_<TYPE>        line){
	return IntersectLineSegment(pstart.x(), pstart.y(),
			                    pend.x(), pend.y(),
								line.a(), line.b(), line.alpha());
}
// Intersection
// Line  ---  Extend Segment
template<class TYPE, St DIM>
std::shared_ptr<Point_<TYPE, DIM> >
IntersectLineExtSegment(
		const Point_<TYPE, DIM>& pstart,
		const Point_<TYPE, DIM>& pend,
		const Line_<TYPE>        line){
	return IntersectLineExtSegment(pstart.x(), pstart.y(),
			                    pend.x(), pend.y(),
								line.a(), line.b(), line.alpha());
}


}

#endif
