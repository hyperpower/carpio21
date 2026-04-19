#ifndef _O_NODE_ITERATOR_H_
#define _O_NODE_ITERATOR_H_

#include "domain/octree/octree_define.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace carpio {

template<class NODE, class POINTER, class REFERENCE>
class ONodeIterator_ {
    template<class, class, class>
    friend class ONodeIterator_;

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef NODE value_type;
    typedef std::ptrdiff_t difference_type;
    typedef POINTER pointer;
    typedef REFERENCE reference;

protected:
    pointer _root;
    pointer _node;

public:
    ONodeIterator_() :
        _root(nullptr),
        _node(nullptr) {
    }

    ONodeIterator_(pointer root, pointer node) :
        _root(root),
        _node(node) {
    }

    template<
        class ON, class OP, class OR,
        std::enable_if_t<
            std::is_convertible<OP, POINTER>::value,
            int> = 0>
    ONodeIterator_(const ONodeIterator_<ON, OP, OR>& other) :
        _root(other._root),
        _node(other._node) {
    }

    reference operator*() const {
        ASSERT(_node != nullptr);
        return *_node;
    }

    pointer operator->() const {
        return &(operator*());
    }

    ONodeIterator_& operator++() {
        ASSERT(_root != nullptr);
        ASSERT(_node != nullptr);

        pointer next = _first_child(_node);
        if (next != nullptr) {
            _node = next;
            return *this;
        }

        pointer cursor = _node;
        while (cursor != _root) {
            next = _next_sibling(cursor);
            if (next != nullptr) {
                _node = next;
                return *this;
            }
            cursor = cursor->father;
        }

        _node = nullptr;
        return *this;
    }

    ONodeIterator_ operator++(int) {
        ONodeIterator_ old(*this);
        ++(*this);
        return old;
    }

    ONodeIterator_& operator--() {
        ASSERT(_root != nullptr);

        if (_node == nullptr) {
            _node = _last_descendant(_root);
            return *this;
        }

        ASSERT(_node != _root);
        pointer prev = _previous_sibling(_node);
        if (prev != nullptr) {
            _node = _last_descendant(prev);
            return *this;
        }

        _node = _node->father;
        return *this;
    }

    ONodeIterator_ operator--(int) {
        ONodeIterator_ old(*this);
        --(*this);
        return old;
    }

    template<class ON, class OP, class OR>
    bool operator==(const ONodeIterator_<ON, OP, OR>& other) const {
        return _root == other._root && _node == other._node;
    }

    template<class ON, class OP, class OR>
    bool operator!=(const ONodeIterator_<ON, OP, OR>& other) const {
        return !(*this == other);
    }

protected:
    static pointer _first_child(pointer node) {
        for (St i = 0; i < NODE::NumChildren; ++i) {
            if (node->child[i] != nullptr) {
                return node->child[i];
            }
        }
        return nullptr;
    }

    static pointer _last_child(pointer node) {
        for (St i = NODE::NumChildren; i > 0; --i) {
            const St idx = i - 1;
            if (node->child[idx] != nullptr) {
                return node->child[idx];
            }
        }
        return nullptr;
    }

    static pointer _next_sibling(pointer node) {
        pointer parent = node->father;
        if (parent == nullptr) {
            return nullptr;
        }
        for (St i = node->idx() + 1; i < NODE::NumChildren; ++i) {
            if (parent->child[i] != nullptr) {
                return parent->child[i];
            }
        }
        return nullptr;
    }

    static pointer _previous_sibling(pointer node) {
        pointer parent = node->father;
        if (parent == nullptr || node->idx() == 0) {
            return nullptr;
        }
        for (St i = node->idx(); i > 0; --i) {
            const St idx = i - 1;
            if (parent->child[idx] != nullptr) {
                return parent->child[idx];
            }
        }
        return nullptr;
    }

    static pointer _last_descendant(pointer node) {
        pointer res = node;
        while (true) {
            pointer last = _last_child(res);
            if (last == nullptr) {
                return res;
            }
            res = last;
        }
    }
};

}// namespace carpio

#endif
