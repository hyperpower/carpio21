#ifndef _GEOMETRY_FRACTION_ERROR_AREA_HPP_
#define _GEOMETRY_FRACTION_ERROR_AREA_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/boolean/intersection_circle_point_chain.hpp"
#include "geometry/boolean/line_box.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/analytic/line.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>

namespace carpio {


template<class NUM>
NUM ErrorArea(const NUM& dx, const NUM& dy, 
              const Line_<NUM>& line, const Circle_<NUM>& circle) {
    const NUM tol = DefaultFloatTolerance<NUM>();

    auto nagpc = NegativeLineBox(dx, dy, line);
    auto area_nagpc= AreaPointChain(nagpc);

    auto pospc = PositiveLineBox(dx, dy, line);

    NUM area_nag = 0.0;
    NUM area_pos = 0.0;
    if (nagpc.size() >= 3) {
        area_nag = IntersectSignedAreaCirclePointChain(circle, nagpc, tol);
    }
    if (pospc.size() >= 3) {
        area_pos = IntersectSignedAreaCirclePointChain(circle, pospc, tol);
    }

    return area_nagpc - std::abs(area_nag) + std::abs(area_pos);
}

}

#endif
