#ifndef S_FIELD_CENTER_HPP
#define S_FIELD_CENTER_HPP

#include "sfield.hpp"

namespace carpio{

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SFieldCenter_: public SField_<DIM, VT, GRID, GHOST, ORDER>{
public:
    static const St Dim = DIM;

    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef StructureType TraitType;
    typedef SField_<Dim, VT, GRID, GHOST, ORDER> Base;
    typedef SFieldCenter_<Dim, VT, GRID, GHOST, ORDER> Self;
    typedef typename Grid::Index Index;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef ArrayListV_<ValueType> Arr; 
public:
    SFieldCenter_(spGrid spg, spGhost spgh):
        _spgrid(spg), _spghost(spgh){
        // Initall a default order_xyz
        this->_sporder = spOrder(new Order(spg, spgh));
        _initial_arr();
    }
    SFieldCenter_(spGrid spg, spGhost spgh, spOrder spo)
        :Base(spg, spgh, spo){
    }
    SFieldCenter_(const Self& other): Base(other){
    }
    SFieldCenter_(const Base& b) :Base(b) {}
    SFieldCenter_(Base&& b) :Base(b) {}

    Self& operator=(const Self& other) {
         if (this == &other) {
            return *this;
        }
        ASSERT(this->is_compatible(other));
        this->_arr = other._arr;
        
        return *this;
    }

    // ===========================================
    // arithmatic operator
    // ===========================================
    Self operator-() const{
        return Self(-Base(*this));
    }
protected:
    void _initial_arr(){
        // make data by order
        _arr.reconstruct(_sporder->size());
    }
};

}

#endif