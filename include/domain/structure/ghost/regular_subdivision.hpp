#ifndef _S_GHOST_REGULAR_SUBDIVISION_HPP_
#define _S_GHOST_REGULAR_SUBDIVISION_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/sindex.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "sghost.hpp"
#include "regular.hpp"

namespace carpio{

// y   ____3___
// ^  |     /4 |
// | 0|    /   |1
// |  |___/____|
// |     5 2
// O-----> x
///
//z

template<St DIM, class GRID>
class SGhostRegularSubdivision_ : public SGhostRegular_<DIM, GRID>{
public:
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
    static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);

    typedef SGhostRegular_<DIM, GRID> Base;
    typedef SIndex_<DIM>              Index;
    typedef GRID                      Grid;
    typedef std::shared_ptr<Grid>   spGrid;

    // input 1. Regular ID
    //       2. x,y,z
    // return  Regular Subdivision ID
    typedef std::function<int(int, Vt, Vt, Vt)> FunBoundaryID;
protected:
    std::array<FunBoundaryID, NumFace> _arrfun;
public:
    SGhostRegularSubdivision_(spGrid spg): Base(spg){
        // default function
        FunBoundaryID fun = [](int bid, Vt x, Vt y, Vt z){
            return bid;
        };
        _arrfun.fill(fun);
    }
    virtual ~SGhostRegularSubdivision_(){
    }

    void set_boundary_id_function(int baseid, FunBoundaryID fun){
        _arrfun[baseid] = fun;
    }

    virtual std::string type() const{
        return "SGhostRegularSubdivision";
    }

    virtual int boundary_id(
                const Index& indexc,
                const Index& indexg,
                const St& axe,
                const St& ori) const{
        int baseid  = Base::boundary_id(indexc, indexg, axe, ori);
        auto bindex = Base::boundary_index(indexc, indexg, axe, ori);
        auto p      = this->_grid->f(axe, ori, bindex);
        auto fun    = _arrfun[baseid];
        return fun(baseid, p.value(_X_),p.value(_Y_),p.value(_Z_));
    };

};


}



#endif
