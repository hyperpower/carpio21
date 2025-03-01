#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/structure.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "domain/structure/operator/sapply_bc.hpp"

#include "domain/structure/operator/scommon.hpp"

#include "equation/advection/advection.hpp"
#include "equation/equation.hpp"
#include "equation/event/event.hpp"

#include "example_define.hpp"
#include "convergence_analysis.hpp"


using namespace carpio;


int run_a_scheme(const std::string& scheme){
    std::cout << "[Advection ] One Step 2D"<<std::endl;
    const int dim = 2;
    const St n   = 50;                           // number of cells
    const Vt CFL = 0.4;                          // CFL
    const Vt dt  = CFL / n;                      // delta time
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p, 
                                        {n,  n},      // num on each direction
                                        1.0 / n,     // cell size
                                        2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Advection_<Domain> Advection;

    // Define the equation
    Advection equ(spgrid, spghost, sporder);

    equ.set_time_term(1000, dt);
    equ.set_space_scheme(scheme);

    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
    spbi->insert(0, spbcxm);
    spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
    spbi->insert(2, spbcym);
    equ.set_boundary_index_phi(spbi);

    // ASSERT_MSG(spbi == nullptr, "MyFunction: requires non-null argument");

     // Set initial condition
    equ.set_phi([](Vt x, Vt y, Vt z){
        return 0.0;
    });

    equ.set_center_velocity(_X_, [](Vt, Vt, Vt){return 1.0;});
    equ.set_center_velocity(_Y_, [](Vt, Vt, Vt){return 1.0;});

    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::AFTER | Event::END));
	equ.add_event("OutputTime", spetime);

   
    // exact field
    typedef EventSetFieldCenter_<Domain> EventSetFieldCenter;
    equ.new_field("exact");
    EventSetFieldCenter esfc("exact", 
        [](Vt x, Vt y, Vt z, Vt t){
            if(y > x){
                return 1.0;
            }else{
                return 0.0;
            }
        }, 
        -1, -1, 1, Event::BEFORE);
    equ.add_event("SetExact", std::make_shared<EventSetFieldCenter>(esfc));


    // plot field
    typedef EventGnuplotField_<Domain> EventGnuplotField;
    EventGnuplotField::FunPlot fun = [&scheme](
                           Gnuplot& gnu,
                     const EventGnuplotField::FieldCenter& f,
                     St step, Vt t, int fob,
                     typename EventGnuplotField::EquationBase& pd){

        
        auto af = ToGnuplotActorContour(f);
        // af.title(scheme);
        af.line_width(2);
        gnu.add(af);

        // add label
        gnu.set_label(10, "Step = " + ToString(step), 0., 1.1);
        gnu.set_label(11, "Time = " + ToString(t, "%.2f"), 0.3, 1.1);
        gnu.plot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER );
    egs.set_figure_font("Fira Code");
    // egs.set_figure_width(fig_width);
    // egs.set_figure_height(fig_height);
    egs.gnuplot().set_xrange(-0.2, 1.2);
    egs.gnuplot().set_yrange(-0.2, 1.2);
    egs.gnuplot().set_xlabel("x");
    egs.gnuplot().set_ylabel("y");
    // egs.gnuplot().set_zrange( 0.0, 1.1);
    egs.gnuplot().set_equal_aspect_ratio();
    egs.gnuplot().set_cbrange(0.0, 1.0);
    egs.gnuplot().set_palette_red_blue();
    egs.set_path(FIG_PATH + scheme + "_" );
    egs.set_format_string("%s_%03d");

    // egs.set_path("./fig/phi_");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    typedef EventNormExactFieldCenter_<Domain> EventErrorNorm;
    typedef std::shared_ptr<EventErrorNorm> spEventErrorNorm;
    EventErrorNorm een(&(equ.field("exact")), "phi", -1, -1, 1, Event::AFTER);
    spEventErrorNorm speen = std::make_shared<EventErrorNorm>(een);
    equ.add_event("ErrorPhi", speen);

    typedef EventConditonNormPrevious_<Domain> ECNormPrev;
    typedef std::shared_ptr<ECNormPrev> spECNormPrev;
    ECNormPrev ec("phi",1e-5, 1e-5, 1e-5, 10);
    spECNormPrev spec = std::make_shared<ECNormPrev>(ec);
    equ.add_stop_condition("ECNorm", spec);


    equ.run();

    auto lt  = speen->get_list_step();
    auto ln1 = speen->get_list_norm1();
    auto ln2 = speen->get_list_norm2();
    auto lni = speen->get_list_norminf();

    std::string ffn = "./fig/" + scheme + "_norm";
    PlotNormWithStep(ffn, lt, ln1, ln2, lni);
}


int main(int argc, char** argv) {
    std::vector<std::string> schemes = {
        "fou",
    };
    for(auto& scheme : schemes){
        run_a_scheme(scheme);
    }
}
