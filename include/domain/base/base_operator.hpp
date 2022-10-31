#ifndef _BASE_OPERATOR_HPP
#define _BASE_OPERATOR_HPP

#include <iostream>
#include "type_define.hpp"
#include "base_operator_impl.hpp"

namespace carpio{

// template<class FIELD, class BI>
// FIELD IntLaplacian(const FIELD& field, BI bi, const std::string& method){
//     IntLaplacianImplement_<FIELD,   FIELD::Dim,
//                         typename FIELD::ValueType,
//                         typename FIELD::Grid, 
//                         typename FIELD::Ghost,
//                         typename FIELD::Order,
//                         typename FIELD::Tag
//                         > imp;
//     // std::cout << "[  Laplace ] field, bi" << std::endl;
//     imp.set_method(method);
//     return imp.execute(field, bi);
// }
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
// Interpolate
template<class FIELD, class BDYIDX>
typename FIELD::ValueType Interpolate(const FIELD& field, const BDYIDX bi){
    std::cout << "Interpolate" << std::endl;
    return 0;
}

// BuildMatrix
template<class FIELD, class MAT, class ARR>
int BuildMatrix(const FIELD& field, MAT& mat, ARR& b){
    // std::cout << "[   INFO   ] Fuction Build Matix" << std::endl;
    BuildMatrixImplement_<FIELD, FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::Tag
                        > imp;
    imp.execute(field, mat, b);
    return 0;
}


}

#endif