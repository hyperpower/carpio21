#ifndef _S_GHOST_REGULAR_HPP_
#define _S_GHOST_REGULAR_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/sindex.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "sghost.hpp"

namespace carpio{

// y   ____3___
// ^  |     /4 |
// | 0|    /   |1
// |  |___/____|
// |     5 2
// O-----> x
///
//z

struct SGhostRegularTag:  public SGhostTag, GhostBaseTag{};

template<St DIM, class GRID>
class SGhostRegular_ : public SGhost_<DIM, GRID>{
public:
    typedef SIndex_<DIM> Index;
    typedef GRID Grid;
    typedef std::shared_ptr<Grid> spGrid;
    typedef SGhostRegularTag Tag;

protected:
    spGrid _grid;
public:
    SGhostRegular_(): _grid(nullptr){

    }
    SGhostRegular_(spGrid spg): _grid(spg){
    }
    virtual ~SGhostRegular_(){
    }

    virtual std::string type() const{
        return "SGhostRegular";
    }

    // virtual St type(const Index& idx) const{
    //     return this->is_ghost(idx) ? _GHOST_ : _NORMAL_;
    // }

    virtual Grid& grid() {
           return *_grid;
    }

    virtual const Grid& grid() const {
           return *_grid;
    }

    St ghost_layer() const{
        return _grid->ghost_layer();
    }

    virtual bool is_ghost(const Index& index) const{
        for (St d = 0; d < DIM; ++d) {
            Idx res = index.value(d);
            if (res < 0) {
                return true;
            } else if (res >= this->_grid->n().value(d)) {
                return true;
            }
        }
        return false;
    };

    virtual bool is_boundary(
                const Index& index,
                const St& a,
                const St& o) const{
        ASSERT(a < DIM);
        Idx idx = index.value(a);
        if(o == _M_){
            return idx == 0;
        }else if(o == _P_){
            return idx == (_grid->n().value(a) - 1);
        }else{
            SHOULD_NOT_REACH;
            return false;
        }
    }

    virtual bool is_cut(const Index& index) const{
        return false;
    }

    virtual bool is_normal(const Index& index) const{
        return !(is_ghost(index));
    }


    virtual int boundary_id(
                const Index& indexc,
                const Index& indexg,
                const St& axe,
                const St& ori) const{
        // get seg idx in BCID
        St ABI[3][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
        Index n = this->_grid->n();
        for (St d = 0; d < DIM; ++d) {
            Idx res = indexg.value(d);
            if (res < 0) {
                return ABI[d][0];
            } else if (res >= n.value(d)) {
                return ABI[d][1];
            }
        }
        SHOULD_NOT_REACH;
        return 0;
    };

    virtual Index boundary_index(const Index& indexc,
                                 const Index& indexg,
                                 const St&    axe,
                                 const St&    ori) const{
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idxb = indexg;
        int  step = 0;
        while(this->is_ghost(idxb)){ // find nearest normal cell or cut;
            Shift(idxb, axe, oori);
            step++;
        }
        return idxb;
    }

    virtual St size_normal() const{
        return _grid->num_cells();
    }
    virtual St size_not_ghost() const{
        return _grid->num_cells();
    }
};


}



#endif
