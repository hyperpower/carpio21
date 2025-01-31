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
public:
    Advection_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("phi");
    }

    int run_one_step(St step, Vt time){};

    int initialize(){}; 

    int finalize(){}; 

    int solve(){};

protected:
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

};



}

#endif