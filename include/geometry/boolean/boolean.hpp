#ifndef _BOOLEAN_HPP_
#define _BOOLEAN_HPP_

#include "geometry/objects/objects.hpp"
#include "intersection_two_segments.hpp"
#include "geometry/ben-ott/ben_ott_intersection.hpp"
#include "intersection_multi_segments.hpp"
#include "intersection_ts.hpp"
// #include "multi_segments.hpp"

#include "line_box.hpp"


namespace carpio{

// typedef IntersectionBenOtt_<Segment_<double, 2> > Intera;
// Interface Function
template<class GEO1, class GEO2, 
        typename std::enable_if<
                   IsGeometry<GEO1>::value
                && IsGeometry<GEO2>::value 
        , bool>::type = true>
auto Intersect(const GEO1& g1, const GEO2& g2){
    typedef typename GEO1::Tag Tag1;
    typedef typename GEO2::Tag Tag2;
    return Intersect(g1, g2, Tag1(), Tag2());
}

template<class GEO1, class GEO2, 
        typename std::enable_if<
                   IsGeometry<GEO1>::value
                && IsGeometry<GEO2>::value 
        , bool>::type = true>
bool IsIntersect(const GEO1& g1, const GEO2& g2){
    typedef typename GEO1::Tag Tag1;
    typedef typename GEO2::Tag Tag2;
    return IsIntersect(g1, g2, Tag1(), Tag2());
}


template<class CONTAINER,
        typename std::enable_if<
                   (! IsGeometry<CONTAINER>::value)
                && IsContainer<CONTAINER>::value
                && IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
auto Intersect(const CONTAINER& con, const std::string& method = ""){
    typedef typename CONTAINER::value_type GEO;
    typedef typename GEO::Tag Tag;
    return Intersect(con, method, Tag());
}


}

#endif