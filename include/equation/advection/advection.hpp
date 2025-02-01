#ifndef _ADVECTION_HPP_
#define _ADVECTION_HPP_

#include "type_define.hpp"
#include "equation/equation_define.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio {

//This file use to solve convection equation
// 1 non-conservation from
//
//       d(phi)              d( phi)                2D --version
//    u ------------ +  v -------------  = 0
//          dx                 dy
//
//       d(phi)       d(phi)       d(phi)            3D --version
//    u -------- + v -------  + w -------  = 0
//        dx           dy          dz
//
// 2 conservation from
//
//      d( u phi)            d( v phi)              2D --version
//     --------------- +   --------------  = 0
//          dx                 dy
//
//      d(u phi)     d( v phi)       d(w phi)       3D --version
//     ----------- + ---------  +  ----------  = 0
//            dx        dy             dz
//
// 2 with time
//
//   d(phi)    d(u phi)    d(v phi)                 2D --version
//   ------ +  ------- +  -------  = 0
//     dt         dx         dy
//
//   d(phi)     d(u phi)  d(v phi)   d(w phi)       3D --version
//   ------ +  ------- +  -------  + -------  = 0
//     dt         dx         dy          dz
template<class D>
class Advection_: public EquationBase_<D>{
public:
    typedef D Domain;
    typedef Advection_<D>           Self;
    typedef EquationBase_<D>        Base;
    static const St Dim = D::Dim; 
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
    typedef typename Domain::VectorCenter       VectorCenter;
    typedef typename Domain::spVectorCenter   spVectorCenter;
    typedef typename Domain::VectorFace       VectorFace;
    typedef typename Domain::spVectorFace   spVectorFace;

    typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;
   
    typedef Solver_<Vt>             Solver; 
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt> Solver_Jacobi;
    typedef SOR_<Vt>    Solver_SOR;
    typedef CG_<Vt>     Solver_CG;


protected:
    VectorCenter _vc;
    VectorFace   _vf;

    typedef void (Self::*FunOneStep)(St, Vt);
	FunOneStep     _fun_one_step;
public:
    Advection_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("phi");
    }

    virtual std::string name() const{
        return "EquationAdvection";
    };

    int run_one_step(St step, Vt time){

    };

    int initialize(){
        // solver ----------------------------
        this->_configs["solver"] = this->_init_solver();
        // time term -------------------------
        // --- has time term -----------------
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
        // --- on time term -----------------
        }else{ 
            this->_configs["field_exp_coe_one"] = this->new_field_exp_coe_one();
        }
        // space scheme ---------------------
        if(! this->has_config("space_scheme")){  // set default space scheme
            this->set_space_scheme("finite_volume_2");  
        }
        return 0;
    }; 

    int finalize(){

    }; 

    int solve(){

    };

protected:
    //new u v w on center and face
    void _new_uvw(){
		std::vector<std::string> vname = {"u", "v", "w"};
		for(St d = 0; d< Dim; ++d){
			this->new_field(vname[d]);
			_vc.set(ToAxes(d), this->_fields[vname[d]]);
		}
		for(St d = 0; d< Dim; ++d){
			this->new_field_face(vname[d]);
			_vf.set(ToAxes(d), this->_ffaces[vname[d]]);
		}     
	}

    void _one_step_fou_explicit(St step, Vt t){
        // UdotNabla_FOU FOU(this->_bis["phi"]);
        VectorFace&   vf = this->_vf;
        VectorCenter& vc = this->_vc;
        FieldCenter& phi = this->field("phi");
        auto         bis = this->get_boundary_index("phi");
        auto          dt = this->_time->dt();
        // auto           t = this->_time->current_time();

        InterpolateCenterToFace(vc, vf,
                this->get_boundary_index("u"),
                this->get_boundary_index("v"),
                this->get_boundary_index("w"));

        for(St d = 0 ; d<Dim; d++){
            phi = phi - UdotNabla(vf, phi, d, t) * dt;
        }
	}

};



}

#endif