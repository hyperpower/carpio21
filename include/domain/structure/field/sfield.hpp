#ifndef S_FIELD_HPP
#define S_FIELD_HPP

#include "domain/domain_define.hpp"
#include "algebra/algebra.hpp"

namespace carpio{

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SField_: public FieldBase_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef StructureType TraitType;
    typedef SField_<Dim, VT, GRID, GHOST, ORDER> Self;
    typedef typename Grid::Index Index;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef ArrayListV_<ValueType> Arr; 
protected:
    spGrid  _spgrid;
    spGhost _spghost;
    spOrder _sporder;
    // for saving data
    Arr _arr;
public:
    SField_(spGrid spg, spGhost spgh):
        _spgrid(spg), _spghost(spgh){
        // Initall a default order_xyz
        _sporder = spOrder(new Order(spg, spgh));
        // make data by order
        _arr.reconstruct(_sporder->size());
    }
    SField_(spGrid spg, spGhost spgh, spOrder spo):
        _spgrid(spg), _spghost(spgh), _sporder(spo), _arr(spo->size()){
    }
    SField_(const Self& other):
        _spgrid(other._spgrid), 
        _spghost(other._spghost), 
        _sporder(other._sporder), 
        _arr(other._arr){
    }
    Self& operator=(const Self& other) {
        if (this == &other) {
            return *this;
        }
        ASSERT(is_compatible(other));
        _arr = other._arr;
        return *this;
    }

    ValueType&      operator()(St i, St j = 0, St k = 0){
        return _arr[sporder->get_order(i,j,k)];
    }

    const ValueType& operator()(St i, St j = 0, St k = 0) const{
        return _arr[sporder->get_order(i,j,k)];
    }

    ValueType& operator()(const Index& index) {
        return _arr[_sporder->get_order(index)];
    }

    const ValueType& operator()(const Index& index) const {
        return _arr[sporder->get_order(index)];
    }

    Self operator-() const{
        Self res(*this);
        res._arr = -res._arr;
        return res;
    }

    Self& operator+=(const Self& rhs){
        // actual addition of rhs to *this
        ASSERT(is_compatible(rhs));
        _arr += rhs._arr;
        return *this;
    }

    Self& operator+=(const Vt& rhs){
        // actual addition of rhs to *this
        _arr += rhs;
        return *this;
    }

    Self& operator-=(const Self& rhs) {
        // actual addition of rhs to *this
        ASSERT(is_compatible(rhs));
        _arr -= rhs._arr;
        return *this;
    }

    Self& operator-=(const Vt& rhs) {
        // actual addition of rhs to *this
        _arr -= rhs;
        return *this;
    }

    Self& operator*=(const Self& rhs) {
        // actual addition of rhs to *this
        ASSERT(is_compatible(rhs));
        _arr *= rhs._arr;
        return *this;
    }

    Self& operator*=(const Vt& rhs) {
        // actual addition of rhs to *this
        _arr *= rhs;
        return *this;
    }

    Self& operator/=(const Self& rhs) {
        // actual addition of rhs to *this
        ASSERT(is_compatible(rhs));
        _arr /= rhs._arr;
        return *this;
    }

    Self& operator/=(const Vt& rhs) {
        // actual addition of rhs to *this
        _arr /= rhs;
        return *this;
    }

    //----------------------------------------------------------------
    bool is_compatible(const Self& o) const{
        return (  _spgrid  == o._spgrid
                &&_spghost == o._spghost
                &&_sporder == o._sporder);
    }
    // return a new scalar with compatible gird, ghost and order
    Self new_compatible() const{
        Self res(this->_grid, this->_ghost, this->_order);
        return res;
    }

    SField_(){}

    virtual ~SField_(){}

    virtual Grid&  grid(){return *_spgrid;};
    virtual Ghost& ghost(){return *_spghost;};
    virtual Order& order(){return *_sporder;};

};

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SField_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SField_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator+(
    SField_<DIM, VT, GRID, GHOST, ORDER> lhs, const Vt& rhs){
    lhs += rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator+(
    const Vt& lhs, SField_<DIM, VT, GRID, GHOST, ORDER> rhs){
    rhs += lhs;
    return rhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator-(
          SField_<DIM, VT, GRID, GHOST, ORDER> lhs, 
    const SField_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs -= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator-(
        SField_<DIM, VT, GRID, GHOST, ORDER> lhs, const Vt& rhs){
    lhs -= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator-(
        const Vt& lhs, SField_<DIM, VT, GRID, GHOST, ORDER> rhs){
    rhs = -rhs + lhs;
    return rhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator*(
          SField_<DIM, VT, GRID, GHOST, ORDER> lhs, 
    const SField_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs *= rhs;
      return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator*(
        SField_<DIM, VT, GRID, GHOST, ORDER> lhs,
        const Vt& rhs){
    lhs *= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator*(
        const Vt& lhs, 
        SField_<DIM, VT, GRID, GHOST, ORDER> rhs){
    rhs *= lhs;
    return rhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator/(
          SField_<DIM, VT, GRID, GHOST, ORDER> lhs, 
    const SField_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs /= rhs;
      return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator/(
    SField_<DIM, VT, GRID, GHOST, ORDER> lhs, const Vt& rhs){
    lhs /= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> operator/(
    const Vt& lhs, 
    const SField_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    SField_<DIM, VT, GRID, GHOST, ORDER> res(rhs);
    res.assign(lhs);
    res /= rhs;
    return res;
}
// a^2
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
SField_<DIM, VT, GRID, GHOST, ORDER> Square(
    const SField_<DIM, VT, GRID, GHOST, ORDER>& a){
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        res(idx)= va * va;
    }
    return res;
}


// a^2 + b^2
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
SField_<DIM, VT, GRID, GHOST, ORDER> SquareSum(
    const SField_<DIM, VT, GRID, GHOST, ORDER>& a, 
    const SField_<DIM, VT, GRID, GHOST, ORDER>& b){
    ASSERT(a.is_compatible(b));
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        auto vb = b(idx);
        res(idx)= va * va + vb * vb;
    }
    return res;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
SField_<DIM, VT, GRID, GHOST, ORDER> SquareSum(
                       const SField_<DIM, VT, GRID, GHOST, ORDER>& a,
                       const SField_<DIM, VT, GRID, GHOST, ORDER>& b,
                       const SField_<DIM, VT, GRID, GHOST, ORDER>& c){
    ASSERT(a.is_compatible(b));
    ASSERT(a.is_compatible(c));
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        auto vb = b(idx);
        auto vc = c(idx);
        res(idx)= va * va + vb * vb + vc * vc;
    }
    return res;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
SField_<DIM, VT, GRID, GHOST, ORDER> Sqrt(const SField_<DIM, VT, GRID, GHOST, ORDER>& a){
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        res(idx)= std::sqrt(va);
    }
    return res;
}

}

#endif