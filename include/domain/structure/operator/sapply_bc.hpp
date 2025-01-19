#ifndef _SAPPLY_BC_HPP
#define _SAPPLY_BC_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"
#include "domain/boundary/boundary_index.hpp"

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
                     const St&          axe,
                     const St&          ori,
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
                        const St&          axe,
                        const St&          ori,
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
        const St&            axe,
        const St&            ori,
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
        }else{
            return fc(idxg);
        }
    }

protected:
    Vt _value_type1(
                    const Field&       fc,
                    const BC&          bc,
                    const Index&       idxc,
                    const Index&       idxg,
                    const St&          axe,
                    const St&          ori,
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
                    const St&          axe,
                    const St&          ori, // center --> ghost
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
template<class FIELD>
Vt GetGhostCenterValue(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const St&            axe,
        const St&            ori,
        const Vt&            time = 0.0){
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
Vt GetGhostCenterValueType1(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const St&            axe,
        const St&            ori,
        const Vt&            time = 0.0){
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
        return vx + (vbc - vx) * (dx + dg) / dx;    
}
template<class FIELD>
Vt GetGhostCenterValueType2(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const St&            axe,
        const St&            ori,
        const Vt&            time = 0.0){
    SHOULD_NOT_REACH;
}
template<class FIELD>
Vt GetGhostCenterValueType3(
        const FIELD&             fc,
        const BoundaryCondition& bc,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idxg,
        const St&            axe,
        const St&            ori,
        const Vt&            time = 0.0){
    SHOULD_NOT_REACH;
}
} // namespace carpio


#endif