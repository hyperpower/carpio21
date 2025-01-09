#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include <string>

#include "domain/base/base_gnuplot_actor.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "io/gnuplot.hpp"
#include "s_stringify.hpp"

namespace carpio{

// ToGnuplotActorXXX(ANY)
// _ToGunplotActorXXX(ANY, XXXTag)
// _ToGnuplotActorXXXDim(ANY, XXXTag, DimTag)

// Objects to be plotted:
//  1. Grid
//  2. Ghost
//  3. Order
//  4. Field Center
//  5. Field Face
//  6. Vector Center
//  7. Vector Face

// Type can be plotted 
//  Points, Lines, LinesPoints, WireFrame, Contour, ContourWire, Label，

//         Type  : Dim  : Config
// Points: Grid  : 1 2  : center, facecenter, vertex
//       : Ghost : 1 2  : center, facecenter, vertex
//       : FC    : 1 2  : vari_color
//       : FF    : 1 2  : vari_color
//       : VC    : 1 2  : vari_color

// LinesPoints
// Lines : Grid  x
//       : Ghost x
//       : FC    : 1    : vari_color
//       : FF    : 1    : vari_color
//       : VC    : 1    : vari_color

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrameDim(const ANY& grid, SGridTag, Dim1Tag){
    GnuplotActor actor; 
    actor.command( "using 1:2 title \"\" ");
    actor.style( "with line lc 0");
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

    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrameDim(const ANY& grid, SGridTag, Dim2Tag){
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

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SGridTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _ToGnuplotActorWireFrameDim(a, t, dt); 
}

template<class ANY>
GnuplotActor _ToGnuplotActorLines(const ANY& a, SFieldCenterTag){
    auto& grid = a.grid();
    return _ToGnuplotActorLines(grid, SGridTag()); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourDim(const ANY& f, SFieldCenterTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3:4:5:6:7 title \"\" ");
    actor.style("with boxxy fs solid palette");
    for(auto& index : f.order()){
        auto pc   = f.grid().c(index);
        auto pmin = f.grid().v(0, index);
        auto pmax = f.grid().v(3, index);
        actor.data().push_back(
            ToString(pc(_X_),   pc(_Y_), 
                     pmin(_X_), pmax(_X_),
                     pmin(_Y_), pmax(_Y_),
                     f(index),  " "));
    }
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorContour(const ANY& a, SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourDim(a, Tag(), DimTag()); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourWireDim(const ANY& f, SFieldCenterTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with line lc palette");
    for (St i = 0; i < f.grid().n(_X_); i++){
        for (St j = 0; j < f.grid().n(_Y_); j++){
            typename ANY::Grid::Index index(i, j);
            auto pc = f.grid().c(index);
            if (f.ghost().is_normal(index) == true){
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
GnuplotActor _ToGnuplotActorContourWire(const ANY& a, SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourWireDim(a, Tag(), DimTag()); 
}


template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(const ANY& order, 
                                const std::string& config, 
                                SOrderXYZTag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");
    
    auto &grid  = order.grid();
    typedef typename ANY::Index Index;
    auto fun = [&grid, &actor, &order, &config](const Index &idx)
    {
        auto cp = grid.c(idx);
        auto o  = order.get_order(idx);
        std::string text = "";
        if (config == "index"){
            text =  ToString(idx);
        }else if(config == "order"){
            text =  ToString(o);
        }else{
            text =  ToString(idx);
        }
        actor.data().push_back(
                    ToString(cp(_X_), cp(_Y_)," ") + " \"" + text + "\"");
    };
    grid.for_each(fun);
    return actor;
}

}

#endif