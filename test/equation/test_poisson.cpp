#include "domain/domain.hpp"
#include "equation/equation.hpp"
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";
const int fig_width  = 800;
const int fig_height = 600;

TEST(equation, DISABLED_explicit_run){
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

    // Set time term
	equ.set_time_term(50, 1e-3);

    // Set solver
	equ.set_solver("Jacobi", 1000, 1e-4);


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
		gnu.add(ToGnuplotActorContour(f));
		gnu.plot();
		gnu.clear();
		return 1;
	};
	EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER);
    egs.set_figure_font("Fira Code");
	egs.gnuplot().set_xrange(-0.1, 1.1);
	egs.gnuplot().set_yrange(-0.1, 1.1);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.gnuplot().set_cbrange(0.0, 1.0);
    egs.gnuplot().set_palette_blue_red();
	egs.set_path("./fig/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    equ.run();

}

TEST(equation, explicit_3d){
	std::cout << "[  Laplace ] Test"<<std::endl;
    const int dim = 3;
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


    typedef Poisson_<Domain> Poisson;

    // Define the equation
    Poisson equ(spgrid, spghost, sporder);

}