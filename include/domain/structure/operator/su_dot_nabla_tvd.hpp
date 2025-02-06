#ifndef _SU_DOT_NABLA_TVD_HPP
#define _SU_DOT_NABLA_TVD_HPP

#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/operator/sapply_bc.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"


namespace carpio{

inline Vt _RCD(Vt sC, Vt sD){
    return (sD + sC) / sC;
}

inline Vt _rCD(Vt vU, Vt vC, Vt vD, Vt sU, Vt sC, Vt sD){
    return (vC - vU) * (sD + sC) / (vD - vC + 1e-16) / (sC + sU);
}

class Limiter
{
public:
    typedef Vt (*Fun)(Vt, Vt);
    typedef std::unordered_map<std::string, Fun> LimiterMap;
private:
    static Limiter * pinstance_;
    static std::mutex mutex_;

    LimiterMap _maplim;

protected:
    Limiter()
    {
        _build_limiter_map();
    }
    ~Limiter() {
        
    }

    void _build_limiter_map(){
        _maplim["SOU"]      = Limiter::_limiter_SOU;
        _maplim["Fromm"]    = Limiter::_limiter_Fromm;
        _maplim["CUI"]      = Limiter::_limiter_CUI;
        _maplim["QUICK"]    = Limiter::_limiter_QUICK;
        _maplim["CDS"]      = Limiter::_limiter_CDS;
        _maplim["VanLeer"]  = Limiter::_limiter_VanLeer;
        _maplim["WAHYD"]    = Limiter::_limiter_WAHYD;
        _maplim["BSOU"]     = Limiter::_limiter_BSOU;
        _maplim["Superbee"] = Limiter::_limiter_Superbee;
        _maplim["Minmod"]   = Limiter::_limiter_Minmod;
        _maplim["MUSCL"]    = Limiter::_limiter_MUSCL;
        _maplim["Koren"]    = Limiter::_limiter_Koren;
        _maplim["WACEB"]    = Limiter::_limiter_WACEB;
        _maplim["UMIST"]    = Limiter::_limiter_UMIST;
        _maplim["SPLmax12"] = Limiter::_limiter_SPLmax12;
        _maplim["SPL13"]    = Limiter::_limiter_SPL13;
        _maplim["SPLmax13"] = Limiter::_limiter_SPLmax13;
        _maplim["Harmonic"] = Limiter::_limiter_Harmonic;
        _maplim["OSPER"]    = Limiter::_limiter_OSPER;
        _maplim["Albada"]   = Limiter::_limiter_Albada;
        _maplim["GVA12"]    = Limiter::_limiter_GVA12;
        _maplim["GVA13"]    = Limiter::_limiter_GVA13;
        _maplim["GPR0"]     = Limiter::_limiter_GPR0;
        _maplim["GPR12"]    = Limiter::_limiter_GPR12;
        _maplim["GPR13"]    = Limiter::_limiter_GPR13;
    }

public:
    
    Limiter(Limiter &other) = delete;
    void operator=(const Limiter &) = delete;

    static Limiter *Get();

    Fun get_limiter(const std::string& name){
        auto it = _maplim.find(name);
        if (it != _maplim.end()) {
            return _maplim[name];
        }else{
            ASSERT_MSG(false, "no " + name);
            return _maplim["QUICK"];
        }
    }

    bool has_limiter(const std::string& name) const{
        auto it = _maplim.find(name);
        if (it != _maplim.end()) {
            return true;
        }else{
            return false;
        }
    }

    

protected:
    //  A review on TVD schemes and a refined flux-limiter
    //  for steady-state calculations
    //  Di Zhang, Chunbo Jiang, Dongfang Liang, Liang Cheng
    //  Journal of Computational Physics 302 (2015) 114–154
    //
    // k-scheme
    //            1 + k           1 - k
    // limiter = ------- r(CD) + -------
    //              2               2
    //          d(phi)/dx (CU)
    // r(CD) = ----------------
    //          d(phi)/dx (DC)
    //
    // SOU                      k = -1  (Second order upwind   upwind2)
    // Fromm                    k = 0
    // CUI                      k = 1/3
    // QUICK                    k = 1/2
    // CDS                      k = 1   (Center Difference Scheme,  center)

    static Vt _limiter_SOU(Vt r, Vt R) {
        return r + 1.0;
    }
    static Vt _limiter_Fromm(Vt r, Vt R){
        return 0.5 * r + 0.5;
    }
    static Vt _limiter_CUI(Vt r, Vt R) {
        return 2.0 / 3.0 * r + 1.0 / 3.0;
    }
    static Vt _limiter_QUICK(Vt r, Vt R){
        return 0.75 * r + 0.25;
    }
    static Vt _limiter_CDS(Vt r, Vt R) {
        return r;
    }
    static Vt _limiter_VanLeer(Vt r, Vt R) {
        return 0.5 * R * (r + std::abs(r)) / (R - 1 + r);
    }
    static Vt _limiter_WAHYD(Vt r, Vt R) {
        if (r <= 1.0) {
            return 0.5 * R * (r + std::abs(r)) / (R - 1 + r);
        } else {
            return std::min((r + R * r * std::abs(r)) / (R + r * r), R);
        }
    }
    // Piece-wise Linear Steady-State Limiter
    static Vt _limiter_BSOU(Vt r, Vt R){
        return std::max(0.0, std::min(2.0 * r, 1.0));
    }
    static Vt _limiter_Superbee(Vt r, Vt R) {
        return std::max(std::max(0.0, std::min(R * r, 1.0)), std::min(r, R));
    }
    static Vt _limiter_Minmod(Vt r, Vt R){
        return std::max(0.0, std::min(r, 1.0));
    }
    static Vt _limiter_MUSCL(Vt r, Vt R){
        return std::max(0.0, std::min(std::min(2.0 * r, (r + 1.0) * 0.5), 2.0));
    }
    static Vt _limiter_Koren(Vt r, Vt R){
        return std::max(0.0, std::min(std::min(2.0 * r, (2.0 * r + 1.0) / 3.0), 2.0));
    }
    static Vt _limiter_WACEB(Vt r, Vt R){
        return std::max(0.0, std::min(std::min(2.0 * r, (3.0 * r + 1.0) / 4.0), 2.0));
    }
    static Vt _limiter_UMIST(Vt r, Vt R){
        Vt m1 = std::min(2.0 * r, (3.0 * r + 1.0) / 4.0);
        Vt m2 = std::min(m1, (r + 3.0) / 4.0);
        return std::max(0.0, std::min(m2, 2.0));
    }
    static Vt _limiter_SPLmax12(Vt r, Vt R){
        Vt m1 = std::max((3.0 * r + 1.0)/ 4.0, (r + 3.0) / 4.0);
        Vt m2 = std::min(m1, 2.0 * r);
        return std::max(0.0, std::min(m2, 2.0));
    }
    static Vt _limiter_SPL13(Vt r, Vt R){
        Vt m1 = std::min((2.0 * r + 1.0)/ 3.0, (r + 2.0) / 3.0);
        Vt m2 = std::min(m1, 2.0 * r);
        return std::max(0.0, std::min(m2, 2.0));
    }
    static Vt _limiter_SPLmax13(Vt r, Vt R){
        Vt m1 = std::min((2.0 * r + 1.0) / 3.0, (r + 2.0) / 3.0);
        Vt m2 = std::min(m1, 2.0 * r);
        return std::max(0.0, std::min(m2, 2.0));
    }
    // Smooth SS-TVD
    static Vt _limiter_Harmonic(Vt r, Vt R){
        return (r + std::abs(r)) / (r + 1.0);
    }
    // Symmetric polynomial-ratio (SPR) schemes
    static Vt _limiter_OSPER(Vt r, Vt R){
        return 3.0 * r * (r + 1.0) / 2.0 / (r * r + r + 1.0);
    }
    static Vt _limiter_Albada(Vt r, Vt R){
        return r * (r + 1.0) / (r * r + 1.0);
    }

    static Vt _limiter_GVA12(Vt r, Vt R){
        return r * (r + 2.0) / (r * r + 2.0);
    }
    static Vt _limiter_GVA13(Vt r, Vt R){
        return r * (r + 3.0) / (r * r + 3.0);
    }
    // Generalized polynomial-ratio (GPR) schemes
    static Vt _limiter_GPR0(Vt r, Vt R){
        return r * (3.0 * r + 1.0) / (2.0 * r * r + r + 1.0);
    }
    static Vt _limiter_GPR12(Vt r, Vt R){
        return 2.0 * r * ( r + 1.0) / (r * r + r + 2.0);
    }
    static Vt _limiter_GPR13(Vt r, Vt R){
        return r * ( 2.0 * r + 1.0) / (r * r + r + 1.0);
    }
    
};

Limiter* Limiter::pinstance_{nullptr};
std::mutex Limiter::mutex_;

Limiter *Limiter::Get()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Limiter();
    }
    return pinstance_;
}

template<class VECTORF, class FIELD, class BI>
FIELD _UdotNablaTVD(const VECTORF& U, const FIELD& phi, 
                    const BI& bi, Vt t, Limiter::Fun lim, 
                    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, DimTag){
    EXPAND_FIELD_TAG(FIELD);
    EXPAND_FIELD(FIELD);

    Field res = phi.new_compatible_zero();
    for (auto &idx : phi.order())
    {
        std::array<Vt, Field::Dim> arr;
        arr.fill(0.0);

        for(auto& d : ArrAxes<Field::Dim>()) {
            arr[d] = _LocalOneAxeTVD(idx, d, U, phi, bi, t, lim, 
                ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
        }

        ValueType sum = 0;
        for(auto& d : ArrAxes<Field::Dim>()) {
            sum += arr[d];
        }
        res(idx) = sum;
    }

    return res;
}

template<class VECTORF, class FIELD, class BI>
auto _LocalOneAxeTVD(const typename FIELD::Index& idx, const Axes& d, 
                      const VECTORF& U, const FIELD& phi, 
                      const BI& bi, Vt t, Limiter::Fun lim,
                      ArithmeticTag, SGridTag, SGhostTag, OrderBaseTag, DimTag){
    EXPAND_FIELD(FIELD);

    auto& grid = phi.grid();

    auto idxp = idx.p(d);
    auto idxm = idx.m(d);
    auto up = U(d, _P_, idx);
    auto um = U(d, _M_, idx);
    auto uc = (up + um) * 0.5;      //average velocity to center
    Index Cm,Um,Dm,Cp,Up,Dp;
    ValueType fp,fm,R,r;
    ValueType VDm, VCm, VUm, VDp, VCp, VUp;
    if (uc >= 0.0) {
        // fm ------------------
        Dm = idx;
        Cm = idxm;
        Um = idxm.m(d);
        // get value
        VDm = phi(idx);
        VCm = Value(phi, bi, idx, Cm, d, _M_, t);
        VUm = Value(phi, bi, idx, Um, d, _M_, t);
        // fp ------------------
        Dp = idxp;
        Cp = idx;
        Up = idxm;
        // get value
        VDp = Value(phi, bi, idx, Dp, d, _P_, t);
        VCp = VDm;
        VUp = VCm;
    } else if (uc < 0.0) {
        // fm ------------------
        Dm = idxm;
        Cm = idx;
        Um = idxp;
        // get value
        VDm = Value(phi, bi, idx, Dm, d, _M_, t);
        VCm = phi(idx);
        VUm = Value(phi, bi, idx, Um, d, _P_, t);
        // fp ------------------
        Dp = idx;
        Cp = idxp;
        Up = idxp.p(d);
        // get value
        VDp = VCm;
        VCp = VUm;
        VUp = Value(phi, bi, idx, Up, d, _P_, t);
    }
    // p -----------------------
    R = _RCD(phi, d, Cp, Dp);
    r = _rCD(phi, d, Up, Cp, Dp, VUp, VCp, VDp);
    fp = VCp + lim(r, R) / R * (VDp - VCp);
    // m ---------------------
    R = _RCD(phi, d, Cm, Dm);
    r = _rCD(phi, d, Um, Cm, Dm, VUm, VCm, VDm);
    fm = VCm + lim(r, R) / R * (VDm - VCm);
   
    return (fp -  fm) * uc / grid.s_(d, idx);
}

template<class FIELD>
Vt _rCD(const FIELD& phi, const Axes& d,
        const typename FIELD::Index&  U,
        const typename FIELD::Index&  C,
        const typename FIELD::Index&  D,
        const typename FIELD::ValueType& VU,
        const typename FIELD::ValueType& VC,
        const typename FIELD::ValueType& VD) {
    auto& grid = phi.grid();
    auto sU = grid.s_(d, U);
    auto sC = grid.s_(d, C);
    auto sD = grid.s_(d, D);
    return _rCD(VU,VC,VD,sU,sC,sD);
}
template<class FIELD>
Vt _RCD(const FIELD& phi, const Axes& d,
        const typename FIELD::Index&  C,
        const typename FIELD::Index&  D) {
    auto& grid = phi.grid();
    auto sC = grid.s_(d, C);
    auto sD = grid.s_(d, D);
    return _RCD(sC,sD);
}



}

#endif