#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/equation.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "convergence_analysis.hpp"

using namespace carpio;

const std::size_t dim = 3;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double,dim> Point;

typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;


void PlotFieldAsVolume(const std::string& fn, const Field& a){
    Plotly_ plotly;
    plotly.margin(0, 0, 0, 0);
    plotly.layout("width",  660.0);
    plotly.layout("height", 400.0);
    plotly.layout("scene_aspectmode", "cube");
    // plotly.layout("scene_camera_center_z", -0.15);
    // plotly.layout("scene_camera_eye_z",    0.8);
    // plotly.layout("scene_camera_eye_y",    -1.5);
    // plotly.layout("scene_xaxis_range", -4.0, 4);
    // plotly.layout("scene_yaxis_range", -4.0, 4);
    // plotly.layout("scene_zaxis_range", -0.0, 8);
    plotly.layout_false("scene_xaxis_showbackground");
    plotly.layout("scene_xaxis_gridcolor",      "black");
    plotly.layout_false("scene_yaxis_showbackground");
    plotly.layout("scene_yaxis_gridcolor",      "black");
    plotly.layout_false("scene_zaxis_showbackground");
    plotly.layout("scene_zaxis_gridcolor",      "black");
    plotly.layout("legend_x", 0.05);
    plotly.layout("legend_y", 0.95);
    plotly.layout("legend_traceorder", "normal");
    plotly.layout("legend_font_size", 13.0);
    
    auto actor = ToPlotlyActorVolume(a);
    actor.update("opacity", 0.1);
    // actor.update("isomin", -1);
    // actor.update("isomax", 1);
    actor.update("colorscale","RdBu");
    // actor.update("color", c_yellow);
    // actor.update("name", "Original Shape");
    actor.update("surface_count", int(21));
    // actor.update_true("showlegend");
    plotly.add(actor);
    plotly.write(fn, "div");
}


void ExactSolution(){
    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 50;
    spGrid spgrid(new Grid(pmin, {n, n, n}, pmax.x()/double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field a(spgrid, spghost, sporder);

    a.assign([](typename Field::ValueType x,
                typename Field::ValueType y,
                typename Field::ValueType z,
                double t){
        return std::sin( _PI_ * x) * std::sin( _PI_ * y) * std::sin( _PI_ * z);
    });

    PlotFieldAsVolume(FIG_PATH + "exact", a);
}

void PoissonSolver(int n, 
                   std::list<double>& l1, 
                   std::list<double>& l2, 
                   std::list<double>& li,
                   std::list<std::list<double> >& lr){
    std::cout << "[  Poisson ] Solver"<<std::endl;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    std::cout << "[   INFO   ] n   = " << n << std::endl;
    Point p(0.,0.,0.);

    spGrid  spgrid(new Grid(p, n, 1.0, 2));
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
	spBC spbcp(new BoundaryConditionValue(BC::_BC1_, 0.0));
	spbi->insert(0, spbcp);
	spbi->insert(1, spbcp);
	spbi->insert(2, spbcp);
	spbi->insert(3, spbcp);
	spbi->insert(4, spbcp);
	spbi->insert(5, spbcp);
	equ.set_boundary_index("phi", spbi);

    // // Set solver
	equ.set_solver("Jacobi", 20000, 1e-7);

    // Set source
    equ.set_source([](typename Domain::ValueType x,
                      typename Domain::ValueType y,
                      typename Domain::ValueType z){
                        return  -3.0 * _PI_ * _PI_ * std::sin(_PI_*x)*std::sin(_PI_*y)*std::sin(_PI_*z);
                      });
    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();
    
    //residual 
    auto spsolver = equ.get_solver();
    lr.push_back(spsolver->get_residual_array());

    // // error
    auto exact = equ.field("phi").new_compatible_zero();
    exact.assign([](typename Field::ValueType x,
                    typename Field::ValueType y,
                    typename Field::ValueType z,
                double t){
        return std::sin(_PI_ * x) * std::sin(_PI_ * y)* std::sin(_PI_ * z);
    });
    auto error = exact - equ.field("phi");

    l1.push_back(Norm1(error));
    l2.push_back(Norm2(error));
    li.push_back(NormInf(error));
}


int main(int argc, char** argv) {
    ExactSolution();
    std::vector<int> vn = {8, 16, 32};
    std::list<double> l1,l2,li;
    std::list<std::list<double> > lr;
    for(auto& n : vn){
        PoissonSolver(n, l1, l2, li, lr);
    }
    // output to a file
    OutputError(FIG_PATH + "error_table.txt",vn, l1, l2, li);

    // plot residual
    PlotResidual( FIG_PATH + "residual", vn, lr);
    PlotError(FIG_PATH + "error", 2, vn, l1, l2, li);
}