#ifndef _O_TREE_HPP_
#define _O_TREE_HPP_

//#include "../typedefine.hpp"
#include "domain/octree/octree_define.hpp"
#include "onode.hpp"
#include <functional>

#include <math.h>

namespace carpio {

template<typename DATA, typename CELL, St DIM>
class OTree_{
public:
    static const St Dim = DIM;
    static const St NumFaces = DIM + DIM;
    static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);
    static const St NumNeighbors = NumFaces;
    static const St NumChildren = NumVertexes;

    typedef OTree_<DATA, CELL, DIM> Self;
    typedef OTree_<DATA, CELL, DIM> *pSelf;

    typedef CELL Cell;
    typedef Cell *pCell;
    typedef const Cell* const_pCell;
    typedef Cell& ref_Cell;
    typedef const Cell& const_ref_Cell;

    typedef DATA  Data;
    typedef Data *pData;
    typedef const Data* const_pData;
    typedef Data& ref_Data;
    typedef const Data& const_ref_Data;

    typedef ONode_<DATA, CELL, DIM> Node;
    typedef Node *pNode;
    typedef const Node const_Node;
    typedef const Node* const_pNode;

protected:
    St _root_idx;
public:
    pNode root;

public:
    OTree_() :
        _root_idx(0),
        root(new Node()) {
    }

    ~OTree_() {
        delete root;
        root = nullptr;
    }

    OTree_(const Self& s) :
        _root_idx(s._root_idx),
        root(s.root == nullptr ? nullptr : new Node(*s.root)) {
        if (root != nullptr) {
            root->father = nullptr;
        }
    }

    Self& operator=(const Self& s) {
        if (this == &s) {
            return *this;
        }

        delete root;
        _root_idx = s._root_idx;
        root = s.root == nullptr ? nullptr : new Node(*s.root);
        if (root != nullptr) {
            root->father = nullptr;
        }
        return *this;
    }
    
};
}

#endif
