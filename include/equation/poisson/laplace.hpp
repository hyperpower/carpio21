#ifndef _LAPLACE_HPP_
#define _LAPLACE_HPP_

#include "type_define.hpp"
#include "equation/equation_base.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio {
//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = 0   2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = 0   3D --version
template<St DIM, class D>
class Laplace_: public EquationBase_<DIM, D>{
public:
    typedef D Domain;
    typedef Laplace_<DIM, D>             Self;
    typedef EquationBase_<DIM, D>        Base;
    typedef typename Domain::ValueType   Vt;
    typedef typename Domain::Index       Index;
    typedef typename Domain::Grid         Grid;
    typedef typename Domain::spGrid      spGrid;
    typedef typename Domain::Ghost       Ghost;
    typedef typename Domain::spGhost     spGhost;
    typedef typename Domain::Order       Order;
    typedef typename Domain::spOrder     spOrder;
    typedef typename Domain::FieldCenter FieldCenter;
public:
    Laplace_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
    }

    virtual ~Laplace_(){};

    virtual int run_one_step(St step){
        return 0;
    };

    virtual int initialize(){
        return 0;
    }; 

    virtual int finalize(){
        return 0;
    }; 

    virtual int solve(){
        return 0;
    };

    virtual void run_events(St step, Vt t, int fob) {
        for (auto& event : this->_events) {
            if (event.second->do_execute(step, t, fob)) {
                event.second->execute(step, t, fob, this);
            }
        }
    }
        



};


}

#endif