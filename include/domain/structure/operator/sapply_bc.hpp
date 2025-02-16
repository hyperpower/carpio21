#ifndef _SAPPLY_BC_HPP
#define _SAPPLY_BC_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "domain/base/base_operator.hpp"

#include "domain/structure/ghost/sghost.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/order/sorder.hpp"
#include "domain/structure/sindex.hpp"


namespace carpio{
// BC Implement 
template<class FIELD, St DIM, 
         class GRID, class GHOST, class ORDER>
class ApplyBCImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename GRID::Index>, 
    GRID, GHOST, ORDER,
    SFieldCenterTag>{
public:
    typedef FIELD Field;

    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
    typedef BoundaryCondition BC;
    
    typedef LinearPolynomial_<Vt, typename GRID::Index> Exp; 
    typedef typename GRID::Index Index;
public:
    ApplyBCImplement_(){
    };

    int execute(FIELD& f, const BI& bi, const Vt& time = 0.0) const{
        for(auto& idx : f.order()){
            // if(idx == GRID::Index(0,0)){
                // std::cout <<"----" << idx << " before --------" <<std::endl;
                // std::cout << f(idx) << std::endl;
            // }
            f(idx) = _execute_local(f, idx, bi, time);
            // if(idx == GRID::Index(4,4)){
                // std::cout <<"----" << idx << " after  --------" <<std::endl;
                // std::cout << f(idx) << std::endl;
            // }
        }
        return 0;
    }

    Exp value(
            const Field&         fc, 
            const BoundaryIndex& bi,
            const Index&         idxc,
            const Index&         idxg,
            const St&            axe,
            const St&            ori,
            const Vt&            time = 0.0){
        if(fc.ghost().is_ghost(idxg)){
            auto bid  = fc.ghost().boundary_id(idxc, idxg, axe, ori);
            auto spbc = bi.find(bid);
            if (spbc->type() == BC::_BC1_) {
                return _value_type1(fc, *spbc, idxc, idxg, axe, ori, time);
            } else if (spbc->type() == BC::_BC2_) {
                return _value_type2(fc, *spbc, idxc, idxg, axe, ori, time);
            }else if(spbc->type() == BC::_BC3_){
                return _value_type3(fc, *spbc, idxc, idxg, axe, ori, time) * time;
            }
        } else {
            return Exp(idxg);
        }
    }
    

protected: 
    Exp _execute_local(FIELD& f, 
                      const Index& idx, 
                      const BI& bi,
                      const Vt& time = 0.0) const{
        Exp res(f(idx).value());
        // GRID::Index idxtest(0,9);
        for (auto& t : f(idx)) {
		    auto idxg = t.first;
		    if(f.ghost().is_ghost(idxg)){
                auto axe  = GetDeltaAxe(idx, idxg);
                auto ori  = GetDeltaOrient(idx, idxg);
                auto bid  = f.ghost().boundary_id(idx, idxg, axe, ori);
                auto spbc = bi.find(bid);
                if(spbc->type() == BC::_BC1_){
                    res += _value_type1(f, *spbc, idx, idxg, axe, ori, time) * t.second;
                }else if(spbc->type() == BC::_BC2_){
                    res += _value_type2(f, *spbc, idx, idxg, axe, ori, time) * t.second;
                }else if(spbc->type() == BC::_BC3_){
                    res += _value_type3(f, *spbc, idx, idxg, axe, ori, time) * t.second;
                }
            }else{
                res.insert(t.second, t.first);
            }

        }
        return res;
    }

    Exp _value_type1(const Field&       fc,
                     const BC&          bc,
                     const Index&       idxc,
                     const Index&       idxg,
                     const Axes&          axe,
                     const Orientation&      ori,
                     const Vt&          time = 0.0) const{
        auto oori = Opposite(Orientation(ori));  // opposite orientation
        auto idxb = idxg.shift(axe, oori);
        int step  = 0;
        while (fc.ghost().is_ghost(idxb)) {
            Shift(idxb, axe, oori);
            step++;
        }
        auto fp = fc.grid().f(axe, ori, idxb);   // face point
        auto idxsym = idxb;
        for (int i = 0; i < step; ++i) {
           Shift(idxsym, axe, oori);
        }
        ASSERT(fc.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation: P orientation
        //  vx - vg     vx - vbc
        // --------- = ----------  ==> vx - vg = (vx - vbc) * (dx + dg) / dx;
        //  dx + dg        dx          vg = vx - (vx - vbc) * (dx + dg) / dx;
        Vt dx = std::abs(fc.grid().c_(axe, idxsym) - fp[axe]);
        Vt dg = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Exp expx(idxsym);
        return expx - (expx - vbc) * (dx + dg) / dx;
    }

    Exp _value_type2(
                        const Field&       fc,
                        const BC&          bc,
                        const Index&       idxc,
                        const Index&       idxg,
                        const Axes&          axe,
                        const Orientation&          ori,
                        const Vt&          time = 0.0) const{
        // boundary condition must be type 2
        // walk back
        auto oori = Opposite(Orientation(ori));
        auto idxb = idxg.shift(axe, oori);
        // int step  = 0;
        while (fc.ghost().is_ghost(idxb)) {
            Shift(idxb, axe, oori);
            // step++;
        }
        auto fp = fc.grid().f(axe, ori, idxb);
        // auto idxsym = idxb;
        // for (int i = 0; i < step; ++i) {
        //    Shift(idxsym, axe, oori);
        // }
        ASSERT(fc.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation:
        //  vx - vg
        // --------- = vbc  ==> vx - vg = vbc * (dx + dg);
        //  dx + dg                  vg = vx - vbc * (dx + dg);
        Vt dx = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
        Vt dg = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Exp expx(idxb);
        return expx - vbc * (dx + dg);
    } 
    Exp _value_type3(const Field&       fc,
                     const BC&          bc,
                     const Index&       idxc,
                     const Index&       idxg,
                     const St&          axe,
                     const St&          ori,
                     const Vt&          time = 0.0) const{
        // only works for regular ghost
        auto& grid = fc.grid();
        auto  n    = grid.n(axe);
        auto ig    = idxg[axe];
        Index idxp(idxg);
        if (ig >= n){
            idxp[axe] = ig % n;
        }else if(ig < 0){
            idxp[axe] = n - std::abs(ig) % n;
        }
        return Exp(idxp);
    } 
};

// BC Implement 
template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class ApplyBCImplement_<
    FIELD, DIM, Vt, 
    GRID, GHOST, ORDER, SFieldCenterTag>{
public:
    typedef FIELD Field;

    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
    typedef BoundaryCondition BC;
    
    typedef Vt ValueType;
    typedef typename GRID::Index Index;
public:
    ApplyBCImplement_(){
        // std::cout << "Apply BC Vt construct" << std::endl;
    };
    
    // get value on ghost cell
    Vt value(
        const Field&         fc,
        const BoundaryIndex& bi,
        const Index&         idxc,
        const Index&         idxg,
        const Axes&            axe,
        const Orientation&      ori,
        const Vt&            time = 0.0){
        if(fc.ghost().is_ghost(idxg)){
            auto bid  = fc.ghost().boundary_id(idxc, idxg, axe, ori);
            auto spbc = bi.find(bid);
            if(spbc->type() == BC::_BC1_){
                return this->_value_type1(fc, *spbc, idxc, idxg, axe, ori, time);
            }else if(spbc->type() == BC::_BC2_){
                return this->_value_type2(fc, *spbc, idxc, idxg, axe, ori, time);
            }else if(spbc->type() == BC::_BC3_){
                return this->_value_type3(fc, *spbc, idxc, idxg, axe, ori, time);
            }
        }
        return fc(idxg);
    }

protected:
    Vt _value_type1(
                    const Field&       fc,
                    const BC&          bc,
                    const Index&       idxc,
                    const Index&       idxg,
                    const Axes&          axe,
                    const Orientation&   ori,
                    const Vt&          time = 0.0){
        // boundary condition must be type 1
        // walk back
        auto oori = Opposite(Orientation(ori));  // opposite oritation
        auto idxb = idxg.shift(axe, oori);
    //    int  step  = 0;
        while(fc.ghost().is_ghost(idxb)){ // find nearest normal cell
            Shift(idxb, axe, oori);
        //    step++;
        }
        auto fp = fc.grid().f(axe, ori, idxb);   // face point
        // auto idxsym = idxb;
        // for(int i = 0; i < step; ++i){
        //    Shift(idxsym, axe, oori);
        // }
        ASSERT(fc.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation:
        //  vg - vx     vbc - vx
        // --------- = ----------  ==> vg - vx = (vbc - vx) * (dx + dg) / dx;
        //  dx + dg        dx          vg = vx + (vbc - vx) * (dx + dg) / dx;
        Vt dx  = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
        Vt dg  = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Vt vx  = fc(idxb);
//        if(idxc == Index(0.0, 0.0)){
//            std::cout << "idxb = " << idxb << std::endl;
//            std::cout << "idxg = " << idxg << std::endl;
//            std::cout << "dx     " << dx   << std::endl;
//            std::cout << "dg     " << dg   << std::endl;
//            std::cout << "vbc    " << vbc  << std::endl;
//        }
        return vx + (vbc - vx) * (dx + dg) / dx;
    }

    Vt _value_type2(
                    const Field&       fc,
                    const BC&          bc,
                    const Index&       idxc,
                    const Index&       idxg,
                    const Axes&          axe,
                    const Orientation&   ori, // center --> ghost
                    const Vt&          time = 0.0){
        // boundary condition must be type 2
        // walk back
        auto oori = Opposite(Orientation(ori));
        auto idxb = idxg.shift(axe, oori);
        // int step  = 0;
        while(fc.ghost().is_ghost(idxb)){
            Shift(idxb, axe, oori);
            // step++;
        }
        auto fp = fc.grid().f(axe, ori, idxb);
//        for(int i = 0; i < step; ++i){
//            Shift(idxb, axe, oori);
//        }
        ASSERT(fc.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation:
        //  vx - vg
        // --------- = vbc  ==> vx - vg = vbc * (dx + dg);
        //  dx + dg                  vg = vx - vbc * (dx + dg);
        Vt dx = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
        Vt dg = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Vt vx = fc(idxb);
        return vx - vbc * (dx + dg);
    }
    Vt _value_type3(const Field&       fc,
                     const BC&          bc,
                     const Index&       idxc,
                     const Index&       idxg,
                     const St&          axe,
                     const St&          ori,
                     const Vt&          time = 0.0) const{
        // only works for regular ghost
        auto& grid = fc.grid();
        auto  n    = grid.n(axe);
        auto ig    = idxg[axe];
        Index idxp(idxg);
        if (ig >= n){
            idxp[axe] = ig % n;
        }else if(ig < 0){
            idxp[axe] = n - std::abs(ig) % n;
        }
        return fc(idxp);
    } 
};

// <--- deprecate

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
typename FIELD::ValueType _ValueCenter(
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
typename FIELD::ValueType _ValueCenter(
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
    return _ValueCenter(field, bi, idxc, idxg, axe, ori, time,
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}

template<class FIELD>
void ApplyBoundaryValue(
    FIELD&               field,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SFieldCenterTag, LinearPolynomialTag)
{
    EXPAND_FIELD_TAG(FIELD); 
    _ApplyBoundaryValue(field, bi, time,
        GridTag(), GhostTag(), OrderTag(), DimTag());
}

template<class FIELD>
void _ApplyBoundaryValue(
    FIELD&               field,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SGridTag , SGhostTag , SOrderTag , DimTag )
{
    EXPAND_FIELD_TAG(FIELD); 
    EXPAND_FIELD(FIELD);
    typedef typename FIELD::ValueType Exp;

    auto& ghost = field.ghost();
    for(auto& idx : field.order()){
        // std::cout << "idx --> " << idx << std::endl;
    // auto idx = Index(0,0);
        _ApplyBoundaryValueLocal(field, idx, bi, time,
           GridTag(), GhostTag(), OrderTag(), DimTag());
    }
}
template<class FIELD>
void _ApplyBoundaryValueLocal(
    FIELD&               field,
    const typename FIELD::Index&  idx,
    const BoundaryIndex& bi,
    const Vt&            time, 
    SGridTag, SGhostTag, SOrderTag, DimTag)
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
    SGridUniformTag, SGhostTag, SOrderTag, DimTag)
{
    EXPAND_FIELD(FIELD);
    EXPAND_FIELD_TAG(FIELD);
    typedef typename FIELD::ValueType Exp;
    // if(Index(0,0) == idx){
    //     std::cout << "ABC Unifrom Local = " << idx << std::endl;
    // }

    auto& ghost = field.ghost();
    auto& exp = field(idx);
    for(auto iter = exp.begin(); iter != exp.end();){
        auto& idxg = iter->first;
        auto& coe  = iter->second;
        if(ghost.is_ghost(idxg)){
            auto v = _FindBoundaryValueInExp(field, idx, idxg, bi, time,
                GridTag(), GhostTag(), OrderTag(), DimTag());

            // if(Index(0,0) == idx){
            //     std::cout << " v                = " << v << std::endl;
            // }
            exp += v * coe;
            iter = exp.erase(iter);
        } else {
            ++iter; 
        }
    }
    // std::cout << "applied exp = " << exp << std::endl;
}
template<class FIELD, class CIDX, class CVALUE>
auto _AverageValueByDistance(FIELD&  field,
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
        return _AverageValueByDistance(field, idx, arridx, arrexp); 
    }
}
} // namespace carpio


#endif