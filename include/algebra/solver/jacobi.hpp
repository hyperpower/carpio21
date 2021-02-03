#ifndef _SOLVER_JACOBI_HPP_
#define _SOLVER_JACOBI_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/blas/blas_1.hpp"
#include "solver_define.hpp"

#include <list>

namespace carpio{

template<class VALUE>
class Jacobi_: public Solver_<VALUE> {
public:
	typedef VALUE Vt;
	typedef typename ArrayListV_<Vt>::size_type St;
	typedef MatrixSCR_<VALUE> MatSCR;
	typedef MatrixV_<VALUE>   Mat;
	typedef ArrayListV_<VALUE> Arr;
	typedef std::list<double> Listr;

	Jacobi_(int max_iter = 100, Vt tol = 1e-6) :
			Solver_<VALUE>(max_iter, tol) {
	}

	int solve(const MatSCR &A, // A  The matrix
			        Arr &x,    // x
			  const Arr& b     // b
			) {
		this->_init();
		return this->_solve(A, x, b);
	}

	int solve(
				const Mat& A,    // dense matrix
				      Arr &x,    // x
				const Arr& b
				){
		this->_init();
		return this->_solve(A, x, b);
	}

protected:
	int _solve(              //
			const MatSCR &A, // A  The matrix
			Arr &x,          // x
			const Arr& b     // b
			) {
		Vt resid;

		St n = b.size();
		MatSCR T(A);
		Arr C(n, 1.0 / SMALL);
		Arr newx(n);

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
#pragma omp parallel for
		for (int i = 0; i < M; ++i) {
			// find D
			Vt dv;
			int flag = -1;
			for (St j = T.row_ptr(i); j < T.row_ptr(i + 1); ++j) {
				if (T.col_ind(j) == i) {
					flag = j;
					dv = T.val(j);
					T.val(j) = 0;
					break;
				}
			}
			for (St j = T.row_ptr(i); j < T.row_ptr(i + 1); ++j) {
				if (j == St(flag)) {
					C(i) = b(T.col_ind(j)) / dv;
				} else {
					T.val(j) = -T.val(j) / dv;
				}
			}
		}

		for (int i = 1; i <= this->_max_iter; ++i) {
			//----
			newx = T * x + C;
			r = b - A * newx;
			//----
			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid <= this->_tol) {
				this->_residual = resid;
				this->_num_iter = i;
				return 0;
			}
			x = newx;
		}

		this->_residual = resid;
		this->_num_iter = this->_max_iter;
		return 1;
	}


	/*
	* Runs the Jacobi method for A*x = b.
	*
	* ON ENTRY :
	* A an n-by-n matrix A[i][i] is not 0;
	* x start vector for the iteration and the return vector
	* b an n-dimensional vector;
	*
	* ON RETURN :
	* The status of the solver */
	int _solve(
			const Mat &A,     // A  The matrix
			      Arr &x,     // x
			const Arr& b     )// b
			{
		Vt resid;

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

		Arr newx(x.size());
		Arr dx(x.size());
		St  n = x.size();
		for (int iter = 1; iter <= this->_max_iter; ++iter) {
			Vt sum = 0.0;
			for (St i = 0; i < n; i++) {
				dx[i] = b[i];
				for (St j = 0; j < n; j++){
					dx[i] -= A[i][j] * x[j];
				}
				dx[i]   /= A[i][i] + SMALL;
				newx[i] += dx[i];
			}
			// critic
			r = b - A * newx;
			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid <= this->_tol) {
				this->_residual = resid;
				this->_num_iter = iter;
				return 0;
			}
			// copy
			x = newx;
		}
		return 1;
	}
};


}

#endif
