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
template<class D>
class Laplace_: public EquationBase_<D>{
public:
    typedef D Domain;
    typedef Laplace_<D>             Self;
    typedef EquationBase_<D>        Base;
    typedef typename Domain::ValueType   Vt;
    typedef typename Domain::Index       Index;
    typedef typename Domain::Grid         Grid;
    typedef typename Domain::spGrid      spGrid;
    typedef typename Domain::Ghost       Ghost;
    typedef typename Domain::spGhost     spGhost;
    typedef typename Domain::Order       Order;
    typedef typename Domain::spOrder     spOrder;
    typedef typename Domain::FieldCenter FieldCenter;
    typedef std::shared_ptr<FieldCenter> spFieldCenter;

    typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;
   
    typedef Solver_<Vt>             Solver; 
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt> Solver_Jacobi;
    typedef SOR_<Vt>    Solver_SOR;
    typedef CG_<Vt>     Solver_CG;
public:
    Laplace_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("phi");
    }

    virtual std::string name() const{
        return "EquationLaplace";
    };

    virtual ~Laplace_(){};

    virtual int run_one_step(St step){
        return 0;
    };

    virtual int initialize(){
        this->_configs["solver"] = this->_init_solver();
        return 0;
    }; 

    virtual int finalize(){
        return 0;
    }; 

    virtual int solve(){
        FieldCenter&    phi  = *(this->_fields["phi"]);
        auto spsolver = any_cast<spSolver>(this->_configs["solver"]);
        auto expf     = this->new_field_exp();
        auto bis      = this->get_boundary_index("phi");

        auto res = Laplacian((*expf), (*bis));

        Mat a;
        Arr b;
        BuildMatrix(res, a, b);
        // prepare x
        Arr x = phi.to_array();
        this->_configs["solver_return_code"] = spsolver->solve(a, x, b);
        // std::cout << "return code = " << any_cast<int>(this->_configs["solver_return_code"]) << std::endl;
        phi.assign(x);
//        x.show();
        return 0;
    }

    // virtual void run_events(St step, Vt t, int fob) {
    //     for (auto& event : this->_events) {
    //         if (event.second->do_execute(step, t, fob)) {
    //             event.second->execute(step, t, fob, this);
    //         }
    //     }
    // }

    void set_phi(spFieldCenter spphi){
        this->_fields["phi"] = spphi;
    }


};


}

#endif