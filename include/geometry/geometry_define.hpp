#ifndef GEOMETRY_DEFINE_HPP_
#define GEOMETRY_DEFINE_HPP_

#include "type_define.hpp"

#include <math.h>

namespace carpio {



struct GeometryTag{
	GeometryTag(){};
};

template<class ANY>
struct IsGeometry{
private:
	typedef char yes;
    typedef long no;

    template<class T>
    static yes  test(int i, typename T::Tag = typename T::Tag());
    template<class T>
    static no test(...);
public:
    static const bool value = sizeof(test<ANY>(0)) == sizeof(yes);

	typedef typename std::integral_constant<bool, value>::type type;
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
