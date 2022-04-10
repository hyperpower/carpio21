/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#include "geometry/geometry.hpp"
#include "utility/tinyformat.hpp"
#include "geometry/io/gplotly_actor.hpp"
#include "gtest/gtest.h"

using namespace carpio;

typedef Point_<double, 3>   Point3;
typedef Point_<double, 2>   Point2;
typedef Segment_<double, 2> Seg2;
typedef Box_<double, 2>     Box2;
typedef Box_<double, 3>     Box3;
typedef Line_<double>       Line;

typedef PointChain_<double, 3> PointChain;
//typedef GGnuplotActor_<double, 2> GA;
typedef GGnuplotActorMaker_<double, 2> GAM;

TEST(bezier, mkdir){
    // Do not disable this test
    MakeDir("./fig/");
}

TEST(box, points){
    std::cout << "Initial box3d" << std::endl;
    Point3 p1(0.0, 0.0, 0.0);
    Point3 p2(1.0, 1.0, 1.0);
    Point3 p3(0.0, 1.0, 1.0);

    std::vector<Point3> arr;
    arr.push_back(p1);  
    arr.push_back(p2);  
    arr.push_back(p3);  


    Plotly_ plotly;

    auto ap = ToPlotlyActor(arr);

    plotly.add(ap);

    // plotly.write("./fig/out", "html");
}



TEST(box, box3_vs_plane_plotly){
    std::cout << "Initial box3d" << std::endl;
    Point3 min1(0.0, 0.0, 0.0);
    Point3 max1(1.0, 1.0, 1.0);
    Box3 box1(min1, max1);
    std::cout << "The box 1 is " << box1 << std::endl;

    std::cout << ToString(max1.max_axes()) << std::endl; 
    Plotly_ plotly;

    auto abox = ToPlotlyActor(box1);
    abox.update("name", "box");
    abox.update("line_color", "black");
    abox.update("line_width", 3.0);
    plotly.add(abox);
    plotly.add(MakePlotlyCoordinateArrow<3>());

    Plane_<double> plane(1.0, 1.0, 1.0, 3.0);
    std::cout << plane << std::endl;

    typedef std::array<double, 2> Arr2;
    auto ap = ToPlotlyActor(plane, Arr2{-2.0,2.0}, Arr2{-2.0,2.0});
    ap.update("color",  "red");
    ap.update("name",   "plane");
    plotly.add(ap);
    
    _FunctorIntersectOrientPlaneUnitBox_<Point3> Functor;
    auto listp = Functor(plane.a(), plane.b(), plane.c(), plane.alpha());
    std::cout << "Return size : " << listp.size() << std::endl;
    for (auto& p : listp){
        std::cout << " " << p << " " << std::endl;
    }
    PointChain pc(listp);
    plotly.add(ToPlotlyActor(pc));
    // plotly.add(ToPlotlyActor(listp, "vertex"));
    // plotly.title("a title name ");
    plotly.margin(0, 0, 0, 0);
    plotly.layout("width", 660.0);
    plotly.layout("height", 400.0);
    plotly.layout("scene_aspectmode", "cube");
    plotly.layout("scene_xaxis_range", -0.5, 1.5);
    plotly.layout("scene_yaxis_range", -0.5, 1.5);
    plotly.layout("scene_zaxis_range", -0.5, 1.5);
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

    // plotly.show();
    // plotly.write("./fig/out", "html");
    
}

TEST(box, box3_plane_plotly){
    std::cout << "Initial box3d" << std::endl;
    Point3 min1(0.3, 0.2, 0.4);
    Point3 max1(1.4, 1.3, 1.6);
    Box3 box1(min1, max1);
    std::cout << "The box 1 is " << box1 << std::endl;

    std::cout << ToString(max1.max_axes()) << std::endl; 
    Plotly_ plotly;

    auto abox = ToPlotlyActor(box1);
    abox.update("name", "box");
    abox.update("line_color", "black");
    abox.update("line_width", 3.0);
    plotly.add(abox);
    plotly.add(MakePlotlyCoordinateArrow<3>());

    Plane_<double> plane(-1.0, -1.1, -1.2, -3.5);
    std::cout << plane << std::endl;

    typedef std::array<double, 2> Arr2;
    auto ap = ToPlotlyActor(plane, Arr2{-2.0,2.0}, Arr2{-2.0,2.0});
    ap.update("color",  "red");
    ap.update("name",   "plane");
    plotly.add(ap);
    
    auto listp = IntersectPlaneBox(min1, max1, plane.a(), plane.b(), plane.c(), plane.alpha());
    std::cout << "Return size : " << listp.size() << std::endl;
    for (auto& p : listp){
        std::cout << " " << p << " " << std::endl;
    }
    PointChain pc(listp);
    plotly.add(ToPlotlyActor(pc));
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

    // plotly.show();
    plotly.write("./fig/out", "html");
    
}