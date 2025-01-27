
#ifndef _DIM3_H_
#define _DIM3_H_

#include <iostream>
#include <memory> 
#include <string>
#include "example_define.hpp"

#include "geometry/geometry.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/grid/nonuniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/base/base_io.hpp"
#include "domain/structure/io/s_stringify.hpp"

#include "domain/structure/io/splotly_actor.hpp"

using namespace carpio;

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
    auto actor = ToPlotlyActorWireFrame(*spgrid);
    actor.update("name", "Grid");
    actor.update_true("showlegend");
    actor.update("line_color", "#24292F");
    plotly.add(actor);

    auto actor_c = ToPlotlyActorPoints(*spgrid);
    actor_c.update("name", "Center Scalar Location");
    actor_c.update("marker_color", "#009E73");
    plotly.add(actor_c);

        
    plotly.write(FIG_PATH + "UniformGrid3d", "div");    
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
    auto actor = ToPlotlyActorWireFrame(*spgrid);
    actor.update("name", "Grid");
    actor.update_true("showlegend");
    actor.update("line_color", "#24292F");
    plotly.add(actor);

    auto actor_c = ToPlotlyActorPoints(*spgrid);
    actor_c.update("name", "Center Scalar Location");
    actor_c.update("marker_color", "#009E73");
    plotly.add(actor_c);

        
    plotly.write(FIG_PATH + "NonUniformGrid3d", "div");    
}



#endif