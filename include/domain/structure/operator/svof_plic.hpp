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
#include "domain/structure/stencil/sstencil_2x2.hpp"
#include "domain/structure/stencil/sstencil_3x3.hpp"

#include "geometry/fraction/piecewise_linear.hpp"



#include <array>
#include <cmath>
#include <memory>
#include <string>
#include <type_traits>

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


template<class FIELD>
auto _SCornerGrad(
    const FIELD& field, const BoundaryIndex& bi,
    const typename FIELD::Index& idx, 
    const CornerDirection& dir, Dim2Tag){
    EXPAND_FIELD(FIELD);

    std::array<ValueType, 2> normal{{ValueType(0), ValueType(0)}};
    typedef SStencil2x2_<ValueType> Stencil;

    Stencil stencil(dir);
    stencil.set_value(field, idx, bi);
    
    normal[0] = -(stencil.grad(field, _X_));
    normal[1] = -(stencil.grad(field, _Y_));
    return normal;
}


// Parker and Youngs' Method
template<class FIELD>
auto _SPLICYoungsNormal(
    const FIELD& field, const BoundaryIndex& bi,
    const typename FIELD::Index& idx, Dim2Tag){
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);

    std::array<ValueType, 2> normal{{ValueType(0), ValueType(0)}};

    std::array<CornerDirection, 4> corner_dirs{
        CornerDirection(_X_, _M_, _Y_, _M_),
        CornerDirection(_X_, _P_, _Y_, _M_),
        CornerDirection(_X_, _M_, _Y_, _P_),
        CornerDirection(_X_, _P_, _Y_, _P_)};
    
    
    for(auto& dir : corner_dirs){
        auto n = _SCornerGrad(field, bi, idx, dir, DimTag());
        normal[0] += n[0];
        normal[1] += n[1];
    }
    normal[0] /= 4.0;
    normal[1] /= 4.0;
    return normal;
}

// Centered Columns Scheme (CCS)
template<class FIELD>
auto _SPLICCCSNormal(
    const FIELD& field, const BoundaryIndex& bi,
    const typename FIELD::Index& idx, Dim2Tag){
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);
    // static_assert(std::is_same<GridTag, SGridUniformTag>::value,
            //   "_SPLICCCSNormal requires FIELD::GridTag to be SGridUniformTag");
    
    std::array<ValueType, 2> normal{{ValueType(0), ValueType(0)}};

    typedef SStencil3x3_<ValueType> Stencil;

    Stencil stencil;
    stencil.set_value(field, idx, bi);

    auto mxc = -(stencil.sum_y(_P_) - stencil.sum_y(_M_)) * 0.5;
    auto myc = -(stencil.sum_x(_P_) - stencil.sum_x(_M_)) * 0.5;

    if(std::abs(mxc) < std::abs(myc)){
        normal[0] = mxc;
        normal[1] = 1.0 * Sign(myc);
    }else{
        normal[0] = 1.0 * Sign(mxc);
        normal[1] = myc;
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

    const bool use_ccs = method == "ccs" || method == "CCS";
    ASSERT(method.empty() || method == "youngs" || method == "Youngs" || use_ccs);

    FieldInterface res(field.spgrid(), field.spghost(), field.sporder());
    const auto tol = DefaultFloatTolerance<ValueType>();
    const auto& grid = field.grid();

    for(auto& idx : field.order()){
        const auto fraction = field(idx);
        if(fraction <= tol || fraction >= ValueType(1) - tol){
            res(idx) = nullptr;
            continue;
        }

        const auto normal = use_ccs
            ? _SPLICCCSNormal(field, bi, idx, DimTag())
            : _SPLICYoungsNormal(field, bi, idx, DimTag());
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
