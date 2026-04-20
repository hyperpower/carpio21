#ifndef _O_GRID_NONUNIFORM_HPP_
#define _O_GRID_NONUNIFORM_HPP_

#include "domain/octree/grid/ogrid.hpp"

#include <numeric>

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
    typedef std::array<std::vector<vt>, Dim> ArrCellSize;

    typedef typename Base::Node Node;
    typedef typename Base::pNode pNode;
    typedef typename Base::const_pNode const_pNode;

protected:
    Point _origin;
    ArrLength _length;
    ArrCellSize _cell_size;

public:
    OGridNonUniform_(const Point& origin,
                     const vt& cell_len, //the length of each cell
                     St nx, // number of cells in x direction
                     St ny = 0, // number of cells in y direction, only used when Dim
                     St nz = 0 // number of cells in z direction, only used when Dim >= 3
                     ) :
        OGridNonUniform_(origin, _make_uniform_cell_size(cell_len, nx, ny, nz)) {
    }

    OGridNonUniform_(const Point& origin,
                     const ArrCellSize& cell_lengths) :
        Base(_normal_size(cell_lengths, _X_),
             Dim >= 2 ? _normal_size(cell_lengths, _Y_) : 0,
             Dim >= 3 ? _normal_size(cell_lengths, _Z_) : 0),
        _origin(origin),
        _length(),
        _cell_size() {
        _set_cell_size(cell_lengths);
        _set_length();
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

    const std::vector<vt>& cell_size(St dim) const {
        ASSERT(dim < Dim);
        return _cell_size[dim];
    }

    vt cell_size(Axes dim) const {
        ASSERT(St(dim) < Dim);
        return _cell_size[dim][Base::GhostLayer];
    }

    const ArrCellSize& cell_size() const {
        return _cell_size;
    }

    vt cell_size(St dim, Int idx) const {
        ASSERT(dim < Dim);
        return _cell_size[dim][this->_to_storage_idx(idx, dim)];
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
        vt hx = _cell_size[_X_][i] * 0.5;
        vt hy = 0;
        vt hz = 0;
        if constexpr (Dim >= 2) {
            hy = _cell_size[_Y_][j] * 0.5;
        }
        if constexpr (Dim >= 3) {
            hz = _cell_size[_Z_][k] * 0.5;
        }
        return Cell(cx, hx, cy, hy, cz, hz);
    }

    vt _center(St storage_index, St dim) const {
        vt face_m = _origin(dim) - _cell_size[dim].front();
        for (St i = 0; i < storage_index; ++i) {
            face_m += _cell_size[dim][i];
        }
        return face_m + _cell_size[dim][storage_index] * vt(0.5);
    }

    Int _locate_root_index(const Point& p, St dim) const {
        ASSERT(dim < Dim);
        const vt max = _origin(dim) + _length[dim];
        if (p(dim) == max) {
            return Int(this->_size(dim)) - 1;
        }
        const Int first = -Int(Base::GhostLayer);
        const Int last = Int(this->_size(dim));
        const Axes axis = ToAxes(dim);
        const vt outer_max = _root_node_on_axis(last, dim)->cell.get(_P_, axis);
        if (p(dim) == outer_max) {
            return last;
        }
        for (Int idx = first; idx <= last; ++idx) {
            const auto& cell = _root_node_on_axis(idx, dim)->cell;
            if (p(dim) >= cell.get(_M_, axis) && p(dim) < cell.get(_P_, axis)) {
                return idx;
            }
        }
        return p(dim) < _root_node_on_axis(first, dim)->cell.get(_M_, axis)
            ? first - 1
            : last + 1;
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

    const_pNode _root_node_on_axis(Int idx, St dim) const {
        ASSERT(dim < Dim);
        if (dim == _X_) {
            return this->root_node(idx);
        }
        if constexpr (Dim >= 2) {
            if (dim == _Y_) {
                return this->root_node(0, idx);
            }
        }
        if constexpr (Dim >= 3) {
            if (dim == _Z_) {
                return this->root_node(0, 0, idx);
            }
        }
        SHOULD_NOT_REACH;
        return nullptr;
    }

    static St _normal_size(const ArrCellSize& cell_lengths, St dim) {
        ASSERT(dim < Dim);
        ASSERT(cell_lengths[dim].size() > 0);
        return cell_lengths[dim].size();
    }

    static ArrCellSize _make_uniform_cell_size(
            const vt& cell_len,
            St nx,
            St ny = 0,
            St nz = 0) {
        ASSERT(cell_len > 0);
        ASSERT(nx > 0);
        ArrCellSize res;
        res[_X_].assign(nx, cell_len);
        if constexpr (Dim >= 2) {
            ASSERT(ny > 0);
            res[_Y_].assign(ny, cell_len);
        }
        if constexpr (Dim >= 3) {
            ASSERT(nz > 0);
            res[_Z_].assign(nz, cell_len);
        }
        return res;
    }

    void _set_cell_size(const ArrCellSize& cell_lengths) {
        for (St dim = 0; dim < Dim; ++dim) {
            const auto& normal = cell_lengths[dim];
            ASSERT(normal.size() == this->_size(dim));
            ASSERT(normal.size() > 0);
            _cell_size[dim].clear();
            _cell_size[dim].reserve(normal.size() + 2 * Base::GhostLayer);
            _cell_size[dim].push_back(normal.front());
            for (auto len : normal) {
                ASSERT(len > vt(0));
                _cell_size[dim].push_back(len);
            }
            _cell_size[dim].push_back(normal.back());
        }
    }

    void _set_length() {
        for (St dim = 0; dim < Dim; ++dim) {
            _length[dim] = std::accumulate(
                _cell_size[dim].begin() + Base::GhostLayer,
                _cell_size[dim].end() - Base::GhostLayer,
                vt(0));
        }
    }

};

}

#endif
