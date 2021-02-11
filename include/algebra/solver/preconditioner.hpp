/************************
 //  \file   ICpre.h
 //  \brief
 //
 //  \author zhou
 //  \date   12 mai 2014
 ***********************/
#ifndef _SOVLER_PRECONDITIONER_HPP_
#define _SOVLER_PRECONDITIONER_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/misc/sort.hpp"
#include "algebra/array/array_list.hpp"

namespace carpio
{

template<class VALUE>
class Preconditioner_{
public:
	typedef VALUE              Vt;
	typedef MatrixV_<VALUE>    Matrix;
	typedef MatrixSCR_<VALUE>  MatrixSCR;
	typedef ArrayListV_<VALUE> Array;
	typedef ArrayListV_<St>    ArraySt;
protected:

public:
	Preconditioner_() {}

	virtual ~Preconditioner_(){};

	virtual Array solve(const Array &x) const{
		SHOULD_NOT_REACH;
		return Array();
	}
	virtual Array trans_solve(const Array &x) const{
		SHOULD_NOT_REACH;
		return Array();
	}
};









}

#endif
