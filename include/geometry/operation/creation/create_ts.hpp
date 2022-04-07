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

#include "io/ply.hpp"

#include <memory>
// #include <numbers>

namespace carpio {


template<typename GEO>
class CreateGeoBase_ {

};


template<typename TYPE, St DIM>
class CreateTS_ : public CreateGeoBase_< TriSurface_<TYPE, DIM> >{
public:
    static const St Dim = DIM;
    typedef TYPE Vt;
    typedef TriSurface_<TYPE, DIM>               Geo;
    typedef std::shared_ptr<Geo>               spGeo;
    typedef TriSurface_<TYPE, 2>                Geo2;
    typedef TriSurface_<TYPE, 3>                Geo3;
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


public:
    spGeo triangle(
            const Point& a,
            const Point& b,
            const Point& c) {
        pVertex pv1 = new Vertex(a);
        pVertex pv2 = new Vertex(b);
        pVertex pv3 = new Vertex(c);

        pEdge pe1 = new Edge(pv1, pv2);
        pEdge pe2 = new Edge(pv2, pv3);
        pEdge pe3 = new Edge(pv3, pv1);

        spGeo sur = std::make_shared<Geo>();
        sur->clear();
        pTriFace pfac = new TriFace(pe1, pe2, pe3, sur.get());
        sur->insert(pfac);
        return sur;
    }

    spGeo circle(uInt n, Vt r) {
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
        for (uInt i = 0; i < n; ++i) {
            pEdge pe = new Edge(v_vertex[0], v_vertex[i + 1]);
            v_edge.push_back(pe);
        }
        for (uInt i = 1; i < n; ++i) {
            pEdge pe = new Edge(v_vertex[i], v_vertex[i + 1]);
            v_edge.push_back(pe);
        }
        pEdge pe = new Edge(v_vertex[n], v_vertex[1]);
        v_edge.push_back(pe);
        //surface
        spGeo sur = std::make_shared<Geo>();
        sur->clear();
        for (uInt i = 0; i < n - 1; i++) {
            pTriFace pfac = new TriFace(v_edge[i], v_edge[i + 1], v_edge[n + i],
                    sur.get());
            sur->insert(pfac);
        }
        pTriFace pfac = new TriFace(v_edge[n - 1], v_edge[0], v_edge[n + n - 1],
                sur.get());
        sur->insert(pfac);
        return sur;
    }

    spGeo cone(St n,                           //the number of triangle
               const Vt& r, const Vt& zbottom, const Vt& zpex) {
        ASSERT(Dim == 3);
        std::vector<pVertex> v_vertex;
        std::vector<pEdge> v_edge;
        std::vector<pTriFace> v_face;
        pVertex pverc = new Vertex(0, 0, zpex);
        double da = 2 * 3.1415926 / n;
        v_vertex.push_back(pverc);
        for (uInt i = 0; i < n; i++) {
            Vt x = r * std::cos(i * da);
            Vt y = r * std::sin(i * da);
            Vt z = zbottom;
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
        spGeo sur = std::make_shared<Geo>();
        for (uInt i = 0; i < n - 1; i++) {
            pTriFace pfac = new TriFace(v_edge[i], v_edge[i + 1], v_edge[n + i], sur.get());
            sur->faces.insert(pfac);
        }
        pTriFace pfac = new TriFace(v_edge[n - 1], v_edge[0], v_edge[n + n - 1],
                        sur.get());
        sur->faces.insert(pfac);
        return sur;
    }

    spGeo from_ply_file(const std::string& filename) {
        std::ifstream ss(filename);
		// Parse the ASCII header fields
		PlyFile file(ss);

        // auto eles = file.get_elements();
        // for (auto& e : eles){
        //     std::cout << e.name << std::endl;
        //     for (auto& p : e.properties){
        //         std::cout << p.name << std::endl;
        //         std::cout << "Property Type : " << int(p.propertyType) << std::endl;
        //     }
        // }
        ASSERT(file.has_element("face"));


		std::vector<float> verts;
		std::vector<uint32_t> faces;
        St vertexCount = file.request_properties_from_element("vertex", { "x","y", "z" }, verts);
		// St faceCount   = file.request_properties_from_element("face",   {"vertex_indices"}, faces);
		St faceCount   = file.request_properties_from_element("face",   {"vertex_indices"}, faces);
		file.read(ss);
        
        // std::cout << "vertex       " << vertexCount << std::endl; 
        // std::cout << "face         " << faceCount << std::endl; 
        // std::cout << "verts.size() " << verts.size() << std::endl; 
        // std::cout << "faces.size() " << faces.size() << std::endl; 

        std::vector<pVertex>  v_vertex;
        std::vector<pEdge>    v_edge;
        std::vector<pTriFace> v_face;
        uInt vertsComponent = verts.size() / vertexCount;
        ASSERT(vertsComponent == 3);
        for (uInt i = 0; i < verts.size(); i += vertsComponent) {
            Vt x = verts[i];
            Vt y = verts[i+1];
            Vt z = verts[i+2];
            pVertex pv = new Vertex(x, y, z);
            v_vertex.push_back(pv);
        }
        // std::cout << "aaa" << std::endl;
        uInt faceComponent = faces.size() / faceCount;
        // ASSERT(faceComponent == 3);
        spGeo sur = std::make_shared<Geo>();
        for (uInt i = 0; i < faces.size(); i += faceComponent) {
            pVertex v1 = v_vertex[faces[i]];
            pVertex v2 = v_vertex[faces[i + 1]];
            pVertex v3 = v_vertex[faces[i + 2]];
            pEdge pe1 = new Edge(v1, v2);
            v_edge.push_back(pe1);
            pEdge pe2 = new Edge(v2, v3);
            v_edge.push_back(pe2);
            pEdge pe3 = new Edge(v3, v1);
            v_edge.push_back(pe3);
            pTriFace pfac = new TriFace(pe1, pe2, pe3, sur.get());
            sur->faces.insert(pfac);
        }
        return sur;
    }



};




}

#endif
