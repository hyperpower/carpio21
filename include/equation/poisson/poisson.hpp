#ifndef _POISSON_HPP_
#define _POISOON_HPP_

#include "type_define.hpp"
#include "equation/poisson/laplace.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio {
//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = s  2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = s   3D --version
template<class D>
class Poisson_: public Laplace_<D>{
public:
    typedef D Domain;
    typedef Poisson_<D>        Self;
    typedef Laplace_<D>        Base;
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

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)> FunXYZ_Value;

public:
    Poisson_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("source");
    }

    virtual std::string name() const{
        return "EquationPoisson";
    };

    virtual ~Poisson_(){};

    virtual int run_one_step(St step, Vt time){
        return 0;
    };

    virtual int initialize(){
        Base::initialize();
        return 0;
    }; 

    virtual int finalize(){
        return 0;
    }; 

    virtual int solve(){
        if(this->has_config("space_scheme")){
            auto m = any_cast<std::string>(this->_configs["space_scheme"]);
            if(m == "finite_volume_2"){
                return _solve_finite_volume_2();
            }
            if(m == "finite_difference_2"){
                return _solve_finite_difference_2();
            }
            return 0;
        } else{
            return _solve_finite_volume_2();
        }
    }
    
    void set_phi(spFieldCenter spphi){
        this->_fields["phi"] = spphi;
    }
    void set_source(spFieldCenter spsource){
        this->_fields["source"] = spsource;
    }

    void set_source(FunXYZT_Value fun){
        auto& source = *(this->_fields["source"]);
        source.assign(fun);
    }
    void set_source(FunXYZ_Value fun){
        auto& source = *(this->_fields["source"]);
        source.assign(fun);
    }
protected:
    int _solve_finite_volume_2(){
        FieldCenter&    phi  = *(this->_fields["phi"]);
        auto spsolver = any_cast<spSolver>(this->_configs["solver"]);
        auto expf     = this->new_field_exp();
        auto bis      = this->get_boundary_index("phi");

        auto res = IntLaplacian((*expf), (*bis)) - IntVolume(*(this->_fields["source"]));

        // typename Domain::Index itest(0,9);
        // std::cout << "res(" << itest << ") = " << res(itest) << std::endl;

        Mat a;
        Arr b;
        BuildMatrix(res, a, b);
        // prepare x
        Arr x = phi.to_array();
        this->_configs["solver_return_code"] = spsolver->solve(a, x, b);
        phi.assign(x);
        return any_cast<int>(this->_configs["solver_return_code"]);
    }

    int _solve_finite_difference_2(){
        FieldCenter&    phi  = *(this->_fields["phi"]);
        auto spsolver = any_cast<spSolver>(this->_configs["solver"]);
        auto expf     = this->new_field_exp();
        auto bis      = this->get_boundary_index("phi");

        auto res = Laplacian((*expf), (*bis)) - (*(this->_fields["source"]));

        Mat a;
        Arr b;
        BuildMatrix(res, a, b);
        // prepare x
        Arr x = phi.to_array();
        this->_configs["solver_return_code"] = spsolver->solve(a, x, b);
        phi.assign(x);
        return any_cast<int>(this->_configs["solver_return_code"]);
    }
};


}

#endif