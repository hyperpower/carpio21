#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include "domain/base/base_gnuplot_actor.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "io/gnuplot.hpp"
#include "s_stringify.hpp"

namespace carpio{

// ToGunplotActor
//  - ToGnuplotActorDim
//   - ToGnuplotActor

template<class ANY>
GnuplotActor _ToGnuplotActorLinesDim(const ANY& grid, SGridTag, Dim1Tag){
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
GnuplotActor _ToGnuplotActorLinesDim(const ANY& grid, SGridTag, Dim2Tag){
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
GnuplotActor _ToGnuplotActorLines(const ANY& a, SGridTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _ToGnuplotActorLinesDim(a, t, dt); 
}

template<class ANY>
GnuplotActor _ToGnuplotActorLines(const ANY& a, SFieldCenterTag){
    auto& grid = a.grid();
    return _ToGnuplotActorLines(grid, SGridTag()); 
}

}

#endif