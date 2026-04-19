#ifndef _O_GRID_UNIFORM_HPP_
#define _O_GRID_UNIFORM_HPP_

#include "domain/octree/grid/ogrid.hpp"

namespace carpio {

template<typename DATA, St DIM>
class OGridUniform_ : public OGrid_<DATA, OCellUniform_<DATA, DIM>, DIM> {
public:
    static const St Dim = DIM;

    typedef OCellUniform_<DATA, DIM> Cell;
    typedef OGrid_<DATA, Cell, DIM> Base;
    typedef OGridUniform_<DATA, DIM> Self;
    typedef OGridUniform_<DATA, DIM> *pSelf;

    typedef typename Base::vt vt;
    typedef typename Base::Point Point;
    typedef std::array<vt, Dim> ArrLength;

    typedef typename Base::Node Node;
    typedef typename Base::pNode pNode;
    typedef typename Base::const_pNode const_pNode;

protected:
    Point _origin;
    vt _cell_len;

public:
    OGridUniform_(const Point& origin,
                  const vt& cell_len,
                  St nx,
                  St ny = 0,
                  St nz = 0) :
        Base(nx, Dim >= 2 ? ny : 0, Dim >= 3 ? nz : 0),
        _origin(origin),
        _cell_len(cell_len) {
        ASSERT(cell_len > 0);
        ASSERT(nx > 0);
        if constexpr (Dim >= 2) {
            ASSERT(ny > 0);
        }
        if constexpr (Dim >= 3) {
            ASSERT(nz > 0);
        }
        _set_root_cells();
    }

    OGridUniform_(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    ArrLength length() const {
        ArrLength res;
        for (St d = 0; d < Dim; ++d) {
            res[d] = length(d);
        }
        return res;
    }

    vt length(St dim) const {
        ASSERT(dim < Dim);
        return _cell_len * this->_size(dim);
    }

    vt cell_size(St dim) const {
        ASSERT(dim < Dim);
        return _cell_len;
    }

    const Point& origin() const {
        return _origin;
    }

    pNode locate_root(const Point& p) {
        return const_cast<pNode>(
            static_cast<const Self*>(this)->locate_root(p));
    }

    const_pNode locate_root(const Point& p) const {
        Int i = _locate_root_index(p, _X_);
        Int j = 0;
        Int k = 0;
        if (!_is_valid_locate_root_index(i, _X_)) {
            return nullptr;
        }
        if constexpr (Dim >= 2) {
            j = _locate_root_index(p, _Y_);
            if (!_is_valid_locate_root_index(j, _Y_)) {
                return nullptr;
            }
        }
        if constexpr (Dim >= 3) {
            k = _locate_root_index(p, _Z_);
            if (!_is_valid_locate_root_index(k, _Z_)) {
                return nullptr;
            }
        }

        const_pNode root = this->root_node(i, j, k);
        return root->cell.is_in_on(p) ? root : nullptr;
    }

    pNode locate(const Point& p) {
        return const_cast<pNode>(
            static_cast<const Self*>(this)->locate(p));
    }

    const_pNode locate(const Point& p) const {
        const_pNode node = locate_root(p);
        if (node == nullptr) {
            return nullptr;
        }

        while (node->has_child()) {
            const St child_idx = _locate_child_index(*node, p);
            if (node->child[child_idx] == nullptr) {
                return node;
            }
            node = node->child[child_idx];
        }
        return node;
    }

protected:
    St _locate_child_index(const Node& node, const Point& p) const {
        St idx = 0;
        for (St d = 0; d < Dim; ++d) {
            if (p(d) >= node.cell.get(_C_, ToAxes(d))) {
                idx |= St(1) << d;
            }
        }
        return idx;
    }

    Cell _make_cell(St i, St j = 0, St k = 0) const {
        vt cx = _center(i, _X_);
        vt cy = 0;
        vt cz = 0;
        if constexpr (Dim >= 2) {
            cy = _center(j, _Y_);
        }
        if constexpr (Dim >= 3) {
            cz = _center(k, _Z_);
        }
        return Cell(_cell_len * 0.5, cx, cy, cz);
    }

    vt _center(St storage_index, St dim) const {
        vt idx = vt(storage_index) - vt(Base::GhostLayer);
        return _origin(dim) + (idx + vt(0.5)) * _cell_len;
    }

    Int _locate_root_index(const Point& p, St dim) const {
        ASSERT(dim < Dim);
        const vt max = _origin(dim) + length(dim);
        if (p(dim) == max) {
            return Int(this->_size(dim)) - 1;
        }
        if (p(dim) == max + _cell_len) {
            return Int(this->_size(dim));
        }
        return Int(std::floor((p(dim) - _origin(dim)) / _cell_len));
    }

    bool _is_valid_locate_root_index(Int idx, St dim) const {
        ASSERT(dim < Dim);
        return idx >= -Int(Base::GhostLayer)
            && idx <= Int(this->_size(dim));
    }

    void _set_root_cells() {
        for (St i = 0; i < this->_roots.size(); ++i) {
            auto& root = this->_roots[i];
            const auto idx = this->_storage_1d_idx_to_storage_indices(i);
            root.cell = _make_cell(St(idx.i()), St(idx.j()), St(idx.k()));
        }
    }
};

}

#endif
