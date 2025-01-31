#ifndef _SU_DOT_NABLA_HPP
#define _SU_DOT_NABLA_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/operator/sapply_bc.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"


namespace carpio{

template<class VECTORF, class FIELD, class BI>
FIELD UdotNabla(const VECTORF& vec, const FIELD& field, 
                const BI& bi, Vt t, const std::string& m, 
        SVectorFaceTag, SFieldCenterTag)
{
    EXPAND_FIELD_TAG(FIELD);
    // method choose
    if(m == "" || m == "fou"){
        return _UdotNablaFOU(vec, field, bi, t,
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    }else{
        return field;
    }
}
template<class VECTORF, class FIELD, class BI>
FIELD _UdotNablaFOU(const VECTORF& U, const FIELD& phi, 
                   const BI& bi, Vt t,
                    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD);
    EXPAND_FIELD(FIELD);

    Field res = phi.new_compatible_zero();
    for (auto &idx : phi.order())
    {
        std::array<Vt, Field::Dim> arr;
        arr.fill(0.0);

        for(auto& d : ArrAxes<Field::Dim>()) {
            arr[d] = _FOULocalOneAxe(idx, d, U, phi, bi, t,
                ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
        }

        ValueType sum = 0;
        for(auto& d : ArrAxes<Field::Dim>()) {
            sum += arr[d];
        }
        res(idx) = sum;
    }

    return res;
}

template<class VECTORF, class FIELD, class BI>
auto _FOULocalOneAxe(const typename FIELD::Index& idx, const Axes& d, 
                      const VECTORF& U, const FIELD& phi, 
                      const BI& bi, Vt t,
                      ArithmeticTag, SGridTag, SGhostTag, OrderBaseTag, DimTag){
    EXPAND_FIELD(FIELD);

    auto& up = U(d, _P_, idx);
    auto& um = U(d, _M_, idx);
    auto  uc = (up + um) * 0.5; // average velocity to center
    auto idxm = idx.m(d);
    auto idxp = idx.p(d);
    Vt phi_u, phi_d;
    if (uc >= 0){
        phi_u = Value(phi, bi, idx, idxm, d, _M_, t);
        phi_d = phi(idx);
        return (up * phi_u - um * phi_d) / (phi.grid().c_(d, idxm) - phi.grid().c_(d, idx));
    }else{ // uc < 0
        phi_u = phi(idx);
        phi_d = Value(phi, bi, idx, idxp, d, _P_, t);
        return (up * phi_u - um * phi_d) / (phi.grid().c_(d, idx) - phi.grid().c_(d, idxp));
    }
}




}

#endif