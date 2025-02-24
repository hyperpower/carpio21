#ifndef _S_LAPLACIAN_DIFFERENTIAL_HPP
#define _S_LAPLACIAN_DIFFERENTIAL_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/sfield_vertex.hpp"
#include "domain/structure/operator/sapply_bc_vertex.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"

namespace carpio{

// DifferenialLaplacian
// 1 FieldCenter
// 2 FieldVertex
// -> _DifferentialLaplacianCDS
//    1 Exp with bi 
//    2 Exp without bi
//    3 Value with bi
//   -> _CDSOneAxe
//      1 Value 
//      2 Value UniformGrid
//      3 Exp
//      4 Exp UniformGrid 

template<class FIELD, class BI>
FIELD DifferenialLaplacian(const FIELD& field, const BI& bi, double t, 
        SFieldCenterTag)
{
    return _DifferenialLaplacian(field, bi, t);
}
template<class FIELD, class BI>
FIELD DifferenialLaplacian(const FIELD& field, const BI& bi, double t, 
        SFieldVertexTag)
{
    return _DifferenialLaplacian(field, bi, t);
}

template<class FIELD, class BI>
FIELD _DifferenialLaplacian(const FIELD& field, const BI& bi, double t)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _DifferentialLaplacianCDS(field, bi, t, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}

template<class FIELD>
auto _CDSOneAxe(
        const FIELD& phi, 
        const typename FIELD::Index& idx, Axes& a,
        const BoundaryIndex& bi, double t, 
        ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);

    const Grid& grid = phi.grid();

    Index idxp = idx.p(a);
    Index idxm = idx.m(a);

    Vt dfdx_p, dfdx_m;
    Vt phi_m = Value(phi, bi, idx, idxm, a, _M_, t);
    Vt phi_p = Value(phi, bi, idx, idxp, a, _P_, t);
    dfdx_m = (phi(idx) - phi_m)
            / (grid.c_(a, idx) - grid.c_(a, idxm));
    dfdx_p = (phi_p - phi(idx))
            / (grid.c_(a, idxp) - grid.c_(a, idx));
    return (dfdx_p - dfdx_m ) / grid.s_(a, idx);
}
template<class FIELD>
auto _CDSOneAxe(
        const FIELD& phi, 
        const typename FIELD::Index& idx, Axes& a,
        const BoundaryIndex& bi, double t, 
        ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);

    const Grid& grid = phi.grid();

    Index idxp = idx.p(a);
    Index idxm = idx.m(a);

    Vt dfdx_p, dfdx_m;
    Vt phi_m = Value(phi, bi, idx, idxm, a, _M_, t);
    Vt phi_p = Value(phi, bi, idx, idxp, a, _P_, t);
    Vt s = grid.s_(a, idx);
    return (phi_p - 2.0 * phi(idx) - phi_m) / (s * s);
}


template<class FIELD>
auto _CDSOneAxe(
        const FIELD& field, 
        const typename FIELD::Index& idx, Axes& a, double t, 
        LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD); 
    typedef ValueType Exp;

    const Grid& grid   = field.grid();
    const Ghost& ghost = field.ghost();

    // std::cout << "_CDSOneAxe : idx = " << idx << std::endl; 

    Index idxp = idx.p(a);
    Index idxm = idx.m(a);
    // std::cout << "           : idxp = " << idxp << std::endl; 
    // std::cout << "           : idxm = " << idxm << std::endl; 

    Exp phi_m(idxm), phi_p(idxp), phi(idx);
    if(!ghost.is_ghost(idx)){
        phi = field(idx);
    }
    auto dfdx_m = (phi - phi_m)
            / (grid.c_(a, idx) - grid.c_(a, idxm));
    auto dfdx_p = (phi_p - phi)
            / (grid.c_(a, idxp) - grid.c_(a, idx));
    return (dfdx_p - dfdx_m ) / grid.s_(a, idx);
}

template<class FIELD>
auto _CDSOneAxe(
        const FIELD& field, 
        const typename FIELD::Index& idx, Axes& a, double t, 
        LinearPolynomialTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD); 
    typedef ValueType Exp;

    const Grid& grid = field.grid();
    const Ghost& ghost = field.ghost();


    Index idxp = idx.p(a);
    Index idxm = idx.m(a);

    Exp phi_m(idxm), phi_p(idxp), phi(idx);
    if(!ghost.is_ghost(idx)){
        phi = field(idx);
    }
    return (phi_p - phi * 2.0 + phi_m) / (grid.dc() * grid.s());
}

template<class FIELD>
FIELD _DifferentialLaplacianCDS(
        const FIELD& phi, const BoundaryIndex& bi, double t, 
        ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);


    Field res        = phi.new_compatible_zero();
    const Grid& grid = phi.grid();
    for (auto& idx : phi.order()) {
        std::array<Vt, Field::Dim> arr;
        arr.fill(0.0);

        for(auto& a : ArrAxes<Field::Dim>()){
            arr[a] = _CDSOneAxe(phi, idx, a, bi, t,
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
        }

        Vt sum = 0;
        for(auto& a : ArrAxes<Field::Dim>()){
            sum += arr[a];
        }
        res(idx) = sum;
    }
    return res;
}

template<class FIELD>
FIELD _DifferentialLaplacianCDS( // No BoundaryIndex
        const FIELD& field, double t, 
        LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD); 
    typedef ValueType Exp;
    Field res = field.new_compatible_zero();
    const auto& grid = res.grid();
    for (auto& idx : res.order()) {
        std::array<Exp, Field::Dim> arr;
        for(auto& d : ArrAxes<Field::Dim>()){
            arr[d] = _CDSOneAxe(field, idx, d, t,
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag()); 
        }
        for(auto& d : ArrAxes<Field::Dim>()){
            res(idx) += arr[d];
        }
    }
    return res;
}

template<class FIELD>
FIELD _DifferentialLaplacianCDS(
        const FIELD& phi, const BoundaryIndex& bi, double t, 
        LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    auto res = _DifferentialLaplacianCDS(phi, t, 
                ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag() );
    ApplyBoundaryValue(res,bi,t);

    return res;
}



}
#endif