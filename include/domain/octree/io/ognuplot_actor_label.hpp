#ifndef _O_GNUPLOT_ACTOR_LABEL_HPP
#define _O_GNUPLOT_ACTOR_LABEL_HPP

#include <string>

#include "domain/base/base_gnuplot_actor.hpp"
#include "domain/octree/grid/ogrid.hpp"
#include "io/gnuplot.hpp"

namespace carpio {

template<class ANY>
std::string _OLabelText(
    const ANY& grid,
    const std::string& config,
    Int i,
    Int j,
    Int k) {
    if (config == "root_index1d") {
        return ToString(grid.to_1d_idx(i, j, k));
    }
    return ToString(typename ANY::Indices(i, j, k));
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& grid,
    const std::string& config,
    OGridTag,
    Dim1Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");

    for (Int i = 0; i < Int(grid.size_i()); ++i) {
        const auto node = grid.root_node(i);
        ASSERT(node != nullptr);
        const auto& cell = node->cell;
        const auto text = _OLabelText(grid, config, i, 0, 0);
        actor.data().push_back(
            ToString(cell.get(_C_, _X_), 0.0, " ") + " \"" + text + "\"");
    }
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& grid,
    const std::string& config,
    OGridTag,
    Dim2Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");

    for (Int j = 0; j < Int(grid.size_j()); ++j) {
        for (Int i = 0; i < Int(grid.size_i()); ++i) {
            const auto node = grid.root_node(i, j);
            ASSERT(node != nullptr);
            const auto& cell = node->cell;
            const auto text = _OLabelText(grid, config, i, j, 0);
            actor.data().push_back(
                ToString(cell.get(_C_, _X_), cell.get(_C_, _Y_), " ") 
                + " \"" + text + "\"");
        }
    }
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& grid,
    const std::string& config,
    OGridTag,
    Dim3Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3:4 title \"\" ");
    actor.style("with labels center textcolor lt 2");

    for (Int k = 0; k < Int(grid.size_k()); ++k) {
        for (Int j = 0; j < Int(grid.size_j()); ++j) {
            for (Int i = 0; i < Int(grid.size_i()); ++i) {
                const auto node = grid.root_node(i, j, k);
                ASSERT(node != nullptr);
                const auto& cell = node->cell;
                const auto text = _OLabelText(grid, config, i, j, k);
                actor.data().push_back(
                    ToString(cell.get(_C_, _X_),
                             cell.get(_C_, _Y_),
                             cell.get(_C_, _Z_),
                             " ") + " \"" + text + "\"");
            }
        }
    }
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorLabel(
    const ANY& grid,
    const std::string& config,
    OGridTag) {
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _OToGnuplotActorLabelDim(grid, config, Tag(), DimTag());
}

}

#endif
