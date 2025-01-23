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

typedef SOrderXYZ_<dim, Grid, Ghost> Order;
typedef std::shared_ptr<Order> spOrder;

typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;
typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;
typedef Laplace_<Domain> Laplace;

// void PlotFieldAsContour(const std::string& ffn, const Field& f){
//     const int fig_width  = 800;
//     const int fig_height = 600;
//     Gnuplot gnu;
// 	gnu.set_xrange(-0.1, 1.1);
// 	gnu.set_yrange(-0.1, 1.1);
// 	gnu.set_ylabel("y");
// 	gnu.set_xlabel("x");
// 	gnu.set_equal_aspect_ratio();
// 	gnu.set_palette_blue_red();
// 	gnu.add(ToGnuplotActorContour(f));
//     gnu.set_terminal_png(OUTPUTPATH + ffn, fig_width, fig_height);
// 	gnu.plot();
// }

// void PlotResidual(const std::string& ffn, 
//                   const std::vector<int>& ln,
//                   const std::list<std::list<double> >& lr){
//     const int fig_width  = 800;
//     const int fig_height = 600;
//     Gnuplot gnu;
//     gnu.set_ylogscale();
// 	// gnu.set_xrange(-0.1, 1.1);
// 	// gnu.set_yrange(-0.1, 1.1);
// 	gnu.set_ylabel("Residual");
// 	gnu.set_xlabel("Number of iteration");
// 	// gnu.set_equal_aspect_ratio();
// 	// gnu.set_palette_blue_red();
//     auto itern = ln.begin();
//     auto iterr = lr.begin();
//     for(;itern != ln.end();){
//         auto a = ToGnuplotActor(*iterr);
//         a.title("Mesh n = " + ToString(*itern));
//         a.style("with lines lw 2");
// 	    gnu.add(a);
//         itern++;
//         iterr++;
//     }
//     gnu.set_terminal_png(OUTPUTPATH + ffn, fig_width, fig_height);
// 	gnu.plot();
// }
// std::list<double> Reference(int order,
//                const std::vector<int>& ln,
//                const std::list<double>& l1){
//     std::list<double> res;
//     res.push_back(l1.front());
//     auto itern = ln.begin();
//     itern++;
//     for(;itern != ln.end();itern++){
//         auto nm = *(std::prev(itern));
//         auto n  = (*itern);
//         auto v  = res.back() / std::pow(double(n) / double(nm), double(order));
//         res.push_back(v);
//     }
//     return res;
// }
// void PlotError(const std::string& ffn,
//                const std::vector<int>& ln,
//                const std::list<double> & l1,
//                const std::list<double> & l2,
//                const std::list<double> & li
//                ){
//     const int fig_width  = 800;
//     const int fig_height = 600;
//     Gnuplot gnu;
//     gnu.set_xlogscale();
//     gnu.set_ylogscale();
// 	gnu.set_ylabel("Norm");
// 	gnu.set_xlabel("1/n");
//     gnu.set_yformat("10^{%L}");
// 	// gnu.set_equal_aspect_ratio();
// 	// gnu.set_palette_blue_red();
//     std::list<double> lh;
//     for(auto& n:ln){
//         lh.push_back(1.0/n);
//     }
//     auto a1 = ToGnuplotActor(lh, l1);
//     a1.title("L1-Norm");
//     a1.style("with linespoints lw 2 pt 7");
//     auto a2 = ToGnuplotActor(lh, l2);
//     a2.title("L2-Norm");
//     a2.style("with linespoints lw 2 pt 7");
//     auto ai = ToGnuplotActor(lh, li);
//     ai.title("Linf-Norm");
//     ai.style("with linespoints lw 2 pt 7");
//     gnu.add(a1);
//     gnu.add(a2);
//     gnu.add(ai);

//     auto l1ref = Reference(2, ln, l1);
//     auto a1r = ToGnuplotActor(lh, l1ref);
//     // a1r.title("2 Order");
//     a1r.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

//     auto l2ref = Reference(2, ln, l2);
//     auto a2r = ToGnuplotActor(lh, l2ref);
//     a2r.title("2 Order Reference");
//     a2r.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

//     auto liref = Reference(2, ln, li);
//     auto air = ToGnuplotActor(lh, liref);
//     // a2r.title("2 Order");
//     air.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

//     gnu.add(a1r);
//     gnu.add(a2r);
//     gnu.add(air);

//     gnu.set_terminal_png(OUTPUTPATH + ffn, fig_width, fig_height);
//     gnu.set_key("top left");
//     // gnu.test();
// 	gnu.plot();
// }
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
    gnu.set_palette_blue_red();
    gnu.add(ToGnuplotActorContour(equ.field("phi")));
    gnu.set_terminal_png(FIG_PATH + "Solved_Phi", 
                        fig_width, fig_height, "Fira Code");
    gnu.plot();   

    return equ.field("phi");
}

void Iterative(const std::string& method, const Field& fe,
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
    equ.set_time_term(50, 1e-4);
    
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
        gnu.splot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER );
    egs.set_figure_font("Fira Code");
    egs.set_figure_width(fig_width);
    egs.set_figure_height(fig_height);
    egs.gnuplot().set_xrange(-0.1, 1.1);
    egs.gnuplot().set_yrange(-0.1, 1.1);
    egs.gnuplot().set_zrange( 0.0, 1.1);
    egs.gnuplot().set_equal_aspect_ratio();
    egs.gnuplot().set_cbrange(0.0, 1.0);
    egs.gnuplot().set_palette_blue_red();
    egs.gnuplot().set_xyplane(0.0);
    egs.set_path(FIG_PATH + method + "_");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));
    // Add events Error Norm
    typedef EventErrorNorm_<Domain> EventErrorNorm;
    auto speen = std::make_shared<EventErrorNorm>(fe, "phi", -1, -1, 1, Event::AFTER);
    equ.add_event("ErrorPhi", speen);

    equ.run();

    Gnuplot gnu;
    // gnu.set_xrange(-0.1, 1.1);
    // gnu.set_yrange(-0.1, 1.1);
    gnu.set_ylabel("Norm1");
    gnu.set_xlabel("time");
    // gnu.set_equal_aspect_ratio();
    // gnu.set_palette_blue_red();
    gnu.add(ToGnuplotActor(speen->get_list_time(), speen->get_list_norminf()));
    gnu.set_terminal_png(FIG_PATH + method+ "_Norm2", 
                    fig_width, fig_height, "Fira Code");
    gnu.plot();
}

// void A_Interative(){
//     std::list<Vt> ln1, ln2, lni, lt;
//     Iterative("explicit", sf, lt, ln1, ln2, lni);
// }

int main(int argc, char** argv) {
    std::cout << "[  Laplace ] Solver"<<std::endl;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    auto sf = SolvedSolution(32);
    
    std::list<Vt> ln1, ln2, lni, lt;
    Iterative("explicit", sf, lt, ln1, ln2, lni);
    // std::cout << "[   INFO   ] n   = " << n << std::endl;
    // ExactSolution();
    // std::vector<int> vn = {10, 20, 40, 80};
    // std::list<double> l1,l2,li;
    // std::list<std::list<double> > lr;
    // for(auto& n : vn){
    //     PoissonSolver(n, l1, l2, li, lr);
    // }
    // // output to a file
    // std::ofstream fout("./fig/error_table.txt",std::ios::out);

    // tfm::format(fout, "n,L1-Norm,L2-Norm,Linf-Norm\n");
    // auto itervn = vn.begin();
    // auto iterl1 = l1.begin();
    // auto iterl2 = l2.begin();
    // auto iterli = li.begin();
    // for(;itervn != vn.end();){
    //     tfm::format(std::cout,
    //                 "n: %8d N1: %10.5e N2: %10.5e Ni: %10.5e\n",
    //                 *itervn, *iterl1, *iterl2, *iterli);
    //     tfm::format(fout,
    //                 "%8d,%10.5e,%10.5e,%10.5e\n",
    //                 *itervn, *iterl1, *iterl2, *iterli);
    //     itervn++;
    //     iterl1++;
    //     iterl2++;
    //     iterli++;
    // }
    // fout.close();

    // // plot residual
    // PlotResidual("residual", vn, lr);
    // PlotError("error", vn, l1, l2, li);
}