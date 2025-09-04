#ifndef _DIM2_H_
#define _DIM2_H_

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

// const std::string FIG_PATH = "./fig/";

// const int fig_width  = 800;
// const int fig_height = 600;

void Uniform2(){
    std::cout << "----- Uniform 2D" << std::endl;
    const std::size_t dim = 2;
    typedef SGridUniform_<dim>                     Grid;
    typedef std::shared_ptr<SGridUniform_<dim> > spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
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

    gnu.set_terminal_png(FIG_PATH + "UniformStructureGrid2", fig_width, fig_height);
    gnu.plot();
}

void NonUniform2(){
    std::cout << "----- NonUniform 2D" << std::endl;
    const std::size_t dim = 2;
    typedef SGridNonUniform_<dim>   Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
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

    gnu.set_terminal_png(FIG_PATH + "NonUniformStructureGrid2", fig_width, fig_height);
    gnu.plot();
}

#endif