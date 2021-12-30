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
    typedef typename GRID::Index Index;
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
        FIELD res(f);
        const auto& grid = res.grid();
        for (auto& idx : res.order()) {
            std::array<Exp, DIM> arr;
            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);
                Exp phi_m(idxm), phi_p(idxp);
                Exp phi(idx);
                auto dfdx_m = (phi - phi_m)
                                / (grid.c_(d, idx) - grid.c_(d, idxm));
                auto dfdx_p = (phi_p - phi)
                                / (grid.c_(d, idxp) - grid.c_(d, idx));

                arr[d] = (dfdx_p * grid.fa(d,_P_,idx) - dfdx_m * grid.fa(d, _M_, idx));
            }
            FOR_EACH_DIM
            {
                res(idx) += arr[d];
            }
        }

        return res;
    }
    
    FIELD execute(const FIELD& f, const BI& bi, const Vt& time = 0.0) const{
        FIELD res = this->execute(f); 

        ApplyBC applybc;
        applybc.execute(res, bi, time);

        return res;
    }
    // void set(const int& other){std::cout << "int" << std::endl;}
};

}


#endif