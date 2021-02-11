#ifndef _CREATE_TS_HPP_
#define _CREATE_TS_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/ts/trisurface.hpp"
#include "geometry/objects/ts/triface.hpp"
#include "geometry/objects/ts/vertex.hpp"
#include "geometry/objects/ts/edge.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {


template<typename TYPE, St DIM>
class InserterTS_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM>                  Point;
	typedef Point_<TYPE, DIM>&             ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;

	typedef Segment_<TYPE, DIM>      Segment;
	typedef Segment&             ref_Segment;
	typedef const Segment& const_ref_Segment;

	typedef TriSurface_<TYPE, DIM>          TriSurface;
	typedef TriFace_<TYPE, DIM, TriSurface> TriFace;
	typedef Edge_<TYPE, DIM, TriFace>       Edge;
	typedef Vertex_<TYPE, DIM, Edge>        Vertex;
	typedef Vertex*                        pVertex;
	typedef Edge*                          pEdge;
	typedef TriFace*                       pTriFace;

	typedef TriSurface_<TYPE, 2> TriSurface2;
	typedef TriSurface_<TYPE, 3> TriSurface3;

	pTriFace new_tri_face(){

	}
};

template<typename TYPE, St DIM>
class CreateTS_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM>                  Point;
	typedef Point_<TYPE, DIM>&             ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;

	typedef Segment_<TYPE, DIM>      Segment;
	typedef Segment&             ref_Segment;
	typedef const Segment& const_ref_Segment;

	typedef TriSurface_<TYPE, DIM>          TriSurface;
	typedef TriFace_<TYPE, DIM, TriSurface> TriFace;
	typedef Edge_<TYPE, DIM, TriFace>       Edge;
	typedef Vertex_<TYPE, DIM, Edge>        Vertex;
	typedef Vertex*                        pVertex;
	typedef Edge*                          pEdge;
	typedef TriFace*                       pTriFace;

	typedef TriSurface_<TYPE, 2> TriSurface2;
	typedef TriSurface_<TYPE, 3> TriSurface3;

public:
	static void TriFaceOne(
			TriSurface& sur,
			const Point& a,
			const Point& b,
			const Point& c) {
		pVertex pv1 = new Vertex(a);
		pVertex pv2 = new Vertex(b);
		pVertex pv3 = new Vertex(c);

		pEdge pe1 = new Edge(pv1, pv2);
		pEdge pe2 = new Edge(pv2, pv3);
		pEdge pe3 = new Edge(pv3, pv1);

		sur.clear();
		pTriFace pfac = new TriFace(pe1, pe2, pe3, &sur);
		sur.insert(pfac);
	}

	static void Circle(TriSurface& sur, uInt n, Vt r) {
		std::vector<pVertex> v_vertex;
		std::vector<pEdge> v_edge;
		std::vector<pTriFace> v_face;
		pVertex pverc = new Vertex(0, 0, 0);
		double da = 2 * _PI_ / n;
		v_vertex.push_back(pverc);
		for (uInt i = 0; i < n; i++) {
			Vt x = r * cos(i * da);
			Vt y = r * sin(i * da);
			Vt z = 0;
			pVertex pv = new Vertex(x, y, z);
			v_vertex.push_back(pv);
		}
		//edge
		for (uInt i = 0; i < n; i++) {
			pEdge pe = new Edge(v_vertex[0], v_vertex[i + 1]);
			v_edge.push_back(pe);
		}
		for (uInt i = 1; i < n; i++) {
			pEdge pe = new Edge(v_vertex[i], v_vertex[i + 1]);
			v_edge.push_back(pe);
		}
		pEdge pe = new Edge(v_vertex[n], v_vertex[1]);
		v_edge.push_back(pe);
		//surface
		sur.clear();
		for (uInt i = 0; i < n - 1; i++) {
			pTriFace pfac = new TriFace(v_edge[i], v_edge[i + 1], v_edge[n + i],
					&sur);
			sur.insert(pfac);
		}
		pTriFace pfac = new TriFace(v_edge[n - 1], v_edge[0], v_edge[n + n - 1],
				&sur);
		sur.insert(pfac);
	}



};




}

#endif
