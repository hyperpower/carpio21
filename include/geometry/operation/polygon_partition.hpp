//Copyright (C) 2011 by Ivan Fratric
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#ifndef _POLYGON_PARTITION_HPP_
#define _POLYGON_PARTITION_HPP_

#include "geometry/geometry_define.hpp"

#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/polygon.hpp"

#include <list> 
#include <set>

namespace carpio {


template<class POINT>
class PolygonPartition_ {
public:
    // static const St Dim = DIM;
    // typedef TYPE Vt;
    // typedef Polygon_<TYPE, DIM> Polygon;
    // typedef Contour_<TYPE> Contour;
    // typedef Point_<TYPE, Dim> Point;
    typedef POINT Point;
    typedef const Point* pcPoint;
    typedef Point::value_type Vt;
    static const St Dim = Point::Dim;
    typedef std::array<pcPoint, 3> Tri;
    typedef std::list<Tri> ListTri;
    typedef PointChain_<Vt, Dim> PointChain;
    // typedef Segment_<TYPE, DIM> Segment;
    // typedef Operation_<TYPE, Dim> Op;
    // typedef Intersection_<TYPE, Dim> Isc;
    // typedef Connector_<TYPE, Dim> Connector;
public:

    struct PartitionVertex {
        bool is_active;
        bool is_convex;
        bool is_ear;

        const Point*     p;
        double           angle;
        PartitionVertex *previous;
        PartitionVertex *next;

        PartitionVertex() :
                previous(nullptr), next(nullptr) {
        }
    };


    //standard helper functions
    // ------------------|
    bool is_convex(const Point& p1, const Point& p2, const Point& p3) { // this function should be change to CCW
        return Orient(p2, p3, p1) > 0;
    }
    bool is_convex(PartitionVertex *v) { // this function should be change to CCW
        return Orient(*(v->p), *(v->next->p), *(v->previous->p)) > 0;
    }

    bool in_cone(const Point &p1, const Point &p2, const Point &p3, const Point &p) {
        bool convex = false;

        convex = is_convex(p1, p2, p);

        if (convex) {
            if (!is_convex(p2, p3, p))
                return false;
            if (!is_convex(p3, p1, p))
                return false;
            return true;
        } 
        // else {
        //     if (IsConvex(p1, p, p2))
        //         return true;
        //     if (IsConvex(p2, p, p3))
        //         return true;
        //     return false;
        // }
        return convex;
    }
    bool in_cone(PartitionVertex *v, const Point &p) {
        auto p1 = v->previous->p;
        auto p2 = v->p;
        auto p3 = v->next->p;
        return in_cone(*p1, *p2, *p3, p);
    }

    // //helper functions for Triangulate_EC
    // void UpdateVertexReflexity(PartitionVertex *v) {
    //     PartitionVertex *v1 = nullptr, *v3 = nullptr;
    //     v1 = v->previous;
    //     v3 = v->next;
    //     v->is_convex = !IsReflex(v1->p, v->p, v3->p);
    // }

    // helper function
    // copy vertices into PartitionVertex
    template<class CONTAINER>
    PartitionVertex* new_partition_vertex(const CONTAINER& pc) {
        ASSERT(pc.size() >= 3);

        long numvertices = pc.size();
//		std::cout << "d nvertices = " << numvertices <<std::endl;
        PartitionVertex* vertices = new PartitionVertex[numvertices];
        long i = 0;
        for (auto& point : pc) {

            vertices[i].is_active = true;
            vertices[i].p         = &point;
            if (i == (numvertices - 1)) { // last one
                vertices[i].next = &(vertices[0]);
            } else {
                vertices[i].next = &(vertices[i + 1]);
            }
            if (i == 0) {                 //first one
                vertices[i].previous = &(vertices[numvertices - 1]);
            } else {
                vertices[i].previous = &(vertices[i - 1]);
            }
//			std::cout << "i = " << i << "  " << vertices[i].p << std::endl;
            i++;
        }
        return vertices;
    }

    auto _minus(const Point& a, const Point& b){
        std::array<typename Point::value_type, 2> res;
        auto aiter = a.begin();
        auto biter = b.begin();
        auto resiter = res.begin();
        for(;resiter!=res.end();){
            (*resiter) = *aiter - *biter;            

            ++aiter;
            ++biter;
            ++resiter;
        }
        return res;
    }    

    double _angle(PartitionVertex* v){
        auto v_prev = v->previous;
        auto v_next = v->next;
        auto vec1 = _minus(*(v_prev->p), *(v->p));
        auto vec3 = _minus(*(v_next->p), *(v->p));
        Normalize(vec1);
        Normalize(vec3);
        return vec1[0] * vec3[0] + vec1[1] * vec3[1];
    }

    

    void update_vertex(
            PartitionVertex *v,
            PartitionVertex *vertices,
            long numvertices) {


        v->is_convex = is_convex(v);
        v->angle = _angle(v);
        // std::cout << "v " << v->p << std::endl;
        // std::cout << "is convex " << v->is_convex << std::endl;

        auto v_prev = v->previous;
        auto v_next = v->next;
        if (v->is_convex) {
            v->is_ear = true;
            for (int i = 0; i < numvertices; i++) {
                if (vertices[i].p == v->p)
                    continue;
                if (vertices[i].p == v_prev->p)
                    continue;
                if (vertices[i].p == v_next->p)
                    continue;

                if (in_cone(v, *(vertices[i].p))) {
                    v->is_ear = false;
                    break;
                }
            }
        } else {
            v->is_ear = false;
        }
    }
public:

    //simple heuristic procedure for removing holes from a list of polygons
    //works by creating a diagonal from the rightmost hole vertex to some visible vertex
    //time complexity: O(h*(n^2)), h is the number of holes, n is the number of vertices
    //space complexity: O(n)
    //params:
    //   inpolys : a list of polygons that can contain holes
    //             vertices of all non-hole polys have to be in counter-clockwise order
    //             vertices of all hole polys have to be in clockwise order
    //   outpolys : a list of polygons without holes
    //returns 1 on success, 0 on failure
    // static int RemoveHoles(
    //         const PointChain& inpolys,
    //         std::list<PointChain>& outpolys) {
    //     std::list<PointChain> polys;
    //     typename std::list<Contour>::const_iterator holeiter, polyiter, iter,
    //             iter2;
    //     long i, i2, holepointindex, polypointindex;
    //     Point holepoint, polypoint, bestpolypoint;
    //     Point linep1, linep2;
    //     Point v1, v2;
    //     Contour newpoly;
    //     bool hasholes;
    //     bool pointvisible;
    //     bool pointfound;

    //     //check for trivial case (no holes)
    //     hasholes = false;
    //     for (auto& contour : inpolys) {
    //         if (contour.is_hole()) {
    //             hasholes = true;
    //             break;
    //         }
    //     }
    //     if (!hasholes) {
    //         for (auto& contour : inpolys) {
    //             outpolys.push_back(contour);
    //         }
    //         return 1;
    //     }
    //     /// copy inplys to ploy
    //     std::copy(inpolys.begin(), inpolys.end(), std::back_inserter(polys));

    //     while (1) {
    //         /// find the hole point with the largest x
    //         hasholes = false;
    //         for (iter = polys.begin(); iter != polys.end(); iter++) {
    //             if (!iter->is_hole())
    //                 continue;

    //             if (!hasholes) {
    //                 hasholes = true;
    //                 holeiter = iter;
    //                 holepointindex = 0;
    //             }

    //             for (i = 0; i < iter->nvertices(); i++) {
    //                 if (iter->v(i).x()
    //                         > holeiter->v(holepointindex).x()) {
    //                     holeiter = iter;
    //                     holepointindex = i;
    //                 }
    //             }
    //         }

    //         if (!hasholes) {
    //             /// no holes break while
    //             break;
    //         }

    //         holepoint = holeiter->v(holepointindex);
    //         pointfound = false;
    //         for (iter = polys.begin(); iter != polys.end(); iter++) {
    //             if (iter->is_hole()) {
    //                 continue;
    //             }
    //             /// for each vertices of NON hole poly
    //             St nv = iter->nvertices(); /// number of vertices of NON hole poly
    //             for (i = 0; i < nv; i++) {
    //                 if (iter->v(i).x() <= holepoint.x()) {
    //                     continue;
    //                 }
    //                 if (!in_cone(
    //                         iter->v((i + nv - 1) % nv),
    //                         iter->v(i),
    //                         iter->v((i + 1) % nv),
    //                         holepoint)) {
    //                     continue;
    //                 }

    //                 polypoint = iter->v(i);
    //                 if (pointfound) {
    //                     v1 = Op::Normalize(polypoint - holepoint);
    //                     v2 = Op::Normalize(bestpolypoint - holepoint);
    //                     if (v2.x() > v1.x())
    //                         continue;
    //                 }
    //                 pointvisible = true;
    //                 for (iter2 = polys.begin(); iter2 != polys.end(); iter2++) {
    //                     if (iter2->is_hole()){
    //                         continue;
    //                     }
    //                     St nv2 = iter2->nvertices();
    //                     for (i2 = 0; i2 < nv2; i2++) {
    //                         linep1 = iter2->v(i2);
    //                         linep2 = iter2->v((i2 + 1) % nv2);
    //                         if (IsIntersect(
    //                                 holepoint,
    //                                 polypoint,
    //                                 linep1,
    //                                 linep2)) {
    //                             pointvisible = false;
    //                             break;
    //                         }
    //                     }
    //                     if (!pointvisible)
    //                         break;
    //                 }
    //                 if (pointvisible) {
    //                     pointfound = true;
    //                     bestpolypoint = polypoint;
    //                     polyiter = iter;
    //                     polypointindex = i;
    //                 }
    //             }
    //         }

    //         if (!pointfound)
    //             return 0;

    //         newpoly.resize_vertices(
    //                 holeiter->nvertices() + polyiter->nvertices() + 2);
    //         i2 = 0;
    //         for (i = 0; i <= polypointindex; i++) {
    //             newpoly.v(i2) = polyiter->v(i);
    //             i2++;
    //         }
    //         for (i = 0; i <= holeiter->nvertices(); i++) {
    //             newpoly.v(i2) = holeiter->v(
    //                     (i + holepointindex) % holeiter->nvertices());
    //             i2++;
    //         }
    //         for (i = polypointindex; i < polyiter->nvertices(); i++) {
    //             newpoly.v(i2) = polyiter->v(i);
    //             i2++;
    //         }

    //         polys.erase(holeiter);
    //         polys.erase(polyiter);
    //         polys.push_back(newpoly);
    //     }

    //     for (iter = polys.begin(); iter != polys.end(); iter++) {
    //         outpolys.push_back(*iter);
    //     }

    //     return 1;
    // }
    

    //triangulates a polygon by ear clipping
    //time complexity O(n^2), n is the number of vertices
    //space complexity: O(n)
    //params:
    //   poly : an input polygon to be triangulated
    //          vertices have to be in counter-clockwise order
    //   triangles : a list of triangles (result)
    //returns 1 on success, 0 on failure
    template<class CONTAINER>
    int ear_clipping(const CONTAINER& poly,
                     ListTri& triangles) {
        long numvertices;
        PartitionVertex *vertices = nullptr;
        PartitionVertex *ear = nullptr;
        PointChain triangle;
        // long i, j;
        bool earfound;

        /// if the vertices is less than 3
        /// return
        if (poly.size() < 3)
            return 0;
        if (poly.size() == 3) {
            St idx = 0;
            Tri tri;
            for(auto& p : poly){
                tri[idx] = &p;
                ++idx;
            }
            triangles.push_back(tri);
            return 1;
        }

        /// copy vertices into PartitionVertex
        numvertices = poly.size();
        vertices = new_partition_vertex(poly);
        // update vertices
        for (long i = 0; i < numvertices; i++) {
            update_vertex(&(vertices[i]), vertices, numvertices);
        }

        for (long i = 0; i < numvertices - 3; i++) {
            earfound = false;
            /// find the most extruded ear
            for (long j = 0; j < numvertices; j++) {
                if (!vertices[j].is_active)
                    continue;
                if (!vertices[j].is_ear)
                    continue;
                if (!earfound) {
                    earfound = true;
                    ear = &(vertices[j]);
                } else {
                    if (vertices[j].angle > ear->angle) {
                        ear = &(vertices[j]);
                    }
                }
            }
            if (!earfound) {
                delete[] vertices;
                return 0;
            }

            triangles.push_back(Tri{
                    (ear->previous->p),
                    (ear->p),
                    (ear->next->p)});

            ear->is_active = false;
            ear->previous->next = ear->next;
            ear->next->previous = ear->previous;

            if (i == numvertices - 4)
                break;

            update_vertex(ear->previous, vertices, numvertices);
            // std::cout << "prev is ear" << ear->previous->is_ear << std::endl;
            update_vertex(ear->next,     vertices, numvertices);
            // std::cout << "next is ear" << ear->next->is_ear << std::endl;
        }
        for (long i = 0; i < numvertices; i++) {
            if (vertices[i].is_active) {
                triangles.push_back(Tri{
                        (vertices[i].previous->p),
                        (vertices[i].p),
                        (vertices[i].next->p)}
                        );
                break;
            }
        }

        delete[] vertices;

        return 1;
    }

    //triangulates a list of polygons that may contain holes by ear clipping algorithm
    //first calls RemoveHoles to get rid of the holes, and then Triangulate_EC for each resulting polygon
    //time complexity: O(h*(n^2)), h is the number of holes, n is the number of vertices
    //space complexity: O(n)
    //params:
    //   inpolys : a list of polygons to be triangulated (can contain holes)
    //             vertices of all non-hole polys have to be in counter-clockwise order
    //             vertices of all hole polys have to be in clockwise order
    //   triangles : a list of triangles (result)
    //returns 1 on success, 0 on failure
    // int EarClippingHoles(const PointChain& inpolys,
    //         std::list<PointChain>& triangles) {

    //     std::list<PointChain> outpolys;
    //     typename std::list<PointChain>::iterator iter;

    //     if (!RemoveHoles(inpolys, &outpolys))
    //         return 0;
    //     for (iter = outpolys.begin(); iter != outpolys.end(); iter++) {
    //         if (!EarClipping(&(*iter), triangles))
    //             return 0;
    //     }
    //     return 1;
    // }

    //creates an optimal polygon triangulation in terms of minimal edge length
    //time complexity: O(n^3), n is the number of vertices
    //space complexity: O(n^2)
    //params:
    //   poly : an input polygon to be triangulated
    //          vertices have to be in counter-clockwise order
    //   triangles : a list of triangles (result)
    //returns 1 on success, 0 on failure
    // int Triangulate_OPT(Contour *poly, std::list<Contour> *triangles);

    //triangulates a polygons by firstly partitioning it into monotone polygons
    //time complexity: O(n*log(n)), n is the number of vertices
    //space complexity: O(n)
    //params:
    //   poly : an input polygon to be triangulated
    //          vertices have to be in counter-clockwise order
    //   triangles : a list of triangles (result)
    //returns 1 on success, 0 on failure
    // int Triangulate_MONO(Contour *poly, std::list<Contour> *triangles);

    //triangulates a list of polygons by firstly partitioning them into monotone polygons
    //time complexity: O(n*log(n)), n is the number of vertices
    //space complexity: O(n)
    //params:
    //   inpolys : a list of polygons to be triangulated (can contain holes)
    //             vertices of all non-hole polys have to be in counter-clockwise order
    //             vertices of all hole polys have to be in clockwise order
    //   triangles : a list of triangles (result)
    //returns 1 on success, 0 on failure
    // int Triangulate_MONO(std::list<Contour> *inpolys,
            // std::list<Contour> *triangles);

    //creates a monotone partition of a list of polygons that can contain holes
    //time complexity: O(n*log(n)), n is the number of vertices
    //space complexity: O(n)
    //params:
    //   inpolys : a list of polygons to be triangulated (can contain holes)
    //             vertices of all non-hole polys have to be in counter-clockwise order
    //             vertices of all hole polys have to be in clockwise order
    //   monotonePolys : a list of monotone polygons (result)
    //returns 1 on success, 0 on failure
    // int MonotonePartition(std::list<Contour> *inpolys,
            // std::list<Contour> *monotonePolys);

    //partitions a polygon into convex polygons by using Hertel-Mehlhorn algorithm
    //the algorithm gives at most four times the number of parts as the optimal algorithm
    //however, in practice it works much better than that and often gives optimal partition
    //uses triangulation obtained by ear clipping as intermediate result
    //time complexity O(n^2), n is the number of vertices
    //space complexity: O(n)
    //params:
    //   poly : an input polygon to be partitioned
    //          vertices have to be in counter-clockwise order
    //   parts : resulting list of convex polygons
    //returns 1 on success, 0 on failure
    // int ConvexPartition_HM(Contour *poly, std::list<Contour> *parts);

    //partitions a list of polygons into convex parts by using Hertel-Mehlhorn algorithm
    //the algorithm gives at most four times the number of parts as the optimal algorithm
    //however, in practice it works much better than that and often gives optimal partition
    //uses triangulation obtained by ear clipping as intermediate result
    //time complexity O(n^2), n is the number of vertices
    //space complexity: O(n)
    //params:
    //   inpolys : an input list of polygons to be partitioned
    //             vertices of all non-hole polys have to be in counter-clockwise order
    //             vertices of all hole polys have to be in clockwise order
    //   parts : resulting list of convex polygons
    //returns 1 on success, 0 on failure
    // int ConvexPartition_HM(std::list<Contour> *inpolys,
            // std::list<Contour> *parts);

    //optimal convex partitioning (in terms of number of resulting convex polygons)
    //using the Keil-Snoeyink algorithm
    //M. Keil, J. Snoeyink, "On the time bound for convex decomposition of simple polygons", 1998
    //time complexity O(n^3), n is the number of vertices
    //space complexity: O(n^3)
    //   poly : an input polygon to be partitioned
    //          vertices have to be in counter-clockwise order
    //   parts : resulting list of convex polygons
    //returns 1 on success, 0 on failure
    // int ConvexPartition_OPT(Contour *poly, std::list<Contour> *parts);
}
;

}

#endif