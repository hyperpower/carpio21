#ifndef _SOLVER_SOR_HPP_
#define _SOLVER_SOR_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/blas/blas_1.hpp"
#include "solver_define.hpp"

#include <list>

namespace carpio{

template<class VALUE>
class SOR_: public Solver_<VALUE> {
public:
	typedef VALUE Vt;
	typedef MatrixSCR_<Vt> Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef std::list<double> Listr;
protected:
	Vt _omega;
public:
	SOR_(int max_iter = 100, Vt tol = 1e-3, Vt omega = 1.0) :
			Solver_<VALUE>(max_iter, tol) {
		_omega = omega;
	}

	int solve(const Mat &A, // A  The matrix
			Arr &x,          // x
			const Arr& b    // b
			) {
		this->_init();
		return this->_solve(A, x, b);
	}

	int _solve( //
			const Mat &A, // A  The matrix
			      Arr &x,          // x
			const Arr& b    // b
			) {
		Vt resid;

		St n = b.size();
		Mat T(A);
		Arr C(n, 1.0 / SMALL);
		//Arr newx(n);

		Vt normb = Nrm2(b);
		Arr r = b - A * x;
		//
		if (normb == 0.0)
			normb = 1;

		if ((resid = Nrm2(r) / normb) <= this->_tol) {
			this->_residual = resid;
			this->_num_iter = 0;
			return 0;
		}

		// construct T
		St M = T.size_i();

		for (int i = 1; i <= this->_max_iter; ++i) {
			//----
#pragma omp parallel for
			for (St i = 0; i < M; ++i) {  //row
				Vt sumkp = 0;
				Vt sumk = 0;
				Vt aa = 1.0;
				for (St idx = T.row_ptr(i); idx < T.row_ptr(i + 1); ++idx) {
					St j = T.col_ind(idx);
					if (j < i) {
						sumkp += T.val(idx) * x[j];
					} else if (j > i) {
						sumk  += T.val(idx) * x[j];
					} else {
						aa = T.val(idx);
					}
				}
				x[i] = (1 - this->_omega) * x[i]
						+ (this->_omega / aa) * (b[i] - sumkp - sumk);
			}
			r = b - A * x;
			//----
			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid <= this->_tol) {
				this->_residual = resid;
				this->_num_iter = i;
				return 0;
			}
		}

		this->_residual = resid;
		this->_num_iter = this->_max_iter;
		return 1;
	}
};




}

#endif
