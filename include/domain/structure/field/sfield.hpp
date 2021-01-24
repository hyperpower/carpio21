#ifndef S_FIELD_HPP
#define S_FIELD_HPP

#include "domain/domain_define.hpp"

namespace carpio{

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
class SField_: public FieldBase_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef StructureType TraitType;


};






}



#endif