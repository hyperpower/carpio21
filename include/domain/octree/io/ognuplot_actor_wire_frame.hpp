#ifndef _O_GNUPLOT_ACTOR_WIRE_FRAME_HPP
#define _O_GNUPLOT_ACTOR_WIRE_FRAME_HPP

#include <string>

#include "domain/base/base_gnuplot_actor.hpp"
#include "domain/octree/grid/ogrid.hpp"
#include "domain/octree/io/ostringify.hpp"
#include "io/gnuplot.hpp"

namespace carpio {

template<class NODE>
void _OAppendNodeWireFrame(GnuplotActor& actor, const NODE* node, Dim1Tag) {
    ASSERT(node != nullptr);
    if (node->is_leaf()) {
        actor.data().splice(actor.data().end(), _StringifyCell1D(node->cell));
        actor.data().push_back("");
        return;
    }
    for (St i = 0; i < NODE::NumChildren; ++i) {
        if (node->child[i] != nullptr) {
            _OAppendNodeWireFrame(actor, node->child[i], Dim1Tag());
        }
    }
}

template<class NODE>
void _OAppendNodeWireFrame(GnuplotActor& actor, const NODE* node, Dim2Tag) {
    ASSERT(node != nullptr);
    if (node->is_leaf()) {
        actor.data().splice(actor.data().end(), _StringifyCell2D(node->cell));
        actor.data().push_back("");
        return;
    }
    for (St i = 0; i < NODE::NumChildren; ++i) {
        if (node->child[i] != nullptr) {
            _OAppendNodeWireFrame(actor, node->child[i], Dim2Tag());
        }
    }
}

template<class NODE>
void _OAppendNodeWireFrame(GnuplotActor& actor, const NODE* node, Dim3Tag) {
    ASSERT(node != nullptr);
    if (node->is_leaf()) {
        actor.data().splice(actor.data().end(), _StringifyCell3D(node->cell));
        actor.data().push_back("");
        return;
    }
    for (St i = 0; i < NODE::NumChildren; ++i) {
        if (node->child[i] != nullptr) {
            _OAppendNodeWireFrame(actor, node->child[i], Dim3Tag());
        }
    }
}

template<class ANY>
GnuplotActor _OToGnuplotActorWireFrameDim(const ANY& grid, OGridTag, Dim1Tag) {
    GnuplotActor actor;
    actor.command("using 1:2 title \"\" ");
    actor.style("with line lc -1");

    for (Int i = 0; i < Int(grid.size_i()); ++i) {
        _OAppendNodeWireFrame(actor, grid.root_node(i), Dim1Tag());
    }
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorWireFrameDim(const ANY& grid, OGridTag, Dim2Tag) {
    GnuplotActor actor;
    actor.command("using 1:2 title \"\" ");
    actor.style("with line lc 0");

    for (Int j = 0; j < Int(grid.size_j()); ++j) {
        for (Int i = 0; i < Int(grid.size_i()); ++i) {
            _OAppendNodeWireFrame(actor, grid.root_node(i, j), Dim2Tag());
        }
    }
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorWireFrameDim(const ANY& grid, OGridTag, Dim3Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with line lc 0");

    for (Int k = 0; k < Int(grid.size_k()); ++k) {
        for (Int j = 0; j < Int(grid.size_j()); ++j) {
            for (Int i = 0; i < Int(grid.size_i()); ++i) {
                _OAppendNodeWireFrame(actor, grid.root_node(i, j, k), Dim3Tag());
            }
        }
    }
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, OGridTag) {
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _OToGnuplotActorWireFrameDim(a, t, dt);
}

}

#endif
