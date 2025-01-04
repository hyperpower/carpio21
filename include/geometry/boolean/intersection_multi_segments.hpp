#ifndef _INTERSECTION_MULTI_SEGMENTS_HPP_
#define _INTERSECTION_MULTI_SEGMENTS_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "intersection_two_segments.hpp"
// #include "utility/profile.hpp"
// #include "utility/avl_tree.hpp"
#include "geometry/ben-ott/ben_ott_intersection.hpp"

namespace carpio{

template<class CONTAINER>
auto IntersectN2(const CONTAINER& con, SegmentTag){
    typedef typename CONTAINER::value_type Seg;
    typedef typename Seg::coord_value_type Cvt;
    typedef Intersection_<Seg, Seg> Inter;

    typedef IntersectionResult_<Seg, Seg> InterRes;
    typedef std::list<InterRes> ListInterRes;
    ListInterRes lres;
    for(auto iter = con.begin(); iter != con.end(); ++iter){
        auto& seg1 = *iter;
        for(auto iterin = std::next(iter); iterin != con.end(); ++iterin){
            auto& seg2 = *iterin;
            Inter inter(seg1, seg2);
            auto res = inter.execute();
            if(res.type != _SS_NO_ && res.type != _SS_INVALID_){
                lres.push_back(res);
            }
        }
    }
    return lres;
}



template<class CONTAINER, 
        typename std::enable_if<
                   (! IsGeometry<CONTAINER>::value)
                && IsContainer<CONTAINER>::value
                && IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
auto Intersect(const CONTAINER& con, const std::string& method, SegmentTag){
    typedef typename CONTAINER::value_type Seg;  
    std::string m = ToLowerCase(method);
    // if (m == "bentley_ottmann"){
    //     IntersectionBenOtt_<Seg> inter(con);
    //     return inter.execute();
    // }
    // else{
        return IntersectN2(con, SegmentTag());
    // }
}
}

#endif