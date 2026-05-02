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
    void _build_order_vector() {
        struct RootEntry {
            const_pNode node;
            typename MortonCode_<Grid::Dim>::Code code;
            St order;
        };

        std::vector<RootEntry> roots;
        roots.reserve(this->grid().size());

        St order = 0;
        for (auto it = this->grid().cbegin(); it != this->grid().cend(); ++it) {
            const auto indices = it.indices();
            roots.push_back({
                &(*it),
                EncodeGridMortonCode<Grid::Dim>(
                    St(indices.i()),
                    Grid::Dim >= 2 ? St(indices.j()) : 0,
                    Grid::Dim >= 3 ? St(indices.k()) : 0),
                order
            });
            ++order;
        }

        std::sort(roots.begin(), roots.end(), [](const RootEntry& lhs, const RootEntry& rhs) {
            if (lhs.code != rhs.code) {
                return lhs.code < rhs.code;
            }
            return lhs.order < rhs.order;
        });

        _ov.clear();
        for (const auto& root : roots) {
            for (auto it = root.node->cbegin(); it != root.node->cend(); ++it) {
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
