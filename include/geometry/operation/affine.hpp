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
                const CONTAINER& container,
                PointTag){
    // expand container, container support std operation
    // container has equal size of point
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Translate(point, container, PointTag(), DimTag()); 
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag, 
                Dim1Tag){
    point[0] += arr[0];
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag, 
                Dim2Tag){
    point[0] += arr[0];
    point[1] += arr[1];
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       point, 
                const CONTAINER& arr,
                PointTag, 
                Dim3Tag){
    point[0] += arr[0];
    point[1] += arr[1];
    point[2] += arr[2];
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _Translate(      GEO&       seg, 
                const CONTAINER& container,
                SegmentTag){
    // expand container, container support std operation
    // container has equal size of point
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Translate(seg[0], container, PointTag(), DimTag()); 
    _Translate(seg[1], container, PointTag(), DimTag()); 
}

template<class A, class X, class B>
void AffineAxb(const A& a, X& x, const B& b, Dim1Tag){
    x[0] =  x[0] + b[0]; 
}
template<class A, class X, class B>
void AffineAxb(const A& a, X& x, const B& b, Dim2Tag){
    // A is matrix 2x2
    // a = [ ( a[0], a[1] ),  
    //       ( a[2], a[3] ) ]
    // b = [ b[0],
    //       b[1] ]
    x[0] =  a[0] * x[0] + a[1] * x[1] + b[0];
    x[1] =  a[2] * x[0] + a[3] * x[1] + b[1];
}
template<class A, class X, class B>
void AffineAxb(const A& a, X& x, const B& b, Dim3Tag){
    // A is matrix 2x2
    // a = [ ( a[0], a[1], a[2] ),  
    //       ( a[3], a[4], a[5] ), 
    //       ( a[6], a[7], a[8] )  ]
    // b = [ b[0],
    //       b[1],
    //       b[2] ]
    x[0] =  a[0] * x[0] + a[1] * x[1] + a[2] * x[2] + b[0];
    x[1] =  a[3] * x[0] + a[4] * x[1] + a[5] * x[2] + b[1];
    x[2] =  a[6] * x[0] + a[7] * x[1] + a[8] * x[2] + b[2];
}

// arr has same length as the Dim  
template<class CONTAINER, class ARR,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _RegularContainerByDim(const CONTAINER& container, ARR& arr, int default_value = 0){
    // ARR support random access
    auto iter  = container.cbegin();
    auto size_container = container.size();
    auto size_arr       = arr.size();
    for(auto i = 0 ; i < size_arr; ++i){
        arr[i] = i < size_container ? (*iter) : default_value;
        std::advance(iter, 1);
    }
}
template<class CONTAINER>
void _NegativeContainer(CONTAINER& container){
    for(auto& a : container){
        a = -a;
    }
}

template<class GEO, class CONTAINER> 
void Translate(GEO& geo, const CONTAINER& container){
    typedef typename GEO::Tag Tag;
    std::array<typename GEO::value_type, GEO::Dim> arr;
    _RegularContainerByDim(container, arr);
    _Translate(geo, arr, Tag());
}
// ==================================================
// Scale
// ==================================================
template<class GEO, class CONTAINER>
void _Scale(      GEO&       point, 
            const CONTAINER& container,
            PointTag){
    // expand container, container support std operation
    // container has equal size of point
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Scale(point, container, PointTag(), DimTag()); 
}
template<class GEO, class CONTAINER>
void _Scale(      GEO&       point, 
            const CONTAINER& arr,
            PointTag, 
            Dim1Tag){
    point[0] *= arr[0];
}
template<class GEO, class CONTAINER>
void _Scale(      GEO&       point, 
            const CONTAINER& arr,
            PointTag, 
            Dim2Tag){
    point[0] *= arr[0];
    point[1] *= arr[1];
}
template<class GEO, class CONTAINER>
void _Scale(      GEO&       point, 
            const CONTAINER& arr,
            PointTag, 
            Dim3Tag){
    point[0] *= arr[0];
    point[1] *= arr[1];
    point[2] *= arr[2];
}
template<class GEO, class CONTAINER, class POINT,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true> 
void Scale(GEO& geo, const CONTAINER& container, const POINT& about){
    typedef typename GEO::Tag Tag;
    std::array<typename GEO::value_type, GEO::Dim> arr_n;
    _RegularContainerByDim(about, arr_n);
    _NegativeContainer(arr_n);
    _Translate(geo, arr_n, Tag()); // make "about" become origin
    Scale(geo, container);
    _NegativeContainer(arr_n);
    _Translate(geo, arr_n, Tag()); // translate back 
}
template<class GEO, class CONTAINER,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true> 
void Scale(GEO& geo, const CONTAINER& container){
    typedef typename GEO::Tag Tag;
    std::array<typename GEO::value_type, GEO::Dim> arr;
    _RegularContainerByDim(container, arr, 1);
    _Scale(geo, arr, Tag());
}
template<class GEO, class CONTAINER> 
void Rotate(GEO& geo, const CONTAINER& container){
    typedef typename GEO::Tag Tag;
    // _Scale(geo, p, Tag());
}
template<class GEO, class CONTAINER, class POINT> 
void Rotate(GEO& geo, const CONTAINER& container, const POINT& point){
    typedef typename GEO::Tag Tag;
    // _Scale(geo, p, Tag());
}
// template<class GEO, class VT>
// void _Translate(const GEO& geo, const VT& x, GeometryTag){}

}

#endif