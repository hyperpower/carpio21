#ifndef _GEO_AFFINE_
#define _GEO_AFFINE_

#include <algorithm>
#include "geometry\geometry_define.hpp"

namespace carpio{
template<class GEO, class VT>
void _Translate(GEO& geo, const VT& x, PointTag){
    std::cout << "VT" <<std::endl;
}
template<class GEO, class VT>
void _Translate(GEO& geo, const typename VT::value_type& x, PointTag){
    std::cout << "value type" <<std::endl;
}
template<class GEO, class TYPE> // TYPE can be a num or num container 
void Translate(GEO& geo, const TYPE& p){
    typedef typename GEO::Tag Tag;
    _Translate(geo, p, Tag());
}

// template<class GEO, class VT>
// void _Translate(const GEO& geo, const VT& x, GeometryTag){}

}

#endif