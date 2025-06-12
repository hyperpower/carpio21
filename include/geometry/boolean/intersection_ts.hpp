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

template<class GEO1, class GEO2>
struct IntersectionResultImplement_<GEO1, GEO2, TriFaceTag, TriFaceTag>{
public:
    typedef GEO1 TriFace;
    typedef IntersectionResultImplement_<GEO1, GEO2, TriFaceTag, TriFaceTag> Self;
    typedef const GEO1* cpTriFace;
    typedef std::list<cpTriFace> ListcpTriFace;
    typedef typename GEO1::Poi Point;
    typedef typename GEO1::Ver Ver;

    cpTriFace _cpt1;
    cpTriFace _cpt2;
    bool _is_intersect;
    bool _is_coplanar;
    Point _ips;
    Point _ipe;
public:
    IntersectionResultImplement_():
        _cpt1(nullptr), _cpt2(nullptr), _is_coplanar(false), _is_intersect(false){};

    IntersectionResultImplement_(cpTriFace t1, cpTriFace t2):
        _cpt1(t1), _cpt2(t2), _is_coplanar(false), _is_intersect(false) {
    }

    void set_intersect_segment(const Point& ips, const Point& ipe) {
        _ips = ips;
        _ipe = ipe;
    }
    void set_coplanar(bool is_coplanar = true) {
        _is_coplanar = is_coplanar;
    }
    void set_intersect(bool is_intersect = true) {
        _is_intersect = is_intersect;
    }
    TriFace get_tri_face(int i) const {
        ASSERT(i == 0 || i == 1);
        ASSERT(_cpt1 != nullptr && _cpt2 != nullptr);
        return i == 0 ? (*_cpt1) : (*_cpt2);
    }
    void show() const {
        std::cout << "Intersection Result: " 
                  << (_is_intersect ? "Intersected" : "Not Intersected") ;
        if(_is_intersect){
                std::cout << ", Coplanar: " << (_is_coplanar ? "Yes" : "No") 
                << ", Intersection Segment: (" 
                << _ips << ", " << _ipe << ")";
        }
        std::cout << std::endl;
    }

};
template<class GEO1, class GEO2>
auto Intersect(const GEO1& g1, const GEO2& g2, 
               const std::string& method,
               TriFaceTag, TriFaceTag){
    std::cout << "Intersecting two TriFaces" << std::endl;
    // Bounding box check -----
    // Todo
    // Iterate through all triangles in g1 and g2
    typedef IntersectionResult_<GEO1, GEO2> Result;
    Result res(&g1, &g2);
    auto& v0 = *(g1.vertex(0));
    auto& v1 = *(g1.vertex(1));
    auto& v2 = *(g1.vertex(2));
    auto& u0 = *(g2.vertex(0));
    auto& u1 = *(g2.vertex(1));
    auto& u2 = *(g2.vertex(2));
    res._is_intersect = 
        Intersect(v0.data(), v1.data(), v2.data(),
                  u0.data(), u1.data(), u2.data(),
                  res._is_coplanar, res._ips.data(), res._ipe.data(), method);
    return res;
} 

} // namespace carpio

#endif // INTERSECTION_TS_HPP