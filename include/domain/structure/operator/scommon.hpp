#ifndef _SCOMMON_HPP
#define _SCOMMON_HPP

#include "domain/structure/structure_define.hpp"

namespace carpio{

template<class FIELD, St DIM, class VT, class GRID, class GHOST, class ORDER>
class SOperatorCommon_{
public:
    SOperatorCommon_(){
        std::cout << "S Operator Impl" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}
};


}



#endif