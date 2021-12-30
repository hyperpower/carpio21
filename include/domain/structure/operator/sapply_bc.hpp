#ifndef _SAPPLY_BC_HPP
#define _SAPPLY_BC_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"
#include "domain/boundary/boundary_index.hpp"

namespace carpio{
// BC Implement 
template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class ApplyBCImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename GRID::Index>, 
    GRID, GHOST, ORDER, StructureType>{
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

    int set_method(const std::string& method){
        return 0;
    }
    int execute(FIELD& f, const BI& bi, const Vt& time = 0.0) const{
        for(auto& idx : f.order()){
            f(idx) = execute_local(f, idx, bi, time);
        }
        return 0;
    }
    
    Exp execute_local(FIELD& f, 
                      const Index& idx, 
                      const BI& bi,
                      const Vt& time = 0.0) const{
        auto& exp   = f(idx);
        for (auto& t : exp) {
		    auto idxg = t.first;
		    if(f.ghost().is_ghost(idxg)){
                auto axe  = GetDeltaAxe(idx, idxg);
                auto ori  = GetDeltaOrient(idx, idxg);
                auto bid  = f.ghost().boundary_id(idx, idxg, axe, ori);
                auto spbc = bi.find(bid);
                if(spbc->type() == BC::_BC1_){
                    std::cout << "Bc1" << std::endl;
        //             return _value_type1(f, *spbc, idx, idxg, axe, ori, time);
                }else if(spbc->type() == BC::_BC2_){
                    std::cout << "Bc2" << std::endl;
        //             return _value_type2(f, *spbc, idx, idxg, axe, ori, time);
                }
            }
        }
        return exp;
    }

protected:
    Exp _value_type1(
                        const Field&       fc,
                        const BC&          bc,
                        const Index&       idxc,
                        const Index&       idxg,
                        const St&          axe,
                        const St&          ori,
                        const Vt&          time = 0.0){
        auto oori = Opposite(Orientation(ori));  // opposite orientation
        auto idxb = idxg.shift(axe, oori);
        int step = 0;
        while (fc.ghost().is_ghost(idxb)) {
            Shift(idxb, axe, oori);
            step++;
        }
        auto fp = fc.grid().f(axe, ori, idxb);   // face point
//        for (int i = 0; i < step; ++i) {
//            Shift(idxb, axe, oori);
//        }
        ASSERT(fc.ghost().is_normal(idxb));
        //  idxb   face  ghost
        // ---x-----|-----g-----
        //    +--dx-+--dg-+
        // equation:
        //  vx - vg     vbc - vx
        // --------- = ----------  ==> vx - vg = (vbc - vx) * (dx + dg) / dx;
        //  dx + dg        dx          vg = vx - (vbc - vx) * (dx + dg) / dx;
        Vt dx = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
        Vt dg = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
        Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
        Exp expx(idxb);
        return expx + (vbc - expx) * (dx + dg) / dx;
    }
    // ValueType _get(
    //         const Field&         fc,
    //         const BoundaryIndex& bi,
    //         const Index&         idxc,
    //         const Index&         idxg,
    //         const St&            axe,
    //         const St&            ori,
    //         const Vt&            time = 0.0){
    //     if(fc.ghost().is_ghost(idxg)){
    //         auto bid  = fc.ghost().boundary_id(idxc, idxg, axe, ori);
    //         auto spbc = bi.find(bid);
    //         if (spbc->type() == BC::_BC1_) {
    //             return _value_type1_exp(fc, *spbc, idxc, idxg, axe, ori, time);
    //         } else if (spbc->type() == BC::_BC2_) {
    //             return _value_type2_exp(fc, *spbc, idxc, idxg, axe, ori, time);
    //         }
    //     } else {
    //         return Exp(idxg);
    //     }
    // }
    // void set(const int& other){std::cout << "int" << std::endl;}
};

    
} // namespace carpio


#endif