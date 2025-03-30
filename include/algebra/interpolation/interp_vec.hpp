#ifndef _INTERP_VEC_HPP_
#define _INTERP_VEC_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <limits>

#include "type_define.hpp"
#include "algebra/algebra_define.hpp"

namespace carpio {
// Method
//  'linear' 
//  'nearest'
//  'next'
//  'previous'
//  'cubic'
//  'pchip'
//  'makima'
//  'spline'

template<class ARRX, class ARRV>
inline ARRV InterpVec(const ARRX& arrx, const ARRV& arrv, const ARRX& arrxq)  {
    
}

template<class ARRX, class ARRV>
inline ARRV InterpVec(const ARRX& arrx, const ARRV& arrv, const ARRX& arrxq, const std::string& method) {

}
template<class ARRX, class ARRV>
inline ARRV InterpVec(const ARRX& arrx, const ARRV& arrv, const ARRX& arrxq, const std::string& method, const bool is_extrapolation) {

}
}

#endif