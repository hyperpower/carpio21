#ifndef _BASE_OPERATOR_HPP
#define _BASE_OPERATOR_HPP

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

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

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
}

#endif