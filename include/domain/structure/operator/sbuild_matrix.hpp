#ifndef _SBUILD_MATRIX_HPP
#define _SBUILD_MATRIX_HPP

#include "scommon.hpp"
#include "algebra/algebra.hpp"
#include "domain/boundary/boundary_index.hpp"



namespace carpio{

template<class FIELD>
void _BuildMatrix(const FIELD& expf, 
                  MatrixSCR_<Vt>& mat, ArrayListV_<Vt>& b,
                  SFieldCenterTag, LinearPolynomialTag){
    auto &grid = expf.grid();
    auto &order = expf.order();
    St n = order.size();

    typedef std::list<St> ListST;
    // Loop 1 ================
    //   1 get size of none zero
    //   2 build b
    b.reconstruct(n);
    std::size_t nz = 0;
    for (auto &index : order){
        auto &exp = expf(index);
        auto nr = exp.size();
        nz += nr;
        auto row = order.get_order(index);
        b[row] = -(exp.value()); // negative is here !!!
    }
    // Loop 2 ================
    //   1 build matrix
    mat.newsize(n, n, nz);
    mat.row_ptr(0) = 0;
    for (auto &index : order){
        auto &exp = expf(index);
        St row = order.get_order(index);
        St nr = exp.size();
        mat.row_ptr(row + 1) = mat.row_ptr(row) + nr;
        // Inner Loop ---------------------
        St ii = mat.row_ptr(row);
        auto iter = exp.begin();
        for (; ii < mat.row_ptr(row + 1);){
            mat.col_ind(ii) = order.get_order(iter->first);
            mat.val(ii) = iter->second;
            ii++;
            iter++;
        }
    }
}



}

#endif