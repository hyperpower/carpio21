#ifndef _O_NODE_H_
#define _O_NODE_H_

//#include "../typedefine.hpp"
#include "domain/octree/octree_define.hpp"
#include "ocell.hpp"
#include "onode_iterator.hpp"
#include <functional>

#include <math.h>

namespace carpio {

struct ONodeTag:  public OctreeTag{};

    //=========================
    //   y
    //   |
    //   ---------------
    //   |  PM  |  PP  |
    //   |  2   |  3   |
    //   |  WN  |  NE  |
    //   ---------------
    //   |  SW  |  SE  |
    //   |  0   |  1   |
    //   |  MM  |  MP  |
    //   ------------------->x
    //
    //   ---------------  ---------------
    //   |  MPM |  MPP |  |  PPM |  PPP |
    //   |  2   |  3   |  |  6   |  7   |
    //   |  WNB |  NEB |  |  WNF |  NEF |
    //   ---------------  ---------------
    //   |  SWB |  SEB |  |  SWP |  SEP |
    //   |  0   |  1   |  |  4   |  5   |
    //   |  MMM |  MMP |  |  PMM |  PMP |
    //   ---------------  ---------------
    //=========================
template<St DIM>
struct ONodeIdx{
    static_assert(DIM >= 1 && DIM <= 3, "ONodeIdx only supports dimensions 1, 2, and 3");
};

template<>
struct ONodeIdx<1>{
    //1D
    static const St _M_ = 0;
    static const St _P_ = 1;
};

template<>
struct ONodeIdx<2>{
    //2D
    static const St _MM_ = 0; //00
    static const St _MP_ = 1; //01
    static const St _PM_ = 2; //10
    static const St _PP_ = 3; //11
};

template<>
struct ONodeIdx<3>{
    // 3D
    static const St _MMM_ = 0;
    static const St _MMP_ = 1;
    static const St _MPM_ = 2;
    static const St _MPP_ = 3;
    static const St _PMM_ = 4;
    static const St _PMP_ = 5;
    static const St _PPM_ = 6;
    static const St _PPP_ = 7;
};

inline bool IsXP(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 6) == 7;
}

inline bool IsXM(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 6) == 6;
}

inline bool IsYP(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 5) == 7;
}

inline bool IsYM(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 5) == 5;
}

inline bool IsZP(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 3) == 7;
}

inline bool IsZM(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 3) == 3;
}

inline bool IsOnDirection(St i, const Direction& dir) {
    ASSERT(i >= 0 && i < 8);
    unsigned short hi = HI(dir);
    unsigned short lo = LO(dir);
    return (hi & i) == (hi & lo);
}


template<typename DATA, typename CELL, St DIM>
class ONode_ {
public:
    static const St Dim = DIM;
    static const St NumFaces = DIM + DIM;
    static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);
    static const St NumNeighbors = NumFaces;
    static const St NumChildren = NumVertexes;

    typedef ONodeIdx<DIM> Idx;

    typedef ONode_<DATA, CELL, DIM> Self;
    typedef ONode_<DATA, CELL, DIM> *pSelf;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef ONodeTag Tag;

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

    typedef Self Node;
    typedef Self *pNode;
    typedef const Self const_Node;
    typedef const Self* const_pNode;
    typedef Self& ref_Node;
    typedef const Self& const_ref_Node;
    typedef std::function<void(ref_Node)> FunNode;
    typedef std::function<void(const_ref_Node)> FunConstNode;
    typedef ONodeIterator_<Node, pNode, Node&> iterator;
    typedef ONodeIterator_<Node, const_pNode, const Node&> const_iterator;

protected:
    St _level;
    St _idx;
    St _root_idx;
public:
    pNode father;
    pNode child[NumChildren];
    pNode neighbor[NumNeighbors];
    Cell cell;
    Data data;

public:
    /*
     *  constructor
     */
    ONode_() :
        _level(0),
        _idx(0),
        _root_idx(0),
        father(nullptr),
        cell(),
        data()
    {
        for (St i = 0; i < NumChildren; ++i) {
            child[i] = nullptr;
        }
        for (St i = 0; i < NumNeighbors; ++i) {
            neighbor[i] = nullptr;
        }
    }

    ONode_(St idx, pNode f, const Cell& c, const Data& d) :
        _level(f == nullptr ? 0 : f->_level + 1),
        _idx(idx),
        _root_idx(f == nullptr ? 0 : f->_root_idx),
        father(f),
        cell(c),
        data(d)
    {
        ASSERT(idx < NumChildren);
        for (St i = 0; i < NumChildren; ++i) {
            child[i] = nullptr;
        }
        for (St i = 0; i < NumNeighbors; ++i) {
            neighbor[i] = nullptr;
        }
    }

    /*
     *  destructor
     */

    ~ONode_() {
        clear_children();
    }

    ONode_(const Self& s) :
        _level(s._level),
        _idx(s._idx),
        _root_idx(s._root_idx),
        father(nullptr),
        cell(s.cell),
        data(s.data)
    {
        for (St i = 0; i < NumChildren; ++i) {
            child[i] = nullptr;
        }
        for (St i = 0; i < NumNeighbors; ++i) {
            neighbor[i] = nullptr;
        }
        for (St i = 0; i < NumChildren; ++i) {
            if (s.child[i] != nullptr) {
                set_child(i, new Node(*s.child[i]));
            }
        }
    }

    Self& operator=(const Self& s) {
        if (this == &s) {
            return *this;
        }

        clear_children();
        _level = s._level;
        _idx   = s._idx;
        _root_idx = s._root_idx;
        father = nullptr;
        cell   = s.cell;
        data   = s.data;
        for (St i = 0; i < NumNeighbors; ++i) {
            neighbor[i] = nullptr;
        }
        for (St i = 0; i < NumChildren; ++i) {
            if (s.child[i] != nullptr) {
                set_child(i, new Node(*s.child[i]));
            }
        }
        return *this;
    }

    St root_idx() const {
        return _root_idx;
    }

    St level() const {
        return _level;
    }

    St idx() const {
        return _idx;
    }

    iterator begin() {
        return iterator(this, this);
    }

    iterator end() {
        return iterator(this, nullptr);
    }

    const_iterator begin() const {
        return const_iterator(this, this);
    }

    const_iterator end() const {
        return const_iterator(this, nullptr);
    }

    const_iterator cbegin() const {
        return const_iterator(this, this);
    }

    const_iterator cend() const {
        return const_iterator(this, nullptr);
    }

    void for_each(FunNode fun) {
        for (auto& node : *this) {
            fun(node);
        }
    }

    void for_each(FunConstNode fun) const {
        for (const auto& node : *this) {
            fun(node);
        }
    }

    void set_root_idx(St idx) {
        _root_idx = idx;
        for (St i = 0; i < NumChildren; ++i) {
            if (child[i] != nullptr) {
                child[i]->set_root_idx(idx);
            }
        }
    }

    /*
     *  child
     */
    inline bool has_child() const {
        for (St i = 0; i < this->NumChildren; ++i) {
            if (this->child[i] != nullptr) {
                return true;
            }
        }
        return false;
    }
    inline bool has_child(St idx) const {
        ASSERT(idx < this->NumChildren);
        if (this->child[idx] != nullptr) {
            return true;
        }
        return false;
    }

    inline bool is_leaf() const {
        return !has_child();
    }

    inline bool is_root() const {
        if (this->father == nullptr) {
            return true;
        } else {
            return false;
        }
    }

    inline bool is_full_child() const {
        bool res = this->child[0] != nullptr;
        for (St i = 1; i < this->NumChildren; ++i) {
            res = res && (this->child[i] != nullptr);
        }
        return res;
    }

    void clear_children() {
        for (St i = 0; i < NumChildren; ++i) {
            delete child[i];
            child[i] = nullptr;
        }
    }

    void delete_child(St idx) {
        ASSERT(idx < NumChildren);
        delete child[idx];
        child[idx] = nullptr;
    }

    void set_child(St idx, pNode pn) {
        ASSERT(idx < NumChildren);
        delete_child(idx);
        child[idx] = pn;
        if (pn != nullptr) {
            pn->father = this;
            pn->_idx = idx;
            pn->_level = this->_level + 1;
            pn->set_root_idx(this->_root_idx);
        }
    }

    void new_child(St idx) {
        ASSERT(idx < NumChildren);
        set_child(idx, new Node(idx, this, MakeSubCell(cell, idx), data));
    }

    void new_full_children() {
        for (St i = 0; i < NumChildren; ++i) {
            new_child(i);
        }
    }

    pNode find_face_neighbor(const Direction& d) {
        return const_cast<pNode>(
            static_cast<const Self*>(this)->find_face_neighbor(d));
    }

    const_pNode find_face_neighbor(const Direction& d) const {
        if (!IsValidFaceDirection<Dim>(d)) {
            return nullptr;
        }
        return _find_face_neighbor(this, d);
    }

    ref_Node find_root() {
        return const_cast<ref_Node>(
            static_cast<const Self*>(this)->find_root());
    }

    const_ref_Node find_root() const {
        const_pNode res = this;
        while (res->father != nullptr) {
            res = res->father;
        }
        return *res;
    }

    /*
     *  height
     */
    St height() const {
        if (this->is_leaf()) {
            return 0;
        } else {
            St res = 0;
            for (St i = 0; i < this->NumChildren; ++i) {
                if (this->child[i] != nullptr) {
                    res = std::max(res, this->child[i]->height());
                }
            }
            return res + 1;
        }
    }


protected:
    const_pNode _find_face_neighbor(const_pNode pnode, const Direction& d) const {
        ASSERT(pnode != nullptr);
        ASSERT(IsValidFaceDirection<Dim>(d));
        if (pnode->is_root()) {
            return pnode->neighbor[FaceDirectionInOrder(d)];
        }

        const St mask = HI(d);
        const St neighbor_child_idx = pnode->_idx ^ mask;
        if (!IsOnDirection(pnode->_idx, d)) {
            return pnode->father->child[neighbor_child_idx];
        }

        const_pNode coarse_neighbor = _find_face_neighbor(pnode->father, d);
        if (coarse_neighbor == nullptr) {
            return nullptr;
        }
        if (coarse_neighbor->_level < pnode->father->_level) {
            return coarse_neighbor;
        }

        const_pNode fine_neighbor = coarse_neighbor->child[neighbor_child_idx];
        return fine_neighbor == nullptr ? coarse_neighbor : fine_neighbor;
    }
	
};


}

#endif
