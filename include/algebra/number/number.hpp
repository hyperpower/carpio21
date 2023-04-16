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

template<class T,
         typename = std::enable_if_t<std::is_floating_point<T>::value> >
int countSigDigits(T num, T delta = 1e-5) {
    int  digits = 0;
    int  digit_max = std::numeric_limits<T>::digits10;

    // Loop through values from 1 to 10
    for (int i = 1; i <= digit_max; i++) {
        auto m = std::pow(10, i);
        if (std::abs(num * m - std::floor(num * m)) < delta * m ){
            digits = i;
            break;
        }
    }
    return digits;
}

};


#endif