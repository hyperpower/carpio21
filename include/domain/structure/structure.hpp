#ifndef _STRUCTURE_HPP
#define _STRUCTURE_HPP

#include "domain/domain_define.hpp"
#include "structure_define.hpp"
#include "sindex.hpp"
#include "grid/sgrid.hpp"
#include "grid/uniform.hpp"
#include "grid/nonuniform.hpp"

#include "ghost/sghost.hpp"
#include "ghost/regular.hpp"
#include "ghost/regular_subdivision.hpp"

#include "order/sorder.hpp"
#include "order/xyz.hpp"
#include "field/sfield.hpp"
#include "field/sfield_center.hpp"

#include "io/sgnuplot_actor.hpp"
// #include "io/splotly_actor.hpp"
#include "io/s_stringify.hpp"

#include "operator/slaplacian.hpp"
#include "operator/sbuild_matrix.hpp"

#include <memory>

namespace carpio{


template<St    DIM, 
         class GRID, 
         class GHOST, 
         class ORDER>
class StructureDomain_{
public:
    static const St Dim = DIM;
    typedef Vt    ValueType;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef typename Grid::Index   Index;
    typedef SFieldCenter_<Dim, ValueType, Grid, Ghost, Order> FieldCenter;
    typedef LinearPolynomial_<Vt, typename GRID::Index> Exp;
    typedef SFieldCenter_<Dim, Exp, Grid, Ghost, Order> FieldCenterExp;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef StructureDomain_<DIM, Grid, Ghost, Order> Self;

public:
    spGrid  _spgrid; 
    spGhost _spghost; 
    spOrder _sporder;

public:
    StructureDomain_(){};

    StructureDomain_(spGrid spgrid, spGhost spghost, spOrder sporder):
        _spgrid(spgrid), _spghost(spghost), _sporder(sporder){};

};

}

#endif