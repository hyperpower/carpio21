#ifndef _S_GNUPLOT_ACTOR_WIRE_FRAME_HPP
#define _S_GNUPLOT_ACTOR_WIRE_FRAME_HPP

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
// ==============================================
//   Top Level 
// ==============================================
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SGridTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _SToGnuplotActorWireFrameDim(a, t, dt); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SGhostTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _SToGnuplotActorWireFrameDim(a, t, dt); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SFieldCenterTag){
    // Todo: not ok for masked grid, need upgrade.
    auto& grid = a.grid();
    return _ToGnuplotActorWireFrame(grid, SGridTag()); 
}

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, 
    const typename ANY::Index& idx, 
    SFieldVertexTag){
    // Todo: not ok for masked grid, need upgrade.
    EXPAND_FIELD_TAG(ANY);
    return _SToGAWireFrameVertexExp(a, idx, FieldTag(), ValueTag(), DimTag());
}

// ==============================================
//   Local Level 
// ==============================================

template<class ANY>
GnuplotActor _SToGAWireFrameVertexExp(
    const ANY& field, const typename ANY::Index& idx,  
    SFieldVertexTag, LinearPolynomialTag, Dim2Tag){
    GnuplotActor actor; 
    actor.command( "using 1:2 title \"\" ");
    actor.style( "with lines lw 2 lc -1");
    auto& grid = field.grid();

    auto pc = grid.v(idx);
    auto& exp  = field(idx);
    for(auto iter = exp.begin(); iter != exp.end(); iter++){
        auto& idxg = iter->first;
        auto& coe  = iter->second;
        auto pg = grid.v(idxg);
        actor.data().push_back(
            ToString(pc.value(_X_), pc.value(_Y_), " ")); 
        actor.data().push_back(
            ToString(pg.value(_X_), pg.value(_Y_), " ")); 
        actor.data().push_back("");
    }
    return actor;
}
template<class ANY>
GnuplotActor _SToGnuplotActorWireFrameDim(const ANY& grid, SGridTag, Dim1Tag){
    GnuplotActor actor; 
    actor.command( "using 1:2 title \"\" ");
    actor.style( "with line lc -1");
    Vt tik = 0.1;

    Vt xs=0.0, xe=0.0;
    for (St i = 0; i < grid.n(_X_); i++){
        typename ANY::Index index(i);
        auto p = grid.f(_X_, _M_, index);
        actor.data().push_back(
            ToString(p.value(_X_), -0.0,  " "));
        actor.data().push_back(
            ToString(p.value(_X_), tik, " "));
        actor.data().push_back("");
        if (i == 0){
            xs = p.value(_X_);
        }
    }
    typename ANY::Index index(grid.n(_X_) - 1);
    auto p = grid.f(_X_, _P_, index);
    actor.data().push_back(
        ToString(p.value(_X_), -0.0, " "));
    actor.data().push_back(
        ToString(p.value(_X_), tik, " "));
    actor.data().push_back("");
    xe = p.value(_X_);

    actor.data().push_back(ToString(xs, 0.0, " "));
    actor.data().push_back(ToString(xe, 0.0, " "));

    // boundary
    actor.data().push_back(ToString(xs, 0.0, " "));
    actor.data().push_back(ToString(xs, 2.0 * tik, " "));
    actor.data().push_back("");

    actor.data().push_back(ToString(xe, 0.0, " "));
    actor.data().push_back(ToString(xe, 2.0 * tik, " "));
    actor.data().push_back("");

    return actor;
}
template<class ANY>
GnuplotActor _SToGnuplotActorWireFrameDim(const ANY& ghost, SGhostTag, Dim1Tag){
    GnuplotActor actor; 
    actor.command( "using 1:2 title \"\" ");
    actor.style( "with line");
    Vt tik = 0.1;

    auto& grid = ghost.grid();
    auto gl    = ghost.ghost_layer();

    Vt xs=0.0, xe=0.0;
    for (int i = -gl; i <= 0; i++){
        typename ANY::Index index(i);
        auto p = grid.f(_X_, _M_, index);
        actor.data().push_back(
            ToString(p.value(_X_), -0.0,  " "));
        actor.data().push_back(
            ToString(p.value(_X_), tik, " "));
        actor.data().push_back("");
        if(i == -gl){
            xs = p.value(_X_);
        }
        if(i == 0){
            xe = p.value(_X_);
        }
    }
    actor.data().push_back(
        ToString(xs, 0.0,  " "));
    actor.data().push_back(
        ToString(xe, 0.0, " "));
    actor.data().push_back("");
    
    for (int i = grid.n(_X_) - 1; i < grid.n(_X_) + gl; i++){
        typename ANY::Index index(i);
        auto p = grid.f(_X_, _P_, index);
        actor.data().push_back(
            ToString(p.value(_X_), -0.0,  " "));
        actor.data().push_back(
            ToString(p.value(_X_), tik, " "));
        actor.data().push_back("");
        if(i == grid.n(_X_) - 1){
            xs = p.value(_X_);
        }
        if(i == grid.n(_X_) + gl -1 ){
            xe = p.value(_X_);
        }
    }
    actor.data().push_back(
        ToString(xs, 0.0,  " "));
    actor.data().push_back(
        ToString(xe, 0.0, " "));
    actor.data().push_back("");
    return actor;
}
template<class ANY>
GnuplotActor _SToGnuplotActorWireFrameDim(const ANY& grid, SGridTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2 title \"\" ");
    actor.style("with line lc 0");

    for (St j = 0; j < grid.n(_Y_); j++) {
        for (St i = 0; i < grid.n(_X_); i++) {
            typename ANY::Index index(i, j);
                
            actor.data().splice(actor.data().end(), 
                        _StringifyCell(grid, index, SGridTag(), Dim2Tag()));

            actor.data().push_back("");
        }
    }
    return actor;
}

}

#endif