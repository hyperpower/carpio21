#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "domain/structure/operator/sapply_bc.hpp"
#include "domain/structure/operator/sintegral_laplacian.hpp"

#include "domain/structure/operator/scommon.hpp"

#include "equation/poisson/laplace.hpp"
#include "equation/equation.hpp"
#include "equation/event/event.hpp"

using namespace carpio;

TEST(equ_laplacian, solve){
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
    equ.set_solver("Jacobi", 1000, 1e-4);
    
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
    gnu.set_palette_red_blue();
    gnu.add(ToGnuplotActorContourWire(equ.field("phi")));
    gnu.set_terminal_png(FIG_PATH + "Solved_Phi", fig_width, fig_height);
    gnu.splot();
}


TEST(equ_laplacian, explicit_step){
    std::cout << "[  Laplace ] Explicit step"<<std::endl;
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
    equ.set_solver("Jacobi", 1000, 1e-4);

    // Set time term
    equ.set_time_term(50, 2e-3);
    
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
    egs.gnuplot().set_palette_red_blue();
    egs.set_path(FIG_PATH + "ex_");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));
    // Add events Error Norm
    typename Domain::FieldCenter exact(spgrid, spghost, sporder);
    typedef EventNormExactFieldCenter_<Domain> EventErrorNorm;
    auto speen = std::make_shared<EventErrorNorm>(exact, "phi", -1, -1, 1, Event::AFTER);
    equ.add_event("ErrorPhi", speen);

    equ.run();

    for(auto& v : speen->get_list_norm1()){
        std::cout << v << std::endl;
    }

    // Gnuplot gnu;
    // gnu.set_xrange(-0.1, 1.1);
    // gnu.set_yrange(-0.1, 1.1);
    // gnu.set_ylabel("y");
    // gnu.set_xlabel("x");
    // gnu.set_equal_aspect_ratio();
    // gnu.set_palette_red_blue();
    // gnu.add(ToGnuplotActorContour(equ.field("phi")));
    // gnu.set_terminal_png(FIG_PATH + "phi", fig_width, fig_height);
    // gnu.plot();
}

TEST(equ_laplacian, implicit_step){
    std::cout << "[  Laplace ] Explicit step"<<std::endl;
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
    equ.set_solver("Jacobi", 1000, 1e-4);

    // Set time term
    equ.set_time_term(50, 5e-3);
    equ.set_time_scheme("CNgeneral");
    
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
    egs.gnuplot().set_palette_red_blue();
    egs.set_path(FIG_PATH + "im_");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    equ.run();

    // Gnuplot gnu;
    // gnu.set_xrange(-0.1, 1.1);
    // gnu.set_yrange(-0.1, 1.1);
    // gnu.set_ylabel("y");
    // gnu.set_xlabel("x");
    // gnu.set_equal_aspect_ratio();
    // gnu.set_palette_red_blue();
    // gnu.add(ToGnuplotActorContour(equ.field("phi")));
    // gnu.set_terminal_png(FIG_PATH + "phi", fig_width, fig_height);
    // gnu.plot();
}
