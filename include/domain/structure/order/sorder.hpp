#ifndef _S_ORDER_HPP_
#define _S_ORDER_HPP_

#include <vector>
#include "domain/structure/structure_define.hpp"

namespace carpio{

struct SOrderTag:  public StructureTag, OrderBaseTag{};

template<St DIM, class GRID, class GHOST, class LOCTAG>
class SOrder_:public OrderBase_<DIM, LOCTAG>{
public:
    static const St Dim = DIM;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef SOrderTag Tag;
    typedef std::shared_ptr<Grid> spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef typename Grid::Index Index;

    typedef std::vector<Index> OrderVector;
    typedef typename OrderVector::iterator iterator;
    typedef typename OrderVector::const_iterator const_iterator;
protected:
    spGrid  _grid;
    spGhost _ghost;

public:
    SOrder_(){}

    SOrder_(spGrid spgrid, 
            spGhost spghost): _grid(spgrid) , _ghost(spghost){}

    virtual ~SOrder_(){}

    virtual St size() const{SHOULD_NOT_REACH;return 0;}

    virtual iterator begin(){return iterator();};

    virtual iterator end(){return iterator();};

    virtual const_iterator begin() const{return const_iterator();}

    virtual const_iterator end() const{return const_iterator();}

    virtual St get_order(const Index&) const{return 0;}

    const Grid& grid() const {
        return (*_grid);
    }
    const Ghost& ghost() const {
        return (*_ghost);
    }
    spGrid spgrid() const {
        return _grid;
    }
    spGhost spghost() const {
        return _ghost;
    }

    // ----------------------------------------------
#ifdef OPENMP
    virtual St num_threads() const{return 1;}
    virtual iterator begin(St) {return iterator();}
    virtual const_iterator begin(St) const {return const_iterator();}
    virtual iterator end(St) {return iterator();}
    virtual const_iterator end(St) const {return iterator();}
#endif

};



}



#endif