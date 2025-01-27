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
    typedef typename _DataInit::ValueTag ValueTag;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;
    typedef std::function<ValueType(const Index&)>   FunIndex_Value;
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
    SFieldCenter_(spGrid spg, spGhost spgh, spOrder spo, FunIndex_Value init_fun)
        :Base(spg, spgh, spo){
        _initial_arr(init_fun);
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
        Base::operator+=(rhs);
        return *this;
    }
    Self& operator+=(const VT& rhs){
        Base::operator+=(rhs);
        return *this;
    }
    template<class VT2, 
             typename = std::enable_if_t<std::is_arithmetic<VT2>::value> >
    Self& operator+=(const SFieldCenter_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator+=(rhs);
        return *this;
    }
    Self& operator-=(const Self& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    Self& operator-=(const VT& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator-=(const SFieldCenter_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    Self& operator*=(const Self& rhs){
        Base::operator*=(rhs);
        return *this;
    }
    Self& operator*=(const Vt& rhs){
        Base::operator*=(rhs);
        return *this;
    }
    template<class VT2,
             typename = std::enable_if_t<std::is_arithmetic<VT2>::value> >
    Self& operator*=(const SFieldCenter_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator*=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator*=(const VT2& rhs){
        Base::operator*=(rhs);
        return *this;
    }

    Self& operator/=(const Self& rhs){
        Base::operator/=(rhs);
        return *this;
    }
    Self& operator/=(const VT& rhs){
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
    void assign(FunIndex_Value fun){
        for(auto& idx : (*this->_sporder)){
            this->operator ()(idx) = fun(idx);
        }
    }
    // return a new scalar with compatible gird, ghost and order
    Self new_compatible_zero() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder);
        return res;
    }
    Self new_compatible_coe_one() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder);
        for(auto& idx : res.order()){
            res(idx) = _DataInit::InitCoeOne(idx);
        }
        return res;
    }
    Self new_inverse_volume() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder);
        for(auto& idx : (*this->_sporder)){
            res(idx) = 1.0 / res.grid().volume(idx);
        }
        return res;
    }
    Self new_inverse() const{
        Self res(*this);
        for(auto& idx : (*this->_sporder)){
            res(idx) = 1.0 / res(idx);
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
        for(auto& idx : this->order()){
            this->operator()(idx) = _DataInit::InitAValue(idx);
        }
    }
    void _initial_arr(FunIndex_Value fun){
        // make data by order
        this->_arr.reconstruct(this->_sporder->size());
        for(auto& idx : this->order()){
            this->operator()(idx) = fun(idx);
        }
    }
};
// ----------------------
//   Add 
// ----------------------
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
auto operator+(const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>&  lhs,
               const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> FieldLeft;
    typedef SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER> FieldRight;
    typedef typename FieldLeft::Tag       TagLeft;
    typedef typename FieldLeft::ValueTag  ValueTagLeft;
    typedef typename FieldRight::Tag      TagRight;
    typedef typename FieldRight::ValueTag ValueTagRight;
    return Add(lhs, rhs, 
                    TagLeft(), TagRight(), ValueTagLeft(), ValueTagRight());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator+(const VALUE& lhs,
               const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag = typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Add(lhs, rhs, 
               ArithmeticTag(), SFieldCenterTag(), VTag(), ValueTag());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator+( const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& lhs,
                const VALUE& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag = typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Add(lhs, rhs, 
               Tag(), ArithmeticTag(), ValueTag(), VTag());
}
template<class LEFT, class RIGHT>
LEFT Add(LEFT lhs, const RIGHT& rhs, 
         SFieldCenterTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    lhs += rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
LEFT Add(LEFT lhs, const RIGHT& rhs, 
         SFieldCenterTag, SFieldCenterTag, LinearPolynomialTag, ArithmeticTag){
    lhs += rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Add(const LEFT& lhs, RIGHT rhs, 
          SFieldCenterTag, SFieldCenterTag, ArithmeticTag, LinearPolynomialTag ){
    rhs += lhs;
    return rhs;
}
template<class LEFT, class RIGHT>
RIGHT Add(const LEFT& lhs, RIGHT rhs, 
          SFieldCenterTag, SFieldCenterTag, LinearPolynomialTag, LinearPolynomialTag ){
    rhs += lhs;
    return rhs;
}
template<class LEFT, class RIGHT>
LEFT Add(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, ArithmeticTag, ArithmeticTag){
    lhs += rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Add(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    rhs += lhs;
    return rhs;
}
template<class LEFT, class RIGHT>
LEFT Add(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, LinearPolynomialTag, ArithmeticTag){
    lhs += rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Add(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, LinearPolynomialTag){
    rhs += lhs;
    return rhs;
}
template<class LEFT, class RIGHT>
LEFT Add(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, LinearPolynomialTag, LinearPolynomialTag){
    lhs += rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Add(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, LinearPolynomialTag, LinearPolynomialTag){
    rhs += lhs;
    return rhs;
}
// ----------------------
//   Minus
// ----------------------
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
auto operator-(const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>&  lhs,
               const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> FieldLeft;
    typedef SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER> FieldRight;
    typedef typename FieldLeft::Tag       TagLeft;
    typedef typename FieldLeft::ValueTag  ValueTagLeft;
    typedef typename FieldRight::Tag      TagRight;
    typedef typename FieldRight::ValueTag ValueTagRight;
    return Minus(lhs, rhs, 
                    TagLeft(), TagRight(), ValueTagLeft(), ValueTagRight());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator-(const VALUE& lhs,
               const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag =typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Minus(lhs, rhs, 
               ArithmeticTag(), SFieldCenterTag(), VTag(), ValueTag());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator-( const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& lhs,
                const VALUE& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag =typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Minus(lhs, rhs, 
               Tag(), ArithmeticTag(), ValueTag(), VTag());
}
template<class LEFT, class RIGHT>
LEFT Minus(LEFT lhs, const RIGHT& rhs, 
         SFieldCenterTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    lhs -= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
LEFT Minus(LEFT lhs, const RIGHT& rhs, 
         SFieldCenterTag, SFieldCenterTag, LinearPolynomialTag, ArithmeticTag){
    lhs -= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Minus(const LEFT& lhs, RIGHT rhs, 
          SFieldCenterTag, SFieldCenterTag, ArithmeticTag, LinearPolynomialTag ){
    return (-rhs) + lhs;
}
template<class LEFT, class RIGHT>
LEFT Minus(LEFT lhs, const RIGHT& rhs, 
          SFieldCenterTag, SFieldCenterTag, LinearPolynomialTag, LinearPolynomialTag ){
    lhs -= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
LEFT Minus(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, ArithmeticTag, ArithmeticTag){
    lhs -= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Minus(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    return (-rhs) + lhs;
}
template<class LEFT, class RIGHT>
LEFT Minus(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, LinearPolynomialTag, ArithmeticTag){
    lhs -= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Minus(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, LinearPolynomialTag){
    return (-rhs) + lhs;
}
template<class LEFT, class RIGHT>
LEFT Minus(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, LinearPolynomialTag, LinearPolynomialTag){
    lhs -= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Minus(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, LinearPolynomialTag, LinearPolynomialTag){
    return (-rhs) + lhs;
}
// ----------------------
//   Multiply
// ----------------------
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
auto operator*(const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>&  lhs,
               const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> FieldLeft;
    typedef SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER> FieldRight;
    typedef typename FieldLeft::Tag       TagLeft;
    typedef typename FieldLeft::ValueTag  ValueTagLeft;
    typedef typename FieldRight::Tag      TagRight;
    typedef typename FieldRight::ValueTag ValueTagRight;
    return Multiply(lhs, rhs, 
                    TagLeft(), TagRight(), ValueTagLeft(), ValueTagRight());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator*(const VALUE& lhs,
               const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag = typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Multiply(lhs, rhs, 
                    ArithmeticTag(), SFieldCenterTag(), VTag(), ValueTag());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator*( const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& lhs,
                const VALUE& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag =typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Multiply(lhs, rhs, 
                     Tag(), ArithmeticTag(), ValueTag(), VTag());
}
template<class LEFT, class RIGHT>
LEFT Multiply(LEFT lhs, const RIGHT& rhs, 
               SFieldCenterTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    lhs *= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
LEFT Multiply(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, SFieldCenterTag, LinearPolynomialTag, ArithmeticTag){
    lhs *= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Multiply(const LEFT& lhs, RIGHT rhs, 
              SFieldCenterTag, SFieldCenterTag, ArithmeticTag, LinearPolynomialTag ){
    rhs *= lhs;
    return rhs;
}
template<class LEFT, class RIGHT>
LEFT Multiply(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, ArithmeticTag, ArithmeticTag){
    lhs *= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Multiply(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    rhs *= lhs;
    return rhs;
}
template<class LEFT, class RIGHT>
LEFT Multiply(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, LinearPolynomialTag, ArithmeticTag){
    lhs *= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Multiply(const LEFT lhs, RIGHT rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, LinearPolynomialTag){
    rhs *= lhs;
    return rhs;
}


// ----------------------
//   Divide
// ----------------------
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
auto operator/(const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>&  lhs,
               const SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> FieldLeft;
    typedef SFieldCenter_<DIM, VT2, GRID, GHOST, ORDER> FieldRight;
    typedef typename FieldLeft::Tag       TagLeft;
    typedef typename FieldLeft::ValueTag  ValueTagLeft;
    typedef typename FieldRight::Tag      TagRight;
    typedef typename FieldRight::ValueTag ValueTagRight;
    return Divide(lhs, rhs, 
                    TagLeft(), TagRight(), ValueTagLeft(), ValueTagRight());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator/(const VALUE& lhs,
               const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag = typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Divide(lhs, rhs, 
                    ArithmeticTag(), SFieldCenterTag(), VTag(), ValueTag());
}
template<class VALUE, St DIM, class VT, class GRID, class GHOST, class ORDER>
auto operator/( const SFieldCenter_<DIM, VT, GRID, GHOST, ORDER>& lhs,
                const VALUE& rhs){
    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef typename Field::ValueTag ValueTag;
    typedef typename Field::Tag      Tag;
    using VTag = typename std::conditional<
        !HasTag<VALUE>::value && std::is_arithmetic<VALUE>::value, 
        ArithmeticTag , 
        LinearPolynomialTag>::type;
    return Divide(lhs, rhs, 
                     Tag(), ArithmeticTag(), ValueTag(), VTag());
}
template<class LEFT, class RIGHT>
LEFT Divide(LEFT lhs, const RIGHT& rhs, 
               SFieldCenterTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    lhs /= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
LEFT Divide(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, SFieldCenterTag, LinearPolynomialTag, ArithmeticTag){
    auto inv = rhs.new_inverse();
    return lhs * inv;
}

template<class LEFT, class RIGHT>
LEFT Divide(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, ArithmeticTag, ArithmeticTag){
    lhs /= rhs;
    return lhs;
}
template<class LEFT, class RIGHT>
RIGHT Divide(const LEFT lhs, const RIGHT& rhs, 
              ArithmeticTag, SFieldCenterTag, ArithmeticTag, ArithmeticTag){
    auto inv = rhs.new_inverse();
    return lhs * inv;
}
template<class LEFT, class RIGHT>
LEFT Divide(LEFT lhs, const RIGHT& rhs, 
              SFieldCenterTag, ArithmeticTag, LinearPolynomialTag, ArithmeticTag){
    auto inv = 1.0 / rhs;
    return lhs * inv;
}


}

#endif