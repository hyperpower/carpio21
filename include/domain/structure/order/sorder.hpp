#ifndef _S_ORDER_HPP_
#define _S_ORDER_HPP_

#include <vector>
#include "domain/structure/structure_define.hpp"

namespace carpio{

template<St DIM, class GRID, class GHOST>
class SOrder_:public OrderBase_<DIM>{
public:
    typedef GRID Grid;
    typedef std::shared_ptr<Grid> spGrid;
    typedef GHOST Ghost;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef typename Grid::Index Index;

    typedef std::vector<Index> OrderVector;
    typedef typename OrderVector::iterator iterator;
    typedef typename OrderVector::const_iterator const_iterator;

    SOrder_(){}

    virtual ~SOrder_(){}

    virtual St size() const{SHOULD_NOT_REACH;return 0;}

    virtual iterator begin(){return iterator();};

    virtual iterator end(){return iterator();};

    virtual const_iterator begin() const{return const_iterator();}

    virtual const_iterator end() const{return const_iterator();}

    virtual St get_order(const Index&) const{return 0;}

    // virtual const Grid& grid() const {
    //     return Grid();
    // }
    // virtual const Ghost& ghost() const {
    //     return Ghost();
    // }
    virtual spGrid spgrid() const{
        return nullptr;
    }
    virtual spGhost spghost() const{
        return nullptr;
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