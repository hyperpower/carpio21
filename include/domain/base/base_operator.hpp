#ifndef _BASE_OPERATOR_HPP
#define _BASE_OPERATOR_HPP

#include <iostream>
#include "type_define.hpp"
#include "base_operator_impl.hpp"

namespace carpio{

template<class FIELD, class BI>
FIELD Laplacian(const FIELD& field, BI bi, const std::string& method){
    LaplacianImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::TraitTag
                        > imp;
    std::cout << "Laplacian----<" << std::endl;
    imp.set_method(method);
    return imp.execute(field, bi);
}
template<class FIELD, class BI>
FIELD Laplacian(const FIELD& field, BI bi){
    std::cout << "Laplacian field with bi" << std::endl;
    LaplacianImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::TraitTag
                        > imp;
    return imp.execute(field, bi);
}
template<class FIELD>
FIELD Laplacian(const FIELD& field){
    std::cout << "Laplacian field NO bi" << std::endl;
    LaplacianImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::TraitTag
                        > imp;
    return imp.execute(field);
}

// Interpolate
template<class FIELD, class BDYIDX>
typename FIELD::ValueType Interpolate(const FIELD& field, const BDYIDX bi){
    std::cout << "Interpolate" << std::endl;
    return 0;
}

// BuildMatrix
template<class FIELD, class MAT, class ARR>
typename int BuildMatrix(const FIELD& field, MAT& mat, ARR& b){
    std::cout << "Build Matix" << std::endl;
    BuildMatrixImplement_<FIELD, FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::TraitTag
                        > imp;
    imp.execute(field, mat, b);
    return 0;
}


}

#endif