#ifndef _DOMAIN_HPP
#define _DOMAIN_HPP

#include <iostream>
#include "domain/domain_define.hpp"
#include "base/base_operator.hpp"
#include "structure/structure.hpp"
#include "domain/boundary/boundary_index.hpp"
// #include "domain/boundary/boundary_condition.hpp"


namespace carpio{


template<class FIELD, class BI>
FIELD Laplacian(const FIELD& field, BI bi, const std::string& method){
    LaplacianImplement_<FIELD,   FIELD::Dim,
                        typename FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::TraitType
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
                        typename FIELD::TraitType
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
                        typename FIELD::TraitType
                        > imp;
    return imp.execute(field);
}

// Interpolate
template<class FIELD, class BDYIDX>
typename FIELD::ValueType Interpolate(const FIELD& field, const BDYIDX bi){
    std::cout << "Interpolate" << std::endl;
    return 0;
}

}

#endif