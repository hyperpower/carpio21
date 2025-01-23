#ifndef _BASE_OPERATOR_HPP
#define _BASE_OPERATOR_HPP

#include <iostream>
#include "type_define.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "base_operator_impl.hpp"

namespace carpio{

//deprecate -->
template<class FIELD, class BI>
FIELD IntLaplacian(const FIELD& field, BI bi, double t=0.0){
    // std::cout << "IntLaplacian field with bi" << std::endl;
    IntLaplacianImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::Tag
                        > imp;
    return imp.execute(field, bi, t); 
}
// <--- deprecate


template<class FIELD, class BI>
FIELD IntegralLaplacian(const FIELD& field, const BI& bi, double t=0.0){
    typename FIELD::Tag field_tag; 
    return IntegralLaplacian(field, bi, t, field_tag);
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
typename FIELD::ValueType Norm1(const FIELD& field,typename FIELD::Tag);

template<class FIELD>
typename FIELD::ValueType Norm1(const FIELD& field){
    typename FIELD::Tag t; 
    return Norm1(field, t);
}
template<class FIELD>
typename FIELD::ValueType Norm2(const FIELD& field,typename FIELD::Tag);

template<class FIELD>
typename FIELD::ValueType Norm2(const FIELD& field){
    typename FIELD::Tag t; 
    return Norm2(field, t);
}
template<class FIELD>
typename FIELD::ValueType NormInf(const FIELD& field,typename FIELD::Tag);

template<class FIELD>
typename FIELD::ValueType NormInf(const FIELD& field){
    typename FIELD::Tag t; 
    return NormInf(field, t);
}

template<class FIELD>
FIELD IntVolume(const FIELD& field,typename FIELD::Tag);

template<class FIELD>
FIELD IntVolume(const FIELD& field){
    typename FIELD::Tag t; 
    return IntVolume(field, t);
}
// Interpolate ==========================
template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, Axes a){
    typename FIELD::Tag field_tag; 
    return InterpolateCenterToFace(field, a, field_tag);
}
template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, const BoundaryIndex& bi, Axes a){
    typename FIELD::Tag field_tag; 
    return InterpolateCenterToFace(field, bi, a, field_tag);
}
template<class FIELD>
typename FIELD::ValueType Value(
    const FIELD& field,
    const BoundaryIndex& bi,
    const typename FIELD::Index& idxc,
    const typename FIELD::Index& idxg,
    const Axes&            axe,
    const Orientation&     ori,
    const Vt&            time = 0.0)
{
    typedef typename FIELD::Tag Tag; 
    return Value(field, bi, idxc, idxg, axe, ori, time, Tag());
}

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

// BuildMatrix
template<class FIELD, class MAT, class ARR>
int BuildMatrix(const FIELD& field, MAT& mat, ARR& b){
    typedef typename FIELD::Tag Tag; 
    typedef typename FIELD::ValueTag ValueTag;
    _BuildMatrix(field, mat, b, Tag(), ValueTag());
    return 0;
}


}

#endif