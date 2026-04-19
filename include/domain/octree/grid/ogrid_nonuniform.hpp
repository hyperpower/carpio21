#ifndef _O_GRID_NONUNIFORM_HPP_
#define _O_GRID_NONUNIFORM_HPP_

#include "domain/octree/grid/ogrid.hpp"

namespace carpio {

template<typename DATA, St DIM>
class OGridNonUniform_ : public OGrid_<DATA, OCellNonUniform_<DATA, DIM>, DIM> {
public:
    static const St Dim = DIM;

    typedef OCellNonUniform_<DATA, Dim> Cell;
    typedef OGrid_<DATA, Cell, Dim> Base;
    typedef OGridNonUniform_<DATA, Dim> Self;
    typedef OGridNonUniform_<DATA, Dim> *pSelf;

    typedef typename Base::vt vt;
    typedef typename Base::Point Point;
    typedef std::array<vt, Dim> ArrLength;

    typedef typename Base::Node Node;
    typedef typename Base::pNode pNode;
    typedef typename Base::const_pNode const_pNode;

protected:
    Point _origin;
    ArrLength _length;

public:
    OGridNonUniform_(const Point& origin,
                     const vt& cell_len, //the length of each cell
                     St nx, // number of cells in x direction
                     St ny = 0, // number of cells in y direction, only used when Dim
                     St nz = 0 // number of cells in z direction, only used when Dim >= 3
                     ) :
        Base(nx, Dim >= 2 ? ny : 0, Dim >= 3 ? nz : 0),
        _origin(origin),
        _length() {
        ASSERT(cell_len > 0);
        ASSERT(nx > 0);
        _length[_X_] = cell_len * nx;
        if constexpr (Dim >= 2) {
            ASSERT(ny > 0);
            _length[_Y_] = cell_len * ny;
        }
        if constexpr (Dim >= 3) {
            ASSERT(nz > 0);
            _length[_Z_] = cell_len * nz;
        }
        _set_root_cells();
    }

    OGridNonUniform_(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    const ArrLength& length() const {
        return _length;
    }

    vt length(St dim) const {
        ASSERT(dim < Dim);
        return _length[dim];
    }

    vt cell_size(St dim) const {
        ASSERT(dim < Dim);
        return _length[dim] / this->_size(dim);
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
        vt hx = cell_size(_X_) * 0.5;
        vt hy = 0;
        vt hz = 0;
        if constexpr (Dim >= 2) {
            hy = cell_size(_Y_) * 0.5;
        }
        if constexpr (Dim >= 3) {
            hz = cell_size(_Z_) * 0.5;
        }
        return Cell(cx, hx, cy, hy, cz, hz);
    }

    vt _center(St storage_index, St dim) const {
        vt idx = vt(storage_index) - vt(Base::GhostLayer);
        return _origin(dim) + (idx + vt(0.5)) * cell_size(dim);
    }

    Int _locate_root_index(const Point& p, St dim) const {
        ASSERT(dim < Dim);
        const vt h = cell_size(dim);
        const vt max = _origin(dim) + _length[dim];
        if (p(dim) == max) {
            return Int(this->_size(dim)) - 1;
        }
        if (p(dim) == max + h) {
            return Int(this->_size(dim));
        }
        return Int(std::floor((p(dim) - _origin(dim)) / h));
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
