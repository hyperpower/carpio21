#ifndef _SAPPLY_BC_HPP
#define _SAPPLY_BC_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "domain/base/base_operator.hpp"

#include "domain/structure/structure.hpp"


namespace carpio{


template<class FIELD>
void ApplyBoundaryValue(
    FIELD&               field,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldCenterTag, LinearPolynomialTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    _ApplyBoundaryValue(field, bi, time,
        FieldTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
// template<class FIELD>
// void ApplyBoundaryValue(
//     FIELD&               field,
//     const BoundaryIndex& bi,
//     const Vt&            time, 
//     SFieldVertexTag, LinearPolynomialTag)
// {
//     EXPAND_FIELD_TAG(FIELD); 
//     _ApplyBoundaryValue(field, bi, time,
//         FieldTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
// }
template<class FIELD>
typename FIELD::ValueType Value(
    const FIELD&         field,
    const BoundaryIndex& bi,
    const typename FIELD::Index& idxc,
    const typename FIELD::Index& idxg,
    const Axes&            axe,
    const Orientation&     ori,
    const Vt&              time, 
    SFieldCenterTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _ValueGhostCenter(field, bi, idxc, idxg, axe, ori, time,
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterValue(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0){
    switch (bc.type()){
    case BoundaryCondition::_BC1_:
        return GetGhostCenterValueType1(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC2_:
        return GetGhostCenterValueType2(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC3_:
        return GetGhostCenterValueType3(fc,bc, idxc, idxg, axe, ori, time);
        break;
    default:
        return 0.0;
        break;
    }
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterExp(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0){
    switch (bc.type()){
    case BoundaryCondition::_BC1_:
        return GetGhostCenterExpType1(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC2_:
        return GetGhostCenterExpType2(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC3_:
        return GetGhostCenterExpType3(fc,bc, idxc, idxg, axe, ori, time);
        break;
    default:
        return 0.0;
        break;
    }
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterValueType1(
        const FIELD&             field,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0){
        // boundary condition must be type 1
        // walk back
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idxb = idxg.shift(axe, oori);
        //    int  step  = 0;
        while(field.ghost().is_ghost(idxb)){ // find nearest normal cell
            Shift(idxb, axe, oori);
        //    step++;
        }
        auto fp = field.grid().f(axe, ori, idxb);   // face point
        // auto idxsym = idxb;
        // for(int i = 0; i < step; ++i){
        //    Shift(idxsym, axe, oori);
        // }
        ASSERT(field.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation:
        //  vg - vx     vbc - vx
        // --------- = ----------  ==> vg - vx = (vbc - vx) * (dx + dg) / dx;
        //  dx + dg        dx          vg = vx + (vbc - vx) * (dx + dg) / dx;
        Vt dx  = std::abs(field.grid().c_(axe, idxb) - fp[axe]);
        Vt dg  = std::abs(field.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Vt vx  = field(idxb);
        return vx + (vbc - vx) * (dx + dg) / dx;
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterValueType2(
        const FIELD&             field,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0){
    // boundary condition must be type 2
    // walk back
    auto oori = Opposite(Orientation(ori));
    auto idxb = idxg.shift(axe, oori);
    // int step  = 0;
    while (field.ghost().is_ghost(idxb))
    {
        Shift(idxb, axe, oori);
        // step++;
    }
    auto fp = field.grid().f(axe, ori, idxb);
    //        for(int i = 0; i < step; ++i){
    //            Shift(idxb, axe, oori);
    //        }
    ASSERT(field.ghost().is_normal(idxb));
    //  idxb   face  ghost
    // ---x-----|-----g-----
    //    +--dx-+--dg-+
    // equation:
    //  vx - vg
    // --------- = vbc  ==> vx - vg = vbc * (dx + dg);
    //  dx + dg                  vg = vx - vbc * (dx + dg);
    Vt dx = std::abs(field.grid().c_(axe, idxb) - fp[axe]);
    Vt dg = std::abs(field.grid().c_(axe, idxg) - fp[axe]);
    Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
    Vt vx  = field(idxb);
    return vx - vbc * (dx + dg);
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterValueType3(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&            time = 0.0){
    // only works for regular ghost
    auto &grid = fc.grid();
    auto n = grid.n(axe);
    auto ig = idxg[axe];
    typename FIELD::Index idxp(idxg);
    if (ig >= n){
        idxp[axe] = ig % n;
    }
    else if (ig < 0){
        idxp[axe] = n - std::abs(ig) % n;
    }
    return fc(idxp);
}

template<class FIELD>
typename FIELD::ValueType GetGhostCenterExpType1_3(
        const FIELD&             field,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0)
{        
        typedef typename FIELD::ValueType Exp;
        auto& grid = field.grid();
        // boundary condition must be type 1
        // walk back
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idx0 = idxg.shift(axe, oori);
        while(field.ghost().is_ghost(idx0)){     // find nearest normal cell i
            Shift(idx0, axe, oori);
        }
        auto idx1 = idx0.shift(axe, oori);     // inner cell i+1
        auto idx2 = idx1.shift(axe, oori);     // inner cell i+1
        auto fp = field.grid().f(axe, ori, idx0);   // face point
        ASSERT(field.ghost().is_normal(idx0));
        // lagrange
        Exp res(0);
        auto xb  = fp[axe];
        auto x0  = grid.c_(axe, idx0);
        auto x1  = grid.c_(axe, idx1);
        auto x2  = grid.c_(axe, idx2);
        auto xg  = grid.c_(axe, idxg);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        res += (xg - x0) * (xg - x1) * (xg - x2)/ (xb - x0) / (xb - x1) / (xb - x2) * vbc;
        res.insert((xg - xb) * (xg - x1) * (xg - x2) / (x0 - xb) / (x0 - x1) / (x0 - x2), idx0);
        res.insert((xg - x0) * (xg - xb) * (xg - x2) / (x1 - x0) / (x1 - xb) / (x1 - x2), idx1);
        res.insert((xg - x0) * (xg - xb) * (xg - x1) / (x2 - x0) / (x2 - xb) / (x2 - x1), idx2);
        return res;
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterExpType1(
        const FIELD&             field,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0)
{        
        typedef typename FIELD::ValueType Exp;
        // boundary condition must be type 1
        // walk back
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idxb = idxg.shift(axe, oori);
        while(field.ghost().is_ghost(idxb)){ // find nearest normal cell
            Shift(idxb, axe, oori);
        }
        auto fp = field.grid().f(axe, ori, idxb);   // face point
        ASSERT(field.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation:
        //  vg - vx     vbc - vx
        // --------- = ----------  ==> vg - vx = (vbc - vx) * (dx + dg) / dx;
        //  dx + dg        dx          vg = vx + (vbc - vx) * (dx + dg) / dx;
        Vt dx  = std::abs(field.grid().c_(axe, idxb) - fp[axe]);
        Vt dg  = std::abs(field.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Exp expx(idxb);
        return expx + (vbc - expx) * (dx + dg) / dx;
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterExpType2(
        const FIELD&             field,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0)
{
    typedef typename FIELD::ValueType Exp;
    // boundary condition must be type 2
    // walk back
    auto oori = Opposite(Orientation(ori));
    auto idxb = idxg.shift(axe, oori);
    while (field.ghost().is_ghost(idxb))
    {
        Shift(idxb, axe, oori);
    }
    auto fp = field.grid().f(axe, ori, idxb);
    ASSERT(field.ghost().is_normal(idxb));
    //  idxb   face  ghost
    // ---x-----|-----g-----
    //    +--dx-+--dg-+
    // equation:
    //  vx - vg
    // --------- = vbc  ==> vx - vg = vbc * (dx + dg);
    //  dx + dg                  vg = vx - vbc * (dx + dg);
    Vt dx = std::abs(field.grid().c_(axe, idxb) - fp[axe]);
    Vt dg = std::abs(field.grid().c_(axe, idxg) - fp[axe]);
    Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
    Exp expx(idxb);
    return expx - vbc * (dx + dg);
}
template<class FIELD>
typename FIELD::ValueType GetGhostCenterExpType3(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0)
{
    typedef typename FIELD::ValueType Exp;
    // only works for regular ghost
    auto& grid = fc.grid();
    auto  n = grid.n(axe);
    auto ig = idxg[axe];
    typename FIELD::Index idxp(idxg);
    if (ig >= n){
        idxp[axe] = ig % n;
    }else if (ig < 0){
        idxp[axe] = n - std::abs(ig) % n;
    }
    return Exp(idxp);
}

template<class FIELD>
typename FIELD::ValueType _ValueGhostCenter(
    const FIELD& field,
    const BoundaryIndex& bi,
    const typename FIELD::Index& idxc,
    const typename FIELD::Index& idxg,
    const Axes&            axe,
    const Orientation&     ori,
    const Vt&              time, 
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);

    auto& ghost = field.ghost();

    if(ghost.is_ghost(idxg)){
        auto bid  = ghost.boundary_id(idxc, idxg, axe, ori);
        auto spbc = bi.find(bid);
        return GetGhostCenterValue(field, *spbc, idxc, idxg, axe, ori, time);
    }else{
        return field(idxg);
    }
}
template<class FIELD>
typename FIELD::ValueType _ValueGhostCenter(
    const FIELD& field,
    const BoundaryIndex& bi,
    const typename FIELD::Index& idxc,
    const typename FIELD::Index& idxg,
    const Axes&            axe,
    const Orientation&     ori,
    const Vt&              time, 
    LinearPolynomialTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);

    auto& ghost = field.ghost();

    if(ghost.is_ghost(idxg)){
        auto bid  = ghost.boundary_id(idxc, idxg, axe, ori);
        auto spbc = bi.find(bid);
        return GetGhostCenterExp(field, *spbc, idxc, idxg, axe, ori, time);
    }else{
        return field(idxg);
    }
}


template<class FIELD>
void _ApplyBoundaryValue(
    FIELD&               field,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldCenterTag, SGridTag , SGhostTag , SOrderTag , DimTag )
{
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef typename FIELD::ValueType Exp;

    auto& ghost = field.ghost();
    for(auto& idx : field.order()){
        _ApplyBoundaryValueLocal(field, idx, bi, time,
           FieldTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
    }
}
template<class FIELD>
void _ApplyBoundaryValueLocal(
    FIELD&               field,
    const typename FIELD::Index&  idx,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldCenterTag, SGridTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    typedef typename FIELD::ValueType Exp;

    auto& ghost = field.ghost();
    auto& exp = field(idx);
    for(auto iter = exp.begin(); iter != exp.end();){
        auto& idxg = iter->first;
        auto& coe  = iter->second;
        if(ghost.is_ghost(idxg)){
            auto axe  = GetDeltaAxe(idx, idxg);
            auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe);
            auto v    = Value(field, bi, idx, idxg, axe, ori, time);
            exp += v * coe;
            iter = exp.erase(iter);
        } else {
            ++iter; 
        }
    }
}
template<class FIELD>
void _ApplyBoundaryValueLocal(
    FIELD&               field,
    const typename FIELD::Index&  idx,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldCenterTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);
    typedef typename FIELD::ValueType Exp;

    auto& ghost = field.ghost();
    auto& exp = field(idx);
    for(auto iter = exp.begin(); iter != exp.end();){
        auto& idxg = iter->first;
        auto& coe  = iter->second;
        if(ghost.is_ghost(idxg)){
            auto v = _FindBoundaryValueInExp(field, idx, idxg, bi, time,
                GridTag(), GhostTag(), OrderTag(), DimTag());

            exp += v * coe;
            iter = exp.erase(iter);
        } else {
            ++iter; 
        }
    }
}
template<class FIELD, class CIDX, class CVALUE>
auto _AverageCenterValueByDistance(FIELD&  field,
        const typename FIELD::Index&  idx,
        const CIDX&    arridxg,
        const CVALUE&  arrv){
    EXPAND_FIELD(FIELD);
    
    ValueType sum;
    Vt sum_dis;

    auto& grid = field.grid();
    auto iteridxg = arridxg.begin();
    auto iterv   = arrv.begin();
    for(;iteridxg != arridxg.end();){
        auto cc  = grid.c(idx);
        auto cg  = grid.c(*iteridxg);
        auto dis = Distance(cc, cg);
        sum += (*iterv) * dis;
        sum_dis += dis;                
        iteridxg++;
        iterv++;
    }
    return sum / sum_dis;
}
template<class FIELD>
auto _FindBoundaryValueInExp(
    FIELD&               field,
    const typename FIELD::Index&  idx,
    const typename FIELD::Index&  idxg,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);

    typedef typename FIELD::ValueType Exp;

    auto& ghost = field.ghost();

    auto didx = GetDeltaIndex(idx, idxg);
    if(OnSameAxe(didx)){
        auto axe  = GetDeltaAxe(didx);
        auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
        return Value(field, bi, idx, idxg, axe, ori, time);
    }else{
        std::array<ValueType, FIELD::Dim> arrexp;
        arrexp.fill(Exp(0));
        std::array<Index, FIELD::Dim> arridx;
        arridx.fill(idx);
        for(auto d : ArrAxes<FIELD::Dim>()){
            if(didx[d] != 0 ){
                auto idxng = idxg.shift(d, -(Sign(didx[d])));
                if(! (ghost.is_ghost(idxng))){
                    auto ori  = GetDeltaOrientOnAxe(idxng, idxg, d); 
                    return Value(field, bi, idxng, idxg, d, ori, time); 
                }else{
                    arridx[d] = idxng;
                    arrexp[d] = _FindBoundaryValueInExp(field, idx, idxng, bi, time,
                        GridTag(), GhostTag(), OrderTag(), DimTag());
                }
            }
        }
        return _AverageCenterValueByDistance(field, idx, arridx, arrexp); 
    }
}
} // namespace carpio


#endif