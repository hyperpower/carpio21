#ifndef _BOOLEAN_INTERSECTION_HPP_
#define _BOOLEAN_INTERSECTION_HPP_

#include "intersection_base.hpp"
#include "segment_segment.hpp"
#include "multi_segments.hpp"


namespace carpio{





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