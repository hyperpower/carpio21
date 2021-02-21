#ifndef S_GHOST_HPP
#define S_GHOST_HPP

#include "domain/domain_define.hpp"

namespace carpio{

template<St DIM, class GRID>
class SGhost_{
public:
    static const St Dim = DIM;
    
    typedef SIndex_<Dim> Index;

    typedef ArrayListT_<Vt> Arr;
    typedef Point_<Vt, Dim> Poi;
    
    typedef St size_type;
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
    static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
    typedef std::function<void(const Index&)> FunIndex;

    SGhost_(){}

};



}

#endif