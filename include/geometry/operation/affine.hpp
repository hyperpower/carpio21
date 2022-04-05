#ifndef _GEO_AFFINE_
#define _GEO_AFFINE_

#include <algorithm>
#include "geometry\geometry_define.hpp"
#include <array>
#include <iterator>
#include <type_traits>

namespace carpio{

template<class GEO, class TRANSVEC>
void _Translate(GEO& point, const TRANSVEC& vec, PointTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Translate(point, vec, PointTag(), DimTag()); 
}

template<class GEO, class VEC>
void _Translate(GEO&point, const VEC& arr, PointTag, 
                Dim1Tag){
    point[0] += arr[0];
}
template<class GEO, class VEC>
void _Translate(GEO& point, const VEC& arr, PointTag, 
                Dim2Tag){
    point[0] += arr[0];
    point[1] += arr[1];
}
template<class GEO, class VEC>
void _Translate(GEO&point, const VEC& arr,  PointTag, 
                Dim3Tag){
    point[0] += arr[0];
    point[1] += arr[1];
    point[2] += arr[2];
}
template<class GEO, class VEC>
void _Translate(GEO& seg, const VEC& arr, SegmentTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Translate(seg[0], arr, PointTag(), DimTag()); 
    _Translate(seg[1], arr, PointTag(), DimTag()); 
}
template<class GEO, class VEC>
void _Translate(      GEO&       box, const VEC& arr,
                BoxTag){ 
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Translate(box.min(), arr, PointTag(), DimTag()); 
    _Translate(box.max(), arr, PointTag(), DimTag()); 
}
template<class GEO, class VEC>
void _Translate(      GEO&       pointchain, const VEC& arr,
                PointChainTag){ 
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    for(auto& p : pointchain ){
        _Translate(p, arr, PointTag(), DimTag()); 
    }
}
template<class GEO, class VEC>
void _Translate(      GEO&       line, const VEC& arr,
                LineTag){ 
    line.alpha() = line.alpha() + line.a() * arr[0] + line.b() * arr[1]; 
}
// arr has same length as the Dim  
template<class CONTAINER, class ARR,
         typename std::enable_if<std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void _RegularVectorByDim(const CONTAINER& container, ARR& arr, double default_value = 0){
    // expand container, container support std operation
    // container has equal size of point
    // ARR support random access 
    auto iter  = container.cbegin();
    auto size_container = container.size();
    auto size_arr       = arr.size();
    for(auto i = 0 ; i < size_arr; ++i){
        if (i < size_container){
            arr[i] = (*iter);
            std::advance(iter, 1);
        }else{
            arr[i] = default_value;
        }
    }
}
template<class CONTAINER>
void _NegativeContainer(CONTAINER& container){
    for(auto& a : container){
        a = -a;
    }
}
template<class GEO, class NUM,
        typename std::enable_if<
            std::integral_constant< bool, 
                   std::is_arithmetic<NUM>::value 
                && IsGeometry<GEO>::value
            >::value, bool>::type = true> 
void Translate(GEO& geo, const NUM& num){
    typedef typename GEO::Tag Tag;
    std::array<NUM, GEO::Dim> arr;
    for(auto& v : arr){
        v = num;
    }
    _Translate(geo, arr, Tag());
}
template<class GEO, class VEC,
         typename std::enable_if<
            std::integral_constant< bool, 
                   IsContainer<VEC>::value 
                && IsGeometry<GEO>::value
            >::value, bool>::type = true> 
void Translate(GEO& geo, const VEC& container){
    typedef typename GEO::Tag Tag;
    std::array<typename VEC::value_type, GEO::Dim> arr;
    _RegularVectorByDim(container, arr);
    _Translate(geo, arr, Tag());
}
template<class CONTAINER, class VEC, 
        typename std::enable_if<
            std::integral_constant< bool, 
                   IsContainer<CONTAINER>::value 
                && IsGeometry<typename CONTAINER::value_type>::value
            >::value, bool>::type = true> // Line: ax + by = alpha
void Translate(CONTAINER& con_geo, const VEC& vec){
    for (auto& geo : con_geo){
        Translate(geo, vec);
    }
}
// ==================================================
// Scale
// ==================================================
template<class GEO, class VEC>
void _Scale(GEO& point, const VEC& container, PointTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Scale(point, container, PointTag(), DimTag()); 
}
template<class GEO, class VEC>
void _Scale(GEO& point, const VEC& arr, PointTag, Dim1Tag){
    point[0] *= arr[0];
}
template<class GEO, class VEC>
void _Scale(GEO& point, const VEC& arr,PointTag, Dim2Tag){
    point[0] *= arr[0];
    point[1] *= arr[1];
}
template<class GEO, class VEC>
void _Scale(GEO& point, const VEC& arr,PointTag, Dim3Tag){
    point[0] *= arr[0];
    point[1] *= arr[1];
    point[2] *= arr[2];
}
template<class GEO, class VEC>
void _Scale(GEO& box, const VEC& arr, BoxTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Scale(box.min(), arr, PointTag(), DimTag()); 
    _Scale(box.max(), arr, PointTag(), DimTag()); 
}
template<class GEO, class VEC>
void _Scale(GEO& line, const VEC& arr, LineTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    line.a() = line.a() / arr[0];
    line.b() = line.b() / arr[1];
}
template<class GEO, class VEC>
void _Scale(GEO& pointchain, const VEC& arr, PointChainTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    for(auto& p : pointchain ){
        _Scale(p, arr, PointTag(), DimTag()); 
    }
}
template<class GEO, class VEC, class POINT,
         typename std::enable_if<IsGeometry<GEO>::value, bool>::type = true>
void Scale(GEO& geo, const VEC& vec, const POINT& about){
    typedef typename GEO::Tag Tag;
    std::array<typename VEC::value_type, GEO::Dim> arr_n;
    _RegularVectorByDim(about, arr_n);
    _NegativeContainer(arr_n);
    _Translate(geo, arr_n, Tag()); // make "about" become origin
    Scale(geo, vec);
    _NegativeContainer(arr_n);
    _Translate(geo, arr_n, Tag()); // translate back 
}
template<class GEO, class VEC,
         typename std::enable_if<IsGeometry<GEO>::value, bool>::type = true>
void Scale(GEO& geo, const VEC& vec){
    typedef typename GEO::Tag Tag;
    std::array<typename VEC::value_type, GEO::Dim> arr;
    _RegularVectorByDim(vec, arr, 1);
    _Scale(geo, arr, Tag());
}
template<class CONTAINER, class VEC, class POINT,
        typename std::enable_if<
            std::integral_constant< bool, 
                   IsContainer<CONTAINER>::value 
                && IsGeometry<typename CONTAINER::value_type>::value
            >::value, bool>::type = true> // Line: ax + by = alpha
void Scale(CONTAINER& con_geo, const VEC& vec, const POINT& about){
    for (auto& geo : con_geo){
        Scale(geo, vec, about);
    }
}
template<class CONTAINER, class VEC, 
        typename std::enable_if<
            std::integral_constant< bool, 
                   IsContainer<CONTAINER>::value 
                && IsGeometry<typename CONTAINER::value_type>::value
            >::value, bool>::type = true> // Line: ax + by = alpha
void Scale(CONTAINER& con_geo, const VEC& vec){
    for (auto& geo : con_geo){
        Scale(geo, vec);
    }
}
// ==================================================
//   Rotate 
// ==================================================
template<class GEO, class VT,
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true> 
void _Rotate(GEO& point, const VT& theta, const int& axe, PointTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Rotate(point, theta, axe, PointTag(), DimTag()); 
}

template<class GEO, class VT>
void _Rotate(GEO& point, const VT& theta, const int& axe, PointTag, Dim2Tag){
    point[0] = point[0] * std::cos(theta) - point[1] * std::sin(theta);
    point[1] = point[0] * std::sin(theta) + point[1] * std::cos(theta);
}
template<class GEO, class VT>
void _Rotate(GEO& point, const VT& theta, const int& axe, PointTag, Dim3Tag){
    if (axe == _X_){
        _Rotate(point, theta, XTag(), PointTag, Dim3Tag);
    }else if (axe == _Y_){
        _Rotate(point, theta, YTag(), PointTag, Dim3Tag);
    }else{
        _Rotate(point, theta, ZTag(), PointTag, Dim3Tag);
    }
}
template<class GEO, class VT>
void _Rotate(GEO& point, const VT& theta, XTag, PointTag, Dim3Tag){
//  point[0] = point[0];
    point[1] =   point[1] * std::cos(theta) + point[2] * std::sin(theta);
    point[2] = - point[1] * std::sin(theta) + point[2] * std::cos(theta);
}
template<class GEO, class VT>
void _Rotate(GEO& point, const VT& theta, YTag, PointTag, Dim3Tag){
    point[0] = point[0] * std::cos(theta) - point[2] * std::sin(theta);
//  point[1] = point[1];
    point[2] = point[0] * std::sin(theta) + point[2] * std::cos(theta);
}
template<class GEO, class VT>
void _Rotate(GEO& point, const VT& theta, ZTag, PointTag, Dim3Tag){
    point[0] = point[0] * std::cos(theta) - point[1] * std::sin(theta);
    point[1] = point[0] * std::sin(theta) + point[1] * std::cos(theta);
//  point[2] = point[2];
}

template<class GEO, class VT,
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true> 
void _Rotate(GEO& pointchain, const VT& theta, const int& axe, PointChainTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    for(auto& p : pointchain ){
        _Rotate(p, theta, axe, PointTag(), DimTag()); 
    }
}
template<class GEO, class VT>
void Rotate(GEO& geo, const VT& theta, const int& axe = _Z_){
    typedef typename GEO::Tag Tag;
    _Rotate(geo, theta, axe, Tag());
}
template<class GEO, class VT, class POINT> 
void Rotate(GEO& geo, const VT& theta,  const POINT& about, const int& axe = _Z_){
    typedef typename GEO::Tag Tag;
    std::array<typename POINT::value_type, GEO::Dim> arr;
    _RegularVectorByDim(about, arr, 0.0);
    std::array<typename POINT::value_type, GEO::Dim> neg_arr(arr);
    _NegativeContainer(neg_arr);
    _Translate(geo, neg_arr, Tag()); // make "about" become origin
    _Rotate(geo, theta, axe, Tag());
    _Translate(geo, arr, Tag());     // translate back 
}
// ==================================================
//   Shear 
// ==================================================
template<class GEO, class VT,
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true> 
void _Shear(GEO& point, const VT& theta, const int& axe, PointTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    typedef typename AxesTagTraits_<axe>::Type AxesTag;
    _Shear(point, theta, AxesTag(), PointTag(), DimTag()); 
}

template<class GEO, class VT>
void _Shear(GEO& point, const VT& theta, const int& axe, PointTag, Dim2Tag){
    if (axe == _X_){
        point[0] = point[0] + point[1] * std::tan(theta);
//      point[1] = point[1];
    }else if( axe == _Y_){
//      point[0] = point[0];
        point[1] = point[0] * std::tan(theta) + point[1];
    }
}

template<class GEO, class VT,
         typename std::enable_if<std::is_arithmetic<VT>::value, bool>::type = true> 
void _Shear(GEO& pointchain, const VT& theta, const int& axe, PointChainTag){
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    for(auto& p : pointchain ){
        _Shear(p, theta, axe, PointTag(), DimTag()); 
    }
}
template<class GEO, class VT>
void Shear(GEO& geo, const VT& theta, const int& axe = _X_){
    typedef typename GEO::Tag Tag;
    _Shear(geo, theta, axe, Tag());
}
template<class GEO, class VT, class POINT,
         typename std::enable_if<std::is_arithmetic<typename POINT::value_type>::value, bool>::type = true> 
void Shear(GEO& geo, const VT& theta, const POINT& about, const int& axe = _X_){
    typedef typename GEO::Tag Tag;
    std::array<typename POINT::value_type, GEO::Dim> arr;
    _RegularVectorByDim(about, arr, 0.0);
    std::array<typename POINT::value_type, GEO::Dim> neg_arr(arr);
    _NegativeContainer(neg_arr);
    _Translate(geo, neg_arr, Tag()); // make "about" become origin
    _Shear(geo, theta, axe, Tag());
    _Translate(geo, arr, Tag());     // translate back 
}
// ========================
// Reflect about axe
// ========================
template<class GEO> 
void _Reflect(GEO& point, const Axes& axe,  PointTag){
    if (axe == _X_){
        point[1] = -point[1];
    }else if (axe ==_Y_){
        point[0] = -point[0];
    }
}
template<class GEO> 
void _Reflect(GEO& point, const Axes& axe, const double& v, PointTag){
    if (axe == _X_){
        point[0] = -(point[0] - v);
    }else if (axe == _Y_){
        point[1] = -(point[1] - v);
    }
}
template<class GEO> 
void _Reflect(GEO& point, PointTag){
    point = -point;
}
template<class GEO> 
void _Reflect(GEO& pointchain, const Axes& axe, PointChainTag){
    for(auto& p : pointchain ){
        _Reflect(p, axe, PointTag());
    }
}
template<class GEO> 
void _Reflect(GEO& pointchain, PointChainTag){
    for(auto& p : pointchain ){
        _Reflect(p, PointTag());
    }
}
// template<class GEO>
// void _Reflect(GEO& geo, const Axes& axe){
//     _Reflect(geo, axe, Tag());
// }
template<class GEO>
void Reflect(GEO& geo, const Axes& axe){
    typedef typename GEO::Tag Tag;
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    assert(GEO::Dim == 2);
    _Reflect(geo, axe, Tag());
}
template<class GEO>  // X = vt or Y = vt
void Reflect(GEO& geo, const Axes& axe, const double& vt){
    typedef typename GEO::Tag Tag;
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    assert(GEO::Dim == 2);
    _Reflect(geo, axe, vt, Tag());
}
template<class GEO>
void Reflect(GEO& geo){  // Reflect about original points
    typedef typename GEO::Tag Tag;
    _Reflect(geo, Tag());
}
template<class GEO, class POINT,
         typename std::enable_if<std::is_arithmetic<typename POINT::value_type>::value, bool>::type = true>
void Reflect(GEO& geo, const POINT& about){
    typedef typename GEO::Tag Tag;
    std::array<typename POINT::value_type, GEO::Dim> arr;
    _RegularVectorByDim(about, arr, 0.0);
    std::array<typename POINT::value_type, GEO::Dim> neg_arr(arr);
    _NegativeContainer(neg_arr);
    _Translate(geo, neg_arr, Tag()); // make "about" become origin
    _Reflect(geo, Tag());
    _Translate(geo, arr, Tag());     // translate back 
}
template<class GEO> // Line: ax + by = alpha
void _Reflect(GEO& point, double a, double b, double alpha, PointTag, Dim2Tag){
    double c = -alpha;
    double spab = a * a + b * b;
    double smab = a * a - b * b;
    auto po = point;
    point[0] = (       - smab * po[0] - 2.0 * a * b * po[1]  - 2.0 * a * c)/spab;  
    point[1] = (- 2.0 * a * b * po[0]        + smab * po[1]  - 2.0 * b * c)/spab;  
}
template<class GEO> // Line: ax + by = alpha
void _Reflect(GEO& pointchain, double a, double b, double alpha, PointChainTag, Dim2Tag){
    for(auto& p : pointchain ){
        _Reflect(p, a, b, alpha, PointTag(), Dim2Tag());
    }
}
template<class GEO, 
         typename std::enable_if<IsGeometry<GEO>::value, bool>::type = true> // Line: ax + by = alpha
void Reflect(GEO& geo, double a, double b, double alpha){
    typedef typename GEO::Tag Tag;
    typedef typename DimTagTraits_<GEO::Dim>::Type DimTag;
    _Reflect(geo, a, b, alpha, Tag(), DimTag());
}
template<class CONTAINER,
        typename std::enable_if<
            std::integral_constant< bool, 
                   IsContainer<CONTAINER>::value 
                && IsGeometry<typename CONTAINER::value_type>::value
            >::value, bool>::type = true> // Line: ax + by = alpha
void Reflect(CONTAINER& con_geo, double a, double b, double alpha){
    for (auto& geo : con_geo){
        Reflect(geo, a, b, alpha);
    }
}
}

#endif