#ifndef _BOOLEAN_INTERSECTION_HPP_
#define _BOOLEAN_INTERSECTION_HPP_

#include "intersection_base.hpp"


namespace carpio{





// Interface Function
template<class GEO1, class GEO2, 
        typename std::enable_if<
                   IsGeometry<GEO1>::value
                && IsGeometry<GEO2>::value 
        , bool>::type = true>
IntersectionReturn_<GEO1, GEO2> Intersect(const GEO1& g1, const GEO2& g2){
    typedef typename GEO1::Tag Tag1;
    typedef typename GEO2::Tag Tag2;
    return Intersect(g1, g2, Tag1(), Tag2());
}

}

#endif