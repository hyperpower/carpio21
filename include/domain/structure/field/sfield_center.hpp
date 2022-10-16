#ifndef S_FIELD_CENTER_HPP
#define S_FIELD_CENTER_HPP

#include "domain/structure/field/sfield.hpp"

namespace carpio{

struct SFieldCenterTag: public SFieldTag{};

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SFieldCenter_: public SField_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef SFieldCenterTag Tag;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef SField_<Dim, VT, GRID, GHOST, ORDER> Base;
    typedef SFieldCenter_<Dim, VT, GRID, GHOST, ORDER> Self;
    typedef typename Grid::Index Index;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef ArrayListV_<ValueType> Arr; 

    typedef _DataInitial_<Dim, VT, GRID, GHOST, ORDER> _DataInit;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;
public:
    SFieldCenter_(spGrid spg, spGhost spgh){
        this->_spgrid  = spg;
        this->_spghost = spgh; 
        // Initall a default order_xyz
        this->_sporder = spOrder(new Order(spg, spgh));
        _initial_arr();
    }
    SFieldCenter_(spGrid spg, spGhost spgh, spOrder spo)
        :Base(spg, spgh, spo){
        _initial_arr();
    }
    SFieldCenter_(const Self&  other): Base(other){}
    SFieldCenter_(      Self&& other): Base(std::move(other)){}
    SFieldCenter_(const Base&  b) :Base(b) {}
    SFieldCenter_(      Base&& b) :Base(std::move(b)) {}

    Self& operator=(const Self& other) {
        if (this == &other) {
            return *this;
        }
        ASSERT(this->is_compatible(other));
        this->_arr = other._arr;
        return *this;
    }
    Self& operator=(Self&& other) {
        if (this == &other) {
            return *this;
        }
        ASSERT(this->is_compatible(other));
        this->_arr = std::move(other._arr);
        return *this;
    }
    
    ValueType&      operator()(St i, St j = 0, St k = 0){
        return this->_arr[this->_sporder->get_order(i,j,k)];
    }

    const ValueType& operator()(St i, St j = 0, St k = 0) const{
        return this->_arr[this->_sporder->get_order(i,j,k)];
    }

    ValueType& operator()(const Index& index) {
        return this->_arr[this->_sporder->get_order(index)];
    }

    const ValueType& operator()(const Index& index) const {
        return this->_arr[this->_sporder->get_order(index)];
    }
    // ===========================================
    // arithmatic operator
    // ===========================================
    Self operator-() const{
        return Self(-(Base(*this)));
    }
    Self& operator+=(const Self& rhs){
        Base::operator+=(Base(rhs));
        return *this;
    }
    Self& operator+=(const Vt& rhs){
        Base::operator+=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator+=(const SFieldCenter_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator+=(rhs);
        return *this;
    }
    Self& operator-=(const Self& rhs){
        Base::operator-=(Base(rhs));
        return *this;
    }
    Self& operator-=(const Vt& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator-=(const SFieldCenter_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    Self& operator*=(const Self& rhs){
        Base::operator*=(Base(rhs));
        return *this;
    }
    Self& operator*=(const Vt& rhs){
        Base::operator*=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator*=(const SFieldCenter_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator*=(rhs);
        return *this;
    }
    Self& operator/=(const Self& rhs){
        Base::operator/=(Base(rhs));
        return *this;
    }
    Self& operator/=(const Vt& rhs){
        Base::operator/=(rhs);
        return *this;
    }
    virtual void assign(const Vt& v){
        Base::assign(v);
    }

    virtual void assign(const Arr& other){
        Base::assign(other);
    }
    void assign(FunXYZT_Value fun, Vt t = 0.0){
        for(auto& idx : (*this->_sporder)){
            auto cp = this->_spgrid->c(idx);
            this->operator ()(idx) = fun(cp.value(_X_),
                                         cp.value(_Y_),
                                         cp.value(_Z_), t);
        }
    }
    void assign(FunXYZ_Value fun){
        for(auto& idx : (*this->_sporder)){
            auto cp = this->_spgrid->c(idx);
            this->operator ()(idx) = fun(cp.value(_X_),
                                         cp.value(_Y_),
                                         cp.value(_Z_));
        }
    }
    // return a new scalar with compatible gird, ghost and order
    Self new_compatible() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder);
        return res;
    }
    Self new_inverse_volume() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder);
        for(auto& idx : (*this->_sporder)){
            res(idx) = 1.0 / res.grid().volume(idx);
        }
        return res;
    }
    Self new_volume() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder);
        for(auto& idx : (*this->_sporder)){
            res(idx) = res.grid().volume(idx);
        }
        return res;
    }
protected:
    void _initial_arr(){
        // make data by order
        this->_arr.reconstruct(this->_sporder->size());
        for(auto& idx : (*(this->_sporder))){
            this->operator()(idx) = _DataInit::InitAValue(idx);
        }
    }
};

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>   lhs, 
          const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator-(      SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>   lhs, 
          const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs -= rhs;
    return lhs;
}
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator*(      SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>   lhs, 
          const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs *= rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator*(      SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs *= rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator*(      SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const VT& rhs){
    lhs *= rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator/(
          SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> lhs, 
    const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs /= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>
operator/(
    SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> lhs, const Vt& rhs){
    lhs /= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> operator/(
    const Vt& lhs, 
    const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> res(rhs);
    res.assign(lhs);
    res /= rhs;
    return res;
}


}

#endif