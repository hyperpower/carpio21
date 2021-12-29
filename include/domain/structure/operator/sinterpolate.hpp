#ifndef _SINTERPOLATE_HPP
#define _SINTERPOLATE_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"

namespace carpio{

template<class FIELD, St DIM, class VT, class GRID, class GHOST, class ORDER>
class InterpolateCenterToFaceImplement_<FIELD, DIM, VT, GRID, GHOST, ORDER, StructureType>: public SOperatorCommon_<FIELD, DIM, VT, GRID, GHOST, ORDER>{
public:
    InterpolateCenterToFaceImplement_(){
        std::cout << "InterpolateCenterToFaceImplement_" << std::endl;
    };

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }
    FIELD execute(const FIELD& f) const{
        std::cout<< "StructureType execute" << std::endl;
        return f;
    }
    // void set(const int& other){std::cout << "int" << std::endl;}
};


}


#endif
