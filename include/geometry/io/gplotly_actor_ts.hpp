#ifndef _GEOMETRY_PLOTLY_ACTOR_TS_HPP_
#define _GEOMETRY_PLOTLY_ACTOR_TS_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
// #include "geometry/objects/objects.hpp"
#include "geometry/objects/ts/trisurface.hpp"
#include "geometry/boolean/intersection_ts.hpp"
// #include "io/python_interpreter.hpp"
// #include "io/plotly.hpp"
#include <memory>
#include <cmath>

namespace carpio {

// template<typename TYPE, St DIM>
// class GPlotlyActor_TS_ {
// public:
// 	static const St Dim = DIM;
// 	typedef TYPE Vt;
// 	typedef Point_<TYPE, DIM> Point;
// 	typedef Point_<TYPE, DIM>& ref_Point;
// 	typedef const Point_<TYPE, DIM>& const_ref_Point;

// 	typedef TriSurface_<TYPE, DIM> TriSurface;
// 	typedef typename TriSurface::pVer pVertex;

// 	typedef TriSurface_<TYPE, 3> TriSurface3;
// 	typedef TriSurface_<TYPE, 2> TriSurface2;

// 	typedef TriFace_<TYPE, 3, TriSurface3> TriFace3;
// 	typedef TriFace_<TYPE, 2, TriSurface2> TriFace2;

// 	typedef std::shared_ptr<Plotly_actor> spPA;
// 	typedef std::shared_ptr<Plotly_actor_scatter> spPA_scatter;
// 	typedef std::shared_ptr<Plotly_actor_scatter3d> spPA_scatter3d;
// 	typedef std::shared_ptr<Plotly_actor_mesh3d> spPA_mesh3d;

// 	typedef std::list<double> Listd;

// public:
// 	static spPA_mesh3d Surface(const TriSurface3& sur) {
// 		Listd lx, li;
// 		Listd ly, lj;
// 		Listd lz, lk;
// 		int count = 0;
// 		for (auto iter = sur.faces.begin(); iter != sur.faces.end(); ++iter) {
// 			typename TriSurface3::pFac pf = (*iter);
// 			for (typename TriSurface3::size_type i = 0; i < 3; i++) {
// 				lx.push_back(pf->vertex(i)->x());
// 				ly.push_back(pf->vertex(i)->y());
// 				lz.push_back(pf->vertex(i)->z());
// 			}
// 			li.push_back(count);
// 			lj.push_back(count + 1);
// 			lk.push_back(count + 2);
// 			count += 3;
// 		}

// 		spPA_mesh3d res = spPA_mesh3d(new Plotly_actor_mesh3d(lx, ly, lz));
// 		res->set_ijk(li, lj, lk);
// 		return res;
// 	}




// 	static spPA_mesh3d Face(const TriFace3& r) {
// 		std::vector<Vt> ax(3);
// 		std::vector<Vt> ay(3);
// 		std::vector<Vt> az(3);
// 		for (typename TriFace3::size_type i = 0; i < 3; i++) {
// 			ax[i] = r.vertex(i, _X_);
// 			ay[i] = r.vertex(i, _Y_);
// 			az[i] = r.vertex(i, _Z_);
// 		}
// 		spPA_mesh3d res = spPA_mesh3d(new Plotly_actor_mesh3d(ax, ay, az));
// 		return res;
// 	}

// 	static spPA_scatter3d WireFrame(const TriFace3& tri) {
// 		Listd lx;
// 		Listd ly;
// 		Listd lz;
// 		//for (auto iter = sur.faces.begin(); iter != sur.faces.end(); ++iter) {
// 		//	typename Surd3::spFac pf = (*iter);
// 		for (typename TriFace3::size_type i = 0; i < 3; i++) {
// 			lx.push_back(tri.vertex(i)->x());
// 			ly.push_back(tri.vertex(i)->y());
// 			lz.push_back(tri.vertex(i)->z());
// 		}
// 		lx.push_back(tri.vertex(0)->x());
// 		ly.push_back(tri.vertex(0)->y());
// 		lz.push_back(tri.vertex(0)->z());
// 		//}
// 		spPA_scatter3d res = spPA_scatter3d(
// 				new Plotly_actor_scatter3d(lx, ly, lz, 4));
// 		res->set_mode("lines");
// 		return res;
// 	}

// 	static spPA_scatter3d TriangleNormal(const TriFace3& face) {
// 		Listd lx;
// 		Listd ly;
// 		Listd lz;
// 		int count = 0;
// 		Point pc = face.centroid();
// 		Point pn = face.normal();
// 		lx.push_back(pc[0]);
// 		ly.push_back(pc[1]);
// 		lz.push_back(pc[2]);
// 		lx.push_back(pn[0] + pc[0]);
// 		ly.push_back(pn[1] + pc[1]);
// 		lz.push_back(pn[2] + pc[2]);

// 		count += 1;

// 		spPA_scatter3d res = spPA_scatter3d(
// 				new Plotly_actor_scatter3d(lx, ly, lz, 2));
// 		res->set_mode("lines");
// 		return res;
// 	}

// 	static spPA_scatter3d TriangleNormal(const TriSurface3& surface,
// 			double scale = 1.0) {
// 		Listd lx;
// 		Listd ly;
// 		Listd lz;
// 		int count = 0;
// 		for (auto& pf : surface) {
// 			Point pc = pf->centroid();
// 			Point pn = pf->normal();
// 			lx.push_back(pc[0]);
// 			ly.push_back(pc[1]);
// 			lz.push_back(pc[2]);
// 			lx.push_back(pc[0] + pn[0] * scale);
// 			ly.push_back(pc[1] + pn[1] * scale);
// 			lz.push_back(pc[2] + pn[2] * scale);

// 			count += 1;
// 		}

// 		spPA_scatter3d res = spPA_scatter3d(
// 				new Plotly_actor_scatter3d(lx, ly, lz, 2));
// 		res->set_mode("lines");
// 		return res;
// 	}

// };



inline std::string _ChooseType(const std::string& type){
    auto def_type = (type == "auto") ? "wireframe" : type;

    def_type = (def_type == "wireframe") ? "Scatter3d" : def_type;
    def_type = (def_type == "vertex") ? "Scatter3d" : def_type;
    def_type = (def_type == "surface") ? "Mesh3d" : def_type;
    return def_type;
}
template<typename ANY >
auto MakePlotlyActor(TriFaceTag, const ANY& face, const std::string& type){
    auto ct = _ChooseType(type);
    PlotlyActor actor(ct);
    if(ct == "Scatter3d"){
        
        actor.update("name", "tri_wireframe");
        actor.update("mode", "lines");

        std::list<typename ANY::Point> list;

        auto pf = &face;
        for (typename ANY::size_type i = 0; i < 3; i++) {
            list.push_back(*(pf->vertex(i)));
        }
        list.push_back(*(pf->vertex(0)));
        actor.data_xyz(list,4);
    }

    if(ct == "Mesh3d"){
        actor.update("name", "tri_surface");
        // actor.update("mode", "lines");

        std::list<typename ANY::Point> list;
        std::list<std::array<double, 3> > ijk;

        double count = 0;
        auto pf = (&face);
        for (typename ANY::size_type i = 0; i < 3; i++) {
            list.push_back(*(pf->vertex(i)));
        }
        ijk.emplace_back(std::array<double,3>{count, count + 1, count + 2});
        count += 3;
        actor.data_xyz(list);
        actor.data(ijk, "i", "j", "k");
    }
    return actor;
}

template<typename ANY >
auto MakePlotlyActor(TriSurfaceTag, const ANY& sur, const std::string& type){
    auto ct = _ChooseType(type);
    PlotlyActor actor(ct);
    if(ct == "Scatter3d"){
        
        actor.update("name", "tri_wireframe");
        actor.update("mode", "lines");

        std::list<typename ANY::Point> list;

        for (auto iter = sur.faces.begin(); iter != sur.faces.end(); ++iter) {
            auto pf = (*iter);
            for (typename ANY::size_type i = 0; i < 3; i++) {
                    list.push_back(*(pf->vertex(i)));
            }
            list.push_back(*(pf->vertex(0)));
        }
        actor.data_xyz(list,4);
    }

    if(ct == "Mesh3d"){
        actor.update("name", "tri_surface");
        // actor.update("mode", "lines");

        std::list<typename ANY::Point> list;
        std::list<std::array<double, 3> > ijk;

        double count = 0;
        for (auto iter = sur.faces.begin(); iter != sur.faces.end(); ++iter) {
            auto pf = (*iter);
            for (typename ANY::size_type i = 0; i < 3; i++) {
                list.push_back(*(pf->vertex(i)));
            }
            ijk.emplace_back(std::array<double,3>{count, count + 1, count + 2});
            count += 3;
        }
        actor.data_xyz(list);
        actor.data(ijk, "i", "j", "k");
    }
    return actor;
}

}

#endif
