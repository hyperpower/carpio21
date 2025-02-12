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
    typedef typename Domain::Grid        Grid;
    typedef typename Domain::spGrid     spGrid;
    typedef typename Domain::Ghost       Ghost;
    typedef typename Domain::spGhost   spGhost;
    typedef typename Domain::Order       Order;
    typedef typename Domain::spOrder   spOrder;
    typedef typename Domain::FieldCenter     FieldCenter;
    typedef typename Domain::spFieldCenter spFieldCenter;
    typedef typename Domain::FieldCenterExp     FieldCenterExp;
    typedef typename Domain::spFieldCenterExp spFieldCenterExp;

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
    Laplace_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("phi");
    }

    virtual std::string name() const{
        return "EquationLaplace";
    };

    virtual ~Laplace_(){};

    virtual int run_one_step(St step, Vt time){
        auto tscheme = any_cast<std::string>(this->_configs["set_time_scheme"]);
        if(tscheme == "explicit"){
            _one_step_explicit(step, time);
        }else if(tscheme == "implicit"){
            _one_step_implicit(step, time);
        }else if(tscheme == "CN"){
            _one_step_cn(step, time);
        }else if(tscheme == "CNgeneral"){
            _one_step_cng(step, time);
        }else{
            std::cout <<" >! Unknown time scheme " << tscheme << std::endl;
            SHOULD_NOT_REACH;
        }
        return -1;
    }

    virtual int initialize(){
        this->_configs["solver"] = this->_init_solver();
        if(this->_time != nullptr){
            // set default time scheme
            if(!this->has_config("set_time_scheme")){
                this->_configs["set_time_scheme"] = std::string("explicit");
            }
            auto& phi = *(this->_fields["phi"]);
            this->_fields["inverse_volume"] = std::make_shared<FieldCenter>(phi.new_inverse_volume());
            
            auto name = any_cast<std::string>(this->_configs["set_time_scheme"]);   
            if(name != "explicit"){
                this->_configs["field_exp_coe_one"] = this->new_field_exp_coe_one();
            }
        }else{ // no time scheme --> slove
            this->_configs["field_exp_coe_one"] = this->new_field_exp_coe_one();
        }
        if(! this->has_config("space_scheme")){  // set default space scheme
            this->set_space_scheme("finite_volume_2");  
        }
        return 0;
    }; 

    virtual int finalize(){
        return 0;
    };

    virtual int solve(){
        auto sscheme = any_cast<std::string>(
            this->_configs["space_scheme"]);
        if(sscheme == "finite_volume_2"){
            return solve_fv2();
        }else if(sscheme == "finite_difference_2"){
            return solve_fd2();
        }else{
            std::cerr << "Wrong space sscheme " << sscheme << std::endl;
            return 1;
        }
    } 

    virtual int solve_fd2(){
        FieldCenter&    phi  = *(this->_fields["phi"]);
        auto expf = any_cast<spFieldCenterExp>(this->_configs["field_exp_coe_one"]);
        auto bis      = this->get_boundary_index("phi");

        auto res = DifferenialLaplacian((*expf), (*bis));

        return this->_build_mat_and_solve(res, phi);
    }    

    virtual int solve_fv2(){
        FieldCenter&    phi  = *(this->_fields["phi"]);
        auto expf = any_cast<spFieldCenterExp>(this->_configs["field_exp_coe_one"]);
        auto bis      = this->get_boundary_index("phi");

        auto res = IntegralLaplacian((*expf), (*bis));

        return this->_build_mat_and_solve(res, phi);
    }

    void set_phi(spFieldCenter spphi){
        this->set_field_center("phi", spphi);
    }

    void set_phi(FunXYZ_Value fun){
        this->set_field_center("phi", fun);
    }

protected:
    virtual int _one_step_explicit(St step, Vt time){
        auto& phi  = *(this->_fields["phi"]);
        auto& invv = *(this->_fields["inverse_volume"]);
        auto  bis  = this->get_boundary_index("phi");
        auto  dt   = this->_time->dt();

        phi = IntegralLaplacian(phi, (*bis), time) * dt * invv + phi;

        return 0;
    }

    // Implicit
    virtual int _one_step_implicit(St step, Vt time){
        auto& phi  = *(this->_fields["phi"]);
        auto& invv = *(this->_fields["inverse_volume"]);
        auto bis      = this->get_boundary_index("phi");

        auto spphif = any_cast<spFieldCenterExp>(this->_configs["field_exp_coe_one"]);
        Vt   dt        = this->_time->dt();

        auto res =  IntegralLaplacian((*spphif), (*bis), time) * dt * invv - (*spphif) + phi;

        return this->_build_mat_and_solve(res, phi);
        
    }

    // Crank–Nicolson method
    virtual int _one_step_cn(St step, Vt time){
        auto& phi  = *(this->_fields["phi"]);
        auto& invv = *(this->_fields["inverse_volume"]);
        auto bis      = this->get_boundary_index("phi");

        auto spphif = any_cast<spFieldCenterExp>(this->_configs["field_exp_coe_one"]);
        Vt   dt        = this->_time->dt();

        auto lap_exp = IntegralLaplacian((*spphif), (*bis), time);
        auto lap_v   = IntegralLaplacian(phi, (*bis), time);

        auto res =  (lap_exp + lap_v) * dt * 0.5 * invv - (*spphif) + phi;

        return this->_build_mat_and_solve(res, phi);
    }

    virtual int _one_step_cng(St step, Vt time){
        auto& phi  = *(this->_fields["phi"]);
        auto& invv = *(this->_fields["inverse_volume"]);
        auto bis      = this->get_boundary_index("phi");

        auto spphif = any_cast<spFieldCenterExp>(this->_configs["field_exp_coe_one"]);
        Vt   dt        = this->_time->dt();
        auto omega    = any_cast<Vt>(this->_configs["cn_omega"]);

        auto lap_exp = IntegralLaplacian((*spphif), (*bis), time) * omega;
        auto lap_v   = IntegralLaplacian(phi, (*bis), time) * (1.0 - omega);

        auto res =  (lap_exp + lap_v) * dt * invv - (*spphif) + phi;

        return this->_build_mat_and_solve(res, phi);
    }

};


}

#endif