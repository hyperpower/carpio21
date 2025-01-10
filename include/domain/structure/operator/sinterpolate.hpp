#ifndef _SINTERPOLATE_HPP
#define _SINTERPOLATE_HPP

#include "scommon.hpp"
#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "algebra/algebra.hpp"

namespace carpio{

template<class FIELD>
auto InterpolateCenterToFace(const FIELD& field, SFieldTag){
    DEF_FIELD_TAG(FIELD); 
    std::cout << "    SField Interpolate C to F" << std::endl;
    _SInterpolateCenterToFace(field, 
       GridTag(), GhostTag(), OrderTag(), DimTag());
    return 0;
}

template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, 
        SGridTag, SGhostTag, SOrderTag, DimTag){
    DEF_FIELD_TAG(FIELD); 
    std::cout << "    SField Interpolate expand tag C to F" << std::endl;
    return 0;
}

template<class FIELD>
auto _SInterpolateCenterToFace(const FIELD& field, 
        SGridUniformTag, SGhostTag, SOrderTag, DimTag){
    DEF_FIELD_TAG(FIELD); 
    std::cout << "    SField Interpolate Grid uniformTag tag C to F" << std::endl;
    return 0;
}


}


#endif
