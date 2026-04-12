#ifndef _O_NODE_H_
#define _O_NODE_H_

//#include "../typedefine.hpp"
#include "domain/octree/octree_define.hpp"
#include "ocell.hpp"
#include <functional>

#include <math.h>

namespace carpio {

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

inline bool is_x_p(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 6) == 7;
}

inline bool is_x_m(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 6) == 6;
}

inline bool is_y_p(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 5) == 7;
}

inline bool is_y_m(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 5) == 5;
}

inline bool is_z_p(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 3) == 7;
}

inline bool is_z_m(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 3) == 3;
}

inline bool is_on_direction(St i, const Direction& dir) {
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

protected:
    St _level;
    St _idx;
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
        }
    }

    void new_child(St idx) {
        ASSERT(idx < NumChildren);
        set_child(idx, new Node(idx, this, MakeSubCell(cell, idx), data));
    }
	
};


}

#endif
