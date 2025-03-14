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
#include "field/sfield_vertex.hpp"
#include "field/svector_center.hpp"
#include "field/svector_face.hpp"

#include "io/sgnuplot_actor.hpp"
// #include "io/splotly_actor.hpp"
#include "io/s_stringify.hpp"

// #include "operator/slaplacian.hpp"
#include "operator/slaplacian_integral.hpp"
#include "operator/slaplacian_differential.hpp"
#include "operator/slaplacian_HOC4.hpp"
#include "operator/sbuild_matrix.hpp"
#include "operator/sinterpolate.hpp"
#include "operator/su_dot_nabla.hpp"
#include "operator/snorm.hpp"

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
    typedef StructureDomain_<Dim, Grid, Ghost, Order> Self;
    typedef typename Order::LocationTag LocationTag;
    typedef typename Grid::Index   Index;
    typedef SFieldCenter_<Dim, ValueType, Grid, Ghost, Order>   FieldCenter;
    typedef SFieldFace_<Dim, ValueType, Grid, Ghost, Order>     FieldFace;
    typedef SFieldVertex_<Dim, ValueType, Grid, Ghost, Order>   FieldVertex;
    typedef SVectorCenter_<Dim, ValueType, Grid, Ghost, Order>  VectorCenter;
    typedef SVectorFace_<Dim, ValueType, Grid, Ghost, Order>    VectorFace;
    typedef LinearPolynomial_<Vt, typename GRID::Index> Exp;
    typedef Exp    ExpType;
    typedef SFieldCenter_<Dim, Exp, Grid, Ghost, Order> FieldCenterExp;
    typedef SFieldFace_<Dim, Exp, Grid, Ghost, Order>   FieldFaceExp;
    typedef SFieldVertex_<Dim, Exp, Grid, Ghost, Order> FieldVertexExp;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef std::shared_ptr<FieldCenter>    spFieldCenter;
    typedef std::shared_ptr<FieldVertex>    spFieldVertex;
    typedef std::shared_ptr<VectorCenter>   spVectorCenter;
    typedef std::shared_ptr<FieldCenterExp> spFieldCenterExp;
    typedef std::shared_ptr<FieldVertexExp> spFieldVertexExp;
    typedef std::shared_ptr<FieldFace>      spFieldFace;
    typedef std::shared_ptr<VectorFace>     spVectorFace;
    typedef std::shared_ptr<FieldFaceExp>   spFieldFaceExp;
    
    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;
    typedef std::function<ValueType(const Index&)>   FunIndex_Value;
    
public:
    StructureDomain_(){};

    static spFieldCenter NewspFieldCenterZero(
        spGrid spgrid, spGhost spghost, spOrder sporder
    ){
        return spFieldCenter(new FieldCenter(spgrid, spghost, sporder));
    }
    static spFieldCenterExp NewspFieldCenterExpZero(
        spGrid spgrid, spGhost spghost, spOrder sporder
    ){
        return spFieldCenterExp(new FieldCenterExp(spgrid, spghost, sporder));
    }
    static spFieldCenterExp NewspFieldCenterExpCoeOne(
        spGrid spgrid, spGhost spghost, spOrder sporder
    ){
        auto fun =[](const Index& idx){
            return Exp(idx);
        };
        return spFieldCenterExp(new FieldCenterExp(spgrid, spghost, sporder, fun));
    }

    static spFieldCenterExp NewspFieldExpCoeOne(
        spGrid spgrid, spGhost spghost, spOrder sporder, CenterTag
    ){
        return NewspFieldCenterExpCoeOne(spgrid, spghost, sporder);
    } 
    static spFieldVertexExp NewspFieldExpCoeOne(
        spGrid spgrid, spGhost spghost, spOrder sporder, VertexTag
    ){
        auto fun =[](const Index& idx){
            return Exp(idx);
        };
        return spFieldVertexExp(new FieldVertexExp(spgrid, spghost, sporder, fun));
    }
};

}

#endif