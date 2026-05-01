#ifndef _OGRID_H_
#define _OGRID_H_

#include "domain/octree/octree_define.hpp"
#include "domain/base/base_field.hpp"
#include "domain/base/indices.hpp"
#include "onode.hpp"
#include "ocell.hpp"
#include "ogrid_root_iterator.hpp"

#include <array>
#include <cmath>
#include <functional>
#include <vector>



namespace carpio {

struct OGridTag:  public OctreeTag, GridBaseTag{};

template<class GRID>
bool IsNodeInGrid(const typename GRID::Node& node, const GRID& grid);

template<typename DATA, typename CELL, St DIM>
class OGrid_ {
public:
    static const St Dim = DIM;
    static const St NumFaces = DIM + DIM;
    static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);
    static const St NumNeighbors = NumNeighborDirections<DIM>();
    static const St GhostLayer = 1;

    typedef OGrid_<DATA, CELL, DIM> Self;
    typedef OGrid_<DATA, CELL, DIM> *pSelf;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef OGridTag Tag;
    typedef Indices_<Dim> Indices;

    typedef CELL Cell;
    typedef typename Cell::vt vt;
    typedef typename Cell::Point Point;

    typedef ONode_<DATA, CELL, DIM> Node;
    typedef Node *pNode;
    typedef const Node *const_pNode;
    typedef Node& ref_Node;
    typedef const Node& const_ref_Node;
    typedef std::function<void(ref_Node)> FunNode;
    typedef std::function<void(const_ref_Node)> FunConstNode;
    typedef OGridRootIterator_<Self, Node, pNode, Node&> iterator;
    typedef OGridRootIterator_<const Self, Node, const_pNode, const Node&> const_iterator;

protected:

    std::array<St, Dim> _len;
    std::vector<Node> _roots;

public:
    /*
     *  constructor
     */
    OGrid_(St nx, St ny = 0, St nz = 0) :
        _len(),
        _roots() {
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
        _roots.resize(storage_size());
        _set_root_indices();
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

    St size(St dim) const {
        ASSERT(dim < Dim);
        return _size(dim);
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

    St storage_size() const {
        St res = 1;
        for (St i = 0; i < Dim; ++i) {
            res *= _len[i];
        }
        return res;
    }

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size());
    }

    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    const_iterator end() const {
        return const_iterator(this, size());
    }

    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }

    const_iterator cend() const {
        return const_iterator(this, size());
    }

    void for_each_root(FunNode fun) {
        for (St idx = 0; idx < size(); ++idx) {
            fun(*root_node_1d_index(idx));
        }
    }

    void for_each_root(FunConstNode fun) const {
        for (St idx = 0; idx < size(); ++idx) {
            fun(*root_node_1d_index(idx));
        }
    }

    pNode root_node_1d_index(St idx) {
        ASSERT(idx < size());
        const auto indices = to_indices(idx);
        return root_node(indices[0], Dim >= 2 ? indices[1] : 0, Dim >= 3 ? indices[2] : 0);
    }

    const_pNode root_node_1d_index(St idx) const {
        ASSERT(idx < size());
        const auto indices = to_indices(idx);
        return root_node(indices[0], Dim >= 2 ? indices[1] : 0, Dim >= 3 ? indices[2] : 0);
    }

    pNode root_node_1d_storage_index(St idx) {
        ASSERT(idx < storage_size());
        return &_roots[idx];
    }

    const_pNode root_node_1d_storage_index(St idx) const {
        ASSERT(idx < storage_size());
        return &_roots[idx];
    }

    pNode root_node(Int i, Int j = 0, Int k = 0) {
        return &_roots[to_1d_storage_idx(i, j, k)];
    }

    const_pNode root_node(Int i, Int j = 0, Int k = 0) const {
        return &_roots[to_1d_storage_idx(i, j, k)];
    }

    pNode root_node(const Indices& idx) {
        return root_node(idx.i(), idx.j(), idx.k());
    }

    const_pNode root_node(const Indices& idx) const {
        return root_node(idx.i(), idx.j(), idx.k());
    }

    pNode find_neighbor(const_pNode node, const DirectionCode& d) {
        return const_cast<pNode>(
            static_cast<const Self*>(this)->find_neighbor(node, d));
    }

    const_pNode find_neighbor(const_pNode node, const DirectionCode& d) const {
        if (node == nullptr) {
            return nullptr;
        }
        ASSERT(node->is_root());
        if (!node->is_root()) {
            return nullptr;
        }
        if (!IsValidNeighborDirection<Dim>(d)) {
            return nullptr;
        }
        ASSERT(node->root_idx() < storage_size());
        return _find_neighbor(
            _storage_1d_idx_to_storage_indices(node->root_idx()),
            d);
    }

    void connect_neighbors() {
        for (St i = 0; i < _roots.size(); ++i) {
            auto& node = _roots[i];
            const auto idx = _storage_1d_idx_to_storage_indices(i);
            for (St n = 0; n < Node::NumNeighbors; ++n) {
                node.neighbor[n] = _find_neighbor(
                    idx, NeighborDirectionInOrder<Dim>(n));
            }
        }
        for (auto& root : _roots) {
            for (auto& node : root) {
                if (!node.is_root()) {
                    node.connect_neighbors();
                }
            }
        }
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

    Indices to_indices(St idx) const {
        Indices res;
        res[0] = Int(idx);
        if constexpr (Dim >= 2) {
            res[0] = Int(idx % size_i());
            res[1] = Int(idx / size_i());
        }
        if constexpr (Dim >= 3) {
            const St ij_size = size_i() * size_j();
            const St rem = idx % ij_size;
            res[0] = Int(rem % size_i());
            res[1] = Int(rem / size_i());
            res[2] = Int(idx / ij_size);
        }
        return res;
    }

    Indices storage_1d_idx_to_indices(St idx) const {
        auto res = _storage_1d_idx_to_storage_indices(idx);
        for (St d = 0; d < Dim; ++d) {
            res[d] -= Int(GhostLayer);
        }
        return res;
    }

protected:
    Indices _storage_1d_idx_to_storage_indices(St idx) const {
        ASSERT(idx < storage_size());
        Indices res;
        res[0] = Int(idx);
        if constexpr (Dim >= 2) {
            res[0] = Int(idx % _len[0]);
            res[1] = Int(idx / _len[0]);
        }
        if constexpr (Dim >= 3) {
            const St ij_size = _len[0] * _len[1];
            const St rem = idx % ij_size;
            res[0] = Int(rem % _len[0]);
            res[1] = Int(rem / _len[0]);
            res[2] = Int(idx / ij_size);
        }
        return res;
    }

    pNode _find_neighbor(const Indices& storage_idx, const DirectionCode& d) {
        return const_cast<pNode>(
            static_cast<const Self*>(this)->_find_neighbor(storage_idx, d));
    }

    const_pNode _find_neighbor(const Indices& storage_idx, const DirectionCode& d) const {
        ASSERT(IsValidNeighborDirection<Dim>(d));
        auto target_indices = storage_idx;
        for (St i = 0; i < Dim; ++i) {
            const Axes axis = ToAxes(i);
            if (IsDirectionOn(d, axis)) {
                target_indices = target_indices.shift(axis, ToOrientation(d, axis));
            }
        }
        for (St i = 0; i < Dim; ++i) {
            if (target_indices[i] < 0 || target_indices[i] >= Int(_len[i])) {
                return nullptr;
            }
        }
        return _root_node_storage(target_indices);
    }

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

    St _size(St dim) const {
        ASSERT(dim < Dim);
        return _len[dim] - GhostLayer - GhostLayer;
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

    void _set_root_indices() {
        for (St i = 0; i < _roots.size(); ++i) {
            _roots[i].set_root_idx(i);
        }
    }

    St _to_storage_idx(Int idx, St dim) const {
        ASSERT(dim < Dim);
        ASSERT(idx >= -Int(GhostLayer));
        ASSERT(idx < Int(_len[dim]) - Int(GhostLayer));
        return St(idx + Int(GhostLayer));
    }

    pNode _root_node_storage(St i, St j = 0, St k = 0) {
        return &_roots[_to_1d_storage_idx(i, j, k)];
    }

    const_pNode _root_node_storage(St i, St j = 0, St k = 0) const {
        return &_roots[_to_1d_storage_idx(i, j, k)];
    }

    pNode _root_node_storage(const Indices& idx) {
        return _root_node_storage(St(idx.i()), St(idx.j()), St(idx.k()));
    }

    const_pNode _root_node_storage(const Indices& idx) const {
        return _root_node_storage(St(idx.i()), St(idx.j()), St(idx.k()));
    }

};// <- End OGrid_

template<class GRID>
bool IsNodeInGrid(const typename GRID::Node& node, const GRID& grid) {
    const auto& root = node.find_root();
    const St root_idx = root.root_idx();
    if (root_idx >= grid.storage_size()) {
        return false;
    }
    return &root == grid.root_node_1d_storage_index(root_idx);
}

}// <- End namespace carpio

#include "domain/octree/grid/ogrid_nonuniform.hpp"
#include "domain/octree/grid/ogrid_uniform.hpp"

#endif
