#ifndef _SVOF_PLIC_HPP
#define _SVOF_PLIC_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/sfield_object.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/operator/sapply_bc.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"

#include "geometry/fraction/piecewise_linear.hpp"

#include <array>
#include <cmath>
#include <memory>
#include <string>

namespace carpio{

template<class FIELD>
auto ReconstructInterfacePLIC(
        const FIELD& field, const BoundaryIndex& bi, std::string method,
        SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SReconstructInterfacePLIC(field, bi, method, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}


// implementation
// Scardovelli, R., Zaleski, S., 2003. 
// Interface reconstruction with least‐square fit and split Eulerian–Lagrangian advection. 
// Numerical Methods in Fluids 41, 251–274. https://doi.org/10.1002/fld.431


// Parker and Youngs' Method
template<class FIELD>
auto _SPLICYoungsNormal(
    const FIELD& field, const BoundaryIndex& bi,
    const typename FIELD::Index& idx, SGridUniformTag){
    EXPAND_FIELD(FIELD);

    std::array<ValueType, 2> normal{{ValueType(0), ValueType(0)}};
    const auto& grid  = field.grid();

    for(auto& main_axes : ArrAxes<2>()){
        const auto normal_axes = main_axes == _X_ ? _Y_ : _X_;
        const auto idxm = idx.m(main_axes);
        const auto idxp = idx.p(main_axes);

        const auto phi_m = Value(field, bi, idx, idxm, main_axes, _M_, 0.0);
        const auto phi_p = Value(field, bi, idx, idxp, main_axes, _P_, 0.0);
        const auto grad = (phi_p - phi_m)
                        / (grid.c_(main_axes, idxp) - grid.c_(main_axes, idxm));
        normal[main_axes] = -grad;
    }
    return normal;
}

template<class FIELD>
auto _SReconstructInterfacePLIC(
    const FIELD& field, const BoundaryIndex& bi, std::string method,
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, Dim2Tag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef Line_<ValueType> Line;
    typedef std::shared_ptr<Line> spLine;
    typedef SFieldObject_<FIELD::Dim, spLine, Grid, Ghost, Order> FieldInterface;

    ASSERT(method.empty() || method == "youngs" || method == "Youngs");

    FieldInterface res(field.spgrid(), field.spghost(), field.sporder());
    const auto tol = DefaultFloatTolerance<ValueType>();
    const auto& grid = field.grid();

    for(auto& idx : field.order()){
        const auto fraction = field(idx);
        if(fraction <= tol || fraction >= ValueType(1) - tol){
            res(idx) = nullptr;
            continue;
        }

        const auto normal = _SPLICYoungsNormal(field, bi, idx, GridTag());
        const auto nx = normal[0];
        const auto ny = normal[1];
        if(std::abs(nx) <= tol && std::abs(ny) <= tol){
            res(idx) = nullptr;
            continue;
        }

        const auto dx = grid.s_(_X_, idx);
        const auto dy = grid.s_(_Y_, idx);
        const auto alpha = CalAlpha(fraction, nx, ny, dx, dy);
        res(idx) = std::make_shared<Line>(nx, ny, alpha);
    }
    return res;
}

}

#endif
