#ifndef _GEOMETRY_PLOTLY_ACTOR_HPP_
#define _GEOMETRY_PLOTLY_ACTOR_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
#include "geometry/objects/objects.hpp"
// #include "io/python_interpreter.hpp"
#include "io/plotly.hpp"
#include "geometry/io/gplotly_actor_ts.hpp"
#include <memory>
#include <cmath>

namespace carpio {

template<typename ANY, typename CONTAINER, 
          typename std::enable_if<
            std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
PlotlyActor MakePlotlyActor(PlaneTag, 
                     const ANY& plane, const CONTAINER& con1, const CONTAINER& con2, 
                     const std::string& type = "auto"){
    assert(con1.size() == 2);
    assert(con2.size() == 2);
    std::string trace_type = (type == "auto") ? "Mesh3d" : type;
    PlotlyActor actor(trace_type);
    actor.update("name",  "plane");
    actor.update("color", "green");
    actor.update("opacity", 0.5);
    typedef Point_<double, ANY::Dim> Point;
    Point norm(plane.a(), plane.b(), plane.c()); 
    Axes  maxd = norm.max_axes();
    Axes d1, d2;
    NormalPlane(maxd, d1, d2);
    
    Point p0;
    p0.set(d1, *(con1.begin()));
    p0.set(d2, *(con2.begin()));
    p0.set(maxd, plane.cal(maxd, p0(d1), p0(d2)));

    Point p1;
    p1.set(d1, *(std::next(con1.begin())));
    p1.set(d2, *(con2.begin()));
    p1.set(maxd, plane.cal(maxd, p1(d1), p1(d2)));

    Point p2;
    p2.set(d1, *(con1.begin()));
    p2.set(d2, *(std::next(con2.begin())));
    p2.set(maxd, plane.cal(maxd, p2(d1), p2(d2)));

    Point p3;
    p3.set(d1, *(std::next(con1.begin())));
    p3.set(d2, *(std::next(con2.begin())));
    p3.set(maxd, plane.cal(maxd, p3(d1), p3(d2)));

    std::list<Point_<double, ANY::Dim> > list;
    list.push_back(p0);
    list.push_back(p1);
    list.push_back(p3);
    list.push_back(p2);

    std::list<std::array<double, 3> > ijk;
    ijk.push_back({0,1,2});
    ijk.push_back({2,3,0});

    if constexpr (ANY::Dim == 2){
        actor.data_xy(list);
        actor.data(ijk, "i", "j", "k");
    }else if constexpr (ANY::Dim == 3){
        actor.data_xyz(list);
        actor.data(ijk, "i", "j", "k");
    }

    return actor;
}

template<class POINT>
PlotlyActorGroup MakePlotlyArrow(const std::string& name, const POINT& p0, const POINT& p1, const std::string& color = "#81BC06"){
    ASSERT(POINT::Dim == 3);
    std::string trace_type = (POINT::Dim == 3) ? "Scatter3d" : "Scatter";
    PlotlyActor actor_line(trace_type);
    PlotlyActorGroup group;

    std::list<POINT> list;
    list.emplace_back(p0);
    list.emplace_back(p1);

    actor_line.data_xyz(list);
    actor_line.update("mode", "lines");
    actor_line.update("name", name + "_arrow_line");
    actor_line.update("line_width", 10);
    actor_line.update("line_color", color);
    actor_line.update_false("showlegend");
    group[name + "_arrow_line"] = actor_line;

    // cone
    auto vec = p1 - p0;
    auto vecn(vec);
    vecn.normalize();

    PlotlyActor actor_cone("Cone");
    std::list<POINT> list_tip;
    list_tip.emplace_back(p1);
    std::list<POINT> list_dir;
    list_dir.emplace_back(vecn);

    actor_cone.data_xyz(list_tip);
    actor_cone.data(list_dir, "u", "v", "w");

    actor_cone.update("name", name + "_arraw_cone");
    actor_cone.update("anchor", "tip");
    actor_cone.update_false("showscale");
    auto len = Distance(p0, p1);
    actor_cone.update("sizeref",  len * 0.25);
    actor_cone.update_colorscale(color, color);
    group[name +"_arrow_cone"] = actor_cone;
    
    return group; 
}

template<St DIM>
PlotlyActorGroup MakePlotlyCoordinateArrow(double xlen =1, double ylen =1, double zlen =1){}

template<>
PlotlyActorGroup MakePlotlyCoordinateArrow<3>(double xlen, double ylen, double zlen){
    typedef Point_<double, 3> Point;
    auto gx = MakePlotlyArrow("x", Point(0, 0, 0), Point(xlen,0,0), "#81BC06");
    auto gy = MakePlotlyArrow("y", Point(0, 0, 0), Point(0,ylen,0), "#F35325");
    auto gz = MakePlotlyArrow("z", Point(0, 0, 0), Point(0,0,zlen), "#05A6F0");

    gx.insert(gy.begin(), gy.end());
    gx.insert(gz.begin(), gz.end());
    return gx;
}

template<typename ANY, 
        typename std::enable_if<
            IsContainer<ANY>::value, 
        bool>::type = true >
auto MakePlotlyActor(PointTag, 
                    const ANY& con_point, 
                    const std::string& type ="auto", int jump =0){
    std::string trace_type = _ChooseType(type);
    PlotlyActor actor(trace_type);
    
    actor.update("name", "points");
    if ( type == "auto" || type == "vertex" ){
        actor.update("mode", "markers");
    }else if( type == "wireframe" ){
        actor.update("mode", "lines");
    }

    if constexpr (ANY::value_type::Dim == 2){
        actor.data_xy(con_point, jump);
    }else if constexpr (ANY::value_type::Dim == 3){
        actor.data_xyz(con_point, jump);
    }
    return actor;
}
template<typename ANY, 
        typename std::enable_if<
            IsGeometry<ANY>::value, 
        bool>::type = true >
auto MakePlotlyActor(PointTag, 
                    const ANY& point1, 
                    const ANY& point2, 
                    const std::string& type ="auto", int jump =0){
    std::list<ANY> list;
    list.emplace_back(point1);
    list.emplace_back(point2);
    return MakePlotlyActor(PointTag(), list, type, jump);
}

template<typename ANY >
auto MakePlotlyActor(PointChainTag, const ANY& pc, const std::string& type ="auto"){
    std::string trace_type = _ChooseType(type);
    PlotlyActor actor(trace_type);
    
    actor.update("name", "points");
    if ( type == "auto" ) { 
        actor.update("mode", "lines+markers");
    }else if( type == "vertex" ){
        actor.update("mode", "markers");
    }else if( type == "wireframe" ){
        actor.update("mode", "lines");
    }

    if (pc.size() <= 0){
        return actor;
    }

    std::list<typename ANY::Point> list;

    for (auto& p : pc){
        list.emplace_back(p);
    }

    if (pc.closed()){
        list.emplace_back(pc.front());
    }

    if constexpr (ANY::Dim == 2){
        actor.data_xy(list, 0);
    }else if constexpr (ANY::Dim == 3){
        actor.data_xyz(list, 0);
    }
    return actor;
    
}
template<typename ANY>
auto MakePlotlyActor(BoxTag, const ANY& box, const std::string& type ="auto"){
    std::string trace_type = ((type == "auto") && (ANY::Dim ==3)) ? "Scatter3d" : "Scatter";
    PlotlyActor actor(trace_type);
    
    actor.update("name", "box");
    actor.update("mode", "lines");

    std::list<typename ANY::Point> list;

    if constexpr (ANY::Dim >= 2){
        list.emplace_back(box.get_point(_M_, _M_, _M_));
        list.emplace_back(box.get_point(_P_, _M_, _M_));

        list.emplace_back(box.get_point(_P_, _M_, _M_));
        list.emplace_back(box.get_point(_P_, _P_, _M_));

        list.emplace_back(box.get_point(_P_, _P_, _M_));
        list.emplace_back(box.get_point(_M_, _P_, _M_));

        list.emplace_back(box.get_point(_M_, _P_, _M_));
        list.emplace_back(box.get_point(_M_, _M_, _M_));
    }
    if constexpr (ANY::Dim == 3){
        list.emplace_back(box.get_point(_M_, _M_, _P_));
        list.emplace_back(box.get_point(_P_, _M_, _P_));

        list.emplace_back(box.get_point(_P_, _M_, _P_));
        list.emplace_back(box.get_point(_P_, _P_, _P_));

        list.emplace_back(box.get_point(_P_, _P_, _P_));
        list.emplace_back(box.get_point(_M_, _P_, _P_));

        list.emplace_back(box.get_point(_M_, _P_, _P_));
        list.emplace_back(box.get_point(_M_, _M_, _P_));

        list.emplace_back(box.get_point(_M_, _M_, _M_));
        list.emplace_back(box.get_point(_M_, _M_, _P_));

        list.emplace_back(box.get_point(_P_, _M_, _M_));
        list.emplace_back(box.get_point(_P_, _M_, _P_));

        list.emplace_back(box.get_point(_P_, _P_, _M_));
        list.emplace_back(box.get_point(_P_, _P_, _P_));

        list.emplace_back(box.get_point(_M_, _P_, _M_));
        list.emplace_back(box.get_point(_M_, _P_, _P_));
    }

    if constexpr (ANY::Dim == 2){
        actor.data_xy(list, 2);
    }else if constexpr (ANY::Dim == 3){
        actor.data_xyz(list, 2);
    }
    return actor;
    
}
template<typename ANY >
auto MakePlotlyActor(TriangleTag, const ANY& tri, 
                     const std::string& name, 
                     const std::string& type){
    std::string trace_type = _ChooseType(type);
    PlotlyActor actor(trace_type);
    
    actor.update("name", name);
    if ( type == "auto" ) { 
        actor.update("mode", "lines+markers");
    }else if( type == "vertex" ){
        actor.update("mode", "markers");
    }else if( type == "wireframe" ){
        actor.update("mode", "lines");
    }

    if (tri.size() <= 0){
        return actor;
    }

    std::list<typename ANY::Point> list;
    std::list<std::array<double, 3> > ijk;

    for (auto& p : tri){
        list.emplace_back(p);
    }
    if( type !="surface"){
        list.emplace_back(tri.front());
    }
    if constexpr (ANY::Dim == 2){
        actor.data_xy(list, 0);
    }else if constexpr (ANY::Dim == 3){
        actor.data_xyz(list, 0);
    }
    if( trace_type == "Mesh3d"){
        ijk.emplace_back(std::array<double,3>{0,1,2});
        actor.data(ijk, "i", "j", "k");
    }
    return actor;
}
template<typename ANY >
auto MakePlotlyActor(TriangleTag, const ANY& tri, 
                     const std::string& type){
    return MakePlotlyActor( TriangleTag(), tri, "triangle", type);
}
// One geometry object
template<typename ANY, ENABLE_IF(ANY, IsGeometry)>
auto ToPlotlyActor(const ANY& geo, const std::string& type ="auto"){
    return MakePlotlyActor( typename ANY::Tag(), geo, type);    
}
// One geometry object with name
template<typename ANY, ENABLE_IF(ANY, IsGeometry)>
auto ToPlotlyActor(const ANY& geo, const std::string& name, const std::string& type){
    return MakePlotlyActor( typename ANY::Tag(), geo, name, type);    
}
// Two geometry objects
template<typename ANY, ENABLE_IF(ANY, IsGeometry)>
auto ToPlotlyActor(const ANY& geo1, const ANY& geo2, const std::string& type ="auto"){
    return MakePlotlyActor( typename ANY::Tag(), geo1, geo2, type);    
}
// Two geometry objects with name
template<typename ANY, ENABLE_IF(ANY, IsGeometry)>
auto ToPlotlyActor(const ANY& geo1, const ANY& geo2, 
                   const std::string& name, const std::string& type){
    return MakePlotlyActor( typename ANY::Tag(), geo1, geo2, name, type);    
}
template<typename ANY,
        typename std::enable_if<
            !(IsGeometry<ANY>::value)
         && IsGeometry<typename ANY::value_type>::value
         && IsContainer<ANY>::value, 
        bool>::type = true >
auto ToPlotlyActor(const ANY& container, const std::string& type ="auto", int jump =0){
    typedef typename ANY::value_type::Tag Tag;
    return MakePlotlyActor( Tag(), container, type, jump);
}
template<typename ANY, typename CONTAINER,
        typename std::enable_if<
            IsGeometry<ANY>::value
         && IsContainer<CONTAINER>::value,
        bool>::type = true >
auto ToPlotlyActor(const ANY& geo, const CONTAINER& con1, const CONTAINER& con2, const std::string& type ="auto"){
    return MakePlotlyActor(typename ANY::Tag(), geo, con1, con2, type);    
}
// template<typename ANY, ENABLE_IF_CONTAINS(ANY, IsGeometry)>
// auto ToPlotlyActor(const ANY& geo, const std::string& type ="auto"){

// }



}

#endif
