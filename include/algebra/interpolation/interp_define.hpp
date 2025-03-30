#ifndef _INTERP_DEFINE_HPP_
#define _INTERP_DEFINE_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <limits>

#include "type_define.hpp"

namespace carpio {

template<class ARRX, class ARRV>
inline auto ArraySize(const ARRX& arrx, const ARRV& arrv)  {
    auto nx = arrx.size();
    auto nv = arrv.size();

    return std::min(nx, nv);
}


}

#endif