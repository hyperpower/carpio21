#ifndef GEOMETRY_DEFINE_HPP_
#define GEOMETRY_DEFINE_HPP_

#include "type_define.hpp"
#include "utility/profile.hpp"

#include <math.h>
DEFINE_HAS_MEMBER(data);



namespace carpio {



struct GeometryTag{
	GeometryTag(){};
};

struct TSTag: public GeometryTag {
};

template<class NUM>
inline NUM Cross2(
        const NUM& ax, const NUM& ay,
        const NUM& bx, const NUM& by) {
    return ax * by - ay * bx;
}

template<class NUM>
inline NUM Dot(
        const NUM& x0, const NUM& y0,
        const NUM& x1, const NUM& y1) {
    return x0 * x1 + y0 * y1;
}

template<class NUM>
inline NUM Dot(
        const NUM& x0, const NUM& y0, const NUM& z0,
        const NUM& x1, const NUM& y1, const NUM& z1) {
    return x0 * x1 + y0 * y1 + z0 * z1;
}

enum IntersectionTypeSS {
	_SS_NO_       = 0,  //
	_SS_CONNECT_  = 1,  //
	_SS_TOUCH_    = 2,  //
	_SS_OVERLAP_  = 3,  //
	_SS_SAME_     = 4,  //
	_SS_INTERSECT_= 5,
	_SS_INVALID_  = -1,
};

}
#endif
