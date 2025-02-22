#ifndef S_FIELD_VERTEX_HPP
#define S_FIELD_VERTEX_HPP

#include "domain/structure/field/sfield.hpp"

namespace carpio{

struct SFieldVertexTag: public SFieldTag{};


template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SFieldVertex_: public SField_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef SFieldVertexTag Tag;
    typedef SFieldVertex_<Dim, VT, GRID, GHOST, ORDER> Self;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef SField_<Dim, VT, GRID, GHOST, ORDER> Base;
    typedef typename Grid::Index Index;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef ArrayListV_<ValueType> Arr; 

    typedef _DataInitial_<Dim, VT, GRID, GHOST, ORDER> _DataInit;
    typedef typename _DataInit::ValueTag ValueTag;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;
protected:

public:
    SFieldVertex_(spGrid spg, spGhost spgh):{
        this->_spgrid  = spg;
        this->_spghost = spgh; 
        // Initall a default order_xyz
        this->_sporder = spOrder(new Order(spg, spgh, af));
        _initial_arr();
    }
    SFieldVertex_(spGrid spg, spGhost spgh, spOrder spo, Axes axe)
        :Base(spg, spgh, spo){
        _initial_arr();
    }
    SFieldVertex_(const Self&  other): Base(other),_axe(other._axe){}
    SFieldVertex_(      Self&& other): Base(std::move(other)), _axe(std::move(other._axe)){}
    SFieldVertex_(const Base&  b) :Base(b) {}
    SFieldVertex_(      Base&& b) :Base(std::move(b)) {}

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
    
    ValueType&      operator()(const Orientation& o, St i, St j = 0, St k = 0){
        Index fidx(i,j,k);
        return this->operator()(o, fidx);
    }

    const ValueType& operator()(const Orientation& o, St i, St j = 0, St k = 0) const{
        Index fidx(i,j,k);
        return this->operator()(o, fidx);
    }

    ValueType& operator()(const Orientation& o, const Index& index) {
        auto fidx    = this->_spgrid->cell_index_to_face_index(index, o, this->_axe);
        auto arr_idx = this->_sporder->get_order_face_index(fidx, this->_axe);
        return this->_arr[arr_idx];
    }

    const ValueType& operator()(const Orientation& o, const Index& index) const {
        auto fidx    = this->_spgrid->cell_index_to_face_index(index, o, this->_axe);
        auto arr_idx = this->_sporder->get_order_face_index(fidx, this->_axe);
        return this->_arr[arr_idx];
    }
    
    ValueType& operator()(const Index& findex) {
        auto arr_idx = this->_sporder->get_order_face_index(findex, this->_axe);
        return this->_arr[arr_idx];
    }

    const ValueType& operator()(const Index& findex) const {
        auto arr_idx = this->_sporder->get_order_face_index(findex, this->_axe);
        return this->_arr[arr_idx];
    }

    Axes face_axe() const{
        return _axe;
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
    Self& operator+=(const Vt& rhs){
        Base::operator+=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator+=(const SFieldVertex_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator+=(rhs);
        return *this;
    }
    Self& operator-=(const Self& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    Self& operator-=(const Vt& rhs){
        Base::operator-=(rhs);
        return *this;
    }
    template<class VT2>
    Self& operator-=(const SFieldVertex_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
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
    template<class VT2>
    Self& operator*=(const SFieldVertex_<Dim, VT2, GRID, GHOST, ORDER>& rhs){
        Base::operator*=(rhs);
        return *this;
    }
    Self& operator/=(const Self& rhs){
        Base::operator/=(rhs);
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
        for(auto& fidx : (*this->_sporder)){
            auto cidx = this->_spgrid->face_index_to_cell_index(fidx, this->_axe);
            auto cp = this->_spgrid->c(cidx);
            auto fp = this->_spgrid->f(this->_axe, _M_, cidx);
            auto value = fun(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), t);
            this->operator ()(_M_, cidx) = value;
            if(this->_spgrid->is_last(cidx, this->_axe)){
                auto fp = this->_spgrid->f(this->_axe, _P_, cidx);
                auto value = fun(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), t);
                this->operator ()(_P_, cidx) = value;
            }
        }
    }
    void assign(FunXYZ_Value fun){
        for(auto& fidx : (*this->_sporder)){
            auto cidx = this->_spgrid->face_index_to_cell_index(fidx, this->_axe);
            auto cp = this->_spgrid->c(cidx);
            auto fp = this->_spgrid->f(this->_axe, _M_, cidx);
            auto value = fun(fp.value(_X_), fp.value(_Y_), fp.value(_Z_));
            this->operator ()(_M_, cidx) = value;
            if(this->_spgrid->is_last(cidx, this->_axe)){
                auto fp = this->_spgrid->f(this->_axe, _P_, cidx);
                auto value = fun(fp.value(_X_), fp.value(_Y_), fp.value(_Z_));
                this->operator ()(_P_, cidx) = value;
            }
        }
    }
    // return a new scalar with compatible gird, ghost and order
    Self new_compatible_zero() const{
        Self res(this->_spgrid, this->_spghost, this->_sporder, this->_axe);
        return res;
    }
protected:
    void _initial_arr(){
        // make data by order
        this->_arr.reconstruct(this->_sporder->size());
        for(auto& idx : (*(this->_sporder))){
            auto arr_idx = this->_sporder->get_order_face_index(idx);
            this->_arr[arr_idx] = _DataInit::InitZero(idx);
        }
    }
};

template<St DIM, class VT, 
         class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}
template<St    DIM,  class VT,    class VT2,
         class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator+(      SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>   lhs, 
          const SFieldVertex_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs += rhs;
    return lhs;
}
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator-(      SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>   lhs, 
          const SFieldVertex_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs -= rhs;
    return lhs;
}
template<St DIM, class VT, class VT2, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator*(      SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>   lhs, 
          const SFieldVertex_<DIM, VT2, GRID, GHOST, ORDER>& rhs){
    lhs *= rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator*(      SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs *= rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator*(      SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> lhs, 
          const VT& rhs){
    lhs *= rhs;
    return lhs;
}
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator/(
          SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> lhs, 
    const SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    lhs /= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>
operator/(
    SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> lhs, const Vt& rhs){
    lhs /= rhs;
    return lhs;
}

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
inline SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> operator/(
    const Vt& lhs, 
    const SFieldVertex_<DIM, VT, GRID, GHOST, ORDER>& rhs){
    SFieldVertex_<DIM, VT, GRID, GHOST, ORDER> res(rhs);
    res.assign(lhs);
    res /= rhs;
    return res;
}


}

#endif