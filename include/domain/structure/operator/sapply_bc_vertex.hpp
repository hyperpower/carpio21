#ifndef _SAPPLY_BC_VERTEX_HPP
#define _SAPPLY_BC_VERTEX_HPP

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
    SFieldVertexTag, LinearPolynomialTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    _ApplyBoundaryValue(field, bi, time,
        FieldTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}

template<class FIELD>
void _ApplyBoundaryValue(
    FIELD&               field,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldVertexTag, SGridTag , SGhostTag , SOrderTag , DimTag )
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
typename FIELD::ValueType Value(
    const FIELD&         field,
    const BoundaryIndex& bi,
    const typename FIELD::Index& idxc,
    const typename FIELD::Index& idxg,
    const Axes&            axe,
    const Orientation&     ori,
    const Vt&              time, 
    SFieldVertexTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    return _ValueBoundaryVertex(field, bi, idxc, idxg, axe, ori, time,
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}
template<class FIELD>
typename FIELD::ValueType GetBoundaryVertexValue(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0){
    switch (bc.type()){
    case BoundaryCondition::_BC1_:
        return GetBoundaryVertexValueType1(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC2_:
        return GetBoundaryVertexValueType2(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC3_:
        return GetBoundaryVertexValueType3(fc,bc, idxc, idxg, axe, ori, time);
        break;
    default:
        return 0.0;
        break;
    }
}
template<class FIELD>
typename FIELD::ValueType GetBoundaryVertexExp(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0){
    switch (bc.type()){
    case BoundaryCondition::_BC1_:
        return GetBoundaryVertexExpType1(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC2_:
        return GetBoundaryVertexExpType2(fc,bc, idxc, idxg, axe, ori, time);
        break;
    case BoundaryCondition::_BC3_:
        return GetBoundaryVertexExpType3(fc,bc, idxc, idxg, axe, ori, time);
        break;
    default:
        return 0.0;
        break;
    }
}
template<class FIELD>
typename FIELD::ValueType GetBoundaryVertexValueType1(
        const FIELD&             field,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&              time = 0.0)
{
        // boundary condition must be type 1
        // walk back
        auto& ghost = field.ghost(); 
        auto& grid  = field.grid(); 
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idxi = idxg.shift(axe, oori);
        while(ghost.is_ghost(idxi, VertexTag())){   // find nearest normal cell
            Shift(idxi, axe, oori);
        }
        auto idxb = idxi.shift(axe, ori);
        auto pb = grid.v(idxb);
        
        return bc.value(pb.value(_X_), pb.value(_Y_), pb.value(_Z_), time);
}
template<class FIELD>
typename FIELD::ValueType GetBoundaryVertexValueType2(
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
typename FIELD::ValueType GetBoundaryVertexValueType3(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const Axes&            axe,
        const Orientation&     ori,
        const Vt&            time = 0.0){
    // only works for regular ghost
    auto& grid = fc.grid();
    auto  n = grid.n(axe);
    auto ig = idxg[axe];
    typename FIELD::Index idxp(idxg);
    auto nn = n - 1;
    // std::cout << "idxg " << idxg << std::endl;
    if (ig >= nn){
        idxp[axe] = ig % nn;
    }else if (ig < 0){
        idxp[axe] = nn - std::abs(ig) % nn;
    }
    // std::cout << "idxp " << idxp << std::endl;
    return fc(idxp);
}


template<class FIELD>
typename FIELD::ValueType GetBoundaryVertexExpType1(
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
        auto& ghost = field.ghost(); 
        auto& grid  = field.grid(); 
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idxi = idxg.shift(axe, oori);
        while(ghost.is_ghost_vertex(idxi)){   // find nearest normal cell
            Shift(idxi, axe, oori);
        }
        auto idxb = idxi.shift(axe, ori);
        auto pb = grid.v(idxb);
        
        Vt vbc = bc.value(pb.value(_X_), pb.value(_Y_), pb.value(_Z_), time);
        return Exp(vbc);
}

template<class FIELD>
typename FIELD::ValueType GetBoundaryVertexExpType2(
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
typename FIELD::ValueType GetBoundaryVertexExpType3(
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
    // std::cout << "here" << std::endl;
    auto& grid = fc.grid();
    typename FIELD::Index idxp(idxg);
    for(auto& a : ArrAxes<FIELD::Dim>(axe)){
        auto  n = grid.n(a);
        auto ig = idxg[a];
        auto nn = n - 1;
        // std::cout << "idxg " << idxg << std::endl;
        if (ig >= nn){
            idxp[a] = ig % nn;
        }else if (ig < 0){
            idxp[a] = nn - std::abs(ig) % nn;
        }
    }
    // std::cout << "idxp " << idxp << std::endl;
    return Exp(idxp);
}

template<class FIELD>
typename FIELD::ValueType _ValueBoundaryVertex(
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

    if(ghost.is_ghost_vertex(idxg)){
        auto bid  = ghost.boundary_id_vertex(idxc, idxg, axe, ori);
        auto spbc = bi.find(bid);
        return GetBoundaryVertexValue(field, *spbc, idxc, idxg, axe, ori, time);
    }else{
        return field(idxg);
    }
}
template<class FIELD>
typename FIELD::ValueType _ValueBoundaryVertex(
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

    if(ghost.is_ghost_vertex(idxg)){
        auto bid  = ghost.boundary_id_vertex(idxc, idxg, axe, ori);
        auto spbc = bi.find(bid);
        return GetBoundaryVertexExp(field, *spbc, idxc, idxg, axe, ori, time);
    }else{
        return field(idxg);
    }
}


template<class FIELD>
void _ApplyBoundaryValueLocal(
    FIELD&               field,
    const typename FIELD::Index&  idx,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldVertexTag, SGridTag, SGhostTag, SOrderTag, DimTag)
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
    SFieldVertexTag, SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);
    typedef typename FIELD::ValueType Exp;


    auto& ghost = field.ghost();
    auto& exp = field(idx);
    
    for(auto iter = exp.begin(); iter != exp.end();){
        auto& idxg = iter->first;
        auto& coe  = iter->second;
        if(ghost.is_ghost_vertex(idxg)){
            
            auto v = _FindBoundaryVertexValueInExp(field, idx, idxg, bi, time,
                GridTag(), GhostTag(), OrderTag(), DimTag());
        
            // auto didx = GetDeltaIndex(idx, idxg);
            // auto axe  = GetDeltaAxe(didx);
            // auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
            // auto v    = Value(field, bi, idx, idxg, axe, ori, time);
            exp += v * coe;
            iter = exp.erase(iter);
        } else {
            ++iter; 
        }
    }
}
template<class FIELD, class CIDX, class CVALUE>
auto _AverageVertxValueByDistance(FIELD&  field,
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
        auto cc  = grid.v(idx);
        auto cg  = grid.v(*iteridxg);
        auto dis = Distance(cc, cg);
        sum += (*iterv) * dis;
        sum_dis += dis;                
        iteridxg++;
        iterv++;
    }
    return sum / sum_dis;
}

template<class FIELD>
auto _FindBoundaryVertexValueInExp(
    FIELD&               field,
    const typename FIELD::Index&  idx,
    const typename FIELD::Index&  idxg,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SGridUniformTag, SGhostRegularTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);

    typedef typename FIELD::ValueType Exp;

    auto& ghost = field.ghost();
    auto didx = GetDeltaIndex(idx, idxg); //ghost - idx

    // if(ghost.is_ghost_vertex(idxg)){
    //     auto bid  = ghost.boundary_id_vertex(idx, idxg, axe, ori);
    //     auto spbc = bi.find(bid);
    //     isbc3 = (spbc->type() == BoundaryCondition::_BC3_);
    // }
    if(OnSameAxe(didx)){
        auto axe  = GetDeltaAxe(didx);
        auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
        return Value(field, bi, idx, idxg, axe, ori, time);
    }else{
        // std::cout << "_FindBoundaryVertexValueInExp Not on same axe" << std::endl;
        // Not on same axes
        // auto axe  = GetDeltaAxe(didx);
        // auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
        // return Value(field, bi, idx, idxg, axe, ori, time);
        
        Exp expone(0);
        // std::array<Index, FIELD::Dim> arridx;
        // arridx.fill(idx);
        std::array<int, FIELD::Dim> arrbid;
        arrbid.fill(-1);
        // std::cout << "input idx  = " << idx <<std::endl; 
        // std::cout << "      idxg = " << idxg <<std::endl; 
        // std::cout << "     didx  = " << didx << std::endl;
        for(auto d : ArrAxes<FIELD::Dim>()){
            // Go Back from ghost
            auto step = Sign(didx[d]) * (std::abs(didx[d]) + 1);
            auto idxng = idxg;
            while( std::abs(step) > 0){
                Shift(idxng, d, ToOrientationSign(-(Sign(step))));
                if(! (ghost.is_ghost_vertex(idxng))){ 
                    auto ori  = GetDeltaOrientOnAxe(idxng, idxg, d); 
                    arrbid[d] = ghost.boundary_id_vertex(idxng, idxg, d, ori); 
                    expone    = Value(field, bi, idxng, idxg, d, ori, time); 
                }
                step = (Sign(step)) * (std::abs(step) - 1);
            }
        }
        int count_bid = std::count_if(arrbid.begin(), arrbid.end(), [](int i) { return i == -1; });
        if(count_bid == 1){
            return expone;
        }
        if(count_bid >= 2){
            // std::cout << "idx  " << idx << std::endl;
            // std::cout << "idxg " << idxg << std::endl;
            // std::cout << "didx " << didx << std::endl;
            // std::cout << "count_bid " << count_bid << std::endl;
            std::array<Exp, FIELD::Dim> arrexp;
            arrexp.fill(Exp(0));
            std::array<Index, FIELD::Dim> arridx;
            arridx.fill(idx);
            std::array<int, FIELD::Dim> arrbtype;
            arrbtype.fill(-1);
            // Go from center
            for(auto d : ArrAxes<FIELD::Dim>()){
                auto step  = didx[d];
                auto idxng = idx;
                while( std::abs(step) > 0){
                    Shift(idxng, d, ToOrientationSign(Sign(step)));
                    // std::cout << "idxng " << idxng << std::endl;  
                    if(ghost.is_ghost_vertex(idxng)){ 
                        // std::cout << "idxng is ghost" << idxng << std::endl;  
                        auto ori  = GetDeltaOrientOnAxe(idx, idxng, d); 
                        auto bid = ghost.boundary_id_vertex(idx, idxng, d, ori); 
                        auto spbc = bi.find(bid);
                        arrbtype[d] = spbc->type();
                        arridx[d]   = idxng;
                        arrexp[d]   = Value(field, bi, idx, idxng, d, ori, time); 
                    }
                    step = (Sign(step)) * (std::abs(step) - 1);
                } 
            }
            int count_b3type = std::count_if(arrbtype.begin(), arrbtype.end(), [](int i) { return i == BoundaryCondition::_BC3_; });
            // std::cout << "count_bidc " << count_b3type << std::endl;
            if(count_b3type >= 2){
                // std::cout << "double 3 bc ------------" << std::endl;
                auto axe  = GetDeltaAxe(didx);
                auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
                return Value(field, bi, idx, idxg, axe, ori, time);
            }
            return _AverageCenterValueByDistance(field, idx, arridx, arrexp);  
        }
    }
}


template<class FIELD>
auto _FindBoundaryVertexValueInExp(
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

    // if(ghost.is_ghost_vertex(idxg)){
    //     auto bid  = ghost.boundary_id_vertex(idx, idxg, axe, ori);
    //     auto spbc = bi.find(bid);
    //     isbc3 = (spbc->type() == BoundaryCondition::_BC3_);
    // }
    if(OnSameAxe(didx)){
        auto axe  = GetDeltaAxe(didx);
        auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
        return Value(field, bi, idx, idxg, axe, ori, time);
    }else{
        // Not on same axes
        // auto axe  = GetDeltaAxe(didx);
        // auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
        // return Value(field, bi, idx, idxg, axe, ori, time);
        std::array<Exp, FIELD::Dim> arrexp;
        arrexp.fill(Exp(0));
        std::array<Index, FIELD::Dim> arridx;
        arridx.fill(idx);
        std::array<int, FIELD::Dim> arrbid;
        arrbid.fill(-1);
        for(auto d : ArrAxes<FIELD::Dim>()){
            if(didx[d] != 0 ){
                auto idxng = idxg.shift(d, -(Sign(didx[d])));
                if(! (ghost.is_ghost(idxng, VertexTag()))){
                    auto ori  = GetDeltaOrientOnAxe(idxng, idxg, d); 
                    arrbid[d] = ghost.boundary_id_vertex(idxng, idxg, d, ori);
                    arridx[d] = idxg;
                    arrexp[d] = Value(field, bi, idxng, idxg, d, ori, time);
                    return Value(field, bi, idxng, idxg, d, ori, time); 
                }else{
                    arridx[d] = idxng;
                    arrexp[d] = _FindBoundaryVertexValueInExp(field, idx, idxng, bi, time,
                        GridTag(), GhostTag(), OrderTag(), DimTag());
                }
            }
        }
        return _AverageCenterValueByDistance(field, idx, arridx, arrexp); 
    }
}
// template<class FIELD>
// auto _SFindBoundaryValue(
//     FIELD&               field,
//     const BoundaryIndex& bi,
//     const typename FIELD::Index&  idx,
//     const typename FIELD::Index&  idxg,
//     const Vt&            time, 
//     SFieldVertexTag, SGridUniformTag , SGhostTag, SOrderTag, DimTag)
// {
//     EXPAND_FIELD(FIELD);
//     EXPAND_FIELD_TAG(FIELD);

//     typedef typename FIELD::ValueType Exp;

//     auto& ghost = field.ghost();

//     auto didx = GetDeltaIndex(idx, idxg);
//     if(OnSameAxe(didx)){
//         auto axe  = GetDeltaAxe(didx);
//         auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
//         return Value(field, bi, idx, idxg, axe, ori, time);
//     }else{
//         std::cout << "here" << std::endl;
//         auto axe  = GetDeltaAxe(didx);
//         auto ori  = GetDeltaOrientOnAxe(idx, idxg, axe); 
//         return Value(field, bi, idx, idxg, axe, ori, time);
//         // std::array<ValueType, FIELD::Dim> arrexp;
//         // arrexp.fill(Exp(0));
//         // std::array<Index, FIELD::Dim> arridx;
//         // arridx.fill(idx);
//         // for(auto d : ArrAxes<FIELD::Dim>()){
//         //     if(didx[d] != 0 ){
//         //         auto idxng = idxg.shift(d, -(Sign(didx[d])));
//         //         if(! (ghost.is_ghost_vertex(idxng))){
//         //             auto ori  = GetDeltaOrientOnAxe(idxng, idxg, d); 
//         //             return Value(field, bi, idxng, idxg, d, ori, time); 
//         //         }else{
//         //             arridx[d] = idxng;
//         //             arrexp[d] = _SFindBoundaryValue(field, bi, idx, idxng, time,
//         //                 FieldTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
//         //         }
//         //     }
//         // }
//         // return _AverageCenterValueByDistance(field, idx, arridx, arrexp); 
//     }
// }
// template<class FIELD>
// auto FindBoundaryValue(
//     FIELD&               field,
//     const BoundaryIndex& bi,
//     const typename FIELD::Index&  idx,
//     const typename FIELD::Index&  idxg,
//     const Vt&            time, 
//     SFieldVertexTag)
// {
//     EXPAND_FIELD_TAG(FIELD);
//     return _SFindBoundaryValue(field, bi, idx, idxg, time,
//         FieldTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
// }

} // namespace carpio


#endif