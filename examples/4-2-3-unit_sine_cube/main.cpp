#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/equation.hpp"
#include "domain/structure/io/splotly_actor.hpp"

using namespace carpio;

const std::size_t dim = 3;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double,dim> Point;

typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;

const std::string OUTPUTPATH = "./fig/";


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
    plotly.write(OUTPUTPATH + fn, "html");
}

void PlotResidual(const std::string& ffn, 
                  const std::vector<int>& ln,
                  const std::list<std::list<double> >& lr){
    const int fig_width  = 800;
    const int fig_height = 600;
    Gnuplot gnu;
    gnu.set_ylogscale();
	// gnu.set_xrange(-0.1, 1.1);
	// gnu.set_yrange(-0.1, 1.1);
	gnu.set_ylabel("Residual");
	gnu.set_xlabel("Number of iteration");
	// gnu.set_equal_aspect_ratio();
	// gnu.set_palette_blue_red();
    auto itern = ln.begin();
    auto iterr = lr.begin();
    for(;itern != ln.end();){
        auto a = ToGnuplotActor(*iterr);
        a.title("Mesh n = " + ToString(*itern));
        a.style("with lines lw 2");
	    gnu.add(a);
        itern++;
        iterr++;
    }
    gnu.set_terminal_png(OUTPUTPATH + ffn, fig_width, fig_height);
	gnu.plot();
}
std::list<double> Reference(int order,
               const std::vector<int>& ln,
               const std::list<double>& l1){
    std::list<double> res;
    res.push_back(l1.front());
    auto itern = ln.begin();
    itern++;
    for(;itern != ln.end();itern++){
        auto nm = *(std::prev(itern));
        auto n  = (*itern);
        auto v  = res.back() / std::pow(double(n) / double(nm), double(order));
        res.push_back(v);
    }
    return res;
}
void PlotError(const std::string& ffn,
               const std::vector<int>& ln,
               const std::list<double> & l1,
               const std::list<double> & l2,
               const std::list<double> & li
               ){
    const int fig_width  = 800;
    const int fig_height = 600;
    Gnuplot gnu;
    gnu.set_xlogscale();
    gnu.set_ylogscale();
	gnu.set_ylabel("Norm");
	gnu.set_xlabel("1/n");
    gnu.set_yformat("10^{%L}");
	// gnu.set_equal_aspect_ratio();
    std::list<double> lh;
    for(auto& n:ln){
        lh.push_back(1.0/n);
    }
    auto a1 = ToGnuplotActor(lh, l1);
    a1.title("L1-Norm");
    a1.style("with linespoints lw 2 pt 7");
    auto a2 = ToGnuplotActor(lh, l2);
    a2.title("L2-Norm");
    a2.style("with linespoints lw 2 pt 7");
    auto ai = ToGnuplotActor(lh, li);
    ai.title("Linf-Norm");
    ai.style("with linespoints lw 2 pt 7");
    gnu.add(a1);
    gnu.add(a2);
    gnu.add(ai);

    auto l1ref = Reference(2, ln, l1);
    auto a1r = ToGnuplotActor(lh, l1ref);
    // a1r.title("2 Order");
    a1r.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

    auto l2ref = Reference(2, ln, l2);
    auto a2r = ToGnuplotActor(lh, l2ref);
    a2r.title("2 Order Reference");
    a2r.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

    auto liref = Reference(2, ln, li);
    auto air = ToGnuplotActor(lh, liref);
    // a2r.title("2 Order");
    air.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

    gnu.add(a1r);
    gnu.add(a2r);
    gnu.add(air);

    gnu.set_terminal_png(OUTPUTPATH + ffn, fig_width, fig_height);
    gnu.set_key("top left");
    // gnu.test();
	gnu.plot();
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

    PlotFieldAsVolume("exact", a);
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
	equ.set_solver("Jacobi", 10000, 1e-8);

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
    
    // PlotFieldAsVolume("Poisson_SolutionContour" + ToString(n), equ.field("phi"));
    // // PlotFieldAsContour("Poisson_SolutionContour" + ToString(n), equ.field("phi"));

    //residual 
    auto spsolver = equ.get_solver();
    lr.push_back(spsolver->get_residual_array());

    // // error
    auto exact = equ.field("phi").new_compatible();
    exact.assign([](typename Field::ValueType x,
                    typename Field::ValueType y,
                    typename Field::ValueType z,
                double t){
        return std::sin(_PI_ * x) * std::sin(_PI_ * y)* std::sin(_PI_ * z);
    });
    auto error = exact - equ.field("phi");
    
    // PlotFieldAsContour("Poisson_ErrorContour"+ ToString(n), error);

    l1.push_back(Norm1(IntVolume(error)));
    l2.push_back(Norm2(error)/std::sqrt(std::pow(n, dim)));
    li.push_back(NormInf(error));

    // std::cout << "[ Error 1  ]  = " << error(Domain::Index(0,0)) << std::endl;
    // std::cout << "[ Norm 2  ]  = " << l2.back() << std::endl;
    // std::cout << "[ Norm Inf]  = " <<  << std::endl;
}

void OutputError(
    const std::string& fn,
    const std::vector<int>& ln,
    const std::list<double> & l1,
    const std::list<double> & l2,
    const std::list<double> & li){
    std::ofstream fout(OUTPUTPATH + fn,std::ios::out);

    tfm::format(fout, "n,L1-Norm,L2-Norm,Linf-Norm\n");
    auto itervn = ln.begin();
    auto iterl1 = l1.begin();
    auto iterl2 = l2.begin();
    auto iterli = li.begin();
    for(;itervn != ln.end();){
        tfm::format(std::cout,
                    "n: %8d N1: %10.5e N2: %10.5e Ni: %10.5e\n",
                    *itervn, *iterl1, *iterl2, *iterli);
        tfm::format(fout,
                    "%8d,%10.5e,%10.5e,%10.5e\n",
                    *itervn, *iterl1, *iterl2, *iterli);
        itervn++;
        iterl1++;
        iterl2++;
        iterli++;
    }
    fout.close();

}

int main(int argc, char** argv) {
    ExactSolution();
    std::vector<int> vn = {10, 20, 30, 40};
    std::list<double> l1,l2,li;
    std::list<std::list<double> > lr;
    for(auto& n : vn){
        PoissonSolver(n, l1, l2, li, lr);
    }
    // output to a file
    OutputError("error_table.txt",vn, l1, l2, li);

    // plot residual
    PlotResidual("residual", vn, lr);
    PlotError("error", vn, l1, l2, li);
}