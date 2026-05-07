#ifndef S_FIELD_OBJECT_HPP
#define S_FIELD_OBJECT_HPP

#include "domain/domain_define.hpp"
#include "algebra/algebra.hpp"
#include "domain/structure/structure_define.hpp"

namespace carpio{


struct SFieldObjectTag: public StructureTag, public FieldBaseTag{};

template<St DIM,
         class VT,
         class GRID,
         class GHOST,
         class ORDER>
class SFieldObject_: public FieldBase_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef SFieldObjectTag Tag;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef typename GRID::Tag  GridTag;
    typedef typename GHOST::Tag GhostTag;
    typedef typename ORDER::Tag OrderTag;

    typedef VT    ValueType;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;
    typedef FieldBase_<Dim, VT, GRID, GHOST, ORDER> Base;
    typedef SFieldObject_<Dim, VT, GRID, GHOST, ORDER> Self;
    typedef typename Grid::Index Index;

    typedef _DataInitial_<Dim, VT, GRID, GHOST, ORDER> _DataInit;

    typedef ArrayListT_<ValueType> Arr;
protected:
    // for saving data
    Arr _arr;
public:
    SFieldObject_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
        _initial_arr();
    }
    SFieldObject_(const Self& other):
        Base(other),
        _arr(other._arr){}

    SFieldObject_(Self&& other):
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

    ValueType& operator()(const Index& index) {
        return this->_arr[this->_sporder->get_order(index)];
    }

    const ValueType& operator()(const Index& index) const {
        return this->_arr[this->_sporder->get_order(index)];
    }

    virtual void assign(const ValueType& v){
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

    SFieldObject_():Base(){}

    ~SFieldObject_(){}

protected:
    void _initial_arr(){
        this->_arr.reconstruct(this->_sporder->size());
        for(auto& idx : this->order()){
            this->operator()(idx) = ValueType();
        }
    }
};


}

#endif
