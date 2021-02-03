#ifndef _SOVLER_PRECONDITIONER_IC_HPP_
#define _SOVLER_PRECONDITIONER_IC_HPP_

#include "algebra/algebra_define.hpp"
#include "preconditioner.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/misc/sort.hpp"
#include "algebra/array/array_list.hpp"

namespace carpio
{

template<class VALUE>
class PreconditionerIC_ : public Preconditioner_<VALUE>{
public:
	typedef VALUE             Vt;
	typedef Preconditioner_<VALUE> Base;
	typedef MatrixV_<VALUE>   Mat;
	typedef MatrixSCR_<VALUE> MatSCR;
	typedef ArrayListV_<VALUE> Arr;
	typedef ArrayListV_<St>    ArrSt;
protected:
	Arr   val_;
	ArrSt pntr_;
	ArrSt indx_;
	int nz_;
	int dim_[2];
public:
	PreconditionerIC_(const Mat &A) :
			val_(0), pntr_(A.size_i() + 1), indx_(0), nz_(0){
		dim_[0] = A.size_i();
		dim_[1] = A.size_j();
		//Count non-zero number in upper triangular part (including diagonal)
		for (int i = 0; i < dim_[0]; i++) {
			for (int j = i; j < dim_[1]; j++) {
				if (A[i][j] != 0.0)
					nz_++;
			}
		}
		val_.reconstruct(nz_);
		indx_.reconstruct(nz_);
		pntr_[0] = 0;
		for (int i = 0; i < dim_[0]; i++) {
			pntr_[i + 1] = pntr_[i];
			for (int j = i; j < dim_[1]; j++) {
				if (A[i][j] != 0.0) {
					int k = pntr_[i + 1]++;
					val_[k] = A[i][j];
					indx_[k] = j;
				}
			}
		}
		_IC_factor(pntr_, indx_, val_);
	}

	PreconditionerIC_(const MatSCR &A) :
			val_(0), pntr_(A.size_i() + 1), indx_(0), nz_(0) {
		dim_[0] = A.size_i();
		dim_[1] = A.size_j();

		int i, j, k;
		//Count non-zero number in upper triangular part (including diagonal)
		for (k = 0; k < dim_[0]; k++) {
			for (j = A.row_ptr(k); j < A.row_ptr(k + 1); j++) {
				if (A.col_ind(j) >= k)
					nz_++;
			}
		}

		val_.reconstruct(nz_);
		indx_.reconstruct(nz_);

		// Copy just triangular part (including diagonal)
		pntr_[0] = 0;
		for (k = 0; k < dim_[0]; k++) {
			pntr_[k + 1] = pntr_[k];
			for (j = A.row_ptr(k); j < A.row_ptr(k + 1); j++) {
				if (A.col_ind(j) >= k) {
					i = pntr_[k + 1]++;
					val_[i] = A.val(j);
					indx_[i] = A.col_ind(j);
				}
			}
		}

		for (i = 0; i < dim_[0]; i++){
			QSort_ascend(indx_, val_, pntr_[i], pntr_[i + 1] - pntr_[i]);
		}
		for (i = 0; i < dim_[0]; i++) {
			if (indx_[pntr_(i)] != i) {
				std::cerr << "IC Preconditioner: diagonal not found!" << i
						<< "\n";
				exit(1);
			}
		}
		_IC_factor(pntr_, indx_, val_);
	}


	Arr solve(const Arr &x) const {
		Arr y(x);
		for (int i = 0; i < dim_[1]; i++) {
			Float z = y[i] / val_[pntr_[i]];
			y[i] = z;
			for (int j = pntr_[i] + 1; j < pntr_[i + 1]; j++) {
				y[indx_[j]] -= z * val_[j];
			}
		}
		for (int i = dim_[0] - 1; i >= 0; i--) {
			Float z = 0;
			for (int j = pntr_[i] + 1; j < pntr_[i + 1]; j++) {
				z += y[indx_[j]] * val_[j];
			}
			y[i] = (y[i] - z) / val_[pntr_[i]];
		}
		return y;
	}

	Arr trans_solve(const Arr &x) const {
		return this->solve(x);
	}

	MatSCR get_preconditioner_mat() {
		MatSCR res(dim_[0], dim_[1], nz_, val_, pntr_, indx_);
		return res;
	}

	MatSCR get_preconditioner_mat_trans() {
		MatSCR res(dim_[0], dim_[1], nz_, val_, pntr_, indx_);
		res.trans();
		return res;
	}

	~PreconditionerIC_(){};
protected:


	void _IC_factor(
			const ArrSt &pntr,
			const ArrSt &indx,
			      Arr &val){

		int d, g, h, i, j, k, n = pntr.size() - 1;
		Vt z;

		for (k = 0; k < n - 1; k++) {
			d = pntr[k];
			z = val[d] = std::sqrt(val[d]);

			for (i = d + 1; i < pntr[k + 1]; i++)
				val[i] /= z;

			for (i = d + 1; i < pntr[k + 1]; i++) {
				z = val[i];
				h = indx[i];
				g = i;

				for (j = pntr[h]; j < pntr[h + 1]; j++)
					for (; g < pntr[k + 1] && indx[g + 1] <= indx[j]; g++)
						if (indx[g] == indx[j])
							val[j] -= z * val[g];
			}
		}
		d = pntr[n - 1];
		val[d] = std::sqrt(val[d]);
	}

};




}

#endif
