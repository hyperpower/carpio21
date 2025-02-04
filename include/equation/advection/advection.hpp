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
    typedef typename Domain::FieldCenter        FieldCenter;
    typedef typename Domain::spFieldCenter    spFieldCenter;
    typedef typename Domain::FieldCenterExp     FieldCenterExp;
    typedef typename Domain::spFieldCenterExp spFieldCenterExp;
    typedef typename Domain::VectorCenter       VectorCenter;
    typedef typename Domain::spVectorCenter   spVectorCenter;
    typedef typename Domain::VectorFace         VectorFace;
    typedef typename Domain::spVectorFace     spVectorFace;
    
    typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

    typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;
   
    typedef Solver_<Vt>             Solver; 
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt> Solver_Jacobi;
    typedef SOR_<Vt>    Solver_SOR;
    typedef CG_<Vt>     Solver_CG;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)> FunXYZ_Value;

protected:
    VectorCenter _vc;
    VectorFace   _vf;

    typedef void (Self::*FunOneStep)(St, Vt);
    FunOneStep     _fun_one_step;
public:
    Advection_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("phi");
            this->_new_uvw();
    }

    virtual std::string name() const{
        return "EquationAdvection";
    };

    int run_one_step(St step, Vt time){
        _one_step_fou_explicit(step, time);
        return 0;
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
        // --- no time term -----------------
        }else{ 
            this->_configs["field_exp_coe_one"] = this->new_field_exp_coe_one();
        }
        // space scheme ---------------------
        if(! this->has_config("space_scheme")){  // set default space scheme
            this->set_space_scheme("fou");  
        }
        return 0;
    }; 

    int finalize(){

    }; 

    int solve(){

    };

    void set_boundary_index_phi(spBoundaryIndex spbi){
        this->set_boundary_index("phi", spbi);
    }

    void set_boundary_index_velocity(Axes a, spBoundaryIndex spbi){
        switch (a) {
        case _X_: {
            this->set_boundary_index("u", spbi);
            break;
        }
        case _Y_: {
            this->set_boundary_index("v", spbi);
            break;
        }
        case _Z_: {
            this->set_boundary_index("w", spbi);
            break;
        }
        }
    }

    void set_phi(spFieldCenter spphi){
        this->set_field_center("phi", spphi);
    }

    void set_phi(FunXYZ_Value fun){
        this->set_field_center("phi", fun);
    }

    void set_center_velocity(const Axes& a, FunXYZ_Value fun){
        auto nv = AxesToVelocityName(a);
        this->set_field_center(nv, fun);
    }

protected:
    //new u v w on center and face
    void _new_uvw(){
        auto vaxes = ArrAxes<Dim>();
        auto vname = ArrVelocityName<Dim>();
        for(auto& a : vaxes){
            this->new_field(vname[a]);
            _vc.set(ToAxes(a), this->_fields[vname[a]]);
        }
        for(auto& a : vaxes){
            this->new_field_face(vname[a], vaxes[a]);
            _vf.set(ToAxes(a), this->_ffaces[vname[a]]);
        }     
    }

    void _one_step_fou_explicit(St step, Vt t){
        // UdotNabla_FOU FOU(this->_bis["phi"]);
        VectorFace&   vf = this->_vf;
        VectorCenter& vc = this->_vc;
        FieldCenter& phi = *(this->_fields["phi"]);
        auto&         bi = *(this->get_boundary_index("phi"));
        auto          dt = this->_time->dt();
        // auto           t = this->_time->current_time();

        InterpolateCenterToFace(vc, vf,
                *(this->get_boundary_index("u")),
                *(this->get_boundary_index("v")),
                *(this->get_boundary_index("w")));

        phi = phi - UdotNabla(vf, phi, bi, t) * dt;
        
    }

};



}

#endif