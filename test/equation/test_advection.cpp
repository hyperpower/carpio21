#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/structure.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "domain/structure/operator/sapply_bc.hpp"
#include "domain/structure/operator/sintegral_laplacian.hpp"

#include "domain/structure/operator/scommon.hpp"

#include "equation/advection/advection.hpp"
#include "equation/equation.hpp"
#include "equation/event/event.hpp"

using namespace carpio;

TEST(s_interpolate, a_test){
    if(TestDir() == false){
        std::abort();
    }
}

TEST(equ_advection, fou){
    std::cout << "[Advection ] Test First order upwind"<<std::endl;
    const int dim = 1;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p, 100, 2.0, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Advection_<Domain> Advection;

    // Define the equation
    Advection equ(spgrid, spghost, sporder);

    equ.set_time_term(100, 0.01);

    equ.set_space_scheme("VanLeer");

    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    spBI spbi(new BoundaryIndex());
    // spbi->insert(0,std::make_shared<BC>(
    //         BoundaryConditionValue(BC::_BC1_, 1.0)));
    equ.set_boundary_index_phi(spbi);

    // ASSERT_MSG(spbi == nullptr, "MyFunction: requires non-null argument");

     // Set initial condition
    equ.set_phi([](Vt x, Vt y, Vt z){
        if(x < 0.5){
            return 1.0;
        }else{
            return 0.0;
        }
    });

    equ.set_center_velocity(_X_, [](Vt, Vt, Vt){return 1.0;});

    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::AFTER | Event::END));
	equ.add_event("OutputTime", spetime);

    typedef EventGnuplotField_<Domain> EventGnuplotField;
    EventGnuplotField::FunPlot fun = [](
                           Gnuplot& gnu,
                     const EventGnuplotField::FieldCenter& f,
                     St step, Vt t, int fob,
                     typename EventGnuplotField::EquationBase& pd){
        gnu.add(ToGnuplotActorContourWire(f));
        gnu.plot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER );
    egs.set_figure_font("Fira Code");
    // egs.set_figure_width(fig_width);
    // egs.set_figure_height(fig_height);
    egs.gnuplot().set_xrange(-0.0, 2.0);
    egs.gnuplot().set_yrange(-0.2, 1.4);
    // egs.gnuplot().set_zrange( 0.0, 1.1);
    // egs.gnuplot().set_equal_aspect_ratio();
    egs.gnuplot().set_cbrange(0.0, 1.0);
    egs.gnuplot().set_palette_red_blue();
    egs.set_path(FIG_PATH + "ex_");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    typedef EventConditonNormPrevious_<Domain> ECNormPrev;
    typedef std::shared_ptr<ECNormPrev> spECNormPrev;
    typedef std::shared_ptr<Event> spEvent;
    typedef std::shared_ptr<Condition_<Domain> > spCondition;

    ECNormPrev ec("phi", 1e-1, 1e-1, 1e-1);
    spECNormPrev spec = std::make_shared<ECNormPrev>(ec);
    spEvent spe = spec;
    spCondition spc = spec;

    equ.add_stop_condition("StopNormPrev", spec);
    


    equ.run();

}