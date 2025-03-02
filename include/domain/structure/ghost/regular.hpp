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
#if __cplusplus >= 201703L
inline static const std::array<std::array<short, 2>, 3> _BID = 
{{
     { 0, 1 }, 
     { 2, 3 }, 
     { 4, 5 } 
}};
#else
    #error "C++17 or later is required"
#endif

struct SGhostRegularTag:  public SGhostTag{};

template<St DIM, class GRID>
class SGhostRegular_ : public SGhost_<DIM, GRID>{
public:
    typedef SGhost_<DIM, GRID> Base;
    typedef SIndex_<Base::Dim> Index;
    typedef typename DimTagTraits_<DIM>::Type DimTag;
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

    virtual bool 
    is_ghost(const Index& cindex) const{
        for (St d = 0; d < DIM; ++d) {
            Idx res = cindex.value(d);
            if (res < 0) {
                return true;
            } else if (res >= this->_grid->n(d)) {
                return true;
            }
        }
        return false;
    };
    
    virtual bool 
    is_ghost(
        const Index& vindex,
        VertexTag) const
    {
        for (St d = 0; d < DIM; ++d) {
            Idx res = vindex.value(d);
            if (res < 0) {
                return true;
            } else if (res >= this->_grid->n(d) - 1) {
                return true;
            }
        }
        return false;
    };
    virtual bool 
    is_ghost_vertex(const Index& vindex) const
    {
        return is_ghost(vindex, VertexTag());
    };

    virtual bool is_boundary(
                const Index& index,
                const Axes& a,
                const Orientation& o) const{
        Idx idx = index.value(a);
        if(o == _M_){
            return idx == 0;
        }else if(o == _P_){
            return idx == (_grid->n(a) - 1);
        }else{
            SHOULD_NOT_REACH;
            return false;
        }
    }
    virtual bool is_boundary(
                const Index& index,
                const Orientation& o) const{
        for(auto& a : ArrAxes<DIM>()){
            if(is_boundary(index, a, o)){
                return true;
            }
        }
        return false;
    }

    virtual bool 
    is_boundary_vertex(const Index& vindex) const{
        for(auto& a : ArrAxes<DIM>()){
            if(is_boundary_vertex(vindex, a)){
                return true;
            }
        }
        return false;
    }
    
    virtual bool 
    is_boundary_vertex(
        const Index& vindex, 
        const Axes& a) const
    {
        Index m = vindex.m(a);
        if(is_ghost_vertex(m)){
            return true;
        }
        Index p = vindex.p(a);
        if(is_ghost_vertex(p)){
            return true;
        }
        return false; 
    }
    virtual bool is_boundary_face(
            const Index& findex,
            const St&    a) const{
        ASSERT(a < DIM);
        Idx idx = findex.value(a);
        return (idx == 0) || (idx == _grid->n().value(a)); 
    };
    
    virtual bool is_cut(const Index& index) const{
        return false;
    }

    virtual bool is_normal(const Index& index) const{
        return !(is_ghost(index));
    }

    virtual bool is_normal_vertex(const Index& vidx) const{
        return (!is_ghost(vidx, VertexTag()));
    }

    virtual int 
    boundary_id(
        const Index& indexc,
        const Index& indexg,
        const St& a,
        const St& ori) const
    {
        // get seg idx in BCID
        // St ABI[3][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
        Index n = this->_grid->n();
        Idx res = indexg.value(a);
        if (res < 0) {
            return _BID[a][0];
        } else if (res >= n.value(a)) {
            return _BID[a][1];
        }
        
        SHOULD_NOT_REACH;
        return 0;
    };

    virtual int 
    boundary_id_vertex(
        const Index& indexc,
        const Index& indexg,
        const St& a,
        const St& ori) const
    {
        // get seg idx in BCID
        // St ABI[3][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
        Index n = this->_grid->n();
        Idx res = indexg[a];
        if (res <= 0) {
            return _BID[a][0];
        } else if (res >= n[a] - 1) {
            return _BID[a][1];
        } 
        std::cout << "idx c = "<<indexc << std::endl;
        std::cout << "idx g = "<<indexg << std::endl;
        std::cout << "axe   = "<<ToString(a) << std::endl;
        std::cout << " o    = "<<ToString(ori) << std::endl;
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
    virtual St size_vertex_normal() const{
        St res = 1;
        for(auto& a : ArrAxes<DIM>()){
            res *= _grid->n(a) - 1;
        }
        return res;
    }
    virtual St size_cell_normal() const{
        return _grid->size_cell();
    }
    virtual St size_face_normal() const{
        return _grid->size_face();
    }
    virtual St size_face_normal(const Axes& a) const{
        return _grid->size_face(a);
    }
    virtual St size_not_ghost() const{
        return _grid->size_cell();
    }
};


}



#endif
