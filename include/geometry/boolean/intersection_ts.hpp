#ifndef INTERSECTION_TS_HPP
#define INTERSECTION_TS_HPP

#include "geometry/objects/ts/trisurface.hpp"
#include "geometry/boolean/boolean.hpp"

namespace carpio {

template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2, TriSurfaceTag, TriSurfaceTag){
    std::cout << "Intersecting two TriSurfaces" << std::endl;
    return 1;
}



}


#endif // INTERSECTION_TS_HPP