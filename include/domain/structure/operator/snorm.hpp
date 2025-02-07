#ifndef _SNORM_HPP
#define _SNORM_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/order/sorder.hpp"
#include "domain/structure/ghost/sghost.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "scommon.hpp"

#include "algebra/misc/linear_polynomial.hpp"

namespace carpio{

// =========================================
//    Norm 1 
// =========================================
template<class FIELD>
typename FIELD::ValueType _Norm1(const FIELD& field, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNorm1(field, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _Norm1(const FIELD& field, FunXYZ_Value fun, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNorm1(field, fun, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _Norm1(const FIELD& field, FunXYZT_Value fun, Vt time, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNorm1(field, fun, time, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _SNorm1(const FIELD& error_field,
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = error_field.grid();
    for(auto& idx : error_field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        e += vi * std::abs(error_field(idx));
    }
    return e / vol;
}
template<class FIELD>
typename FIELD::ValueType _SNorm1(const FIELD& field, FunXYZ_Value fun, 
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        auto pc = grid.c(idx);
        auto exact_v = fun(pc.value(_X_), pc.value(_Y_), pc.value(_Z_));
        e += vi * std::abs(exact_v - field(idx));
    }
    return e / vol;
}
template<class FIELD>
typename FIELD::ValueType _SNorm1(const FIELD& field, FunXYZT_Value fun, Vt time,  
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        auto pc = grid.c(idx);
        auto exact_v = fun(pc.value(_X_), pc.value(_Y_), pc.value(_Z_), time);
        e += vi * std::abs(exact_v - field(idx));
    }
    return e / vol;
}
// =========================================
//    Norm 2 
// =========================================
template<class FIELD>
typename FIELD::ValueType _Norm2(const FIELD& field, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNorm2(field, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _Norm2(const FIELD& field, FunXYZ_Value fun, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNorm2(field, fun, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _Norm2(const FIELD& field, FunXYZ_Value fun, Vt time, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNorm2(field, fun, time, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _SNorm2(const FIELD& error_field,
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = error_field.grid();
    for(auto& idx : error_field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        Vt avalue = std::abs(error_field(idx));
        e += vi * avalue * avalue;
    }
    return std::sqrt(e / vol);
}
template<class FIELD>
typename FIELD::ValueType _SNorm2(const FIELD& field, FunXYZ_Value fun,  
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        auto pc = grid.c(idx);
        auto exact_v = fun(pc.value(_X_), pc.value(_Y_), pc.value(_Z_));
        Vt avalue = std::abs(exact_v - field(idx));
        e += vi * avalue * avalue;
    }
    return std::sqrt(e / vol);
}
template<class FIELD>
typename FIELD::ValueType _SNorm2(const FIELD& field, FunXYZT_Value fun,  Vt time, 
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        auto pc = grid.c(idx);
        auto exact_v = fun(pc.value(_X_), pc.value(_Y_), pc.value(_Z_), time);
        Vt avalue = std::abs(exact_v - field(idx));
        e += vi * avalue * avalue;
    }
    return std::sqrt(e / vol);
}
// =========================================
//    Norm Inf 
// =========================================
template<class FIELD>
typename FIELD::ValueType _NormInf(const FIELD& field, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNormInf(field, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _NormInf(const FIELD& field, FunXYZ_Value fun, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNormInf(field, fun, ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _NormInf(const FIELD& field, FunXYZT_Value fun, Vt time, SFieldCenterTag){
    EXPAND_FIELD_TAG(FIELD); 
    return _SNormInf(field, fun, time,
            ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType _SNormInf(const FIELD& field, 
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    double m = 0.0;
    for(auto& idx : field.order()){
        Vt avalue = std::abs(field(idx));
        if(avalue > m){
            m = avalue;
        }
    }
    return m;
}
template<class FIELD>
typename FIELD::ValueType _SNormInf(const FIELD& field, FunXYZ_Value fun, 
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    double m = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        auto pc = grid.c(idx);
        auto exact_v = fun(pc.value(_X_), pc.value(_Y_), pc.value(_Z_));
        Vt avalue = std::abs(exact_v - field(idx));
        if(avalue > m){
            m = avalue;
        }
    }
    return m;
}
template<class FIELD>
typename FIELD::ValueType _SNormInf(const FIELD& field, FunXYZT_Value fun, Vt time,
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    double m = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        auto pc = grid.c(idx);
        auto exact_v = fun(pc.value(_X_), pc.value(_Y_), pc.value(_Z_), time);
        Vt avalue = std::abs(exact_v - field(idx));
        if(avalue > m){
            m = avalue;
        }
    }
    return m;
}
} // end namespace
#endif