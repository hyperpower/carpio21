#ifndef _O_GHOST_HPP_
#define _O_GHOST_HPP_

#include "domain/octree/octree_define.hpp"
#include "domain/base/indices.hpp"
#include "domain/octree/grid/ogrid.hpp"

#include <memory>
#include <string>

namespace carpio {

struct OGhostTag : public OctreeTag, public GhostBaseTag {};

template<class GRID>
class OGhost_ {
public:
    typedef GRID Grid;
    static const St Dim = Grid::Dim;

    typedef OGhost_<Grid> Self;
    typedef std::shared_ptr<Grid> spGrid;
    typedef OGhostTag Tag;

    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef Indices_<Dim> Indices;

    typedef typename Grid::Cell Cell;
    typedef typename Grid::Node Node;
    typedef typename Grid::pNode pNode;
    typedef typename Grid::const_pNode const_pNode;

protected:
    spGrid _grid;

public:
    OGhost_() :
        _grid(nullptr) {
    }

    explicit OGhost_(spGrid spg) :
        _grid(spg) {
    }

    virtual ~OGhost_() = default;

    virtual std::string type() const {
        return "OGhost";
    }

    void set_grid(spGrid spg) {
        _grid = spg;
    }

    bool has_grid() const {
        return _grid != nullptr;
    }

    virtual Grid& grid() {
        ASSERT(_grid != nullptr);
        return *_grid;
    }

    virtual const Grid& grid() const {
        ASSERT(_grid != nullptr);
        return *_grid;
    }

    St ghost_layer() const {
        ASSERT(_grid != nullptr);
        return _grid->ghost_layer();
    }

    virtual bool is_ghost(const_pNode) const {
        return false;
    }

    virtual bool is_boundary(const_pNode, const Direction&) const {
        return false;
    }

    virtual const_pNode boundary_node(const_pNode, const Direction&) const {
        return nullptr;
    }
};

}

#endif
