#include "domain/domain.hpp"
#include "equation/equation.hpp"
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";
const int fig_width  = 800;
const int fig_height = 600;

TEST(equation, equation_base){
    std::cout << "Equation Test"<<std::endl;
    const int dim = 2;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p,10, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef SField_<dim, Vt, Grid, Ghost, Order> Field;

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef EquationBase_<Domain> EquationBase;

    // EquationBase equ(spgrid, spghost, sporder);
    // auto s = Stringify(*spgrid);
    // for(auto& row : s){
        // std::cout << row << std::endl;
    // }
    // OutputFile("./test_output/grid.txt", *spgrid);

    // TextFile f("./test_output/grid.txt");
    // f.read();
    // f.parse_config();
    // f.show_config();
}

TEST(equation, DISABLED_laplace){
    std::cout << "[  Laplace ] Test"<<std::endl;
    const int dim = 2;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p,10, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef SField_<dim, Vt, Grid, Ghost, Order> Field;

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Laplace_<Domain> Laplace;

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
	equ.set_solver("Jacobi", 100, 1e-4);

    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();
    
    Gnuplot gnu;
	gnu.set_xrange(-0.1, 1.1);
	gnu.set_yrange(-0.1, 1.1);
	gnu.set_ylabel("y");
	gnu.set_xlabel("x");
	gnu.set_equal_aspect_ratio();
	gnu.set_palette_blue_red();
	gnu.add(ToGnuplotActorContour(equ.field("phi")));
    gnu.set_terminal_png(OUTPUTPATH + "SolutionContour", fig_width, fig_height);
	gnu.plot();

}

TEST(equation, Poisson){
    std::cout << "[  Poisson ] Test"<<std::endl;
    const int dim = 2;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p,10, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Poisson_<Domain> Poisson;

    // Define the equation
    Poisson equ(spgrid, spghost, sporder);

    // Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
	// spBC spbcym(new BoundaryConditionValue(BC::_BC2_, 0.0));
	// spbi->insert(0, spbcym);
	// spbi->insert(1, spbcym);
	// spbi->insert(2, spbcym);
	// spbi->insert(3, spbcxm);
	equ.set_boundary_index("phi", spbi);

    // Set solver
	equ.set_solver("Jacobi", 100, 1e-4);

    // Set source
    equ.set_source([](typename Domain::ValueType x,
                      typename Domain::ValueType y,
                      typename Domain::ValueType z){
                        return -8.0 * _PI_ * _PI_ * std::sin(2.0*_PI_*x)*std::sin(2.0*_PI_*y);
                      });
    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();
    
    Gnuplot gnu;
	gnu.set_xrange(-0.1, 1.1);
	gnu.set_yrange(-0.1, 1.1);
	gnu.set_ylabel("y");
	gnu.set_xlabel("x");
	gnu.set_equal_aspect_ratio();
	gnu.set_palette_blue_red();
	gnu.add(ToGnuplotActorContour(equ.field("phi")));
    gnu.set_terminal_png(OUTPUTPATH + "Poisson_SolutionContour", fig_width, fig_height);
	gnu.plot();

}