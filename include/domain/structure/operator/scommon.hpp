#ifndef _SCOMMON_HPP
#define _SCOMMON_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "algebra/misc/linear_polynomial.hpp"

namespace carpio{


template<class FIELD, St DIM, class VT,
         class GRID, class GHOST, class ORDER>
class SOperatorCommon_{
public:
    typedef FIELD Field;
    typedef GRID  Grid;
    typedef typename GRID::Index Index;
    typedef GHOST Ghost;
    typedef ORDER Order;

    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
    // typedef ApplyBCImplement_<FIELD, 
    //                           DIM, ValueType,
    //                           GRID, GHOST, ORDER, 
    //                           StructureTag> ApplyBC;
protected:
    spBI _spbi;
public:
    SOperatorCommon_():_spbi(nullptr){};

    void set_boundary_index(const BI& bi){
        _spbi = std::make_shared<BI>(bi);
    };

    template<class ANY>
    void set(const ANY& other){}

    void Loop(Field& f){}

};


}



#endif