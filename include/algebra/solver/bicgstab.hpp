#ifndef _SOLVER_BICGSTAB_HPP_
#define _SOLVER_BICGSTAB_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/blas/blas_1.hpp"
#include "solver_define.hpp"
#include "preconditioner_ic.hpp"

#include <list>

namespace carpio{
// BiCGSTAB solves the unsymmetric linear system Ax = b
// using the Preconditioned BiConjugate Gradient Stabilized method

template<class VALUE>
class BiCGSTAB_: public Solver_<VALUE> {
public:
	typedef VALUE Vt;
	typedef typename ArrayListV_<Vt>::size_type St;
	typedef MatrixSCR_<VALUE> MatSCR;
	typedef MatrixV_<VALUE>   Mat;
	typedef ArrayListV_<VALUE> Arr;
	typedef std::list<double> Listr;
	typedef Preconditioner_<VALUE> Preconditioner;
	typedef PreconditionerIC_<VALUE> PreIC;
	typedef PreconditionerDiagonal_<VALUE> PreDia;
protected:
	std::string _strpre;

public:
	BiCGSTAB_(int max_iter = 100, Vt tol = 1e-6) :
		Solver_<VALUE>(max_iter, tol), _strpre("None"){
	}

	BiCGSTAB_(const std::string& prename, int max_iter = 100, Vt tol = 1e-6) :
		Solver_<VALUE>(max_iter, tol), _strpre(prename){
	}

	int solve(const MatSCR &A, // A  The matrix
			        Arr &x,    // x
			  const Arr& b     // b
				) {
		this->_init();
		if (_strpre == "None"){
			PreDia pre(A, 2);
			return this->_solve(&pre, A, x, b);
		}
		if (_strpre == "Dia"){
			PreDia pre(A, 1);
			return this->_solve(&pre, A, x, b);
		}
		if (_strpre == "IC"){
			PreIC pre(A);
			return this->_solve(&pre, A, x, b);
		}
		return 0;
	}

protected:
	int _solve(Preconditioner* preA,
			   const MatSCR &A,     // A  The matrix
					    Arr &x,     // x
			      const Arr& b     // b
			) {
//		PreDia preA(A, dia);
		Vt resid;
		Vt rho_1, rho_2, alpha, beta, omega;
		Arr p, phat, s, shat, t, v;

		Vt normb = Nrm2(b);
		Arr r = b - A * x;
		Arr rtilde = r;

		if (normb == 0.0)
			normb = 1;

		if ((resid = Nrm2(r) / normb) <= this->_tol) {
			this->_residual = resid;
			this->_num_iter = 0;
			return 0;
		}

		for (int i = 1; i <= this->_max_iter; i++) {
			rho_1 = Dot(rtilde, r);          //dot(v,v)
			if (rho_1 == 0) {
				resid = Nrm2(r) / normb;
				this->_lresid.push_back(resid);
				this->_residual = resid;
				this->_num_iter = i;
				return 2;
			}
			if (i == 1)
				p = r;                       // v=v
			else {
				beta = (rho_1 / rho_2) * (alpha / omega);   // s
				p    = r + beta * (p - omega * v);          // v + s*(v-s*v)
			}
			phat  = preA->solve(p);
			v     = A * phat;
			alpha = rho_1 / Dot(rtilde, v);
			s     = r - alpha * v;
			if ((resid = Nrm2(s) / normb) < this->_tol) {
				x += alpha * phat;
				this->_tol = resid;
				return 0;
			}
			shat  = preA->solve(s);
			t     = A * shat;
			omega = Dot(t, s) / Dot(t, t);
			x    += alpha * phat + omega * shat;
			r     = s - omega * t;

			rho_2 = rho_1;

			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid < this->_tol) {
				this->_residual = resid;
				this->_num_iter = i;
				return 0;
			}

			if (omega == 0) {
				resid = Nrm2(r) / normb;
				this->_lresid.push_back(resid);
				this->_residual = resid;
				this->_num_iter = i;
				return 3;
			}
		}

		this->_residual = resid;
		this->_num_iter = this->_max_iter;
		return 1;

	}
};


}

#endif
