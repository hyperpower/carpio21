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

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;
protected:
    spGrid  _spgrid;
    spGhost _spghost;
    spOrder _sporder;
public:
    SField_(){}

    virtual ~SField_(){}

    virtual Grid&  grid(){return *_spgrid;};
    virtual Ghost& ghost(){return *_spghost;};
    virtual Order& order(){return *_sporder;};

};






}



#endif