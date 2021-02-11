#ifndef GEOMETRY_DEFINE_HPP_
#define GEOMETRY_DEFINE_HPP_

#include "type_define.hpp"

#include <math.h>

namespace carpio {



struct TagGeometry{
	TagGeometry(){};
};

enum IntersectionTypeSS {
	_SS_NO_       = 0,  //
	_SS_CONNECT_  = 1,  //
	_SS_TOUCH_    = 2,  //
	_SS_OVERLAP_  = 3,  //
	_SS_SAME_     = 4,  //
	_SS_INTERSECT_= 5
};

}
#endif
