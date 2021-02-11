#ifndef _SOLVER_BICG_HPP_
#define _SOLVER_BICG_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/blas/blas_1.hpp"
#include "solver_define.hpp"
#include "preconditioner_ic.hpp"

#include <list>

namespace carpio{
// BiCG solves the unsymmetric linear system Ax = b
// using the Preconditioned BiConjugate Gradient method

template<class VALUE>
class BiCG_: public Solver_<VALUE> {
public:
	typedef VALUE Vt;
	typedef typename ArrayListV_<Vt>::size_type St;
	typedef MatrixSCR_<VALUE> MatSCR;
	typedef MatrixV_<VALUE>   Mat;
	typedef ArrayListV_<VALUE> Arr;
	typedef std::list<double> Listr;
	typedef Preconditioner_<VALUE> Preconditioner;
	typedef PreconditionerIC_<VALUE> PreIC;

	BiCG_(int max_iter = 100, Vt tol = 1e-6) :
			Solver_<VALUE>(max_iter, tol) {
	}

	int solve(const MatSCR &A, // A  The matrix
			        Arr &x,    // x
			  const Arr& b     // b
				) {
		this->_init();
		return this->_solve_IC(A, x, b);
		return 0;
	}

protected:
	int _solve_IC(const MatSCR &A, // A  The matrix
					    Arr &x,          // x
				  const Arr& b     // b
			) {
		PreIC preA(A);
		Vt resid;
		Vt rho_1, rho_2, alpha, beta;
		Arr z, ztilde, p, ptilde, q, qtilde;

		Vt normb = Nrm2(b);
		Arr r(b - A * x);
		Arr rtilde = r;

		if (normb == 0.0)
			normb = 1;

		if ((resid = Nrm2(r) / normb) <= this->_tol) {
			this->_residual = resid;
			this->_num_iter = 0;
			return 0;
		}

		for (int i = 1; i <= this->_max_iter; i++) {
			z      = preA.solve(r);
			ztilde = preA.trans_solve(rtilde);
			rho_1  = Dot(z, rtilde);
			if (rho_1 == 0) {
				resid = Nrm2(r) / normb;
				this->_lresid.push_back(resid);
				this->_residual = resid;
				this->_num_iter = i;
				return 2;
			}
			if (i == 1) {
				p = z;
				ptilde = ztilde;
			} else {
				beta = rho_1 / rho_2;
				p = z + beta * p;
				ptilde = ztilde + beta * ptilde;
			}
			q       = A * p;
			qtilde  = A.trans_mult(ptilde);
			alpha   = rho_1 / Dot(ptilde, q);
			x      += alpha * p;
			r      -= alpha * q;
			rtilde -= alpha * qtilde;

			rho_2 = rho_1;

			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid < this->_tol) {
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
