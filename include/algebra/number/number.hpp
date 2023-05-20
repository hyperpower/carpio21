#ifndef _NUMBER_HPP_
#define _NUMBER_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>

#include "number_define.hpp"
#include "rational.hpp"
#include "big_int.hpp"

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

};


#endif