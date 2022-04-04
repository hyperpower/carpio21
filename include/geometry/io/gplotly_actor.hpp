#ifndef _GEOMETRY_PLOTLY_ACTOR_HPP_
#define _GEOMETRY_PLOTLY_ACTOR_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
#include "geometry/objects/objects.hpp"
#include "io/python_interpreter.hpp"
#include "io/plotly.hpp"
#include <memory>
#include <cmath>

namespace carpio {

// using namespace PlotlyActor;

// template<typename TYPE, St DIM>
// class GPlotlyActor_ {
// public:
// 	static const St Dim = DIM;
// 	typedef TYPE Vt;
// 	typedef Point_<TYPE, DIM> Point;
// 	typedef Point_<TYPE, DIM>& ref_Point;
// 	typedef const Point_<TYPE, DIM>& const_ref_Point;

// 	typedef Segment_<TYPE, DIM> Segment;
// 	typedef Segment_<TYPE, DIM>& ref_Segment;
// 	typedef const Segment_<TYPE, DIM>& const_ref_Segment;

// 	typedef std::shared_ptr<Plotly_actor> spPA;
// 	typedef std::shared_ptr<Plotly_actor_scatter> spPA_scatter;
// 	typedef std::shared_ptr<Plotly_actor_scatter3d> spPA_scatter3d;
// 	typedef std::shared_ptr<Plotly_actor_mesh3d> spPA_mesh3d;

// 	typedef std::list<double> Listd;

// 	template<class ContainerSegment>
// 	static spPA_scatter LinesPoints(
// 			const ContainerSegment& con,
// 			const Segment& dummy) {
// 		Listd lx;
// 		Listd ly;
// 		for (auto& seg : con) {
// 			lx.push_back(seg.psx());
// 			ly.push_back(seg.psy());
// 			lx.push_back(seg.pex());
// 			ly.push_back(seg.pey());
// 		}
// 		spPA_scatter res = spPA_scatter(new Plotly_actor_scatter(lx, ly, 2));
// 		res->set_mode("lines+markers");
// 		return res;
// 	}

// 	template<class ContainerSegment>
// 	static spPA_scatter LinesPoints(
// 			const ContainerSegment& con,
// 			Segment* dummy) {
// 		Listd lx;
// 		Listd ly;
// 		for (auto& seg : con) {
// 			lx.push_back(seg->psx());
// 			ly.push_back(seg->psy());
// 			lx.push_back(seg->pex());
// 			ly.push_back(seg->pey());
// 		}
// 		spPA_scatter res = spPA_scatter(new Plotly_actor_scatter(lx, ly, 2));
// 		res->set_mode("lines+markers");
// 		return res;
// 	}

// //	template<class ContainerSegment>
// //	static spPA_scatter LinesPoints(
// //			const ContainerSegment& con,
// //			Point dummy = typename ContainerSegment::value_type()){
// //		std::cout << "Point";
// //		return nullptr;
// //	}


// };


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
	actor_cone.update("sizeref",   0.25);
    actor_cone.update_colorscale(color, color);
    group[name +"_arrow_cone"] = actor_cone;
    
    return group; 
}

template<St DIM>
PlotlyActorGroup MakePlotlyCoordinateArrow(){}

template<>
PlotlyActorGroup MakePlotlyCoordinateArrow<3>(){
    typedef Point_<double, 3> Point;
    auto gx = MakePlotlyArrow("x", Point(0, 0, 0), Point(1,0,0), "#81BC06");
    auto gy = MakePlotlyArrow("y", Point(0, 0, 0), Point(0,1,0), "#F35325");
    auto gz = MakePlotlyArrow("z", Point(0, 0, 0), Point(0,0,1), "#05A6F0");

    gx.insert(gy.begin(), gy.end());
    gx.insert(gz.begin(), gz.end());
    return gx;
}


template<typename ANY >
auto MakePlotlyActor(const ANY& box, const std::string& type ="auto", BoxTag = BoxTag()){
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


template<typename ANY, ENABLE_IF(ANY, IsGeometry)>
auto ToPlotlyActor(const ANY& geo, const std::string& type ="auto"){
    return MakePlotlyActor(geo, type, ANY::Tag());    
}

template<typename ANY, ENABLE_IF_CONTAINS(ANY, IsGeometry)>
auto ToPlotlyActor(const ANY& geo, const std::string& type ="auto"){

}



}

#endif
