#ifndef _SINTERPOLATE_HPP
#define _SINTERPOLATE_HPP

#include "scommon.hpp"
#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "algebra/misc/interpolate.hpp"
#include "domain/structure/operator/sapply_bc.hpp"


namespace carpio{
// =========================================
//    Center to Face
// =========================================
// Reture Face No Bi
template<class FIELD>
auto InterpolateCenterToFace(
        const FIELD& field, Axes a, 
        SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    // std::cout << "    SField Interpolate C to F" << std::endl;
    return _SInterpolateCenterToFace(field, a, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
// Reture Face Have Bi
template<class FIELD>
auto InterpolateCenterToFace(
        const FIELD& field, const BoundaryIndex& bi, Axes a, 
        SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    // std::cout << "    SField Interpolate C to F with boundary" << std::endl;
    return _SInterpolateCenterToFace(field, bi, a, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
// Change Face No Bi
template<class CENTER, class FACE>
void InterpolateCenterToFace(
        const CENTER& field, FACE& face,
        SFieldCenterTag, SFieldFaceTag){
    EXPAND_FIELD_TAG(CENTER); 
    _SInterpolateCenterToFace(field, face, 
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
// =========================================
//    Vetor Center to Vector Face
// =========================================
template<class VCENTER>
auto InterpolateCenterToFace(const VCENTER& vector_center, SVectorFaceTag){
    EXPAND_FIELD_TAG(VCENTER); 
    return _SInterpolateCenterToFace(vector_center, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class VCENTER>
auto InterpolateCenterToFace(const VCENTER& vector_center,
    const BoundaryIndex& bix, 
    const BoundaryIndex& biy,
    const BoundaryIndex& biz, SVectorFaceTag){
    EXPAND_FIELD_TAG(VCENTER); 
    return _SInterpolateCenterToFace(vector_center, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class VCENTER, class VFACE>
void InterpolateCenterToFace(const VCENTER& vector_center, VFACE& vector_face, 
        SVectorCenterTag, SVectorFaceTag){
    EXPAND_FIELD_TAG(VCENTER); 
    _SInterpolateCenterToFace(vector_center, 
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class VCENTER, class VFACE>
void InterpolateCenterToFace(
        const VCENTER& vector_center, VFACE& vector_face,
        const BoundaryIndex& bix, const BoundaryIndex& biy, const BoundaryIndex& biz, 
        SVectorCenterTag, SVectorFaceTag){
    EXPAND_FIELD_TAG(VCENTER); 
    _SInterpolateCenterToFace(vector_center, vector_face, bix, biy, biz, 
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
// =========================================
//    Impliment by Tags 
// =========================================
template<class FIELD, class FACE>
void _SInterpolateCenterToFace(
    const FIELD& field, FACE& face, 
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef AInterpolate_<Vt, ValueType> Inter;
    
    auto& grid  = field.grid();
    auto& ghost = field.ghost();
    auto a      = face.face_axe();
    
    for(auto& findex : face.order()){
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
            face(findex) = fv;
        }else if(ghost.is_boundary(cindex, a, _P_) 
                    && ghost.is_boundary_face(findex, a)){
            auto fx = grid.f(cindex, a, _P_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = field(mindex);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            face(findex) = fv;
        }else{
            auto fx = grid.f(cindex, a, _M_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = field(mindex);
            auto fv = Inter::Linear(fx, mx, cx, mv, cv);
            face(findex) = fv;
        }    
    }
}

template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, Axes a, 
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef AInterpolate_<Vt, ValueType> Inter;
    typedef SFieldFace_<FIELD::Dim, ValueType, Grid, Ghost, Order> FieldFace;

    FieldFace res(field.spgrid(), field.spghost(), a);
    _SInterpolateCenterToFace(field, res, 
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    return res;
}
template<class FIELD, class FACE>
void _SInterpolateCenterToFace(const FIELD& field, FACE& face, const BoundaryIndex&bi, 
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    auto& grid  = field.grid();
    auto& ghost = field.ghost();
    auto a      = face.face_axe();
    typedef AInterpolate_<Vt, ValueType> Inter;
    
    for(auto& findex : face.order()){
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
            face(findex) = fv;
        }else if(ghost.is_boundary(cindex, a, _P_) 
                    && ghost.is_boundary_face(findex, a)){
            auto fx = grid.f(cindex, a, _P_).at(a);
            auto cx = grid.c_(a, cindex);
            auto pindex = Plus(cindex, a);
            auto px     = grid.c_(a, pindex);
            auto pv     = Value(field, bi, cindex, pindex, a, _P_);
            auto fv = Inter::Linear(fx, px, cx, pv, cv);
            face(findex) = fv;
        }else{
            auto fx = grid.f(cindex, a, _M_).at(a);
            auto cx = grid.c_(a, cindex);
            auto mindex = Minus(cindex, a);
            auto mx     = grid.c_(a, mindex);
            auto mv     = field(mindex);
            auto fv     = Inter::Linear(fx, mx, cx, mv, cv);
            face(findex) = fv;
        }
    }
}
template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, const BoundaryIndex&bi, Axes a, 
    ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef SFieldFace_<FIELD::Dim, ValueType, Grid, Ghost, Order> FieldFace;
    typedef AInterpolate_<Vt, ValueType> Inter;
    
    FieldFace res(field.spgrid(), field.spghost(), a);
    _SInterpolateCenterToFace(field, res, bi, 
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    return res;
}

template<class VCENTER, class VFACE>
void _SInterpolateCenterToFace(const VCENTER& vector_center, VFACE& vector_face,
        const BoundaryIndex& bix, const BoundaryIndex& biy, const BoundaryIndex& biz, 
        ArithmeticTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    typedef typename VCENTER::Component FieldCenter;
    EXPAND_FIELD_TAG(FieldCenter); 
    std::array<const BoundaryIndex*, 3> bis{&bix, &biy, &biz};

    for(auto& a : ArrAxes<VCENTER::Dim>()){
        _SInterpolateCenterToFace(vector_center[a], vector_face[a], *(bis[a]), 
            ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    }
}

} //end namespace


#endif
