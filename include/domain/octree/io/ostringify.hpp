#ifndef _O_STRINGIFY_HPP
#define _O_STRINGIFY_HPP

#include "io/text_file.hpp"
#include "domain/base/base_io.hpp"
#include "domain/octree/grid/ogrid.hpp"
#include "domain/base/indices.hpp"

namespace carpio {

template<class CELL>
Strings _StringifyCell1D(const CELL& cell) {
    Strings res;
    res.push_back(ToString(cell.get(_M_, _X_), 0.0, 0.0, " "));
    res.push_back(ToString(cell.get(_P_, _X_), 0.0, 0.0, " "));
    return res;
}

template<class CELL>
Strings _StringifyCell2D(const CELL& cell) {
    Strings res;
    const short order[][2] = {
        { _M_, _M_ },
        { _P_, _M_ },
        { _P_, _P_ },
        { _M_, _P_ },
        { _M_, _M_ },
    };
    for (auto& o : order) {
        res.push_back(ToString(cell.get(ToOrientation(o[_X_]), _X_),
                               cell.get(ToOrientation(o[_Y_]), _Y_),
                               0.0,
                               " "));
    }
    return res;
}

template<class CELL>
Strings _StringifyCell3D(const CELL& cell) {
    Strings res;
    const short order[][3] = {
        { _M_, _M_, _M_ },
        { _P_, _M_, _M_ },
        { _P_, _P_, _M_ },
        { _M_, _P_, _M_ },
        { _M_, _M_, _P_ },
        { _P_, _M_, _P_ },
        { _M_, _P_, _P_ },
        { _P_, _P_, _P_ },
        { _M_, _M_, _M_ },
    };
    for (auto& o : order) {
        res.push_back(ToString(cell.get(ToOrientation(o[_X_]), _X_),
                               cell.get(ToOrientation(o[_Y_]), _Y_),
                               cell.get(ToOrientation(o[_Z_]), _Z_),
                               " "));
    }
    return res;
}

template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, OGridTag, Dim1Tag) {
    const auto pn = grid.root_node(index.i());
    ASSERT(pn != nullptr);
    return _StringifyCell1D(pn->cell);
}

template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, OGridTag, Dim2Tag) {
    const auto pn = grid.root_node(index.i(), index.j());
    ASSERT(pn != nullptr);
    return _StringifyCell2D(pn->cell);
}

template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, OGridTag, Dim3Tag) {
    const auto pn = grid.root_node(index.i(), index.j(), index.k());
    ASSERT(pn != nullptr);
    return _StringifyCell3D(pn->cell);
}

template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, OctreeTag) {
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _StringifyCell(grid, index, Tag(), DimTag());
}

}

#endif
