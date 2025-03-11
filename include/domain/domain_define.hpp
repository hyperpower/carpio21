#ifndef _DOMAIN_DEFINE_HPP
#define _DOMAIN_DEFINE_HPP

#include <iostream>
#include <memory>
#include <functional>
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

struct LocationTag{};

struct CenterTag: public LocationTag{};
struct FaceTag:   public LocationTag{};
struct VertexTag: public LocationTag{};




template<St DIM>
auto ArrAxes(){
    SHOULD_NOT_REACH;
    return std::array<Axes, 1>{_X_};
}
template<>
constexpr auto ArrAxes<1>(){
    return std::array<Axes, 1>{_X_};
}
template<>
constexpr auto ArrAxes<2>(){
    return std::array<Axes, 2>{_X_, _Y_};
}
template<>
constexpr auto ArrAxes<3>(){
    return std::array<Axes, 3>{_X_, _Y_, _Z_};
}
template<St DIM>
auto ArrAxes(const St& start){
    SHOULD_NOT_REACH;
    return std::array<Axes, 1>{ToAxes(start)};
}
template<>
constexpr auto ArrAxes<1>(const St& start){
    return std::array<Axes, 1>{ToAxes(start)};
}
template<>
constexpr auto ArrAxes<2>(const St& start){
    return std::array<Axes, 2>{ToAxes(start), ToAxes((start + 1) % 2)};
}
template<>
constexpr auto ArrAxes<3>(const St& start){
    return std::array<Axes, 3>{ToAxes(start), ToAxes((start + 1) % 3), ToAxes((start + 2) % 3)};
}

typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;

}

#endif