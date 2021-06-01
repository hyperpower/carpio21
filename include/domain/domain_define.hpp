#ifndef _DOMAIN_DEFINE_HPP
#define _DOMAIN_DEFINE_HPP

#include <iostream>
#include "type_define.hpp"

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

struct BaseType;

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
class FieldBase_ {
public:
    static const St Dim = DIM;

    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;
    typedef BaseType TraitType;
public:
    FieldBase_(){}

    virtual ~FieldBase_(){}

    // virtual Grid&  grid(){return Grid();};
    // virtual Ghost& ghost(){return Ghost();};
    // virtual Order& order(){return Order();};

};

template<St DIM, class VT, class GRID, class GHOST, class ORDER, class TRAIT>
class OperatorImplement_{
public:
    OperatorImplement_(){
        std::cout << "Operator Impl" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}
};

// Operator ============================
//
// TRAIT
// - STURCTURE
// - OCTUREE
// - TRIANGLE
// - POLY
template<St DIM, class VT, class GRID, class GHOST, class ORDER, class TRAIT>
class LaplacianImplement_{
public:
    LaplacianImplement_(){
        std::cout << "Laplacian Implement" << std::endl;
    };

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }

    // void set(const int& other){std::cout << "int" << std::endl;}
        
};


}

#endif