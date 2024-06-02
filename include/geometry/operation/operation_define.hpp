#ifndef _GEO_OPERATION_DEFINE_
#define _GEO_OPERATION_DEFINE_
#include <algorithm>

#include "geometry/geometry_define.hpp"
#include "geometry/predicate.hpp"
#include "geometry/objects/objects.hpp"

DEFINE_HAS_MEMBER(data);

namespace carpio{

template<typename ARR,  
          typename std::enable_if<
                Has_data<ARR,typename ARR::value_type*(void)>::value,
            bool>::type = true>
double Orient(
        const ARR& v1,
        const ARR& v2,
        const ARR& v3) {
    return exact::orient2d(v1.data(), v2.data(), v3.data());
}

template<typename ARR,  
          typename std::enable_if<Has_data<ARR, typename ARR::value_type*(void)>::value, bool>::type = true>
double Orient(
        const ARR& v1,
        const ARR& v2,
        const ARR& v3,
        const ARR& v4) {
    return exact::orient3d(v1.data(), v2.data(), v3.data(), v4.data());
}


}
#endif