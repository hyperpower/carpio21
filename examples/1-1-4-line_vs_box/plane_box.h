#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"
#include "geometry/io/gplotly_actor.hpp"


using namespace carpio;

typedef Point_<double, 3>              Point3;
typedef Box_<double, 3>                Box3;
typedef Plane_<double>                 Plane;
typedef PointChain_<double, 3>         PointChain3;


void PlaneBoxIntersection(const std::string& name, 
                          const Box3& box, const Plane& plane){
    std::cout << "The box is " << box << std::endl;
    
    Plotly_ plotly;
    std::cout << plotly.version() << std::endl;

    auto abox = ToPlotlyActor(box);
    abox.update("name", "box");
    abox.update("line_color", "black");
    abox.update("line_width", 3.0);
    plotly.add(abox);
    plotly.add(MakePlotlyCoordinateArrow<3>());

    std::cout << "The plane is " << plane << std::endl;

    typedef std::array<double, 2> Arr2;
    auto ap = ToPlotlyActor(plane, Arr2{-2.5,2.5}, Arr2{-2.5,2.5});
    ap.update("color",  "red");
    ap.update("name",   "plane");
	ap.update_true("showlegend");
    plotly.add(ap);
    
    auto listp = IntersectPlaneBox(box.min(), box.max(), plane.a(), plane.b(), plane.c(), plane.alpha());

    std::cout << "Intersection points size : " << listp.size() << std::endl;
    std::cout << "Intersection Points :" << std::endl;
    int count = 0; 
    for (auto& p : listp){
        std::cout << count <<  " " << p << " " << std::endl;
        count++;
    }

    PointChain3 pc(listp);
    auto apc = ToPlotlyActor(pc);
    apc.update("name", "Intersection Points");
    apc.update("line_color", "blue");
    apc.update("line_width", 2.0);
	apc.update_true("showlegend");
    plotly.add(apc);
    // plotly.add(ToPlotlyActor(listp, "vertex"));
    // plotly.title("a title name ");
    plotly.margin(0, 0, 0, 0);
    plotly.layout("width", 660.0);
    plotly.layout("height", 400.0);
    plotly.layout("scene_aspectmode", "cube");
    plotly.layout("scene_xaxis_range", -0.5, 2.5);
    plotly.layout("scene_yaxis_range", -0.5, 2.5);
    plotly.layout("scene_zaxis_range", -0.5, 2.5);
    plotly.layout("scene_xaxis_dtick", 0.5);
    plotly.layout("scene_yaxis_dtick", 0.5);
    plotly.layout("scene_zaxis_dtick", 0.5);
    // plotly.layout_true("scene_xaxis_zeroline");
    // plotly.layout_false("scene_xaxis_visible");
    // plotly.layout("scene_xaxis_zerolinewidth" , 3);
    // plotly.layout("scene_xaxis_zerolinecolor" , "black");

    // plotly.layout_false("scene_yaxis_showgrid");
    // plotly.layout_false("scene_zaxis_showgrid");
    plotly.layout_false("scene_xaxis_showbackground");
    plotly.layout("scene_xaxis_gridcolor",      "grey");
    plotly.layout_false("scene_yaxis_showbackground");
    plotly.layout("scene_yaxis_gridcolor",      "grey");
    plotly.layout_false("scene_zaxis_showbackground");
    plotly.layout("scene_zaxis_gridcolor",      "grey");
    plotly.layout("scene_camera_eye_y", -2.0);
    plotly.layout("scene_camera_eye_z", 0.5);
    // plotly.layout("scene_camera_projection_type", "orthographic");
    plotly.layout("legend_x", 0.05);
    plotly.layout("legend_y", 0.95);
    plotly.layout("legend_traceorder", "normal");
    plotly.layout("legend_font_size", 13.0);
    // plotly.show();
    plotly.write("./fig/" + name , "div");
    
}