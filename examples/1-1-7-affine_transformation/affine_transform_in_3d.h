#ifndef _EXAMPLE_AT_3D_
#define _EXAMPLE_AT_3D_

#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"
#include "geometry/io/gplotly_actor.hpp"

using namespace carpio;

typedef Point_<double, 3>              Point3;
typedef Segment_<double, 3>            Segment3;

typedef TriSurface_<double, 3>     Surface;
typedef std::shared_ptr<Surface> spSurface;

std::string c_yellow = "#FFBA08";
std::string c_blue   = "#05A6F0";

auto MakeShape3d(){
    CreateTS_<double, 3> creator;
    return creator.from_ply_file("./teapot.ply");
}

void AddActorOriginal(Plotly& plot, const Surface& sur){
    auto actor = ToPlotlyActor(sur, "surface");
    actor.update("opacity", 0.3);
    actor.update("color", c_yellow);
    actor.update("name", "Original Shape");
	actor.update_true("showlegend");
    plot.add(actor);
}

void AddActorTransformed(Plotly& plot, const Surface& sur){
    auto actor = ToPlotlyActor(sur, "surface");
    actor.update("opacity", 0.3);
    // actor.update("line_color", c_blue);
    actor.update("color", c_blue);
    actor.update("name", "Transformed Shape");
	actor.update_true("showlegend");
	// actor.update_false("showlegend");
    plot.add(actor);
}

void PlotlyLayoutSetup(Plotly& plotly){
    plotly.add(MakePlotlyCoordinateArrow<3>(2.0,2.0,2.0));

    plotly.margin(0, 0, 0, 0);
    plotly.layout("width",  660.0);
    plotly.layout("height", 400.0);
    plotly.layout("scene_aspectmode", "cube");
    plotly.layout("scene_camera_center_z", -0.15);
    plotly.layout("scene_camera_eye_z",    0.8);
    plotly.layout("scene_camera_eye_y",    -1.5);
    plotly.layout("scene_xaxis_range", -4.0, 4);
    plotly.layout("scene_yaxis_range", -4.0, 4);
    plotly.layout("scene_zaxis_range", -0.0, 8);
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
}
void TestTranslateIn3d(){
    auto teapot = MakeShape3d();

    Plotly plotly;
    PlotlyLayoutSetup(plotly);

    AddActorOriginal(plotly, *teapot);
    // 
    std::array<double, 3> vec   {-1.5,-1.4,2.3};
    // std::array<double, 3> about {-4.0 , -4.0, 0.0};
    Translate(*teapot, vec);
    // 
    AddActorTransformed(plotly, *teapot);
    plotly.write("./fig/translate_3d", "div");    
}

void TestScaleIn3d(){
    auto teapot = MakeShape3d();

    Plotly plotly;
    PlotlyLayoutSetup(plotly);

    AddActorOriginal(plotly, *teapot);
    // 
    std::array<double, 3> vec   {1.1,1.1,1.3};
    std::array<double, 3> about {-4.0 , -4.0, 0.0};
    Scale(*teapot, vec, about);
    // 
    AddActorTransformed(plotly, *teapot);
    plotly.write("./fig/scale_3d", "div");    
}
void TestRotateIn3d(){
    auto teapot = MakeShape3d();

    Plotly plotly;
    PlotlyLayoutSetup(plotly);

    AddActorOriginal(plotly, *teapot);
    // 
    std::array<double, 3> vec   {1.1,1.1,1.3};
    std::array<double, 3> about {-4.0 , -4.0, 0.0};
    Rotate(*teapot, 0.5, _Z_);
    // 
    AddActorTransformed(plotly, *teapot);
    plotly.write("./fig/rotate_z_3d", "div");    
}
void TestShearIn3d(){
    auto teapot = MakeShape3d();

    Plotly plotly;
    PlotlyLayoutSetup(plotly);

    AddActorOriginal(plotly, *teapot);
    // 
    Shear(*teapot, 0.5, _X_);
    // 
    AddActorTransformed(plotly, *teapot);
    plotly.write("./fig/shear_x_3d", "div");    
}
void TestReflectIn3d(){
    auto teapot = MakeShape3d();

    Plotly plotly;
    PlotlyLayoutSetup(plotly);
    plotly.layout("scene_zaxis_range", -4.0, 4);

    AddActorOriginal(plotly, *teapot);
    // 
    Reflect(*teapot);
    // 
    AddActorTransformed(plotly, *teapot);
    plotly.write("./fig/reflect_o_3d", "div");    
}

#endif