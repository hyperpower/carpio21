#ifndef _S_GNUPLOT_ACTOR_COUTOUR_WIRE_HPP
#define _S_GNUPLOT_ACTOR_COUTOUR_WIRE_HPP

#include <string>

#include "domain/base/base_gnuplot_actor.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/sghost.hpp"
#include "domain/structure/order/sorder.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/field/sfield_vertex.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "io/gnuplot.hpp"
#include "s_stringify.hpp"

namespace carpio{
    
template<class ANY>
GnuplotActor _ToGnuplotActorContourWire(const ANY& a, SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourWireDim(a, Tag(), DimTag()); 
}

template<class ANY>
GnuplotActor _ToGnuplotActorContourWireDim(const ANY& f, 
    SFieldCenterTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with line lc palette");
    for (St i = 0; i < f.grid().n(_X_); i++){
        for (St j = 0; j < f.grid().n(_Y_); j++){
            typename ANY::Grid::Index index(i, j);
            auto pc = f.grid().c(index);
            if (f.ghost().is_normal(index)){
                actor.data().push_back(
                    ToString(pc(_X_), pc(_Y_),
                             f(index), f(index), " "));
            }
        }
        actor.data().push_back("");
    }
    return actor;
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourWireDim(
        const ANY& f, SFieldCenterTag, Dim1Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with lines lc palette");
    for (St i = 0; i < f.grid().n(_X_); i++){
        typename ANY::Grid::Index index(i);
        auto pc = f.grid().c(index);
        if (f.ghost().is_normal(index) == true){
            actor.data().push_back(
                ToString(pc(_X_), f(index), f(index), " "));
        }
    }
    return actor;
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourWireDim(const ANY& f, 
    SFieldVertexTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with line lc palette");
    for (St i = 0; i < f.grid().n(_X_) - 1; i++){
        for (St j = 0; j < f.grid().n(_Y_) - 1; j++){
            typename ANY::Grid::Index index(i, j);
            auto pc = f.grid().v(index);
            if (f.ghost().is_normal_vertex(index) ){
                actor.data().push_back(
                    ToString(pc(_X_), pc(_Y_),
                             f(index), f(index), " "));
            }
        }
        actor.data().push_back("");
    }
    actor.data().push_back("");
    for (St j = 0; j < f.grid().n(_Y_)-1; j++){
        for (St i = 0; i < f.grid().n(_X_)-1; i++){
            typename ANY::Grid::Index index(i, j);
            auto pc = f.grid().v(index);
            if (f.ghost().is_normal_vertex(index) ){
                actor.data().push_back(
                    ToString(pc(_X_), pc(_Y_),
                             f(index), f(index), " "));
            }
        }
        actor.data().push_back("");
    }
    return actor;
}
template<class ANY, class BI>
GnuplotActor _ToGnuplotActorContourWireDim(const ANY& f, const BI& bi, 
    SFieldVertexTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with line lc palette");
    
    auto& ghost = f.ghost();
    auto& grid  = f.grid();
    typedef typename ANY::Grid::Index Index;
    Index prev(0, 0);
    for (int i = 0; i < grid.n(_X_) - 1; i++){
        for (int j = -1; j < grid.n(_Y_); j++){
            Index index(i, j);
            auto pv = grid.v(index);
            if (ghost.is_normal_vertex(index) ){
                actor.data().push_back(
                    ToString(pv(_X_), pv(_Y_),
                             f(index), f(index), " "));
            }
            if (ghost.is_ghost_vertex(index)){
                auto v = FindBoundaryValue(f, bi, prev, index, 0.0);
                actor.data().push_back(
                    ToString(pv(_X_), pv(_Y_),
                             v, v, " "));    
            }
            prev = index;
            
        }
        actor.data().push_back("");
    }
    actor.data().push_back("");
    for (int j = 0; j < grid.n(_Y_) - 1; j++){
        for (int i = -1; i < grid.n(_X_); i++){
            Index index(i, j);
            auto pv = grid.v(index);
            if (ghost.is_normal_vertex(index) ){
                actor.data().push_back(
                    ToString(pv(_X_), pv(_Y_),
                             f(index), f(index), " "));
            }
            if (ghost.is_ghost_vertex(index)){
                auto v = FindBoundaryValue(f, bi, prev, index, 0.0);
                actor.data().push_back(
                    ToString(pv(_X_), pv(_Y_), v, v, " "));    
            }
            prev = index;
        }
        actor.data().push_back("");
    } 
    return actor;
}


template<class ANY>
GnuplotActor _ToGnuplotActorContourWire(const ANY& a, SFieldVertexTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourWireDim(a, Tag(), DimTag()); 
}
template<class ANY, class BI>
GnuplotActor _ToGnuplotActorContourWire(const ANY& a, const BI& bi, SFieldVertexTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourWireDim(a, bi, Tag(), DimTag()); 
}

}

#endif