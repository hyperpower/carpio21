#ifndef _EXAMPLE_FD_H
#define _EXAMPLE_FD_H

#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/equation.hpp"

#include "example_define.hpp"
#include "convergence_analysis.hpp"


using namespace carpio;

namespace fd{

const std::size_t dim = 2;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost, VertexTag> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double,dim> Point;

typedef SFieldVertex_<dim, double, Grid, Ghost, Order> Field;

void PlotFieldAsContour(const std::string& ffn, const Field& f){
    const int fig_width  = 800;
    const int fig_height = 600;
    Gnuplot gnu;
	gnu.set_xrange(-0.1, 1.1);
	gnu.set_yrange(-0.1, 1.2);
	gnu.set_ylabel("y");
	gnu.set_xlabel("x");
	gnu.set_equal_aspect_ratio();
	gnu.set_palette_red_blue();
	gnu.add(ToGnuplotActorContourPoints(f));
	gnu.add(ToGnuplotActorWireFrame(f.grid()));
    // plot boundary
    std::list<Vt> lx = {0.0, 1.0,1.0,0.0,0.0};
    std::list<Vt> ly = {0.0, 0.0,1.0, 1.0,0.0};
    auto a = ToGnuplotActor(lx, ly);
    a.style("with lines lw 2 dashtype 2 lc \"black\"");
    a.title("Periodic Boundary");
    gnu.add(a);
    gnu.set_terminal_png(FIG_PATH + ffn, fig_width, fig_height);
    gnu.set_key("top right");
	gnu.plot();
}



void PoissonSolver(const std::string& scheme, int n, 
                   std::list<double>& l1, 
                   std::list<double>& l2, 
                   std::list<double>& li,
                   std::list<std::list<double> >& lr){
    std::cout << "[  Poisson ] Solver Finite Difference"<<std::endl;
    std::cout << "[   INFO   ] Dim    = " << dim << std::endl;
    std::cout << "[   INFO   ] n      = " << n << std::endl;
    std::cout << "[   INFO   ] Scheme = " << scheme << std::endl;
    Point p(0,0,0);
    Vt l = 1.0;
    Vt h = 1.0 / n;
    Vt nl = l * (1.0 + h);
    Vt nh = nl / n;
    Point np(p[0] - 0.5 * nh,p[1] - 0.5 * nh, p[2] - 0.5 * nh);

    spGrid  spgrid(new  Grid(np, n, nl, 2));
    spGhost spghost(new Ghost(spgrid));
    spOrder sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;
    typedef Poisson_<Domain> Poisson;

    // Define the equation
    Poisson equ(spgrid, spghost, sporder);

    // Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
	spBC spbcp(new BoundaryConditionValue(BC::_BC3_, 0.0));
	spbi->insert(0, spbcp);
	spbi->insert(1, spbcp);
	spbi->insert(2, spbcp);
	spbi->insert(3, spbcp);
	equ.set_boundary_index("phi", spbi);

    // Set solver
	equ.set_solver("Jacobi", 20000, 1e-10);
    equ.set_space_scheme(scheme);

    // Set source
    equ.set_source([](typename Domain::ValueType x,
                      typename Domain::ValueType y,
                      typename Domain::ValueType z){
        return  -8.0 * _PI_ * _PI_ * std::sin(2.0* _PI_ *x)*std::sin(2.0*_PI_*y);
    });
    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();
    
    if( n == 10){
        PlotFieldAsContour("PoissonFD_SolutionContour" + ToString(n), equ.field("phi"));
    }

    //residual 
    auto spsolver = equ.get_solver();
    lr.push_back(spsolver->get_residual_array());

    // error
    auto exact = equ.field("phi").new_compatible_zero();
    exact.assign([](typename Field::ValueType x,
                    typename Field::ValueType y,
                    typename Field::ValueType z){
        return std::sin(2.0 * _PI_ * x) * std::sin(2.0 * _PI_ * y);
    });
    auto error = exact - equ.field("phi");
    
    // PlotFieldAsContour("PoissonFD_ErrorContour"+ ToString(n), error);

    l1.push_back(Norm1(error));
    l2.push_back(Norm2(error));
    li.push_back(NormInf(error));
}

int AScheme(const std::string& scheme){
    std::vector<int> vn = {10, 20, 40, 80};
    std::list<double> l1,l2,li;
    std::list<std::list<double> > lr;
    for(auto& n : vn){
        PoissonSolver(scheme, n, l1, l2, li, lr);
    }
    // output to a file

    OutputError(FIG_PATH + "fd_error_table.txt",vn, l1, l2, li);

    // plot residual
    PlotResidual(FIG_PATH + "fd_residual", vn, lr);
    PlotError(FIG_PATH + "fd_error", 2, vn, l1, l2, li);

    return 0;
}

}

#endif