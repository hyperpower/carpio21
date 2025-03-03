#ifndef _S_LAPLACIAN_DIFFERENTIAL_HOC4_HPP
#define _S_LAPLACIAN_DIFFERENTIAL_HOC4_HPP


#include "slaplacian_differential.hpp"

namespace carpio{

template<class FIELD, class BI>
FIELD DifferenialLaplacianHOC4(
        const FIELD& field, const BI& bi, double t, 
        SFieldVertexTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _DifferentialLaplacianHOC4(field, bi, t, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD >
FIELD DifferenialLaplacianHOC4(
        const FIELD& field, 
        SFieldVertexTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _DifferentialLaplacianHOC4(field, 
           ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
double CoeH2_12(
        const FIELD& field,
        SFieldVertexTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _CoeH2_12(field, GridTag());
}
template<class FIELD>
double _CoeH2_12(
        const FIELD& field,
        SGridUniformTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    auto& grid = field.grid();
    return grid.dc() * grid.dc() / 12.0;
}


template<class FIELD>
auto _CDSTwoAxes(
        const FIELD& field, 
        const typename FIELD::Index& idx, Axes& a, 
        LinearPolynomialTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);

    typedef ValueType Exp;
    auto na = Next(a);
    if (na >= FIELD::Dim){
        return Exp(0);
    }

    const auto& grid = field.grid();
    auto dnac = _CDSOneAxe(field, idx, na, 
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    auto dnap = _CDSOneAxe(field, idx.p(a), na, 
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    auto dnam = _CDSOneAxe(field, idx.m(a), na, 
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());

    return (dnap - dnac * 2.0 + dnam) / (grid.dc() * grid.s());
}

template<class FIELD>
FIELD _DifferentialLaplacianHOC4( // No BoundaryIndex
        const FIELD& field, 
        LinearPolynomialTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);
    typedef ValueType Exp;
    Field res = field.new_compatible_zero();
    const auto& grid = res.grid();
    
    Vt coe_h2_6 = grid.dc() * grid.dc() / 6.0;

    for (auto& idx : res.order()) {
        std::array<Exp, Field::Dim> arr;
        for(auto& d : ArrAxes<Field::Dim>()){
            arr[d] = _CDSOneAxe(field, idx, d,
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());  
            // if(idx == Index(1,1)){
            //     std::cout << "d = " << ToString(d) << std::endl;
            //     std::cout << _CDSOneAxe(field, idx, d, t,
            //    ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag()) << std::endl;
            // }

            arr[d] += _CDSTwoAxes(field, idx, d,
               ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag()) * coe_h2_6;
            // std::cout << arr[d] << std::endl;
            // if(idx == Index(1,1)){
            //     std::cout << "Two Axes d = " << ToString(d) << std::endl;
            //     std::cout << _CDSTwoAxes(field, idx, d, t,
            //     ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag()) << std::endl;
            // }

        }
        

        for(auto& d : ArrAxes<Field::Dim>()){
            res(idx) += arr[d];
        }
    }
    return res;
}

template<class FIELD>
FIELD _DifferentialLaplacianHOC4(
        const FIELD& phi, const BoundaryIndex& bi, double t, 
        LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD);
    EXPAND_FIELD(FIELD) 
    auto res = _DifferentialLaplacianHOC4(phi, 
                ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag() );
    ApplyBoundaryValue(res,bi,t);
    // std::cout << res(0,8) << std::endl;
    // Index idx(0,0, 0);
    // std::cout << idx  << phi.grid().v(idx) << std::endl;
    // Index idxm(-1,-1, -1);
    // std::cout << idxm << phi.grid().v(idxm) << std::endl;

    return res;
}




}

#endif