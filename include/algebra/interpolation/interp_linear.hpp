#ifndef _INTERP_LINEAR_HPP_
#define _INTERP_LINEAR_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <limits>

#include "type_define.hpp"
#include "algebra/algebra_define.hpp"

namespace carpio {

/// Linear interpolate.
/// Linear reference
/// u is a normalized u
template<class CVT, class VT>
inline VT InterpLinearRatio(const CVT& u, const VT& a, const VT& b) {
   return ((a * (1.0 - u)) + (b * u));
}

// linear interpolation
// https://en.wikipedia.org/wiki/Linear_interpolation
template<class CVT, class VT>
inline VT InterpLinear(const CVT& x, const CVT& x1, const CVT& x2, const VT& a, const VT& b) {
    CVT u = (x - x1) / (x2 - x1);
    return InterpLinearRatio(u, a, b);
}

}

#endif