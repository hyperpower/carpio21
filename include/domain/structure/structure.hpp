#ifndef _STRUCTURE_HPP
#define _STRUCTURE_HPP

#include "domain/domain_define.hpp"
#include "structure_define.hpp"
#include "sindex.hpp"
#include "grid/sgrid.hpp"
#include "grid/uniform.hpp"
#include "ghost/sghost.hpp"
#include "order/sorder.hpp"
#include "field/sfield.hpp"

#include "operator/slaplacian.hpp"

namespace carpio{


template<St Dim>
class StructureDomain_{
public:
    static const St Dim = DIM;

    typedef SIndex_<Dim> Index;

};



}

#endif