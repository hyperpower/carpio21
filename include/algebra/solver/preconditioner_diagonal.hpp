#ifndef _SOVLER_PRECONDITIONER_DIAGONAL_HPP_
#define _SOVLER_PRECONDITIONER_DIAGONAL_HPP_

#include "algebra/algebra_define.hpp"
#include "preconditioner.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/misc/sort.hpp"
#include "algebra/array/array_list.hpp"

namespace carpio
{

template<class VALUE>
class PreconditionerDiagonal_: public Preconditioner_<VALUE>{
public:
	typedef VALUE              Vt;
	typedef MatrixV_<VALUE>    Matrix;
	typedef MatrixSCR_<VALUE>  MatrixSCR;
	typedef ArrayListV_<VALUE> Array;
	typedef ArrayListV_<St>    ArraySt;
private:
	Array diag_;
	int   inv_;   //if inv_==1  diag_= 1. / M.val(i,j);
			      //else        diag_= M.val(i,j);

public:
	//DiaPre (const CompCol_Mat_double &);
	//DiaPre(const Matrix &M, int inv);
	PreconditionerDiagonal_(const MatrixSCR &M, int inv = 1) :
			diag_(M.size_i()) {
		inv_ = inv;
		int flagi = -1;
		/* Find the diagonal elements */
		for (St i = 0; i < M.size_i(); i++) {
			for (St j = M.row_ptr(i); j < M.row_ptr(i + 1); j++) {
				if (M.col_ind(j) == i) {
					if (M.val(j) == 0.0) {
						flagi = i;
						break;
					} else {
						if (inv_ == 1) {
							diag_[i] = 1. / (M.val(j));
						} else {
							diag_[i] = M.val(j);
						}
						break;
					}
				}
			}
			if (diag_[i] == 0) {
				flagi = i;
				break;
			}
		}

		if (flagi != -1) {
			std::cerr << " >! Diagonal preconditioner failure.";
			std::cerr << " >! Zero detected  \n";
			exit(0);
		}
	}
	~PreconditionerDiagonal_()
	{
	}
	;

	Array solve(const Array &x) const
	{
		Array y(x.size());

		for (St i = 0; i < x.size(); i++)
			y[i] = x[i] * diag_[i];

		return y;
	}
	Array trans_solve(const Array &x) const
	{
		Array y(x.size());

		for (int i = 0; i < x.size(); i++)
			y[i] = x[i] * diag_[i];

		return y;
	}

	const Vt& diagonal(St i) const
	{
		return diag_[i];
	}

	Vt& diagonal(St i)
	{
		return diag_[i];
	}

};



}

#endif
