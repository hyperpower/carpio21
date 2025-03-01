#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "domain/structure/operator/sapply_bc_vertex.hpp"
#include "domain/structure/operator/slaplacian_differential.hpp"

#include "domain/structure/operator/scommon.hpp"

#include "equation/poisson/laplace.hpp"
#include "equation/equation.hpp"
#include "equation/event/event.hpp"

using namespace carpio;
#include "test_define.hpp"


// TEST(equ_laplacian, a_test){
//     if(TestDir() == false){
//         std::abort();
//     }
// }

TEST(equ_laplacian_vertex, solve){
    std::cout << "[  Laplace ] Test Vertex Version"<<std::endl;
    const int dim = 2;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost, VertexTag> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p,10, 1, 2));
    std::cout << "n = " << spgrid->n() << std::endl;
    std::cout << "N = " << spgrid->N() << std::endl;
    std::cout << "v = " << Order::Index(-1, -1) << spgrid->v(Order::Index(-1,-1)) << std::endl;
    std::cout << "v = " << Order::Index(9, 9) << spgrid->v(Order::Index(9,9)) << std::endl;
    std::cout << "c = " << spgrid->c(Order::Index(0,0)) << std::endl;
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

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
    equ.set_solver("Jacobi", 10000, 1e-7);
    equ.set_space_scheme("HOC4");
    // equ.set_space_scheme("finite_difference_2");
    
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
    // gnu.set_ylabel("y");
    // gnu.set_xlabel("x");
    gnu.set_equal_aspect_ratio();
    // gnu.add(ToGnuplotActorWireFrame(*spgrid));
    gnu.add(ToGnuplotActorContourWire(equ.field("phi")));
    gnu.add(ToGnuplotActorContourPoints(equ.field("phi"), *spbi));
    gnu.set_palette_red_blue_dark();
    gnu.set_grid();
    gnu.set_view(60.0,40.0,1.1,1.0);
    gnu.set_ticslevel(0.0);
    gnu.set_terminal_png(FIG_PATH + "Solved_Phi_Ver", fig_width, fig_height);
    gnu.plot();


    
}