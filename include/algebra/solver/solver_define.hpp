#ifndef _SOLVER_DEFINE_HPP_
#define _SOLVER_DEFINE_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/matrix/matrix.hpp"

#include <list>

namespace carpio{

template<class VALUE>
class Solver_ {
public:
	typedef VALUE Vt;
	typedef typename ArrayListV_<Vt>::size_type St;
	typedef MatrixSCR_<Vt> MatSCR;
	typedef MatrixV_<Vt>   Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef std::list<double> Listr;
protected:
	int _max_iter;   //max_iter
	double _tol;     // Tolerance

	int    _num_iter; // number of iteration
	double _residual; // residual
	Listr  _lresid;   // list of residual
public:

	Solver_(int max_iter = 100, Vt tol = 1e-6) {
		_max_iter = max_iter;
		_tol = tol;

		_num_iter = 0;
		_residual = 1e10;
	}

	virtual int solve(
			const MatSCR &A, // A  The matrix (compress row)
			      Arr &x,    // x
			const Arr& b     // b
			) {
		SHOULD_NOT_REACH;
		return 0;
	}

	virtual int solve(
			const Mat&,    // dense matrix
			      Arr&,    // x
			const Arr&){
		SHOULD_NOT_REACH;
		return 0;
	}



	int max_iter() const {
		return this->_max_iter;
	}

	int tolerance() const {
		return this->_tol;
	}

	int num_iter() const {
		return this->_num_iter;
	}

	double residual() const {
		return this->_residual;
	}

	Arr get_residual_array() const{
		Arr res(this->_lresid.size());
		St i = 0;
		for(auto& val : this->_lresid){
			res[i] = val;
			++i;
		}
		return res;
	}

	virtual ~Solver_() {
	}
	;
protected:
	void _init() {
		this->_lresid.clear();
	}

};



}

#endif
