#ifndef S_STENCIL_HPP
#define S_STENCIL_HPP

#include "domain/domain_define.hpp"
#include "algebra/algebra.hpp"
#include "domain/structure/structure_define.hpp"

namespace carpio{

struct SStencilTag: public StructureTag{};

template<St DIM, class VT>
class SStencil_{
public:
    static const St Dim = DIM;
    typedef SStencil_<Dim, VT> Self;

    typedef SStencilTag Tag;
    typedef typename DimTagTraits_<Dim>::Type DimTag;

    typedef VT ValueType;
    typedef _DataInitial_<VT> _DataInit;
    typedef typename _DataInit::ValueTag ValueTag;
protected:
    bool _value_setted; // flag to indicate whether the stencil values have been set
public:

    SStencil_():
        _value_setted(false){
    }

protected:
    template<class INDEX, class REL_INDEX>
    INDEX _absolute_index(const INDEX& idxc, const REL_INDEX& rel) const {
        INDEX res(idxc);
        for(St d = 0; d < Dim; ++d){
            res[d] = idxc[d] + rel[d];
        }
        return res;
    }

};




}


#endif
