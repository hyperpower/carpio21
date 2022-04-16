#ifndef _GEO_OPERATION_DEFINE_
#define _GEO_OPERATION_DEFINE_
#include <algorithm>

#include "geometry/geometry_define.hpp"
#include "geometry/predicate.hpp"
#include "geometry/objects/objects.hpp"


namespace carpio{

template<typename ARR,  
          typename std::enable_if<HasData<ARR>::value, bool>::type = true>
double Orient(
        const ARR& v1,
        const ARR& v2,
        const ARR& v3) {
    return exact::orient2d(v1.data(), v2.data(), v3.data());
}

template<typename ARR,  
          typename std::enable_if<HasData<ARR>::value, bool>::type = true>
double Orient(
        const ARR& v1,
        const ARR& v2,
        const ARR& v3,
        const ARR& v4) {
    return exact::orient3d(v1.data(), v2.data(), v3.data(), v4.data());
}
template<typename ARR,  
          typename std::enable_if<
                IsContainer<ARR>::value
             && std::arithmatic<typename ARR::value>::value, bool>::type = true>
void Normailize(ARR& arr) {
    typename ARR::value n = 0;
    for (auto& v : arr) {
        n += double(v * v);
    }
    n = std::sqrt(n);
    for (auto& v : arr) {
        if (n != 0) {
            v /= n;
        } else {
            v = 0;
        }
    }
}
#endif