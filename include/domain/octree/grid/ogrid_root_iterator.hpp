#ifndef _OGRID_ROOT_ITERATOR_H_
#define _OGRID_ROOT_ITERATOR_H_

#include "domain/octree/octree_define.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace carpio {

template<class GRID, class NODE, class POINTER, class REFERENCE>
class OGridRootIterator_ {
    template<class, class, class, class>
    friend class OGridRootIterator_;

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef NODE value_type;
    typedef std::ptrdiff_t difference_type;
    typedef POINTER pointer;
    typedef REFERENCE reference;
    typedef typename GRID::Indices Indices;

protected:
    GRID* _grid;
    St _idx;
    Indices _indices;

public:
    OGridRootIterator_() :
        _grid(nullptr),
        _idx(0),
        _indices() {
    }

    OGridRootIterator_(GRID* grid, St idx) :
        _grid(grid),
        _idx(idx),
        _indices() {
        _update_indices();
    }

    template<
        class OG, class ON, class OP, class OR,
        std::enable_if_t<
            std::is_convertible<OG*, GRID*>::value
            && std::is_convertible<OP, POINTER>::value,
            int> = 0>
    OGridRootIterator_(const OGridRootIterator_<OG, ON, OP, OR>& other) :
        _grid(other._grid),
        _idx(other._idx),
        _indices(other._indices) {
    }

    reference operator*() const {
        ASSERT(_grid != nullptr);
        ASSERT(_idx < _grid->size());
        return *(_grid->root_node_1d_index(_idx));
    }

    pointer operator->() const {
        return &(operator*());
    }

    OGridRootIterator_& operator++() {
        ASSERT(_grid != nullptr);
        ASSERT(_idx < _grid->size());
        ++_idx;
        _update_indices();
        return *this;
    }

    OGridRootIterator_ operator++(int) {
        OGridRootIterator_ old(*this);
        ++(*this);
        return old;
    }

    OGridRootIterator_& operator--() {
        ASSERT(_grid != nullptr);
        ASSERT(_idx > 0);
        --_idx;
        _update_indices();
        return *this;
    }

    OGridRootIterator_ operator--(int) {
        OGridRootIterator_ old(*this);
        --(*this);
        return old;
    }

    bool operator==(const OGridRootIterator_& other) const {
        return _grid == other._grid && _idx == other._idx;
    }

    bool operator!=(const OGridRootIterator_& other) const {
        return !(*this == other);
    }

    Indices indices() const {
        ASSERT(_grid != nullptr);
        ASSERT(_idx < _grid->size());
        return _indices;
    }

protected:
    void _update_indices() {
        _indices = Indices();
        if (_grid == nullptr || _idx >= _grid->size()) {
            return;
        }
        _indices = _grid->to_indices(_idx);
    }
};

}// namespace carpio

#endif
