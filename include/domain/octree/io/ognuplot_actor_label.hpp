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
std::string _ONodeLabelText(
    const ANY& node,
    const std::string& config) {
    if (config == "level") {
        return ToString(node.level());
    }
    if (config == "idx") {
        return ToString(node.idx());
    }
    return config;
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& grid,
    const std::string& config,
    OGridTag) {
    GnuplotActor actor;
    if constexpr (ANY::Dim == 3) {
        actor.command("using 1:2:3:4 title \"\" ");
    } else {
        actor.command("using 1:2:3 title \"\" ");
    }
    actor.style("with labels center textcolor lt 2");

    for (auto it = grid.cbegin(); it != grid.cend(); ++it) {
        const auto& node = *it;
        const auto idx = it.indices();
        const auto& cell = node.cell;
        const auto text = _OLabelText(grid, config, idx.i(), idx.j(), idx.k());
        if constexpr (ANY::Dim == 1) {
            actor.data().push_back(
                ToString(cell.get(_C_, _X_), 0.0, " ") + " \"" + text + "\"");
        } else if constexpr (ANY::Dim == 2) {
            actor.data().push_back(
                ToString(cell.get(_C_, _X_), cell.get(_C_, _Y_), " ")
                + " \"" + text + "\"");
        } else if constexpr (ANY::Dim == 3) {
            actor.data().push_back(
                ToString(cell.get(_C_, _X_),
                         cell.get(_C_, _Y_),
                         cell.get(_C_, _Z_),
                         " ") + " \"" + text + "\"");
        }
    }
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& node,
    const std::string& config,
    ONodeTag,
    Dim1Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");

    const auto& cell = node.cell;
    const auto text = _ONodeLabelText(node, config);
    actor.data().push_back(
        ToString(cell.get(_C_, _X_), 0.0, " ") + " \"" + text + "\"");
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& node,
    const std::string& config,
    ONodeTag,
    Dim2Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");

    const auto& cell = node.cell;
    const auto text = _ONodeLabelText(node, config);
    actor.data().push_back(
        ToString(cell.get(_C_, _X_), cell.get(_C_, _Y_), " ")
        + " \"" + text + "\"");
    return actor;
}

template<class ANY>
GnuplotActor _OToGnuplotActorLabelDim(
    const ANY& node,
    const std::string& config,
    ONodeTag,
    Dim3Tag) {
    GnuplotActor actor;
    actor.command("using 1:2:3:4 title \"\" ");
    actor.style("with labels center textcolor lt 2");

    const auto& cell = node.cell;
    const auto text = _ONodeLabelText(node, config);
    actor.data().push_back(
        ToString(cell.get(_C_, _X_),
                 cell.get(_C_, _Y_),
                 cell.get(_C_, _Z_),
                 " ") + " \"" + text + "\"");
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorLabel(
    const ANY& grid,
    const std::string& config,
    OGridTag) {
    typedef typename ANY::Tag Tag;
    return _OToGnuplotActorLabelDim(grid, config, Tag());
}

template<class ANY>
GnuplotActor _ToGnuplotActorLabel(
    const ANY& node,
    const std::string& config,
    ONodeTag) {
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _OToGnuplotActorLabelDim(node, config, Tag(), DimTag());
}

}

#endif
