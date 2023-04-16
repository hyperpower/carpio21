#ifndef _NUMBER_DEFINE_HPP_
#define _NUMBER_DEFINE_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <limits>

#include <type_define.hpp>

namespace carpio {



// true = overflow
// false = not overflow
template<class INT>
bool CheckAddOverFlow(const INT& a, const INT& b){
    auto max = std::numeric_limits<INT>::max();
    if(b >=0 && (a > max - b)){
        return true;
    }
    auto min = std::numeric_limits<INT>::min();
    if(b <= 0 && (a < min -b) ){
        return true;
    }
    return false;
}
template<class INT>
bool CheckSubOverFlow(const INT& a, const INT& b){
    auto max = std::numeric_limits<INT>::max();
    if(b < 0 && (a > max + b)){
        return true;
    }
    auto min = std::numeric_limits<INT>::min();
    if(b > 0 && (a < min + b) ){
        return true;
    }
    return false;
}
template<class INT>
bool CheckMultiOverFlow(const INT& a, const INT& b){
    auto min = std::numeric_limits<INT>::min();
    if (a <= -1 && b == min )  return true;
    if (b <= -1 && a == min )  return true;
    if (a > 0 && b < 0 && (b < min / a)) return true;
    if (a < 0 && b > 0 && (a < min / b)) return true;
    auto max = std::numeric_limits<INT>::max();
    if (a > 0 && b > 0 && (a > max / b)) return true;
    if (a < 0 && b < 0 && (-a > max / -b)) return true;
    return false;
}


};


#endif