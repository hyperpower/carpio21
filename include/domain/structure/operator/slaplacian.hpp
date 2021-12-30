#ifndef _SLAPLACIAN_HPP
#define _SLAPLACIAN_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"

#include "sapply_bc.hpp"

namespace carpio{

// -----------------------
// Linear Poly 

template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class LaplacianImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename GRID::Index>, 
    GRID, GHOST, ORDER, StructureType>: 
    public SOperatorCommon_<FIELD, DIM, LinearPolynomial_<Vt, typename GRID::Index>, GRID, GHOST, ORDER>{     
public:
    typedef LinearPolynomial_<Vt, typename GRID::Index> Exp;
    typedef ApplyBCImplement_<FIELD, DIM, Exp, GRID, GHOST, ORDER, StructureType> ApplyBC;
public:
    LaplacianImplement_(){
        std::cout << "Laplacian Exp Structure" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }
    FIELD execute(const FIELD& f) const{
        std::cout<< "StructureType Exp execute" << std::endl;
        FIELD res(f);
        ApplyBC applybc;
        // applybc.execute(res, *(this->_spbi));

        return res;
    }
    
    FIELD execute(const FIELD& f, const BI& bi) const{
        std::cout<< "Execute Exp BI" << std::endl;
        FIELD res(f);
        ApplyBC applybc;
        applybc.execute(res, bi);

        return res;
    }
    // void set(const int& other){std::cout << "int" << std::endl;}
};

}


#endif