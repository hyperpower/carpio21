#ifndef _OBJECTS_HPP_
#define _OBJECTS_HPP_

//#include <array>
//#include <geometry/objects/_box.hpp>
#include "geometry/geometry_define.hpp"
#include "analytic/line.hpp"
#include "analytic/plane.hpp"

#include "basic/point.hpp"
#include "basic/ray.hpp"
#include "basic/point_chain.hpp"
#include "basic/segment.hpp"
#include "basic/box.hpp"

#include "curve/curve.hpp"
#include "curve/bezier.hpp"
#include "curve/bezier_quadratic.hpp"
#include "curve/bezier_cubic.hpp"
#include "curve/bezier_high_order.hpp"

#include "ts/trisurface.hpp"


namespace carpio {

/// Geometry objects
///  Point
///  Line
///  Plane
///  Segment
///  Contour
///  Triangle
///  PointChain
///  Polygon

template <typename T>
struct IsGeoObj {
  static const bool value = false;
};


template<class ANY>
struct IsGeometry{
private:
	typedef char yes;
    typedef long no;

    template<class T, typename std::enable_if<
            std::is_base_of<GeometryTag, typename T::Tag>::value, bool>::type = true > 
    static yes  test(int i, typename T::Tag = typename T::Tag());
    template<class T>
    static no test(...);
public:
    static const bool value = sizeof(test<ANY>(0)) == sizeof(yes);

	typedef typename std::integral_constant<bool, value>::type type;
};
}

#endif
