#ifndef _SOLVER_CG_HPP_
#define _SOLVER_CG_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/blas/blas_1.hpp"
#include "solver_define.hpp"
#include "preconditioner_ic.hpp"

#include <list>

namespace carpio{
// CG solves the symmetric positive definite linear
// system Ax=b using the Conjugate Gradient method.

template<class VALUE>
class CG_: public Solver_<VALUE> {
public:
	typedef VALUE Vt;
	typedef typename ArrayListV_<Vt>::size_type St;
	typedef MatrixSCR_<VALUE> MatSCR;
	typedef MatrixV_<VALUE>   Mat;
	typedef ArrayListV_<VALUE> Arr;
	typedef std::list<double> Listr;
	typedef Preconditioner_<VALUE> Preconditioner;
	typedef PreconditionerIC_<VALUE> PreIC;
protected:
	std::string _strpre;
public:

	CG_(int max_iter = 100, Vt tol = 1e-6) :
			Solver_<VALUE>(max_iter, tol), _strpre("None"){
	}

	CG_(const std::string& prename, int max_iter = 100, Vt tol = 1e-6) :
			Solver_<VALUE>(max_iter, tol), _strpre(prename) {
	}

	int solve(const MatSCR &A, // A  The matrix
			        Arr &x,    // x
			  const Arr& b     // b
			) {
		this->_init();
		if(_strpre == "None")
			return this->_solve(A, x, b);
		if(_strpre == "IC")
			return this->_solve_IC(A, x, b);
		return 0;
	}

	int solve(
				const Mat& A,    // dense matrix
				      Arr &x,    // x
				const Arr& b
				){
		SHOULD_NOT_REACH;
		return 0;
	}

protected:
	int _solve(              //
			const MatSCR &A, // A  The matrix
			Arr &x,          // x
			const Arr& b     // b
			) {
		Vt resid;
		Arr p(b.size()), z(b.size()), q(b.size());
		Vt alpha, beta, rho, rho_1;

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

		for (int i = 1; i <= this->_max_iter; ++i) {
			z = r;           //z = M.solve(r);   ===!!!!!
			rho = Dot(r, z); //rho(0) = dot(r, z);

			if (i == 1)
				p = z;
			else {
				beta = rho / rho_1; //beta(0) = rho(0) / rho_1(0);
				p = z + beta * p;   //p = z + beta(0) * p;
			}

			q = A * p;
			alpha = rho / Dot(p, q);  //alpha(0) = rho(0) / dot(p, q);
			x += alpha * p;           //x += alpha(0) * p;
			r -= alpha * q;           //r -= alpha(0) * q;

			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid <= this->_tol) {
				this->_residual = resid;
				this->_num_iter = i;
				return 0;
			}
			rho_1 = rho;            //rho_1(0) = rho(0);
		}

		this->_residual = resid;
		this->_num_iter = this->_max_iter;
		return 1;
	}

	int _solve_IC(              //
				const MatSCR &A, // A  The matrix
				Arr &x,          // x
				const Arr& b     // b
			) {
		Vt resid;
		Arr p(b.size()), z(b.size()), q(b.size());
		Vt alpha, beta, rho, rho_1;

		PreIC preA(A);

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

		for (int i = 1; i <= this->_max_iter; ++i) {
			z = preA.solve(r);     //z = M.solve(r);   ===!!!!!
			rho = Dot(r, z);       //rho(0) = dot(r, z);

			if (i == 1)
				p = z;
			else {
				beta = rho / rho_1; //beta(0) = rho(0) / rho_1(0);
				p = z + beta * p;   //p = z + beta(0) * p;
			}

			q = A * p;
			alpha = rho / Dot(p, q);  //alpha(0) = rho(0) / dot(p, q);
			x += alpha * p;           //x += alpha(0) * p;
			r -= alpha * q;           //r -= alpha(0) * q;

			resid = Nrm2(r) / normb;
			this->_lresid.push_back(resid);
			if (resid <= this->_tol) {
				this->_residual = resid;
				this->_num_iter = i;
				return 0;
			}
			rho_1 = rho;            //rho_1(0) = rho(0);
		}

		this->_residual = resid;
		this->_num_iter = this->_max_iter;
		return 1;
	}
};




}

#endif
