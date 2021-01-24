#ifndef _DOMAIN_HPP
#define _DOMAIN_HPP

#include <iostream>
#include "domain/domain_define.hpp"
#include "structure/structure.hpp"

namespace carpio{


template<class FIELD, class BDYIDX>
FIELD Laplacian(const FIELD& field, const BDYIDX bi, const std::string& method){
    LaplacianImplement_<FIELD::Dim,
                        FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order,
                        typename FIELD::TraitType
                        > imp;
    imp.set_method(method);
    imp.set_boundary_index(bi);

    std::cout << "Laplacian----<" << std::endl;
    return field;
}
template<class FIELD, class BDYIDX>
FIELD Laplacian(const FIELD& field, const BDYIDX bi){
    std::cout << "Laplacian" << std::endl;
    return field;
}

}

#endif