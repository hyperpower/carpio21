#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "example_define.hpp"
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/poisson/laplace.hpp"
#include "equation/equation.hpp"

using namespace carpio;

const std::size_t dim = 2;
typedef Point_<double,dim> Point;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
typedef std::shared_ptr<Order> spOrder;

typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;
typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;
typedef Laplace_<Domain> Laplace;


auto SolvedSolution(int meshn){
    const int dim = 2;
    std::cout << "[  Laplace ] Solved Solution"<<std::endl;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    Point p(0,0,0);

    std::shared_ptr<Grid>  spgrid(new Grid(p, meshn, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    // Define the equation
    Laplace equ(spgrid, spghost, sporder);

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
    spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
    spbi->insert(0, spbcym);
    spbi->insert(1, spbcym);
    spbi->insert(2, spbcym);
    spbi->insert(3, spbcxm);
    equ.set_boundary_index("phi", spbi);

    // Set solver
    equ.set_solver("Jacobi", 10000, 1e-6);
    
    // Add events
    typedef Event_<Domain> Event;
    typedef std::shared_ptr<Event>  spEvent;
    spEvent spetime(new EventOutputTime_<Domain>(std::cout,
                                                  -1, -1, 1, Event::END | Event::START));
    equ.add_event("OutputTime", spetime);

    equ.run();

    Gnuplot gnu;
    gnu.set_xrange(-0.1, 1.1);
    gnu.set_yrange(-0.1, 1.1);
    gnu.set_zrange( 0.0, 1.1);
    gnu.set_cbrange(0.0, 1.0);
    gnu.set_ylabel("y");
    gnu.set_xlabel("x");
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_red_blue();
    gnu.add(ToGnuplotActorContour(equ.field("phi")));
    gnu.set_terminal_png(FIG_PATH + "Solved_Phi", 
                        fig_width, fig_height, "Fira Code");
    gnu.plot();   

    return equ.field("phi");
}

void Iterative(const std::string& method, const Field& fe, double dt,
                std::list<Vt>& lt,
                std::list<Vt>& ln1,
                std::list<Vt>& ln2,
                std::list<Vt>& lni){
    std::cout << "[  Laplace ] Iterative"<<std::endl;
    std::cout << "[  Method  ] " << method <<std::endl;
    const int dim = 2;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;

    // Define the equation
    Laplace equ(fe.spgrid(), fe.spghost(), fe.sporder());

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
    spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
    spbi->insert(0, spbcym);
    spbi->insert(1, spbcym);
    spbi->insert(2, spbcym);
    spbi->insert(3, spbcxm);
    equ.set_boundary_index("phi", spbi);

    // Set solver
    equ.set_solver("Jacobi", 1000, 1e-5);

    // Set time term
    equ.set_time_term(50, dt);
    equ.set_time_scheme(method);
    
    // Add events
    typedef Event_<Domain> Event;
    typedef std::shared_ptr<Event>  spEvent;
    spEvent spetime(new EventOutputTime_<Domain>(std::cout,
                                                  -1, -1, 1, Event::AFTER));
    equ.add_event("OutputTime", spetime);

    typedef EventGnuplotField_<Domain> EventGnuplotField;
    EventGnuplotField::FunPlot fun = [](
                           Gnuplot& gnu,
                     const EventGnuplotField::FieldCenter& f,
                     St step, Vt t, int fob,
                     typename EventGnuplotField::EquationBase& pd){
        gnu.add(ToGnuplotActorContourWire(f));
        gnu.set_label(1, "Step = "+ToString(step), 0.0, 0.0, 1.0);
        gnu.set_label(2, "Time = "+ToString(t, "%.2e"), 0.0, 0.0, 0.9);
        // gnu.add(ToGnuplotActorLabel(0.0, 1, "Step = "+ToString(step)));
        gnu.splot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER );

    egs.set_figure_font("Fira Code");
    egs.set_figure_width(fig_width);
    egs.set_figure_height(fig_height);
    egs.set_path(FIG_PATH + method + "_" );
    egs.set_format_string("%s_%03d");
    egs.gnuplot().set_xrange(-0.1, 1.1);
    egs.gnuplot().set_xlabel("x");
    egs.gnuplot().set_ylabel("y");
    egs.gnuplot().set_zlabel("phi");
    egs.gnuplot().set_grid();
    egs.gnuplot().set_yrange(-0.1, 1.1);
    egs.gnuplot().set_zrange( 0.0, 1.1);
    egs.gnuplot().set_equal_aspect_ratio();
    egs.gnuplot().set_cbrange(0.0, 1.0);
    egs.gnuplot().set_palette_red_blue();
    egs.gnuplot().set_xyplane(0.0);
    egs.gnuplot().set_view(45,30,1.0,1.5);
    if(method == "implicit" && dt ==5e-4){
        equ.set_time_term(150, dt);
        equ.add_event("Gnuplotphi", std::make_shared<EventGnuplotField>(egs));
    }
    // Add events Error Norm
    typedef EventNormExactFieldCenter_<Domain> EventErrorNorm;
    auto speen = std::make_shared<EventErrorNorm>(&fe, "phi", -1, -1, 1, Event::AFTER);
    equ.add_event("ErrorPhi", speen);
    equ.run();
    lt  = speen->get_list_time();
    ln1 = speen->get_list_norm1();
    ln2 = speen->get_list_norm2();
    lni = speen->get_list_norminf();

}

struct Res{
    std::string method;
    Vt dt;
    std::list<Vt> ln1, ln2, lni, lt;

    Res(){}

    Res(const std::string& m, Vt _dt): method(m), dt(_dt){
    }
};

void A_Iterative(Res& res, const Field& fe){
    Iterative(res.method, fe, res.dt, res.lt, res.ln1, res.ln2, res.lni);
}

void set_gnu(const std::string& m, Gnuplot& gnu){
    if(m == "explicit"){
        gnu.set_yrange(1e-1, 100);
        gnu.set_xrange(0, 0.02);
    }else{
        // gnu.set_xrange(0, 0.02);
        gnu.set_yrange(1e-4, 1);
    }
    gnu.set_yformat("10^{%T}");
    gnu.set_xlabel("time");
    gnu.set_ylogscale();
}

void A_Run(std::string& method) {
    std::cout << "[  Laplace ] Iterative"<<std::endl;
    std::cout << "[   INFO   ] Dim    = " << dim << std::endl;
    std::cout << "[   INFO   ] Method = " << method << std::endl;
    auto sf = SolvedSolution(32);
    std::vector<Vt> ldt{1e-4, 2.5e-4, 5e-4, 1e-3, 1e-2}; 
    std::vector<Res> vres(ldt.size());

    for(int i =0; i < ldt.size(); i++){
        auto& res= vres[i];
        res.method = method;
        res.dt = ldt[i];
        std::cout << "[   INFO   ] dt = " << res.dt << std::endl;
        A_Iterative(res, sf);
    }
    Gnuplot gnu1, gnu2, gnui;
    for(auto& res : vres){
        auto an1 = ToGnuplotActor(res.lt, res.ln1);
        an1.title("dt = " + ToString(res.dt, "%.1e"));
        an1.style("with lines ");  
        an1.line_width(2);
        gnu1.add(an1);
        auto an2 = ToGnuplotActor(res.lt, res.ln2);
        an2.title("dt = " + ToString(res.dt, "%.1e"));
        an2.style("with lines ");  
        an2.line_width(2);
        gnu2.add(an2);
        
        auto ani = ToGnuplotActor(res.lt, res.lni);
        ani.title("dt = " + ToString(res.dt, "%.1e"));
        ani.style("with lines ");  
        ani.line_width(2);
        gnui.add(ani);
    }
    set_gnu(method, gnu1);
    gnu1.set_ylabel("L1-Norm");
    gnu1.set_terminal_png(FIG_PATH + method +"_Norm1Compare", 
                    fig_width, fig_height, "Fira Code");
    gnu1.plot();
    
    set_gnu(method, gnu2);
    gnu2.set_ylabel("L2-Norm");
    gnu2.set_terminal_png(FIG_PATH + method +"_Norm2Compare", 
                    fig_width, fig_height, "Fira Code");
    gnu2.plot();

    set_gnu(method, gnui);
    gnui.set_ylabel("Linf-Norm");
    gnui.set_terminal_png(FIG_PATH + method +"_NormInfCompare", 
                    fig_width, fig_height, "Fira Code");
    gnui.plot();
    
}
int main(int argc, char** argv) {
    std::list<std::string> lstr{"explicit", "implicit", "CN"};
    for(auto& m : lstr){
        A_Run(m);
    }
    return 0;
}