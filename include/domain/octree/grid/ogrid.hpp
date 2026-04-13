#ifndef _OGRID_H_
#define _OGRID_H_

#include "domain/octree/octree_define.hpp"
#include "domain/base/base_field.hpp"
#include "domain/base/indices.hpp"
#include "onode.hpp"
#include "ocell.hpp"
#include "otree.hpp"

#include <array>
#include <vector>



namespace carpio {

struct OGridTag:  public OctreeTag, GridBaseTag{};


template<typename DATA, typename CELL, St DIM>
class OGrid_ {
public:
    static const St Dim = DIM;
    static const St NumFaces = DIM + DIM;
    static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);
    static const St NumNeighbors = NumFaces;
    static const St GhostLayer = 1;

    typedef OGrid_<DATA, CELL, DIM> Self;
    typedef OGrid_<DATA, CELL, DIM> *pSelf;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef OGridTag Tag;
    typedef Indices_<Dim> Indices;

    typedef CELL Cell;
    typedef typename Cell::vt vt;
    typedef typename Cell::Point Point;

    typedef OTree_<DATA, CELL, DIM> Tree;
    typedef Tree *pTree;
    typedef const Tree *const_pTree;

    typedef typename Tree::Node Node;
    typedef Node *pNode;
    typedef const Node *const_pNode;

protected:

    std::array<St, Dim> _len;
    std::vector<Tree> _trees;

public:
    /*
     *  constructor
     */
    OGrid_(St nx, St ny = 0, St nz = 0) :
        _len(),
        _trees() {
        ASSERT(nx > 0);
        _len[0] = nx + 2;
        if constexpr (Dim >= 2) {
            ASSERT(ny > 0);
            _len[1] = ny + 2;
        }
        if constexpr (Dim >= 3) {
            ASSERT(nz > 0);
            _len[2] = nz + 2;
        }
        _trees.resize(_storage_size());
    }

    OGrid_(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    St size() const {
        St res = 1;
        for (St i = 0; i < Dim; ++i) {
            res *= _size(i);
        }
        return res;
    }

    St size_i() const {
        return _size(_X_);
    }

    St size_j() const {
        if constexpr (Dim >= 2) {
            return _size(_Y_);
        } else {
            return 0;
        }
    }

    St size_k() const {
        if constexpr (Dim >= 3) {
            return _size(_Z_);
        } else {
            return 0;
        }
    }

    St ghost_layer() const {
        return GhostLayer;
    }

    Tree& tree_1d_index(St idx) {
        ASSERT(idx < size());
        const auto arr = _to_idx(idx);
        return tree(arr[0], Dim >= 2 ? arr[1] : 0, Dim >= 3 ? arr[2] : 0);
    }

    const Tree& tree_1d_index(St idx) const {
        ASSERT(idx < size());
        const auto arr = _to_idx(idx);
        return tree(arr[0], Dim >= 2 ? arr[1] : 0, Dim >= 3 ? arr[2] : 0);
    }

    Tree& tree(Int i, Int j = 0, Int k = 0) {
        return _trees[to_1d_storage_idx(i, j, k)];
    }

    const Tree& tree(Int i, Int j = 0, Int k = 0) const {
        return _trees[to_1d_storage_idx(i, j, k)];
    }

    pNode root_node(Int i, Int j = 0, Int k = 0) {
        return tree(i, j, k).root;
    }

    const_pNode root_node(Int i, Int j = 0, Int k = 0) const {
        return tree(i, j, k).root;
    }

    St to_1d_idx(Int i, Int j = 0, Int k = 0) const {
        ASSERT(i >= 0);
        ASSERT(i < Int(size_i()));
        St idx = St(i);
        if constexpr (Dim >= 2) {
            ASSERT(j >= 0);
            ASSERT(j < Int(size_j()));
            idx += size_i() * St(j);
        }
        if constexpr (Dim >= 3) {
            ASSERT(k >= 0);
            ASSERT(k < Int(size_k()));
            idx += size_i() * size_j() * St(k);
        }
        return idx;
    }

    St to_1d_storage_idx(Int i, Int j = 0, Int k = 0) const {
        return _to_1d_storage_idx(
            _to_storage_idx(i, _X_),
            Dim >= 2 ? _to_storage_idx(j, _Y_) : 0,
            Dim >= 3 ? _to_storage_idx(k, _Z_) : 0);
    }

protected:
    St _to_1d_storage_idx(St i, St j = 0, St k = 0) const {
        ASSERT(i < _len[0]);
        St idx = i;
        if constexpr (Dim >= 2) {
            ASSERT(j < _len[1]);
            idx += _len[0] * j;
        }
        if constexpr (Dim >= 3) {
            ASSERT(k < _len[2]);
            idx += _len[0] * _len[1] * k;
        }
        return idx;
    }

    std::array<Int, Dim> _to_idx(St idx) const {
        std::array<Int, Dim> arr;
        arr[0] = Int(idx);
        if constexpr (Dim >= 2) {
            arr[0] = Int(idx % size_i());
            arr[1] = Int(idx / size_i());
        }
        if constexpr (Dim >= 3) {
            const St ij_size = size_i() * size_j();
            const St rem = idx % ij_size;
            arr[0] = Int(rem % size_i());
            arr[1] = Int(rem / size_i());
            arr[2] = Int(idx / ij_size);
        }
        return arr;
    }

    St _size(St dim) const {
        ASSERT(dim < Dim);
        return _len[dim] - GhostLayer - GhostLayer;
    }

    St _storage_size() const {
        St res = 1;
        for (St i = 0; i < Dim; ++i) {
            res *= _len[i];
        }
        return res;
    }

    St _storage_size_i() const {
        return _len[0];
    }

    St _storage_size_j() const {
        if constexpr (Dim >= 2) {
            return _len[1];
        } else {
            return 0;
        }
    }

    St _storage_size_k() const {
        if constexpr (Dim >= 3) {
            return _len[2];
        } else {
            return 0;
        }
    }

    St _to_storage_idx(Int idx, St dim) const {
        ASSERT(dim < Dim);
        ASSERT(idx >= -Int(GhostLayer));
        ASSERT(idx < Int(_len[dim]) - Int(GhostLayer));
        return St(idx + Int(GhostLayer));
    }

    Tree& _tree_storage(St i, St j = 0, St k = 0) {
        return _trees[_to_1d_storage_idx(i, j, k)];
    }

    const Tree& _tree_storage(St i, St j = 0, St k = 0) const {
        return _trees[_to_1d_storage_idx(i, j, k)];
    }

    pNode _root_node_storage(St i, St j = 0, St k = 0) {
        return _tree_storage(i, j, k).root;
    }

    const_pNode _root_node_storage(St i, St j = 0, St k = 0) const {
        return _tree_storage(i, j, k).root;
    }

};// <- End OGrid_

template<typename DATA, typename CELL, St DIM>
class OGridNonUniform_ : public OGrid_<DATA, CELL, DIM> {
public:
    static const St Dim = DIM;

    typedef OGrid_<DATA, CELL, DIM> Base;
    typedef OGridNonUniform_<DATA, CELL, DIM> Self;
    typedef OGridNonUniform_<DATA, CELL, DIM> *pSelf;

    typedef typename Base::Cell Cell;
    typedef typename Base::vt vt;
    typedef typename Base::Point Point;
    typedef std::array<vt, Dim> ArrLength;

    typedef typename Base::Tree Tree;
    typedef typename Base::Node Node;

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
        return _length[dim] / _num_cells(dim);
    }

    const Point& origin() const {
        return _origin;
    }

protected:
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

    St _num_cells(St dim) const {
        ASSERT(dim < Dim);
        return this->_size(dim);
    }

    void _set_root_cells() {
        if constexpr (Dim == 1) {
            for (St i = 0; i < this->_storage_size_i(); ++i) {
                ASSERT(this->_root_node_storage(i) != nullptr);
                this->_root_node_storage(i)->cell = _make_cell(i);
            }
        } else if constexpr (Dim == 2) {
            for (St i = 0; i < this->_storage_size_i(); ++i) {
                for (St j = 0; j < this->_storage_size_j(); ++j) {
                    ASSERT(this->_root_node_storage(i, j) != nullptr);
                    this->_root_node_storage(i, j)->cell = _make_cell(i, j);
                }
            }
        } else if constexpr (Dim == 3) {
            for (St i = 0; i < this->_storage_size_i(); ++i) {
                for (St j = 0; j < this->_storage_size_j(); ++j) {
                    for (St k = 0; k < this->_storage_size_k(); ++k) {
                        ASSERT(this->_root_node_storage(i, j, k) != nullptr);
                        this->_root_node_storage(i, j, k)->cell = _make_cell(i, j, k);
                    }
                }
            }
        }
    }

};// <- End OGridNonUniform_
}// <- End namespace carpio


#endif
