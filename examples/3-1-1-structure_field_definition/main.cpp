#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/grid/nonuniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/base/base_io.hpp"
#include "domain/structure/io/s_stringify.hpp"

#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;


const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 800;
const int fig_height = 600;

void Uniform1(){
    std::cout << "----- Uniform 1D" << std::endl;
    typedef SGridUniform_<1>                     Grid1;
    typedef std::shared_ptr<SGridUniform_<1> > spGrid1;

    typedef SGhostRegular_<1, Grid1> Ghost1;
    typedef std::shared_ptr<Ghost1> spGhost1;

    typedef SOrderXYZ_<1, Grid1, Ghost1> Order1;
    typedef std::shared_ptr<Order1> spOrder1;

    typedef SFieldCenter_<1, double, Grid1, Ghost1, Order1> Field1;

    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    spGrid1 spgrid(new Grid1(pmin, {5, 5}, 0.5, 2));
    spGhost1 spghost(new Ghost1(spgrid));

    spOrder1 sporder(new Order1(spgrid,spghost));

    Field1 field(spgrid, spghost, sporder);

    Gnuplot gnu;
    gnu.set_xrange(-0.5, 3.0);
    gnu.set_yrange(-0.5, 2.0);
    gnu.set_xlabel("Location");
    gnu.set_ylabel("Scalar");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    gnu.add(agrid);

    //center location
    std::list<double> lx, ly;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        lx.push_back(p.x());
        ly.push_back(0.0);
    }
    auto aloc = ToGnuplotActor(lx, ly);
    aloc.title("Center scalar location");
    aloc.style("with points ps 3 pt 7");
    gnu.add(aloc);

    //scalar on field
    std::list<double> vx, vy;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        field(index) = std::sin(p.x());
        vx.push_back(p.x());
        vy.push_back(field(index));
    }
    auto ascalar = ToGnuplotActor(vx, vy);
    ascalar.title("Scalar");
    ascalar.style("with points ps 3 pt 9");
    gnu.add(ascalar);

    // index label
    int count = 1;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        gnu.set_label(count, ToString(count-1), p.x(), - 0.15, "center noenhance");
        count++;
    }

    auto& grid = field.grid();

    GnuplotActorDistanceIndicator anno_s(0.5, 0.0, 1.0, 0.0);
    anno_s.set_space(0.12);
    anno_s.set_height(0.2);
    anno_s.set_normal_line_length(0.35);
    // anno_dis.set_offset(0.5);
    anno_s.build_actors();
    gnu.add(anno_s);
    auto c = anno_s.text_center(0.08);
    gnu.set_label(10, "s_1", c[0], c[1], "center noenhance");


    gnu.set_terminal_png(OUTPUTPATH + "UniformStructureGrid1", fig_width, fig_height);
    gnu.plot();
}
void NonUniform1(){
    std::cout << "----- NonUniform 1D" << std::endl;
    typedef SGridNonUniform_<1>      Grid1;
    typedef std::shared_ptr<Grid1> spGrid1;
    
    typedef SGhostRegular_<1, Grid1> Ghost1;
    typedef std::shared_ptr<Ghost1> spGhost1;

    typedef SOrderXYZ_<1, Grid1, Ghost1> Order1;
    typedef std::shared_ptr<Order1> spOrder1;

    typedef SFieldCenter_<1, double, Grid1, Ghost1, Order1> Field1;

    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);

    // Generator a non-uniform array
    Grid1::Arr a(10);
    double len  = 2.5;
    double tlen = len;
    for(auto i = 0; i < a.size(); i++){
        a[i] = tlen / 4.0;
        tlen = tlen * 3 /4.0; 
    }

    spGrid1 spgrid(new Grid1(pmin, 2, a));
    spGhost1 spghost(new Ghost1(spgrid));
    spOrder1 sporder(new Order1(spgrid,spghost));
    Field1 field(spgrid, spghost, sporder);
    
    Gnuplot gnu;
    gnu.set_xrange(-0.5, 3.0);
    gnu.set_yrange(-0.5, 2.0);
    gnu.set_xlabel("Location");
    gnu.set_ylabel("Scalar");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    gnu.add(agrid);

    //center location
    std::list<double> lx, ly;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        lx.push_back(p.x());
        ly.push_back(0.0);
    }
    auto aloc = ToGnuplotActor(lx, ly);
    aloc.title("Center scalar location");
    aloc.style("with points ps 3 pt 7");
    gnu.add(aloc);

    //scalar on field
    std::list<double> vx, vy;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        field(index) = std::sin(p.x());
        vx.push_back(p.x());
        vy.push_back(field(index));
    }
    auto ascalar = ToGnuplotActor(vx, vy);
    ascalar.title("Scalar");
    ascalar.style("with points ps 3 pt 9");
    gnu.add(ascalar);

    // index label
    int count = 1;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        gnu.set_label(count, ToString(count-1), p.x(), - 0.15, "center noenhance");
        count++;
    }
    gnu.set_terminal_png(OUTPUTPATH + "NonUniformStructureGrid1", fig_width, fig_height);
    gnu.plot();
}

void Uniform2(){
    std::cout << "----- Uniform 2D" << std::endl;
    const std::size_t dim = 2;
    typedef SGridUniform_<dim>                     Grid;
    typedef std::shared_ptr<SGridUniform_<dim> > spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {5, 5}, 0.5, 2));
    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field field(spgrid, spghost, sporder);

    Gnuplot gnu;
    gnu.set_xrange(-0.5, 3.5);
    gnu.set_yrange(-0.5, 3.5);
    gnu.set_xlabel("X");
    gnu.set_ylabel("Y");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    gnu.add(agrid);

    //center location
    std::list<double> lx, ly;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        lx.push_back(p.x());
        ly.push_back(p.y());
    }
    auto aloc = ToGnuplotActor(lx, ly);
    aloc.title("Center scalar location");
    aloc.style("with points ps 3 pt 7");
    gnu.add(aloc);

    gnu.set_terminal_png(OUTPUTPATH + "UniformStructureGrid2", fig_width, fig_height);
    gnu.plot();
}
void PlotlyLayoutSetup(Plotly_& plotly){
    plotly.margin(0, 0, 0, 0);
    plotly.layout("width",  660.0);
    plotly.layout("height", 400.0);
    plotly.layout("font_family", "Fira Code");
    plotly.layout("scene_aspectmode", "cube");
    plotly.layout("scene_camera_center_z", -0.15);
    plotly.layout("scene_camera_eye_z",    0.8);
    plotly.layout("scene_camera_eye_y",    -1.5);
    plotly.layout("scene_xaxis_range", -0.5, 2);
    plotly.layout("scene_yaxis_range", -0.5, 2);
    plotly.layout("scene_zaxis_range", -0.5, 2);
    plotly.layout_false("scene_xaxis_showbackground");
    plotly.layout("scene_xaxis_gridcolor",      "black");
    plotly.layout_false("scene_yaxis_showbackground");
    plotly.layout("scene_yaxis_gridcolor",      "black");
    plotly.layout_false("scene_zaxis_showbackground");
    plotly.layout("scene_zaxis_gridcolor",      "black");
    plotly.layout("legend_x", 0.05);
    plotly.layout("legend_y", 0.95);
    plotly.layout("legend_traceorder", "normal");
    plotly.layout("legend_font_size", 14.0);
}

void Uniform3(){
    std::cout << "----- Uniform 3D" << std::endl;
    const std::size_t dim = 3;
    typedef SGridUniform_<dim>                     Grid;
    typedef std::shared_ptr<SGridUniform_<dim> > spGrid;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {3, 3, 3}, 0.5, 2));

    Plotly_ plotly;
    PlotlyLayoutSetup(plotly);
    auto actor = ToPlotlyActorLines(*spgrid);
    actor.update("name", "Grid");
    actor.update_true("showlegend");
    actor.update("line_color", "#24292F");
    plotly.add(actor);

    auto actor_c = ToPlotlyActorPoints(*spgrid);
    actor_c.update("name", "Center Scalar Location");
    actor_c.update("marker_color", "#009E73");
    plotly.add(actor_c);

        
    plotly.write("./fig/UniformGrid3d", "div");    
}
void NonUniform2(){
    std::cout << "----- NonUniform 2D" << std::endl;
    const std::size_t dim = 2;
    typedef SGridNonUniform_<dim>   Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    // Generator a non-uniform array
    Grid::Arr a(5);
    double len  = 2.5;
    double tlen = len;
    for(auto i = 0; i < a.size(); i++){
        a[i] = tlen / 4.0;
        tlen = tlen * 3 /4.0; 
    }

    spGrid spgrid(new Grid(pmin, 2, a, a));
    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field field(spgrid, spghost, sporder);

    Gnuplot gnu;
    gnu.set_xrange(-0.5, 2.5);
    gnu.set_yrange(-0.5, 2.5);
    gnu.set_xlabel("X");
    gnu.set_ylabel("Y");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    gnu.add(agrid);

    //center location
    std::list<double> lx, ly;
    for(auto& index : field.order()){
        auto p = field.grid().c(index);
        lx.push_back(p.x());
        ly.push_back(p.y());
    }
    auto aloc = ToGnuplotActor(lx, ly);
    aloc.title("Center scalar location");
    aloc.style("with points ps 2 pt 7");
    gnu.add(aloc);

    gnu.set_terminal_png(OUTPUTPATH + "NonUniformStructureGrid2", fig_width, fig_height);
    gnu.plot();
}
void NonUniform3(){
    std::cout << "----- NonUniform 3D" << std::endl;
    const std::size_t dim = 3;
    typedef SGridNonUniform_<dim>   Grid;
    typedef std::shared_ptr<Grid> spGrid;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    // Generator a non-uniform array
    Grid::Arr a(5);
    double len  = 2.5;
    double tlen = len;
    for(auto i = 0; i < a.size(); i++){
        a[i] = tlen / 4.0;
        tlen = tlen * 3 /4.0; 
    }
    spGrid spgrid(new Grid(pmin, 2, a, a, a));

    Plotly_ plotly;
    PlotlyLayoutSetup(plotly);
    plotly.layout("scene_xaxis_range", -0.5, 2.5);
    plotly.layout("scene_yaxis_range", -0.5, 2.5);
    plotly.layout("scene_zaxis_range", -0.5, 2.5);
    auto actor = ToPlotlyActorLines(*spgrid);
    actor.update("name", "Grid");
    actor.update_true("showlegend");
    actor.update("line_color", "#24292F");
    plotly.add(actor);

    auto actor_c = ToPlotlyActorPoints(*spgrid);
    actor_c.update("name", "Center Scalar Location");
    actor_c.update("marker_color", "#009E73");
    plotly.add(actor_c);

        
    plotly.write("./fig/NonUniformGrid3d", "div");    
}
int main(int argc, char** argv) {
    std::cout << "-------- Main ------" << std::endl;
    Uniform1();
    Uniform2();
    Uniform3();
    
    NonUniform1();
    NonUniform2();
    NonUniform3();
}