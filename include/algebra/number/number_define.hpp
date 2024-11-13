#ifndef _NUMBER_DEFINE_HPP_
#define _NUMBER_DEFINE_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <limits>

#include "type_define.hpp"
#include "algebra/algebra_define.hpp"

namespace carpio {


/**
 *  Get zero in type T.
 *
 *  \param num - a number 
 *
 *  \return T - Zero in type T
 */
template<class T>
auto Zero(const T& num){
    return T(0.0);
}
template<class T, typename = std::enable_if_t<!std::is_integral<T>::value> >
T EpsilonForComparison(const T& x, const T& y){
    auto maxXYOne = std::max( { T(1.0), std::fabs(x) , std::fabs(y) } ) ;
    return std::numeric_limits<T>::epsilon()*maxXYOne;
}
template<class T ,
         typename std::enable_if<
            std::is_integral<T>::value, bool>::type = true>
inline bool IsEqual(const T& x, const T& y){
    return x == y;
}
//compare float number
template<class T, typename = std::enable_if_t<
        !std::is_integral<T>::value && std::is_arithmetic_v<T> > >
inline bool IsEqual(const T& x, const T& y){
    auto maxXYOne = std::max( { T(1.0), std::fabs(x) , std::fabs(y) } ) ;
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon()*maxXYOne ;
}

template<class T, typename = std::enable_if_t<
        !std::is_integral<T>::value && std::is_arithmetic_v<T> > >
inline bool IsLess(const T& x, const T& y){  // x < y
    auto maxXYOne = std::max( { T(1.0), std::fabs(x) , std::fabs(y) } ) ;
    return y - x > std::numeric_limits<T>::epsilon()*maxXYOne ;
}

template<class T, typename = std::enable_if_t<
        !std::is_integral<T>::value && std::is_arithmetic_v<T> > >
inline bool IsGreater(const T& x, const T& y){  // x > y
    auto maxXYOne = std::max( { T(1.0), std::fabs(x) , std::fabs(y) } ) ;
    return x - y > std::numeric_limits<T>::epsilon()*maxXYOne ;
}

//implements ULP method
//use this when you are only concerned about floating point precision issue
//for example, if you want to see if a is 1.0 by checking if its within
//10 closest representable floating point numbers around 1.0.
template<typename T>
inline bool IsWithinPrecisionInterval(T a, T b, unsigned int interval_size = 1)
{
    T min_a = a - (a - std::nextafter(a, std::numeric_limits<T>::lowest())) * interval_size;
    T max_a = a + (std::nextafter(a, std::numeric_limits<T>::max()) - a) * interval_size;

    return min_a <= b && max_a >= b;
}

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