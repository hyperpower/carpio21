#ifndef _SINTEGRAL_LAPLACIAN_HPP
#define _SINTEGRAL_LAPLACIAN_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/operator/sapply_bc.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"

namespace carpio{


template<class FIELD, class BI>
FIELD IntegralLaplacian(
        const FIELD& field, const BI& bi, double t, 
        SFieldCenterTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _IntegralLaplacianCenter(field, bi, t, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}

template<class FIELD>
FIELD _IntegralLaplacianCenter(
        const FIELD& phi, const BoundaryIndex& bi, double t, 
        ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);

    auto  res  = phi.new_compatible_zero();
    auto& grid = phi.grid();
    for (auto &idx : phi.order()){
        std::array<Vt, Field::Dim> arr;
        arr.fill(0.0);

        for(auto& a : ArrAxes<Field::Dim>()){
            // std::cout << ToString(a) << std::endl;
            Index idxp = idx.p(a);
            Index idxm = idx.m(a);
            
            Vt dfdx_p, dfdx_m;
            Vt phi_m = Value(phi, bi, idx, idxm, a, _M_, t);
            Vt phi_p = Value(phi, bi, idx, idxp, a, _P_, t);
            dfdx_m = (phi(idx) - phi_m)
                    / (grid.c_(a, idx) - grid.c_(a, idxm));
            dfdx_p = (phi_p - phi(idx))
                    / (grid.c_(a, idxp) - grid.c_(a, idx));
            arr[a] = (dfdx_p * grid.fa(a, _P_, idx) 
                    - dfdx_m * grid.fa(a, _M_, idx));
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
FIELD _IntegralLaplacianCenter( // No BoundaryIndex
        const FIELD& field, double t, 
        LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    typedef ValueType Exp;
    Field res = field.new_compatible_zero();
    const auto& grid = res.grid();
    for (auto& idx : res.order()) {
        std::array<Exp, Field::Dim> arr;
        for(auto& d : ArrAxes<Field::Dim>()){
            Index idxp = idx.p(d);
            Index idxm = idx.m(d);
            Exp phi_m(idxm), phi_p(idxp);
            Exp phi = field(idx);
            auto dfdx_m = (phi - phi_m)
                        / (grid.c_(d, idx ) - grid.c_(d, idxm));
            auto dfdx_p = (phi_p - phi) 
                        / (grid.c_(d, idxp) - grid.c_(d, idx ));

            arr[d] = (dfdx_p * grid.fa(d, _P_, idx) 
                    - dfdx_m * grid.fa(d, _M_, idx));
        }
        for(auto& d : ArrAxes<Field::Dim>()){
            res(idx) += arr[d];
        }
    }
    return res;
}

template<class FIELD>
FIELD _IntegralLaplacianCenter(
        const FIELD& phi, const BoundaryIndex& bi, double t, 
        LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    auto res = _IntegralLaplacianCenter(phi, t, 
                ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag() );
    ApplyBoundaryValue(res,bi,t);
    return res;
}


} //end namespace

#endif