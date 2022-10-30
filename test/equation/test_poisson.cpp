#include "domain/domain.hpp"
#include "equation/equation.hpp"
#include "geometry/geometry.hpp"
// #include "io/plotly.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "gtest/gtest.h"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";
const int fig_width  = 800;
const int fig_height = 600;

TEST(equation, explicit_run){
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

TEST(equation, DISABLED_explicit_3d){
    std::cout << "[  Laplace ] Test"<<std::endl;
    const int dim = 3;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p,20, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef SFieldCenter_<dim, Vt, Grid, Ghost, Order> Field;

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;


    typedef Poisson_<Domain> Poisson;

    // Define the equation
    
    Field a(spgrid, spghost, sporder);

    a.assign([](typename Field::ValueType x,
                typename Field::ValueType y,
                typename Field::ValueType z,
                double t){
        return std::sin(2 * _PI_ * x) * std::sin(2 * _PI_ * y) * std::sin(2 * _PI_ * z);
    });

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
    actor.update("isomin", -1);
    actor.update("isomax", 1);
    actor.update("colorscale","RdBu");
    // actor.update("color", c_yellow);
    // actor.update("name", "Original Shape");
    actor.update("surface_count", int(21));
    // actor.update_true("showlegend");
    plotly.add(actor);
    plotly.write("./fig/scale_3d", "html");    
}