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

#include "example_define.hpp"

#include <cmath>

using namespace carpio;

void plot_error_norm(const std::string& fn_prefix, 
                     const std::list<St>& ls,
                     const std::list<Vt>& ln1,
                     const std::list<Vt>& ln2,
                     const std::list<Vt>& lni
){
    Gnuplot gnu;
    auto an1 = ToGnuplotActor(ls, ln1);
    an1.title("Norm1");
    an1.style("with lines ");  
    an1.line_width(2);
    gnu.add(an1);
    auto an2 = ToGnuplotActor(ls, ln2);
    an2.title("Norm2");
    an2.style("with lines ");  
    an2.line_width(2);
    gnu.add(an2);
        
    auto ani = ToGnuplotActor(ls, lni);
    ani.title("Norm inf");
    ani.style("with lines ");  
    ani.line_width(2);
    gnu.add(ani);

    gnu.set_xlabel("Step");
    gnu.set_ylabel("Error Norm");
    gnu.set_ylogscale();
    gnu.set_yformat("10^{%L}");

    gnu.set_terminal_png("./fig/" + fn_prefix + "_norm", 
	                    fig_width, fig_height);

    gnu.plot();
}


int run_a_scheme(const std::string& scheme, std::list<GnuplotActor>& list_a){
    std::cout << "[Advection ] Rotational one gate"<<std::endl;
    const int dim = 2;
    const St n   = 60;                           // number of cells
    const Vt CFL = 0.6;                          // CFL
    const Vt dt  = CFL / n;                      // delta time
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    std::cout << "[  Scheme  ] Scheme = "<< scheme <<std::endl;
    typedef Point_<double,dim> Point;
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegularSubdivision_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    Point p(-1.0,0.0);
    std::shared_ptr<Grid>  spgrid(new Grid(p, 
                                        {2 * n,  n}, // num on each direction
                                        1.0 / n,     // cell size
                                        2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    typename Ghost::FunBoundaryID funbid = [](int bid, Vt x, Vt y, Vt z){
        if(x < 0.0){
            return 21; // inlet boundary
        }else{
            return 22; // outlet boundary
        }
    };
    spghost->set_boundary_id_function(2, funbid);

    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Advection_<Domain> Advection;

    // Define the equation
    Advection equ(spgrid, spghost, sporder);

    equ.set_time_term(500, dt);
    equ.set_space_scheme(scheme);

    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    BoundaryConditionFunXYZ::FunXYZ_Value fun = [](Vt x, Vt y, Vt z){
        if((x >= -0.8 && x <= -0.6) || 
            (x >= -0.4 && x <= -0.2)){
            return 1.0;
        }else{
            return 0.0;
        }
    };
    spBC spbcxm(new BoundaryConditionFunXYZ(BC::_BC1_, fun));
    spbi->insert(21, spbcxm);
    
    equ.set_boundary_index_phi(spbi);

    // ASSERT_MSG(spbi == nullptr, "MyFunction: requires non-null argument");

     // Set initial condition
    equ.set_phi([](Vt x, Vt y, Vt z){
        return 0.0;
    });

    equ.set_center_velocity(_X_, [](Vt x, Vt y , Vt z){return y;});
    equ.set_center_velocity(_Y_, [](Vt x, Vt y , Vt z){return -x;});

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
            if(((x * x + y * y) > (0.6 * 0.6)
                 && (x * x + y * y) < (0.8 * 0.8))
                || ((x * x + y * y) < (0.4 * 0.4)
                 && (x * x + y * y) > (0.2 * 0.2)) ){
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

        
        auto af = ToGnuplotActorContour(f);
        // af.title(scheme);
        af.line_width(2);
        gnu.add(af);

        // add label
        gnu.set_label(10, "Step = " + ToString(step), -0.7, 1.1);
        gnu.set_label(11, "Time = " + ToString(t, "%.2f"), -0.2, 1.1);
        gnu.plot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun_plot, -1, -1, 1, Event::AFTER );
    egs.set_figure_font("Fira Code");
    // egs.set_figure_width(fig_width);
    // egs.set_figure_height(fig_height);
    egs.gnuplot().set_xrange(-1.2, 1.2);
    egs.gnuplot().set_yrange(-0.2, 1.2);
    egs.gnuplot().set_xlabel("x");
    egs.gnuplot().set_ylabel("y");
    // egs.gnuplot().set_zrange( 0.0, 1.1);
    egs.gnuplot().set_equal_aspect_ratio();
    egs.gnuplot().set_cbrange(0.0, 1.0);
    // egs.gnuplot().set_palette_red_blue_dark();
    egs.gnuplot().set_palette_red_blue();
    egs.set_path(FIG_PATH + scheme + "_" );
    egs.set_format_string("%s_%03d");

    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));
    // }

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

    auto lt  = spec->get_list_step();
    auto ln1 = spec->get_list_norm1();
    auto ln2 = spec->get_list_norm2();
    auto lni = spec->get_list_norminf();

    plot_error_norm(scheme, lt, ln1, ln2, lni);

    // Plot last section
    Gnuplot gun_section;
    gun_section.set_terminal_png("./fig/" + scheme +"_section");

    gun_section.set_xrange(0.0, 1.0);
    gun_section.set_yrange(-0.1, 1.1);
    gun_section.set_xlabel("y");
    gun_section.set_ylabel("value");

    auto a_exact = ToGnuplotActorSection(equ.field("exact"), _Y_, 1e-3);
    a_exact.title("Exact");
    a_exact.with_lines();
    a_exact.line_color_black();
    gun_section.add(a_exact);

    auto a_section = ToGnuplotActorSection(equ.field("phi"), _Y_, 1e-3);
    a_section.title(scheme);
    a_section.point_type(7);
    gun_section.add(a_section);
    gun_section.set_palette_red_blue();
    gun_section.set_key("top left");
    gun_section.plot();

    if(scheme == "fou"){
        list_a.push_back(a_exact);
    }
    a_section.point_type(list_a.size());
    list_a.push_back(a_section);
}


int main(int argc, char** argv) {
    std::vector<std::string> schemes = {
        "fou",
        "Superbee", "Minmod"
    };
    std::list<GnuplotActor> la;
    for(auto& scheme : schemes){
        run_a_scheme(scheme, la);
    }
    Gnuplot gun;
    gun.set_terminal_png("./fig/section_compare");

    gun.set_xrange(0.0, 1.0);
    // gun.set_yrange(-0.2, 1.2);
    gun.set_xlabel("y");
    gun.set_ylabel("value"); 
    gun.set_key("top left");
    gun.set_palette_red_blue();
    gun.add(la);
    gun.plot();
}
