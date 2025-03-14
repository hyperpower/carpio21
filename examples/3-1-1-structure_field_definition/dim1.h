#ifndef _DIM1_H_
#define _DIM1_H_
#include <iostream>
#include <memory> 
#include <string>
#include "example_define.hpp"

#include "geometry/geometry.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/base/base_io.hpp"
#include "domain/structure/io/s_stringify.hpp"

#include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

void GnuplotSetup(Gnuplot& gnu){
    gnu.set_xrange(-0.5, 3.0);
    gnu.set_yrange(-0.5, 2.0);
    gnu.set_xlabel("Location");
    gnu.set_ylabel("Scalar");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
}


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
    GnuplotSetup(gnu);
    
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

    gnu.set_terminal_png(FIG_PATH + "UniformStructureGrid1", fig_width, fig_height);
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
    GnuplotSetup(gnu); 
    
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
    gnu.set_terminal_png(FIG_PATH + "NonUniformStructureGrid1", fig_width, fig_height);
    gnu.plot();
}

#endif