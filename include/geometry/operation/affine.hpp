#ifndef _GEO_AFFINE_
#define _GEO_AFFINE_

#include <algorithm>
#include "geometry\geometry_define.hpp"
#include <array>
#include <iterator>
#include <type_traits>

namespace carpio{
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag){
    // expand container, container support std operation
    // container has equal size of point
    typedef typename DimTagTraits_<GEO::Dim>::Type Dim;
    _Translate(point, arr, PointTag(), Dim()); 
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag, 
                Dim1Tag){
    point[0] += *(arr.cbegin());
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag, 
                Dim2Tag){
    auto iend  = arr.cend();
    auto iter  = arr.cbegin();
    auto x     = *(iter);
    std::advance(iter, 1);
    auto y     = (iter == iend) ? 0.0 : (*iter);
    point[0] += x;
    point[1] += y;
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag, 
                Dim3Tag){
    auto iend  = arr.cend();
    auto iter  = arr.cbegin();
    auto x     = *(iter);
    std::advance(iter, 1);
    auto y     = (iter == iend) ? 0.0 : (*iter);
    std::advance(iter, 1);
    auto z     = (iter == iend) ? 0.0 : (*iter);
    point[0] += x;
    point[1] += y;
    point[2] += z;
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       seg, 
                const CONTAINER& arr,
                SegmentTag){
    // expand container, container support std operation
    // container has equal size of point
    if constexpr (GEO::Dim == 1){
        _Translate(seg[0], *(arr.cbegin()), PointTag());
        _Translate(seg[1], *(arr.cbegin()), PointTag());
    }else if constexpr (GEO::Dim == 2){
        auto iter0 = arr.cbegin();
        auto iter1 = std::next(iter0);
        _Translate(seg[0], *(iter0), *(iter1), PointTag())
        _Translate(seg[1], *(iter0), *(iter1), PointTag())
    }else if constexpr (GEO::Dim == 3){
        auto iter0 = arr.cbegin();
        auto iter1 = std::next(iter0);
        auto iter2 = std::next(iter1);
        _Translate(seg[0], *(iter0), *(iter1), *(iter2), PointTag());
        _Translate(seg[1], *(iter0), *(iter1), *(iter2), PointTag());
    }
}
template<class GEO, class VT, 
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true>
void _Translate(      GEO& point, 
                const VT&  x,
                PointTag){
    point[0] += x;
}
template<class GEO, class VT, 
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true>
void _Translate(      GEO& point, 
                const VT&  x,
                const VT&  y,
                PointTag){
    point[0] += x;
    point[1] += y;
}
template<class GEO, class VT, 
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true>
void _Translate(      GEO& point, 
                const VT&  x,
                const VT&  y,
                const VT&  z,
                PointTag){
    point[0] += x;
    point[1] += y;
    point[3] += z;
}
template<class GEO, class TYPE> // TYPE can be a num or num container 
void Translate(GEO& geo, const TYPE& p){
    typedef typename GEO::Tag Tag;
    _Translate(geo, p, Tag());
}
template<class GEO, class TYPE> // TYPE is a num
void Translate(GEO& geo, const TYPE& x, const TYPE& y){
    typedef typename GEO::Tag Tag;
    std::array<TYPE, 2> arr{x, y};
    _Translate(geo, arr, Tag());
}
template<class GEO, class TYPE> // TYPE is a num
void Translate(GEO& geo, const TYPE& x, const TYPE& y, const TYPE& z){
    typedef typename GEO::Tag Tag;
    std::array<TYPE, 3> arr{x, y, z};
    _Translate(geo, arr, Tag());
}
// template<class GEO, class VT>
// void _Translate(const GEO& geo, const VT& x, GeometryTag){}

}

#endif