#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/equation.hpp"

using namespace carpio;

const std::size_t dim = 2;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double,dim> Point;

typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;

void PlotFieldAsContour(const std::string& ffn, const Field& f){
    const std::string OUTPUTPATH = "./fig/";
    const int fig_width  = 800;
    const int fig_height = 600;
    Gnuplot gnu;
	gnu.set_xrange(-0.1, 1.1);
	gnu.set_yrange(-0.1, 1.1);
	gnu.set_ylabel("y");
	gnu.set_xlabel("x");
	gnu.set_equal_aspect_ratio();
	gnu.set_palette_blue_red();
	gnu.add(ToGnuplotActorContour(f));
    gnu.set_terminal_png(OUTPUTPATH + ffn, fig_width, fig_height);
	gnu.plot();
}

void PlotResidual(const std::string& ffn, 
                  std::vector<int>& ln,
                  const std::list<std::list<double> >& lr){
    const std::string OUTPUTPATH = "./fig/";
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
void ExactSolution(){
    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 100;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x()/double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field a(spgrid, spghost, sporder);

    a.assign([](typename Field::ValueType x,
                typename Field::ValueType y,
                typename Field::ValueType z,
                double t){
        return std::sin(2 * _PI_ * x) * std::sin(2 * _PI_ * y);
    });

    PlotFieldAsContour("ExactSolutionContour", a);
}

void PoissonSolver(int n, 
                   std::list<double>& l1, 
                   std::list<double>& l2, 
                   std::list<double>& li,
                   std::list<std::list<double> >& lr){
    std::cout << "[  Poisson ] Solver"<<std::endl;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    std::cout << "[   INFO   ] n   = " << n << std::endl;
    Point p(0,0,0);

    spGrid  spgrid(new Grid(p, n, 1, 2));
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
	equ.set_solver("Jacobi", 10000, 1e-12);

    // Set source
    equ.set_source([](typename Domain::ValueType x,
                      typename Domain::ValueType y,
                      typename Domain::ValueType z){
                        return  -8.0 * _PI_ * _PI_ * std::sin(2.0*_PI_*x)*std::sin(2.0*_PI_*y);
                      });
    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();
    
    PlotFieldAsContour("Poisson_SolutionContour" + ToString(n), equ.field("phi"));

    //residual 
    auto spsolver = equ.get_solver();
    lr.push_back(spsolver->get_residual_array());

    // error
    auto exact = equ.field("phi").new_compatible();
    exact.assign([](typename Field::ValueType x,
                    typename Field::ValueType y,
                    typename Field::ValueType z,
                double t){
        return std::sin(2 * _PI_ * x) * std::sin(2 * _PI_ * y);
    });
    auto error = exact - equ.field("phi");
    
    PlotFieldAsContour("Poisson_ErrorContour"+ ToString(n), error);

    l1.push_back(Norm1(error));
    l2.push_back(Norm2(error));
    li.push_back(NormInf(error));

    // std::cout << "[ Norm 1  ]  = " << l1.back() << std::endl;
    // std::cout << "[ Norm 2  ]  = " << l2.back() << std::endl;
    // std::cout << "[ Norm Inf]  = " <<  << std::endl;
}

int main(int argc, char** argv) {
    ExactSolution();
    std::vector<int> vn = {10, 20, 40, 80};
    std::list<double> l1,l2,li;
    std::list<std::list<double> > lr;
    for(auto& n : vn){
        PoissonSolver(n, l1, l2, li, lr);
    }
    // output to a file
    std::ofstream fout("./fig/error_table.txt",std::ios::out);

    tfm::format(fout, "n,norm1,norm2,norm inf\n");
    auto itervn = vn.begin();
    auto iterl1 = l1.begin();
    auto iterl2 = l2.begin();
    auto iterli = li.begin();
    for(;itervn != vn.end();){
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

    // plot residual
    PlotResidual("residual", vn, lr);
}