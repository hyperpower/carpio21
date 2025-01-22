#ifndef _DOMAIN_DEFINE_HPP
#define _DOMAIN_DEFINE_HPP

#include <iostream>
#include <memory>
#include "type_define.hpp"

namespace carpio{

// cell type
enum CellType{
    _Inner_       = 1,
    _BoundaryIn_  = 2,
    _BoundaryOut_ = 3,
    _Ghost_       = 4,
    _Virtual_     = 5,
    _Cut_         = 6,
    _Leaf_        = 7,
};

struct DomainTag{};

struct StructureTag: public DomainTag{};
struct OctreeTag:    public DomainTag{};

template<St DIM>
auto ArrAxes(){
    SHOULD_NOT_REACH;
    return 0.0;
}
template<>
auto ArrAxes<1>(){
    return std::array<Axes, 1>{_X_};
}
template<>
auto ArrAxes<2>(){
    return std::array<Axes, 2>{_X_, _Y_};
}
template<>
auto ArrAxes<3>(){
    return std::array<Axes, 3>{_X_, _Y_, _Z_};
}


}

#endif