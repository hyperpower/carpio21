#ifndef _POISSONIMPL_FINITE_DIFFERENCE_HPP_
#define _POISSONIMPL_FINITE_DIFFERENCE_HPP_

#include "type_define.hpp"
#include "equation/event/event.hpp"
#include <vector>

#include "equation/poisson/poisson_fv.hpp"
namespace carpio {
//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = s  2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = s   3D --version

template<class D>
class PoissonImpl_<D, VertexTag>: public LaplaceImpl_<D, VertexTag>{
public:
    typedef D Domain;
    typedef PoissonImpl_<D, VertexTag>  Self;
    typedef LaplaceImpl_<D, VertexTag>  Base;
    typedef typename Domain::ValueType   Vt;
    typedef typename Domain::Index       Index;
    typedef typename Domain::Grid        Grid;
    typedef typename Domain::spGrid    spGrid;
    typedef typename Domain::Ghost       Ghost;
    typedef typename Domain::spGhost   spGhost;
    typedef typename Domain::Order       Order;
    typedef typename Domain::spOrder   spOrder;
    typedef typename Domain::FieldCenter     FieldCenter;
    typedef typename Domain::spFieldCenter spFieldCenter;
    typedef typename Domain::FieldCenterExp     FieldCenterExp;
    typedef typename Domain::spFieldCenterExp spFieldCenterExp;
    
    typedef typename Domain::FieldVertex     FieldVertex;
    typedef typename Domain::spFieldVertex spFieldVertex;
    typedef typename Domain::FieldVertexExp     FieldVertexExp;
    typedef typename Domain::spFieldVertexExp spFieldVertexExp; 
    
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
    PoissonImpl_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
            this->new_field("phi");
            this->new_field("source");
    }

    virtual std::string name() const{
        return "EquationPoissonVertex";
    };

    virtual ~PoissonImpl_(){};

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
            if(m == "finite_difference_2"){
                return _solve_finite_difference_2();
            }else if(m == "HOC4"){
                return _solve_hoc4();
            }
            return 0;
        } else{
            return _solve_finite_difference_2();
        }
    }
    
    void set_source(spFieldVertex spsource){
        this->_fields["source"] = spsource;
    }

    void set_source(FunXYZ_Value fun){
        auto& source = *(this->_fields["source"]);
        source.assign(fun);
    }
    void set_analytical_d2x_source(FunXYZ_Value fun){
        if(!(this->has_field("d2x_source"))){
            this->new_field("d2x_source");
        }
        auto& source = *(this->_fields["d2x_source"]);
        source.assign(fun);
    }
    void set_analytical_d2y_source(FunXYZ_Value fun){
        if(!(this->has_field("d2y_source"))){
            this->new_field("d2y_source");
        }
        auto& source = *(this->_fields["d2y_source"]);
        source.assign(fun);
    }
    void set_analytical_d2z_source(FunXYZ_Value fun){
        if(!(this->has_field("d2z_source"))){
            this->new_field("d2z_source");
        }
        auto& source = *(this->_fields["d2z_source"]);
        source.assign(fun);
    }
protected:
    int _solve_finite_difference_2(){
        FieldVertex& phi  = *(this->_fields["phi"]);
        auto expf     = any_cast<spFieldVertexExp>(this->_configs["field_exp_coe_one"]);
        auto bis      = this->get_boundary_index("phi");
        auto& fsource = *(this->_fields["source"]);

        auto res = DifferenialLaplacian((*expf), (*bis)) - fsource;

        return this->_build_mat_and_solve(res, phi);
    }

    FieldVertex _sum_d2_source(){
        std::array<std::string, 3> ns{"d2x_source", "d2y_source", "d2z_source"};
        FieldVertex res(this->field(ns[0]));
        for(int d= 1 ; d < D::Dim; d++){
            res += this->field(ns[d]);
        }
        return res;
    }

    int _solve_hoc4(){
        FieldVertex& phi  = *(this->_fields["phi"]);
        auto& expf    = *(any_cast<spFieldVertexExp>(this->_configs["field_exp_coe_one"]));
        auto& bis     = *(this->get_boundary_index("phi"));
        auto& fsource = *(this->_fields["source"]);
        auto& d2x = *(this->_fields["d2x_source"]);
        auto& d2y = *(this->_fields["d2y_source"]);

        auto res = DifferenialLaplacianHOC4(expf, bis) 
                   - fsource
                   - this->_sum_d2_source() * CoeH2_12(fsource);

        // std::cout << "solve hoc4" << std::endl;
        // Gnuplot gnu;
        // auto fout = DifferenialLaplacianHOC4(expf, bis); 
        // auto ff   = -fsource - (d2x + d2y) * CoeH2_12(fsource); 
        // auto idx  = Index(0,0); 
        // std::cout << "f side = " << ff(idx) << std::endl;
        // auto r = ApproximateRange(res, idx);
        // r.scale(2.5);
        // gnu.set_xlabel("x");
        // gnu.set_ylabel("y");
        // gnu.set_label_with_box(10, "Index C = " + ToString(idx), 
        //     r.max(_X_) - 0.4 * r.get_d(_X_), 
        //     r.max(_Y_) - 0.1 * r.get_d(_Y_) );
        // gnu.set_label_with_box(12, "Num     = " + ToString(res(idx).value()), 
        //     r.max(_X_) - 0.4 * r.get_d(_X_), 
        //     r.max(_Y_) - 0.2 * r.get_d(_Y_) );
        // gnu.set_xrange(r.min(_X_), r.max(_X_));
        // gnu.set_yrange(r.min(_Y_), r.max(_Y_));
        // gnu.add(ToGnuplotActorWireFrame(phi.grid()));
        // gnu.add(ToGnuplotActorWireFrame(res, idx));
        // gnu.add(ToGnuplotActorContourPoints(res, idx));
        // gnu.add(ToGnuplotActorLabel(res, idx, "value"));
        // gnu.add(ToGnuplotActorLabel(res, idx, "index"));
        // gnu.set_palette_red_blue_dark();
        // gnu.set_equal_aspect_ratio();
        
        // gnu.set_terminal_png("./fig/local_exp");
        // gnu.plot();

        return this->_build_mat_and_solve(res, phi);
    }
};


}

#endif