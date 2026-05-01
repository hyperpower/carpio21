#ifndef _O_GHOST_REGULAR_HPP_
#define _O_GHOST_REGULAR_HPP_

#include "domain/octree/ghost/oghost.hpp"

namespace carpio {

struct OGhostRegularTag : public OGhostTag {};

template<class GRID>
class OGhostRegular_ : public OGhost_<GRID> {
public:
    typedef OGhost_<GRID> Base;
    typedef GRID Grid;
    static const St Dim = Grid::Dim;

    typedef OGhostRegular_<Grid> Self;
    typedef typename Base::spGrid spGrid;
    typedef OGhostRegularTag Tag;

    typedef typename Base::DimTag DimTag;
    typedef typename Base::Indices Indices;
    typedef typename Base::Cell Cell;
    typedef typename Base::Node Node;
    typedef typename Base::pNode pNode;
    typedef typename Base::const_pNode const_pNode;
    typedef typename Grid::ref_Node ref_Node;
    typedef typename Grid::const_ref_Node const_ref_Node;
    typedef std::function<void(ref_Node)> FunNode;
    typedef std::function<void(const_ref_Node)> FunConstNode;

public:
    OGhostRegular_() :
        Base() {
    }

    explicit OGhostRegular_(spGrid spg) :
        Base(spg) {
    }

    virtual ~OGhostRegular_() = default;

    virtual std::string type() const {
        return "OGhostRegular";
    } 

    void for_each_root(FunNode fun) {
        ASSERT(this->has_grid());
        if (!this->has_grid()) {
            return;
        }

        for (St idx = 0; idx < this->grid().storage_size(); ++idx) {
            auto node = this->grid().root_node_1d_storage_index(idx);
            if (is_ghost(node)) {
                fun(*node);
            }
        }
    }

    void for_each_root(FunConstNode fun) const {
        ASSERT(this->has_grid());
        if (!this->has_grid()) {
            return;
        }

        for (St idx = 0; idx < this->grid().storage_size(); ++idx) {
            auto node = this->grid().root_node_1d_storage_index(idx);
            if (is_ghost(node)) {
                fun(*node);
            }
        }
    }

    virtual bool is_ghost(const_pNode node) const {
        if (!this->has_grid() || node == nullptr) {
            return false;
        }
        if (!IsNodeInGrid<Grid>(*node, this->grid())) {
            return false;
        }

        const auto& root = node->find_root();
        auto indices = this->grid().storage_1d_idx_to_indices(root.root_idx());

        for (St d = 0; d < Dim; ++d) {
            Idx res = indices.value(d);
            if (res < 0) {
                return true;
            } else if (res >= this->grid().size(d)) {
                return true;
            }
        }
        return false;
    }

    virtual bool is_boundary(const_pNode node, const DirectionCode& dir) const {
        if (!IsValidFaceDirection<Dim>(dir) || node == nullptr) {
            return false;
        }
        if (!this->has_grid()
            || !IsNodeInGrid<Grid>(*node, this->grid())
            || is_ghost(node)) {
            return false;
        }

        const_pNode neighbor = node->find_neighbor(dir);
        return neighbor != nullptr && is_ghost(neighbor);
    }

    // TODO: implement octree boundary mapping logic.
    virtual const_pNode boundary_node(const_pNode, const DirectionCode&) const {
        return nullptr;
    }
};

}

#endif
