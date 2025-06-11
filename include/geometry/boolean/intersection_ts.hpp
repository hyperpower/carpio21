#ifndef INTERSECTION_TS_HPP
#define INTERSECTION_TS_HPP

#include "geometry/objects/ts/trisurface.hpp"
#include "geometry/boolean/boolean.hpp"
#include "geometry/tri-tri/tri_tri_intersection.hpp"

namespace carpio {



template<class GEO1, class GEO2>
auto IsIntersectN2(const GEO1& g1, const GEO2& g2, TriSurfaceTag, TriSurfaceTag){
    std::cout << "Intersecting two TriSurfaces" << std::endl;
    // Bounding box check -----
    // Todo
    // Iterate through all triangles in g1 and g2
    for (auto& pfac1 : g1){
        for (auto& pfac2 : g2){
            // Check if the triangles intersect
            auto& v0 = *(pfac1->vertex(0));
            auto& v1 = *(pfac1->vertex(1));
            auto& v2 = *(pfac1->vertex(2));
            auto& u0 = *(pfac2->vertex(0));
            auto& u1 = *(pfac2->vertex(1));
            auto& u2 = *(pfac2->vertex(2));
            if(IsIntersect(v0.data(),v1.data(),v2.data(),
                           u0.data(),u1.data(),u2.data())){
                return true; // Intersection found
            }
            }
        }
        return false; // No intersection found
} 

template<class GEO1, class GEO2>
auto IsIntersect(const GEO1& g1, const GEO2& g2, const std::string& method, TriSurfaceTag, TriSurfaceTag){
    std::cout << "Intersecting two TriSurfaces" << std::endl;
    // Bounding box check -----
    // Todo
    // Iterate through all triangles in g1 and g2
    return IsIntersectN2(g1, g2, TriSurfaceTag(), TriSurfaceTag());
} 


} // namespace carpio

#endif // INTERSECTION_TS_HPP