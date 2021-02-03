#ifndef _GEOMETRY_PLOTLY_ACTOR_HPP_
#define _GEOMETRY_PLOTLY_ACTOR_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
#include "geometry/objects/objects.hpp"
#include "io/plotly.hpp"
#include <memory>
#include <cmath>

namespace carpio {

using namespace PlotlyActor;

template<typename TYPE, St DIM>
class GPlotlyActor_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;

	typedef Segment_<TYPE, DIM> Segment;
	typedef Segment_<TYPE, DIM>& ref_Segment;
	typedef const Segment_<TYPE, DIM>& const_ref_Segment;

	typedef std::shared_ptr<Plotly_actor> spPA;
	typedef std::shared_ptr<Plotly_actor_scatter> spPA_scatter;
	typedef std::shared_ptr<Plotly_actor_scatter3d> spPA_scatter3d;
	typedef std::shared_ptr<Plotly_actor_mesh3d> spPA_mesh3d;

	typedef std::list<double> Listd;

	template<class ContainerSegment>
	static spPA_scatter LinesPoints(
			const ContainerSegment& con,
			const Segment& dummy) {
		Listd lx;
		Listd ly;
		for (auto& seg : con) {
			lx.push_back(seg.psx());
			ly.push_back(seg.psy());
			lx.push_back(seg.pex());
			ly.push_back(seg.pey());
		}
		spPA_scatter res = spPA_scatter(new Plotly_actor_scatter(lx, ly, 2));
		res->set_mode("lines+markers");
		return res;
	}

	template<class ContainerSegment>
	static spPA_scatter LinesPoints(
			const ContainerSegment& con,
			Segment* dummy) {
		Listd lx;
		Listd ly;
		for (auto& seg : con) {
			lx.push_back(seg->psx());
			ly.push_back(seg->psy());
			lx.push_back(seg->pex());
			ly.push_back(seg->pey());
		}
		spPA_scatter res = spPA_scatter(new Plotly_actor_scatter(lx, ly, 2));
		res->set_mode("lines+markers");
		return res;
	}

//	template<class ContainerSegment>
//	static spPA_scatter LinesPoints(
//			const ContainerSegment& con,
//			Point dummy = typename ContainerSegment::value_type()){
//		std::cout << "Point";
//		return nullptr;
//	}


};
}

#endif
