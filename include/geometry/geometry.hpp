#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_

#include <array>
#include "geometry_define.hpp"
#include "predicate.hpp"

#include "objects/analytic/line.hpp"

#include "objects/basic/point.hpp"
#include "objects/basic/segment.hpp"
#include "objects/basic/box.hpp"
#include "objects/basic/point_chain.hpp"

#include "objects/ts/vertex.hpp"
#include "objects/ts/edge.hpp"
#include "objects/ts/triface.hpp"
#include "objects/ts/trisurface.hpp"

#include "objects/bbtree/aabbox.hpp"
#include "objects/bbtree/bbnode.hpp"
#include "objects/bbtree/bbtree.hpp"

#include "objects/objects.hpp"

#include "geometry/affine.hpp"
#include "geometry/boolean/boolean.hpp"

// #include "geometry/operation/operation.hpp"

#include "geometry/io/ggnuplot_actor.hpp"
#include "geometry/io/ggnuplot_actor_maker.hpp"
// #include "geometry/io/gplotly_actor.hpp"

#include "geometry/io/gfile.hpp"

#include "geometry/operation/creation/create_ts.hpp"

// #include "geometry/cuboidtool/cuboidtool.hpp"

namespace carpio {

//typedef Point_<Float, 2> Point_2D;
//typedef Point_<Float, 3> Point_3D;

//typedef Segment_<Float, 2> Segment_2D;
//typedef Segment_<Float, 3> Segment_3D;

//typedef Polygon_<Float> Polygon;

}

#endif
