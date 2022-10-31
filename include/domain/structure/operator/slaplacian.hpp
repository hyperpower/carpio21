#ifndef _SLAPLACIAN_HPP
#define _SLAPLACIAN_HPP

#include "domain/base/base_operator_impl.hpp"
#include "scommon.hpp"
#include "algebra/algebra.hpp"

#include "sapply_bc.hpp"

namespace carpio{

// -----------------------
// Linear Poly 

template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class IntLaplacianImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename GRID::Index>, 
    GRID, GHOST, ORDER, SFieldCenterTag>: 
    public SOperatorCommon_<FIELD, DIM, LinearPolynomial_<Vt, typename GRID::Index>, GRID, GHOST, ORDER>{     
public:
    typedef LinearPolynomial_<Vt, typename GRID::Index> Exp;
    typedef ApplyBCImplement_<FIELD, DIM, Exp, GRID, GHOST, ORDER, SFieldCenterTag> ApplyBC;
    typedef typename GRID::Index Index;

    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
public:
    IntLaplacianImplement_(){
        // std::cout << "IntLaplacian Exp Structure" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }

    FIELD execute(const FIELD& f) const{
        FIELD res(f);
        const auto& grid = res.grid();
        for (auto& idx : res.order()) {
            std::array<Exp, DIM> arr;
            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);
                Exp phi_m(idxm), phi_p(idxp);
                Exp phi(idx);
                auto dfdx_m = (phi - phi_m)
                                / (grid.c_(d, idx) - grid.c_(d, idxm));
                auto dfdx_p = (phi_p - phi)
                                / (grid.c_(d, idxp) - grid.c_(d, idx));

                arr[d] = (dfdx_p * grid.fa(d,_P_,idx) - dfdx_m * grid.fa(d, _M_, idx));
            }
            FOR_EACH_DIM
            {
                res(idx) += arr[d];
            }
        }

        return res;
    }
    
    FIELD execute(const FIELD& f, 
                  const BI& bi, 
                  const Vt& time = 0.0) const{
        FIELD res = this->execute(f); 

        ApplyBC applybc;
        applybc.execute(res, bi, time);

        return res;
    }
    // void set(const int& other){std::cout << "int" << std::endl;}
};


template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class IntLaplacianImplement_<
    FIELD, DIM, Vt,
    GRID, GHOST, ORDER, SFieldCenterTag>: 
    public SOperatorCommon_<FIELD, DIM, Vt, GRID, GHOST, ORDER>{     
public:
    typedef SFieldCenterTag Tag;
    typedef Vt ValueType;
    typedef ApplyBCImplement_<FIELD, DIM, Vt, GRID, GHOST, ORDER, SFieldCenterTag> ApplyBC;
    typedef typename GRID::Index Index;
    typedef FIELD Field;
    typedef GRID  Grid;
    
    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
public:
    IntLaplacianImplement_(){
        // std::cout << "IntLaplacian Exp Structure" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }
    FIELD execute(const FIELD& phi, const BI& bi, const Vt& time = 0.0) const{
        // std::cout << "Laplace Vt " << std::endl;
        ApplyBC abc;
        Field res        = phi.new_compatible();
        const Grid& grid = phi.grid();
        for (auto& idx : phi.order()) {
            std::array<Vt, DIM> arr;
            arr.fill(0.0);

            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);

                Vt dfdx_p, dfdx_m;
                Vt phi_m = abc.value(phi, bi, idx, idxm, d, _M_, time);
                Vt phi_p = abc.value(phi, bi, idx, idxp, d, _P_, time);
                dfdx_m = (phi(idx) - phi_m)
                        / (grid.c_(d, idx) - grid.c_(d, idxm));
                dfdx_p = (phi_p - phi(idx))
                        / (grid.c_(d, idxp) - grid.c_(d, idx));
                arr[d] = (dfdx_p * grid.fa(d,_P_,idx) - dfdx_m * grid.fa(d, _M_, idx));
            }

            Vt sum = 0;
            FOR_EACH_DIM
            {
                sum += arr[d];
            }
            res(idx) = sum;
        }

        return res;
    }
};


// ------------------------------------------------------
// Finite Difference Method
// ------------------------------------------------------
template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class LaplacianImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename GRID::Index>, 
    GRID, GHOST, ORDER, SFieldCenterTag>: 
    public SOperatorCommon_<FIELD, DIM, LinearPolynomial_<Vt, typename GRID::Index>, GRID, GHOST, ORDER>{     
public:
    typedef LinearPolynomial_<Vt, typename GRID::Index> Exp;
    typedef ApplyBCImplement_<FIELD, DIM, Exp, GRID, GHOST, ORDER, SFieldCenterTag> ApplyBC;
    typedef typename GRID::Index Index;

    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
public:
    LaplacianImplement_(){
        // std::cout << "IntLaplacian Exp Structure" << std::endl;
    };

    template<class ANY>
    void set(const ANY& other){}

    int set_method(const std::string& method){
        return 0;
    }

    FIELD execute(const FIELD& f) const{
        FIELD res(f);
        const auto& grid = res.grid();
        for (auto& idx : res.order()) {
            std::array<Exp, DIM> arr;
            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);
                Exp phi_m(idxm), phi_p(idxp);
                Exp phi(idx);
                auto dfdx_m = (phi - phi_m)
                                / (grid.c_(d, idx) - grid.c_(d, idxm));
                auto dfdx_p = (phi_p - phi)
                                / (grid.c_(d, idxp) - grid.c_(d, idx));

                arr[d] = (dfdx_p - dfdx_m) / (grid.s_(d, idx));
            }
            FOR_EACH_DIM
            {
                res(idx) += arr[d];
            }
        }

        return res;
    }
    
    FIELD execute(const FIELD& f, 
                  const BI& bi, 
                  const Vt& time = 0.0) const{
        FIELD res = this->execute(f); 

        ApplyBC applybc;
        applybc.execute(res, bi, time);

        return res;
    }
    // void set(const int& other){std::cout << "int" << std::endl;}
};

template<class FIELD, St DIM, class GRID, class GHOST, class ORDER>
class LaplacianImplement_<
    FIELD, DIM, Vt,
    GRID, GHOST, ORDER, SFieldCenterTag>: 
    public SOperatorCommon_<FIELD, DIM, Vt, GRID, GHOST, ORDER>{     
public:
    typedef SFieldCenterTag Tag;
    typedef Vt ValueType;
    typedef ApplyBCImplement_<FIELD, DIM, Vt, GRID, GHOST, ORDER, SFieldCenterTag> ApplyBC;
    typedef typename GRID::Index Index;
    typedef FIELD Field;
    typedef GRID  Grid;
    
    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
public:
    LaplacianImplement_(){
        // std::cout << "IntLaplacian Exp Structure" << std::endl;
    };

    int set_method(const std::string& method){
        return 0;
    }
    FIELD execute(const FIELD& phi, const BI& bi, const Vt& time = 0.0) const{
        // std::cout << "Laplace Vt " << std::endl;
        ApplyBC abc;
        Field res        = phi.new_compatible();
        const Grid& grid = phi.grid();
        for (auto& idx : phi.order()) {
            std::array<Vt, DIM> arr;
            arr.fill(0.0);

            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);

                Vt dfdx_p, dfdx_m;
                Vt phi_m = abc.value(phi, bi, idx, idxm, d, _M_, time);
                Vt phi_p = abc.value(phi, bi, idx, idxp, d, _P_, time);
                dfdx_m = (phi(idx) - phi_m)
                        / (grid.c_(d, idx) - grid.c_(d, idxm));
                dfdx_p = (phi_p - phi(idx))
                        / (grid.c_(d, idxp) - grid.c_(d, idx));
                arr[d] = (dfdx_p - dfdx_m ) / grid.s_(d, idx);
            }

            Vt sum = 0;
            FOR_EACH_DIM
            {
                sum += arr[d];
            }
            res(idx) = sum;
        }

        return res;
    }
};

//   d^4 phi       d^4 phi       d^4 phi
// ----------- + ----------- + -----------
//  dx^2 dy^2     dy^2 dz^2     dz^2 dx^2    
template<class FIELD, St DIM, class GHOST, class ORDER>
class L4AlterImplement_<
    FIELD, DIM, 
    LinearPolynomial_<Vt, typename SGridUniform_<DIM>::Index>, 
    SGridUniform_<DIM>, 
    GHOST, ORDER, SFieldCenterTag>: public SOperatorCommon_<FIELD, DIM, Vt, SGridNonUniform_<DIM>, GHOST, ORDER>{         
public:
    typedef SFieldCenterTag Tag;
    typedef Vt ValueType;
    typedef SGridUniform_<DIM> Grid;
    typedef LinearPolynomial_<Vt, typename Grid::Index> Exp;
    typedef ApplyBCImplement_<FIELD, DIM, Vt, Grid, GHOST, ORDER, SFieldCenterTag> ApplyBC;
    typedef typename Grid::Index Index;
    typedef FIELD Field;
    
    typedef BoundaryIndex BI;
    typedef std::shared_ptr<BI> spBI;
public:
    L4AlterImplement_(){
        std::cout << "L4Alter" << std::endl;
    };
    FIELD execute(const FIELD& f, const BI& bi, const Vt& time = 0.0) const{
        // std::cout << "Laplace Vt " << std::endl;
        FIELD res(f);
        const auto& grid = res.grid();
        for (auto& idx : res.order()) {
            std::array<Exp, DIM> arr;
            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);
                Exp phi_m(idxm), phi_p(idxp);
                Exp phi(idx);
                auto dfdx_m = (phi - phi_m)
                                / (grid.c_(d, idx) - grid.c_(d, idxm));
                auto dfdx_p = (phi_p - phi)
                                / (grid.c_(d, idxp) - grid.c_(d, idx));

                arr[d] = (dfdx_p - dfdx_m) / (grid.s_(d, idx));
            }
            FOR_EACH_DIM
            {
                res(idx) += arr[d];
            }
        }

        return res;
    }
};

}


#endif