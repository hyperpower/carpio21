#include "domain/domain.hpp"
#include "geometry/geometry.hpp"
// #include "io/plotly.hpp"
#include "gtest/gtest.h"

#include "equation/equation.hpp"
#include "equation/poisson/poisson.hpp"
#include "equation/event/event.hpp"

#include "test_define.hpp"
using namespace carpio;


const std::size_t dim = 2;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost, VertexTag> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double,dim> Point;

typedef SFieldVertex_<dim, double, Grid, Ghost, Order> Field;
typedef typename Field::Index Index;

void PlotFieldAsContour(const std::string& ffn, const Field& f){
    const int fig_width  = 800;
    const int fig_height = 600;
    Gnuplot gnu;
	gnu.set_xrange(-0.1, 1.1);
	gnu.set_yrange(-0.1, 1.1);
	gnu.set_ylabel("y");
	gnu.set_xlabel("x");
	gnu.set_equal_aspect_ratio();
	gnu.set_palette_red_blue();
	gnu.add(ToGnuplotActorContourPoints(f));
	gnu.add(ToGnuplotActorWireFrame(f.grid()));
    gnu.set_terminal_png(FIG_PATH + ffn, fig_width, fig_height);
	gnu.plot();
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
    Vt h = 1.0 / (n - 1);
    Vt l = h * n;
    Point np(p[0] - 0.5 * h,p[1] - 0.5 * h, p[2] - 0.5 * h);

    spGrid  spgrid(new Grid(np, n, l, 2));
    spGhost spghost(new Ghost(spgrid));
    spOrder sporder(new Order(spgrid, spghost));
    std::cout << "[   INFO   ] h   = " << spgrid->dc() << std::endl;

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
	equ.set_solver("SOR", 20000, 1e-6, 1.0);
    equ.set_space_scheme("HOC4");

    // Set source
    equ.set_source([](typename Domain::ValueType x,
                      typename Domain::ValueType y,
                      typename Domain::ValueType z){
        return  - 8.0 * _PI_ * _PI_ * std::sin(2.0*_PI_*x)*std::sin(2.0*_PI_*y);
    });
    equ.set_analytical_d2x_source([](
        typename Domain::ValueType x,
        typename Domain::ValueType y,
        typename Domain::ValueType z){
            return   32.0 * std::pow(_PI_,4.0) * std::sin(2.0*_PI_*x)*std::sin(2.0*_PI_*y);
        });
    equ.set_analytical_d2y_source([](
        typename Domain::ValueType x,
        typename Domain::ValueType y,
        typename Domain::ValueType z){
            return   32.0 * std::pow(_PI_,4.0) * std::sin(2.0*_PI_*x)*std::sin(2.0*_PI_*y);
        });
    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();

    auto& phi = equ.field("phi");
    std::cout << "[   INFO   ] phi(0,0) = " << phi(0,0) << std::endl;
    
    PlotFieldAsContour("Poisson_SolutionContour" + ToString(n), equ.field("phi"));

    std::cout << "a phi " << equ.field("phi")(Index(0,0))<< std::endl;
    //residual 
    auto spsolver = equ.get_solver();
    lr.push_back(spsolver->get_residual_array());
    std::cout << "Number of Iter : " << spsolver->num_iter() << std::endl;

    // error
    auto exact = equ.field("phi").new_compatible_zero();
    exact.assign([](typename Field::ValueType x,
                    typename Field::ValueType y,
                    typename Field::ValueType z){
        return std::sin(2.0 * _PI_ * x) * std::sin(2.0 * _PI_ * y);
    });
    auto error = exact - equ.field("phi");
    
    PlotFieldAsContour("Poisson_ErrorContour"+ ToString(n), error);
    std::cout << "Norm 1   = " << Norm1(error) << std::endl;
    std::cout << "Norm 2   = " << Norm2(error) << std::endl;
    std::cout << "Norm inf = " << NormInf(error) << std::endl;

    l1.push_back(Norm1(error));
    l2.push_back(Norm2(error));
    li.push_back(NormInf(error));
}

TEST(possion, solver){
    std::vector<int> vn = {11};
    std::list<double> l1,l2,li;
    std::list<std::list<double> > lr;
    for(auto& n : vn){
        PoissonSolver(n, l1, l2, li, lr);
    }
}

