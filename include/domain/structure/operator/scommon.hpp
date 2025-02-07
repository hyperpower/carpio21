#ifndef _SCOMMON_HPP
#define _SCOMMON_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/field/sfield_center.hpp"
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


template<class FIELD>
FIELD IntVolume(const FIELD& field, SFieldCenterTag){
    FIELD res(field);
    auto& grid = res.grid();
    for(auto& idx : res.order()){
        res(idx) = res(idx) * grid.volume(idx);
    }
    return res;
}
// typedef std::function<void(const Index&)> FunIndex;
template<class FIELD>
void _ForEach(FIELD& field, 
              std::function<void(
                typename FIELD::Self&,
                const typename FIELD::Index&)> fun,
                SFieldCenterTag, SGridTag, SGhostRegularTag, SOrderXYZTag){
    for(auto& idx : field.order()){
        fun(field, idx);
    }
}
template<typename FIELD>
void ForEach(FIELD& field, 
             std::function<
                void(typename FIELD::Self&,
                const typename FIELD::Index&)> fun){
    typedef typename FIELD::Index Index;
    typedef typename FIELD::Grid::Tag  GridTag;
    typedef typename FIELD::Ghost::Tag  GhostTag;
    typedef typename FIELD::Order::Tag  OrderTag;
    typedef typename FIELD::Tag FieldTag;
    _ForEach(field, fun, FieldTag(), GridTag(), GhostTag(), OrderTag()); 
    // std::cout << "For Each" << std::endl;

}



} // end namespace
#endif