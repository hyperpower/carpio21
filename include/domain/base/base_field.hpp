#ifndef _BASE_FIELD_HPP
#define _BASE_FIELD_HPP

#include <iostream>
#include "domain/domain_define.hpp"

namespace carpio{

template<St DIM>
class GridBase_{
public:
    static const St Dim = DIM;
public:
    GridBase_(){}

    virtual ~GridBase_(){}
};
template<St DIM>
class GhostBase_{
public:
    static const St Dim = DIM;
public:
    GhostBase_(){};

    virtual ~GhostBase_(){};
};
template<St DIM>
class OrderBase_{
public:
    static const St Dim = DIM;
public:
    OrderBase_(){};

    virtual ~OrderBase_(){};
};

struct FieldBaseTag{};

template<St DIM, 
         class VT, 
         class GRID, 
         class GHOST, 
         class ORDER>
class FieldBase_ {
public:
    static const St Dim = DIM;

    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef FieldBaseTag Tag;
    typedef VT    ValueType;
    typedef typename Grid::Index Index;
    typedef DomainTag TraitTag;
    typedef FieldBase_<Dim, VT, Grid, Ghost, Order> Self;

    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;
    typedef const std::shared_ptr<Grid>  spcGrid;
    typedef const std::shared_ptr<Ghost> spcGhost;
    typedef const std::shared_ptr<Order> spcOrder;

protected:
    spGrid  _spgrid;
    spGhost _spghost;
    spOrder _sporder;
public:
    FieldBase_(){};
    FieldBase_(spGrid spg,spGhost spgh):_spgrid(spg), _spghost(spgh){};
    FieldBase_(spGrid spg,spGhost spgh, spOrder spo):
        _spgrid(spg), _spghost(spgh), _sporder(spo){};
    FieldBase_(const Self& other):
        _spgrid(other._spgrid), 
        _spghost(other._spghost), 
        _sporder(other._sporder){};
    FieldBase_(Self&& other):
        _spgrid(std::move(other._spgrid)), 
        _spghost(std::move(other._spghost)), 
        _sporder(std::move(other._sporder)){
        };

    virtual ~FieldBase_(){};

    virtual St dim(){
        return DIM;
    };

    virtual const ValueType& operator()(const Index&) const = 0;
    virtual       ValueType& operator()(const Index&)       = 0;

    Grid&  grid() {return *_spgrid;};
    const Grid&  grid() const{return *_spgrid;};
    Ghost& ghost(){return *_spghost;};
    const Ghost& ghost() const{return *_spghost;}
    Order& order(){return *_sporder;};
    const Order& order() const{return *_sporder;};
    
    spGrid  spgrid() {return _spgrid;};
    const spGrid  spgrid() const{return _spgrid;};
    spGhost& spghost(){return _spghost;};
    const spGhost& spghost() const{return _spghost;}
    spOrder& sporder(){return _sporder;};
    const spOrder& sporder() const{return _sporder;};
protected:
    void _copy(const Self& other){
        this->_spgrid  = other._spgrid;
        this->_spghost = other._spghost;
        this->_sporder = other._sporder; 
    }
public:
    bool is_compatible(const Self& other) const{
        return (this->_spgrid  == other._spgrid) &&
               (this->_spghost == other._spghost) &&
               (this->_sporder == other._sporder);
    }
    bool is_compatible(spGrid spgrid, spGhost spghost, spOrder sporder) const{
        return (this->_spgrid  == spgrid) &&
               (this->_spghost == spghost) &&
               (this->_sporder == sporder);
    }
    
};
}


#endif