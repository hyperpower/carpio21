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
