#ifndef _BASE_OPERATOR_HPP
#define _BASE_OPERATOR_HPP

#include <iostream>
#include "type_define.hpp"
#include "domain/domain_define.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "base_operator_impl.hpp"

namespace carpio{

//deprecate -->
// template<class FIELD, class BI>
// FIELD IntLaplacian(const FIELD& field, BI bi, double t=0.0){
//     // std::cout << "IntLaplacian field with bi" << std::endl;
//     IntLaplacianImplement_<FIELD,   FIELD::Dim,
//                         typename FIELD::ValueType,
//                         typename FIELD::Grid, 
//                         typename FIELD::Ghost,
//                         typename FIELD::Order,
//                         typename FIELD::Tag
//                         > imp;
//     return imp.execute(field, bi, t); 
// }
// <--- deprecate


template<class FIELD, class BI>
FIELD IntegralLaplacian(const FIELD& field, const BI& bi, double t=0.0){
    typename FIELD::Tag field_tag; 
    return IntegralLaplacian(field, bi, t, field_tag);
}
template<class FIELD, class BI>
FIELD DifferenialLaplacian(const FIELD& field, const BI& bi, double t=0.0){
    typename FIELD::Tag field_tag; 
    return DifferenialLaplacian(field, bi, t, field_tag);
}
// Laplacian Finite Difference Method
template<class FIELD, class BI>
FIELD Laplacian(const FIELD& field, BI bi, double t=0.0){
    // std::cout << "IntLaplacian field with bi" << std::endl;
    LaplacianImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::Tag
                        > imp;
    return imp.execute(field, bi, t); 
}
// Laplacian Finite Difference Method
template<class FIELD, class BI>
FIELD L4Alter(const FIELD& field, BI bi, double t=0.0, const std::string& method = ""){
    // std::cout << "IntLaplacian field with bi" << std::endl;
    L4AlterImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::Tag
                        > imp;
    imp.set(method);
    return imp.execute(field, bi, t); 
}

template<class FIELD>
typename FIELD::ValueType Norm1(const FIELD& field){
    typename FIELD::Tag t; 
    return _Norm1(field, t);
}
template<class FIELD>
typename FIELD::ValueType Norm1(const FIELD& field, FunXYZ_Value efun){
    typename FIELD::Tag t; 
    return _Norm1(field, efun, t);
}
template<class FIELD>
typename FIELD::ValueType Norm1(const FIELD& field, FunXYZT_Value efun, Vt time){
    typename FIELD::Tag tag;
    return _Norm1(field, efun, time, tag);
}

template<class FIELD>
typename FIELD::ValueType Norm2(const FIELD& field){
    typename FIELD::Tag t; 
    return _Norm2(field, t);
}
template<class FIELD>
typename FIELD::ValueType Norm2(const FIELD& field, FunXYZ_Value efun){
    typename FIELD::Tag t; 
    return _Norm2(field, efun, t);
}
template<class FIELD>
typename FIELD::ValueType Norm2(const FIELD& field, FunXYZT_Value efun, Vt time){
    typename FIELD::Tag tag; 
    return _Norm2(field, efun, time, tag);
}

template<class FIELD>
typename FIELD::ValueType NormInf(const FIELD& field){
    typename FIELD::Tag t; 
    return _NormInf(field, t);
}
template<class FIELD>
typename FIELD::ValueType NormInf(const FIELD& field, FunXYZ_Value efun){
    typename FIELD::Tag t; 
    return _NormInf(field, efun, t);
}
template<class FIELD>
typename FIELD::ValueType NormInf(const FIELD& field, FunXYZT_Value efun, Vt time){
    typename FIELD::Tag tag; 
    return _NormInf(field, efun, time, tag);
}
template<class FIELD>
FIELD IntVolume(const FIELD& field,typename FIELD::Tag);

template<class FIELD>
FIELD IntVolume(const FIELD& field){
    typename FIELD::Tag t; 
    return IntVolume(field, t);
}
// Interpolate =============================
template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, Axes a){
    typename FIELD::Tag field_tag; 
    return InterpolateCenterToFace(field, a, field_tag);
}
template<class CENTER, class FACE>
void InterpolateCenterToFace(const CENTER& field, FACE& face){
    typename CENTER::Tag center_tag; 
    typename FACE::Tag face_tag; 
    InterpolateCenterToFace(field, face, center_tag, face_tag);
}
template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, const BoundaryIndex& bi, Axes a){
    typename FIELD::Tag field_tag; 
    return InterpolateCenterToFace(field, bi, a, field_tag);
}
template<class CENTER, class FACE>
void InterpolateCenterToFace(const CENTER& field, FACE& face, const BoundaryIndex& bi){
    typename CENTER::Tag center_tag; 
    typename FACE::Tag face_tag; 
    InterpolateCenterToFace(field, face, bi, center_tag, face_tag);
}
template<class VECTOR>
auto InterpolateCenterToFace(const VECTOR& field){
    typedef typename VECTOR::Tag Tag; 
    return InterpolateCenterToFace(field, Tag());
}
template<class VECTOR>
auto InterpolateCenterToFace(const VECTOR& field,
    const BoundaryIndex& bix, 
    const BoundaryIndex& biy = BoundaryIndex(),
    const BoundaryIndex& biz = BoundaryIndex()){
    typedef typename VECTOR::Tag Tag; 
    return InterpolateCenterToFace(field, bix, biy, biz, Tag());
}
template<class VCENTER, class VFACE>
void InterpolateCenterToFace(const VCENTER& vcenter, VFACE& vface,
    const BoundaryIndex& bix, 
    const BoundaryIndex& biy = BoundaryIndex(),
    const BoundaryIndex& biz = BoundaryIndex()){
    typedef typename VCENTER::Tag CenterTag; 
    typedef typename VFACE::Tag   FaceTag; 
    InterpolateCenterToFace(vcenter, vface, bix, biy, biz, 
        CenterTag(), FaceTag());
}
// Advection ===============================
template<class VECTORF, class FIELD, class BI>
FIELD UdotNabla(const VECTORF& vec, const FIELD& field, const BI& bi, 
                Vt time = 0.0, const std::string& method = ""){
    typedef typename FIELD::Tag FieldTag; 
    typedef typename VECTORF::Tag VectorTag;
    return UdotNabla(vec, field, bi, time, method, VectorTag(), FieldTag());
}

// BoundaryValue ===========================
template<class FIELD>
void ApplyBoundaryValue(
     FIELD& field,
    const BoundaryIndex& bi,
    const Vt&            time = 0.0)
{
    typedef typename FIELD::Tag Tag; 
    typedef typename FIELD::ValueTag ValueTag;
    ApplyBoundaryValue(field, bi, time, Tag(), ValueTag());
}

template<class FIELD>
typename FIELD::ValueType Value(
    const FIELD& field,
    const BoundaryIndex& bi,
    const typename FIELD::Index& idxc,
    const typename FIELD::Index& idxg,
    const Axes&            axe,
    const Orientation&     ori,
    const Vt&              time = 0.0)
{
    typedef typename FIELD::Tag Tag; 
    return Value(field, bi, idxc, idxg, axe, ori, time, Tag());
}
// BuildMatrix =============================
template<class FIELD, class MAT, class ARR>
int BuildMatrix(const FIELD& field, MAT& mat, ARR& b){
    typedef typename FIELD::Tag Tag; 
    typedef typename FIELD::ValueTag ValueTag;
    _BuildMatrix(field, mat, b, Tag(), ValueTag());
    return 0;
}


}

#endif