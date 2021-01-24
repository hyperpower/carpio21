#ifndef _SLAPLACIAN_HPP
#define _SLAPLACIAN_HPP

#include "scommon.hpp"

namespace carpio{


template<St DIM, class VT, class GRID, class GHOST, class ORDER>
class LaplacianImplement_<DIM, VT, GRID, GHOST, ORDER, StructureType>: public SOperatorCommon_<DIM, VT, GRID, GHOST, ORDER>{
public:
    LaplacianImplement_(){
        std::cout << "Laplacian Structure" << std::endl;
    };

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }

    // void set(const int& other){std::cout << "int" << std::endl;}
        
};

}


#endif