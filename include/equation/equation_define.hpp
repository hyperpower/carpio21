#ifndef _EQUATION_DEFINE_HPP_
#define _EQUATION_DEFINE_HPP_

#include "type_define.hpp"

namespace carpio {

template<St DIM>
auto ArrVelocityName(){
    SHOULD_NOT_REACH;
    return std::array<std::string, 1>{"u"};
}
template<>
auto ArrVelocityName<1>(){
    return std::array<std::string, 1>{"u"};
}
template<>
auto ArrVelocityName<2>(){
    return std::array<std::string, 2>{"u","v"};
}
template<>
auto ArrVelocityName<3>(){
    return std::array<std::string, 3>{"u","v","w"};
}

std::string AxesToVelocityName(const Axes& a){
    auto vname = ArrVelocityName<3>();
    return vname[a];
}

}

#endif