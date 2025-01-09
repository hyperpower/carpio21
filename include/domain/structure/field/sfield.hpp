#ifndef S_FIELD_HPP
#define S_FIELD_HPP

#include "domain/domain_define.hpp"
#include "algebra/algebra.hpp"
#include "domain/structure/structure_define.hpp"

namespace carpio{

struct SFieldTag: public StructureTag, public FieldBaseTag{};

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SField_: public FieldBase_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef SFieldTag Tag;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef FieldBase_<Dim, VT, GRID, GHOST, ORDER> Base;
    typedef SField_<Dim, VT, GRID, GHOST, ORDER> Self;
    typedef typename Grid::Index Index;


    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef ArrayListV_<ValueType> Arr; 
    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
protected:
    // for saving data
    Arr _arr;
public:
    SField_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
    }
    SField_(const Self& other): 
        Base(other) , 
        _arr(other._arr){}
    
    SField_(Self&& other): 
        Base(std::move(other)), 
        _arr(std::move(other._arr)){}

    Self& operator=(const Self& other) {
        if (this == &other) {
            return *this;
        }
        ASSERT(this->is_compatible(other));
        this->_arr = other._arr;
        return *this;
    }

    Self& operator=(Self&& other){
        if (this == &other){
            return *this;
        }
        ASSERT(this->is_compatible(other));
        this->_arr = std::move(other._arr);
        return *this;
    }
    
    virtual void assign(const Vt& v){
        _arr.assign(v);
    }
    virtual void assign(const Arr& other){
        ASSERT(this->_sporder->size() == other.size());
        this->_arr = other;
    }
    
    Arr to_array() const{
        return Arr(_arr);
    }

    Arr& data(){
        return this->_arr;
    }
    const Arr& data() const{
        return this->_arr;
    }
    
    // ===========================================
    // arithmatic operator
    // ===========================================
    // Self operator-() const{
    //     Self res(*this);
    //     res._arr = -(this->_arr);
    //     return res;
    // }

    Self& operator+=(const Self& rhs){
        // actual addition of rhs to *this
        ASSERT(this->is_compatible(rhs));
        _arr += rhs._arr;
        return *this;
    }
    template<class VT2>
    Self& operator+=(const SField_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        ASSERT(this->is_compatible(rhs.spgrid(), rhs.spghost(), rhs.sporder()));
        _arr += rhs.data();
        return *this;
    }

    Self& operator+=(const Vt& rhs){
        _arr += rhs;
        return *this;
    }

    Self& operator-=(const Self& rhs) {
        ASSERT(is_compatible(rhs));
        _arr -= rhs._arr;
        return *this;
    }
    template<class VT2>
    Self& operator-=(const SField_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        ASSERT(this->is_compatible(rhs.spgrid(), rhs.spghost(), rhs.sporder()));
        _arr -= rhs.data();
        return *this;
    }
    Self& operator-=(const Vt& rhs) {
        _arr -= rhs;
        return *this;
    }

    Self& operator*=(const Self& rhs) {
        ASSERT(this->is_compatible(rhs));
        _arr *= rhs._arr;
        return *this;
    }

    Self& operator*=(const Vt& rhs) {
        _arr *= rhs;
        return *this;
    }
    template<class VT2>
    Self& operator*=(const SField_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        ASSERT(this->is_compatible(rhs.spgrid(), rhs.spghost(), rhs.sporder()));
        _arr *= rhs.data();
        return *this;
    }
    Self& operator/=(const Self& rhs) {
        ASSERT(this->is_compatible(rhs));
        _arr /= rhs._arr;
        return *this;
    }

    Self& operator/=(const Vt& rhs) {
        _arr /= rhs;
        return *this;
    }

    SField_():Base(){}

    ~SField_(){}
};

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SField_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SField_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SField_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SField_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SField_<DIM, VT, GRID, GHOST, ORDER> 
operator+(SField_<DIM, VT, GRID, GHOST, ORDER> lhs, const Vt& rhs){
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

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class _DataInitial_{
public:
    static void InitAValue(){
        std::cout << "Abstract" << std::endl;
    }
};

template<St DIM, 
         class GRID, 
         class GHOST, 
         class ORDER>
class _DataInitial_<DIM, Vt, GRID, GHOST, ORDER>{
public:
    typedef typename GRID::Index  Index;

    static Vt InitAValue(const Index&){
        return 0.0;
    }
};

template<St DIM, 
         class GRID, 
         class GHOST, 
         class ORDER>
class _DataInitial_<DIM, 
                    LinearPolynomial_<Vt, typename GRID::Index>,
                    GRID, GHOST, ORDER>{
public:
    typedef LinearPolynomial_<Vt, typename GRID::Index>  Poly;
    typedef typename GRID::Index  Index;
    static Poly InitAValue(const Index& index){
        return Poly();
    }
};

}

#endif