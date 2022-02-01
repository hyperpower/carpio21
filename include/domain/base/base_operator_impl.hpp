#ifndef _BASE_OPERATOR_IMPL_HPP
#define _BASE_OPERATOR_IMPL_HPP

#include "domain/domain_define.hpp"

namespace carpio{
// Operator ============================
//
// TRAIT
// - STURCTURE
// - OCTUREE
// - TRIANGLE
// - POLY
template<class FIELD, St DIM, class VT, class GRID, class GHOST, class ORDER, class TRAIT>
class LaplacianImplement_{
public:
    LaplacianImplement_(){
        std::cout << "Laplacian Implement" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }

    FIELD execute(const FIELD& a){
        std::cout<< "Template Class Do Nothing" << std::endl;
        FIELD res(a);
        return res;
    }
    template<class BDYIDX>
    int execute(const FIELD&, const BDYIDX&){};

    // void set(const int& other){std::cout << "int" << std::endl;}
        
};
template<class FIELD, St DIM, class VT, class GRID, class GHOST, class ORDER, class TRAIT>
class ApplyBCImplement_{
public:
    ApplyBCImplement_(){
        std::cout << "ApplyBC Implement Basic" << std::endl;
    };
};
template<class FIELD, St DIM, class VT, class GRID, class GHOST, class ORDER, class TRAIT>
class BuildMatrixImplement_{
public:
    BuildMatrixImplement_(){
        std::cout << "BuildMatrix Implement Basic" << std::endl;
    };
};
}

#endif