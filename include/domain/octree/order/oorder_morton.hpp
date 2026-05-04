#ifndef _O_ORDER_MORTON_HPP_
#define _O_ORDER_MORTON_HPP_

#include "morton.hpp"
#include "oorder.hpp"

#include <algorithm>
#include <vector>

namespace carpio {

template<class GRID, class GHOST, class LOCTAG>
class OOrderMorton_ : public OOrder_<GRID, GHOST, LOCTAG> {
public:
    typedef OOrder_<GRID, GHOST, LOCTAG> Base;
    typedef typename Base::Grid Grid;
    typedef typename Grid::Data Data;
    typedef typename Base::spGrid spGrid;
    typedef typename Base::spGhost spGhost;
    typedef typename Base::const_pNode const_pNode;

    static_assert(
        HasIdx<Data>::value,
        "OOrderMorton_ requires GRID::Data to provide idx() const returning a type convertible to St"
    );

    typedef std::vector<const_pNode> OrderVector;
    typedef typename OrderVector::iterator iterator;
    typedef typename OrderVector::const_iterator const_iterator;

protected:
    OrderVector _roots_sorted;
    OrderVector _ov;

public:
    OOrderMorton_() {
    }

    OOrderMorton_(spGrid spgrid, spGhost spghost) :
        Base(spgrid, spghost) {
        _build_order_vector();
    }

    St size() const {
        return _ov.size();
    }

    iterator begin() {
        return _ov.begin();
    }

    iterator end() {
        return _ov.end();
    }

    const_iterator begin() const {
        return _ov.begin();
    }

    const_iterator end() const {
        return _ov.end();
    }

protected:
    void _build_roots_sorted() {
        _roots_sorted.clear();
        _roots_sorted.reserve(this->grid().size());

        for (auto it = this->grid().cbegin(); it != this->grid().cend(); ++it) {
            _roots_sorted.push_back(&(*it));
        }

        std::sort(_roots_sorted.begin(), _roots_sorted.end(), [this](const_pNode lhs, const_pNode rhs) {
            const auto lhs_indices = this->grid().storage_1d_idx_to_indices(lhs->root_idx());
            const auto rhs_indices = this->grid().storage_1d_idx_to_indices(rhs->root_idx());
            const auto lhs_code = EncodeGridMortonCode<Grid::Dim>(
                St(lhs_indices.i()),
                Grid::Dim >= 2 ? St(lhs_indices.j()) : 0,
                Grid::Dim >= 3 ? St(lhs_indices.k()) : 0);
            const auto rhs_code = EncodeGridMortonCode<Grid::Dim>(
                St(rhs_indices.i()),
                Grid::Dim >= 2 ? St(rhs_indices.j()) : 0,
                Grid::Dim >= 3 ? St(rhs_indices.k()) : 0);

            if (lhs_code != rhs_code) {
                return lhs_code < rhs_code;
            }
            return lhs->root_idx() < rhs->root_idx();
        });
    }

    void _build_order_vector() {
        _build_roots_sorted();
        _ov.clear();
        for (const auto& root : _roots_sorted) {
            for (auto it = root->cbegin(); it != root->cend(); ++it) {
                const_pNode node = &(*it);
                if (node->is_leaf() && !this->ghost().is_ghost(node)) {
                    _ov.push_back(node);
                }
            }
        }
    }
};

}

#endif
