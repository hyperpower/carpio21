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
#include <cmath>

using namespace carpio;


template<class EQU>
inline void plot_section_compare(const std::string& scheme, const EQU& equ){
    Gnuplot gun_section;
    gun_section.set_terminal_png("./fig/" + scheme +"_section");

    gun_section.set_xrange(0.0, 40.0);
    gun_section.set_yrange(-0.1, 1.1);
    gun_section.set_xlabel("y");
    gun_section.set_ylabel("value");

    auto a_exact = ToGnuplotActorSection(equ.field("exact"), _Y_, 40.0);
    a_exact.title("Exact");
    a_exact.with_lines();
    a_exact.line_color_black();
    gun_section.add(a_exact);

    auto a_section = ToGnuplotActorSection(equ.field("phi"), _Y_, 40.0);
    a_section.title(scheme);
    a_section.point_type(7);
    gun_section.add(a_section);
    gun_section.set_palette_red_blue();
    gun_section.set_key("top left");
    gun_section.plot();
}


int run_a_scheme(const std::string& scheme){
    std::cout << "[Advection ] Rotational square"<<std::endl;
    const int dim = 2;
    const St n     = 80;                           // number of cells
    const Vt CFL   = 0.4;                          // CFL
    const Vt dt    = CFL * 80.0 / n;               // delta time
    const St steps = 360.0 / dt;

    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    std::cout << "[  Scheme  ] Scheme = "<< scheme <<std::endl;
    typedef Point_<double,dim> Point;
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;

    Point p(-1.0,0.0);
    std::shared_ptr<Grid>  spgrid(new Grid(p, 
                                        {n,  n}, // num on each direction
                                        80.0 / n,     // cell size
                                        2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));

    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Advection_<Domain> Advection;

    // Define the equation
    Advection equ(spgrid, spghost, sporder);

    equ.set_time_term(steps, dt);
    // equ.set_time_term(10, dt);

    equ.set_space_scheme(scheme);

    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    equ.set_boundary_index_phi(spbi);

    // ASSERT_MSG(spbi == nullptr, "MyFunction: requires non-null argument");

     // Set initial condition
    equ.set_phi([](Vt x, Vt y, Vt z){
        Vt xc = 20;
        Vt yc = 40;
        Vt l  = 8;
        if(   IsInRange(xc-l, x, xc+l, _cc_) 
           && IsInRange(yc-l, y, yc+l, _cc_)){
            return 1.0;
        }else{
            return 0.0;
        }
    });

    equ.set_center_velocity(_X_, [](Vt x, Vt y , Vt z){
        return -(y - 40.0)*2*3.1415926/360.0;
    });
    equ.set_center_velocity(_Y_, [](Vt x, Vt y , Vt z){
        return (x - 40.0)*2*3.1415926/360.0;
    });

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
            Vt xc = 20;
            Vt yc = 40;
            Vt l  = 8;
            if(   IsInRange(xc-l, x, xc+l, _cc_) 
            && IsInRange(yc-l, y, yc+l, _cc_)){
                return 1.0;
            }else{
                return 0.0;
            }
        }, 
        -1, -1, 1, Event::BEFORE);
    equ.add_event("SetExact", std::make_shared<EventSetFieldCenter>(esfc));


    // plot field
    // if(scheme == "fou"){
    typedef EventGnuplotField_<Domain> EventGnuplotField;
    EventGnuplotField::FunPlot fun_plot = [&scheme](
                           Gnuplot& gnu,
                     const EventGnuplotField::FieldCenter& f,
                     St step, Vt t, int fob,
                     typename EventGnuplotField::EquationBase& pd){

        
        auto af = ToGnuplotActorContourWire(f);
        // af.title(scheme);
        af.line_width(1);
        gnu.add(af);

        // add label
        gnu.set_label(10, "Step = " + ToString(step), 5, 10, 3);
        gnu.set_label(11, "Time = " + ToString(t, "%.2f"), 5, 10, 2.5);
        gnu.splot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun_plot, -1, -1, 1, Event::AFTER );
    egs.set_figure_font("Fira Code");
    // egs.set_figure_width(fig_width);
    // egs.set_figure_height(fig_height);
    egs.gnuplot().set_xrange(0, 80);
    egs.gnuplot().set_yrange(0, 80);
    egs.gnuplot().set_zrange(-0.2, 1.1);
    egs.gnuplot().set_view(25,25,1.1,1.0);
    egs.gnuplot().set_ticslevel(0.0);
    egs.gnuplot().set_xlabel("x");
    egs.gnuplot().set_ylabel("y");
    // egs.gnuplot().set_zrange( 0.0, 1.1);
    // egs.gnuplot().set_equal_aspect_ratio();
    egs.gnuplot().set_cbrange(-0.2, 1.2);
    egs.gnuplot().set_palette_red_blue_dark();
    // egs.gnuplot().set_palette_red_blue();
    egs.set_path(FIG_PATH + scheme + "_" );
    egs.set_format_string("%s_%03d");

    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));
    // }

    equ.run();

    // Plot last section
    plot_section_compare(scheme, equ); 
}


int main(int argc, char** argv) {
    std::vector<std::string> schemes = {
        "fou",
        "Superbee", "Minmod"
    };
    std::list<GnuplotActor> la;
    for(auto& scheme : schemes){
        run_a_scheme(scheme);
    }
    // Gnuplot gun;
    // gun.set_terminal_png("./fig/section_compare");

    // gun.set_xrange(0.0, 1.0);
    // // gun.set_yrange(-0.2, 1.2);
    // gun.set_xlabel("y");
    // gun.set_ylabel("value"); 
    // gun.set_key("top left");
    // gun.set_palette_red_blue();
    // gun.add(la);
    // gun.plot();
}
