#ifndef _SINTERPOLATE_HPP
#define _SINTERPOLATE_HPP

#include "scommon.hpp"
#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "algebra/misc/interpolate.hpp"
#include "domain/structure/operator/sapply_bc.hpp"


namespace carpio{

template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, Axes a, SFieldTag){
    EXPAND_FIELD_TAG(FIELD); 
    std::cout << "    SField Interpolate C to F" << std::endl;
    return _SInterpolateCenterToFace(field, a, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, const BoundaryIndex& bi, Axes a, SFieldTag){
    EXPAND_FIELD_TAG(FIELD); 
    std::cout << "    SField Interpolate C to F with boundary" << std::endl;
    return _SInterpolateCenterToFace(field, bi, a, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, Axes a, 
    BaseTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    std::cout << "    SField Interpolate expand tag C to F" << std::endl;
    return 0;
}

template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, Axes a, 
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef AInterpolate_<Vt, ValueType> Inter;
    
    auto& grid  = field.grid();
    auto& ghost = field.ghost();
    
    typedef SFieldFace_<FIELD::Dim, ValueType, Grid, Ghost, Order> FieldFace;
    FieldFace res(field.spgrid(), field.spghost(), a);
    for(auto& findex : res.order()){
        auto cindex = grid.face_index_to_cell_index(findex, a);
        auto cv     = field(cindex);

        if(ghost.is_boundary(cindex, a, _M_) 
                && ghost.is_boundary_face(findex, a)){
            auto fx = grid.f(cindex, a, _M_).at(a);
            auto cx = grid.c_(a, cindex);
            auto pindex = Plus(cindex, a);
            auto mx     = grid.c_(a, pindex);
            auto mv     = field(pindex);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            res(findex) = fv;
        }else if(ghost.is_boundary(cindex, a, _P_) 
                    && ghost.is_boundary_face(findex, a)){
            auto fx = grid.f(cindex, a, _P_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = field(mindex);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            res(findex) = fv;
        }else{
            auto fx = grid.f(cindex, a, _M_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = field(mindex);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            res(findex) = fv;
        }    
    }
    return res;
}
template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, const BoundaryIndex&bi, Axes a, 
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    auto& grid  = field.grid();
    auto& ghost = field.ghost();
    typedef AInterpolate_<Vt, ValueType> Inter;
    
    typedef SFieldFace_<FIELD::Dim, ValueType, Grid, Ghost, Order> FieldFace;
    FieldFace res(field.spgrid(), field.spghost(), a);
    for(auto& findex : res.order()){
        auto cindex = grid.face_index_to_cell_index(findex, a);
        auto cv     = field(cindex);

        if(ghost.is_boundary(cindex, a, _M_) 
                && ghost.is_boundary_face(findex, a)){
            auto fx = grid.f(cindex, a, _M_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = Value(field, bi, cindex, mindex, a, _M_);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            res(findex) = fv;
        }else if(ghost.is_boundary(cindex, a, _P_) 
                    && ghost.is_boundary_face(findex, a)){
            auto fx = grid.f(cindex, a, _P_).at(a);
            auto cx = grid.c_(a, cindex);
            auto pindex = Plus(cindex, a);
            auto px     = grid.c_(a, pindex);
            auto pv     = Value(field, bi, cindex, pindex, a, _P_);
            auto fv = Inter::Linear(fx, px, cx, pv, cv);
            res(findex) = fv;
        }else{
            auto fx = grid.f(cindex, a, _M_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = field(mindex);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            res(findex) = fv;
        }    
    }
    return res;
}



} //end namespace


#endif
