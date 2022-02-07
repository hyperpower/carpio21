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





}

#endif