#ifndef _SLAPLACIAN_HPP
#define _SLAPLACIAN_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"

namespace carpio{

// typedef LinearPolynomial_<double, std::string> Poly;

template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class LaplacianImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename GRID::Index>, 
    GRID, GHOST, ORDER, StructureType>: 
    public SOperatorCommon_<FIELD, DIM, LinearPolynomial_<Vt, typename GRID::Index>, GRID, GHOST, ORDER>{
public:
    LaplacianImplement_(){
        std::cout << "Laplacian Exp Structure" << std::endl;
    };

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }
    FIELD execute(const FIELD& f) const{
        std::cout<< "StructureType Exp execute" << std::endl;
        FIELD res(f);
        return res;
    }
    // void set(const int& other){std::cout << "int" << std::endl;}
};

}


#endif