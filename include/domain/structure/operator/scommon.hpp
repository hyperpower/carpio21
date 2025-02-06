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

template<class FIELD,
        typename std::enable_if<
            std::is_base_of<typename FIELD::Ghost::Tag, SGhostRegularTag>::value, bool>::type = true>
typename FIELD::ValueType Norm1(const FIELD& field, SFieldCenterTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        e += vi * std::abs(field(idx));
    }
    return e / vol;
}
template<class FIELD,
        typename std::enable_if<
            std::is_base_of<typename FIELD::Ghost::Tag, SGhostRegularTag>::value, bool>::type = true>
typename FIELD::ValueType Norm2(const FIELD& field, SFieldCenterTag){
    Vt vol = 0.0;
    Vt e   = 0.0;
    auto& grid = field.grid();
    for(auto& idx : field.order()){
        Vt vi = grid.volume(idx);
        vol += vi;
        Vt avalue = std::abs(field(idx));
        e += vi * avalue * avalue;
    }
    return std::sqrt(e / vol);
}
template<class FIELD,
        typename std::enable_if<
            std::is_base_of<typename FIELD::Ghost::Tag, SGhostRegularTag>::value, bool>::type = true>
typename FIELD::ValueType NormInf(const FIELD& field, SFieldCenterTag){
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