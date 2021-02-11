#ifndef _SOLVER_DIRECT_
#define _SOLVER_DIRECT_


#include "type_define.hpp"
#include "algebra/array/array_list.hpp"
#include "algebra/matrix/matrix.hpp"
#include <cmath>
#include <functional>

namespace carpio{

template<class TYPE>
TYPE SolveDichotomy(TYPE left, TYPE right,
		            const TYPE& thershold,
					const TYPE& tol,
		            std::function<TYPE(TYPE)> fun,
					int maxiter = 1000){
	// trivial case
	if(std::abs(fun(left) - thershold) < tol){
		return left;
	}
	if(std::abs(fun(right) - thershold) < tol){
		return right;
	}
	TYPE middle = (left + right) * 0.5;
	int iter = 0;
	while ((std::abs(fun(middle) - thershold) > tol)
			&& (iter < maxiter)) {
		iter++;
		if (IsSameSign(fun(middle) - thershold,
				       fun(right)  - thershold)) {
			right = middle;
			middle = (middle + left) * 0.5;
			continue;
		} else {
			left = middle;
			middle = (middle + right) * 0.5;
			continue;
		}
	}
	return middle;
}


template<class TYPE>
int SolveQuadraticEquation(const TYPE &a, const TYPE &b, const TYPE &c,
                           double &x1, double &x2) {
	double discri = 0;
    int numroot;
    discri = b * b - 4.0 * a * c;
    if (discri == 0) {
        numroot = 1;
    } else if (discri > 0) {
        numroot = 2;
    } else {
        numroot = 0;
    }

    if (numroot == 2) {
        x1 = (-b - std::sqrt(discri)) / 2 / a;
        x2 = (-b + std::sqrt(discri)) / 2 / a;
        return 2;
    } else if (numroot == 1) {
        x1 = -b / 2 / a;
        x2 = x1;
        return 1;
    } else {
        return 0;
    }
}



template<class TYPE>
inline void SortIncrease(TYPE &a, TYPE &b, TYPE &c) //
{
    if (b < a) {
        Swap(a, b);
    }
    if (c < a) {
        Swap(a, c);
    }
    if (c < b) {
        Swap(b, c);
    }
}

template<class TYPE>
int SolveCubicEquation(const TYPE &a, const TYPE &b, const TYPE &c,
                       const TYPE &d, double &x1, double &x2, double &x3) {
    ASSERT(a != 0);
    double A = b * b - 3.0 * a * c;
    double B = b * c - 9.0 * a * d;
    double C = c * c - 3.0 * b * d;
    double discri = B * B - 4.0 * A * C;
    //case 1 has three equal real roots
    if (A == 0 && B == 0) {
        x1 = -b / 3.0 / a;
        x2 = x1;
        x3 = x1;
        return 1;
    }
    if (discri > 0) {
        double Y1 = A * b + 1.5 * a * (-B + std::sqrt(discri));
        double Y2 = A * b + 1.5 * a * (-B - std::sqrt(discri));
        double cuberY1 = Y1 < 0 ? - std::pow(-Y1, 1.0 / 3.0) : std::pow(Y1, 1.0 / 3.0);
        double cuberY2 = Y2 < 0 ? - std::pow(-Y2, 1.0 / 3.0) : std::pow(Y2, 1.0 / 3.0);
        x1 = (-b - cuberY1 - cuberY2) / 3.0 / a;
        //ignore complex roots
        x2 = x1;
        x3 = x1;
        return 2;
    }
    if (discri == 0) {
        double K = B / A;
        x1 = -b / a + K;
        x2 = K / 2.0;
        x3 = x2;
        SortIncrease(x1, x2, x3);
        return 3;
    }
    if (discri < 0) {
        double T = (2.0 * A * b - 3.0 * a * B) / (2.0 * pow(A, 1.5));
        double sita3 = std::acos(T) / 3.0;
        x1 = (-b - 2.0 * std::sqrt(A) * cos(sita3)) / (3.0 * a);
        x2 = (-b + std::sqrt(A) * (std::cos(sita3) + std::sqrt(3.0) * std::sin(sita3))) / (3.0 * a);
        x3 = (-b + std::sqrt(A) * (std::cos(sita3) - std::sqrt(3.0) * std::sin(sita3))) / (3.0 * a);
        SortIncrease(x1, x2, x3);
        return 4;
    }
    return -1;
}

/**
 * solve
 * a1 x + b1 y = c1
 * a2 x + b2 y = c2
 *
 */
template<class VALUE>
int Solve(VALUE a1, VALUE b1, VALUE c1,
		  VALUE a2, VALUE b2, VALUE c2,
		  VALUE& x, VALUE& y) {
	ASSERT(!(a2 == 0 && b2 == 0));
	ASSERT(!(a1 == 0 && b1 == 0));
	if (a2 == 0) {
		y = double(c2) / double(b2);
	} else {
		y = double(c1 - a1 * c2 / a2) / double(b1 - a1 * b2 / a2 + SMALL);
	}
	x = double(c1 - b1 * y) / double(a1 + SMALL);
}


// Gaussian Elimination
template<class VALUE>
int Solve(  //solver
		MatrixV_<VALUE>& A,      //the Matrix     [in]       solver will change matrix
		ArrayListV_<VALUE>& b    //the b          [in][out]  solver will change b
		) {
	//Assert
	ASSERT(A.size_i() == A.size_j());   //the matrix must be n x n;
	ASSERT(b.size()   == A.size_i());     //the size b=n
	int n = b.size();
	int i, j, max_i;
	VALUE max, dum;
	// for each variable find pivot row and perform forward substitution
	for (i = 0; i < (n - 1); ++i) {
		//  find the pivot row
		max_i = i;
		max = std::abs(A[i][i]);
		for (int ii = i + 1; ii < n; ++ii)
			if ((dum = std::abs(A[ii][i])) > max) {
				max = dum;
				max_i = ii;
			}
		if (max == 0.0) {
			return -1;                // the matrix A is singular
		}
		// and if it differs from the current row, interchange the two rows.
		if (max_i != i) {
			for (j = 0; j < n; j++) {
				dum = A[i][j];
				A[i][j] = A[max_i][j];
				A[max_i][j] = dum;
			}
			dum = b[i];
			b[i] = b[max_i];
			b[max_i] = dum;
		}
		// Perform forward substitution
		for (int ii = i + 1; ii < n; ++ii) {
			dum = -A[ii][i] / A[i][i];
			for (j = i + 1; j < n; ++j) {
				A[ii][j] += dum * A[i][j];
			}
			b[ii] += dum * b[i];
		}
	}
	// Perform backward substitution
	for (i = n - 1; i >= 0; i--) {
		if (A[i][i] == 0.0)
			return -1;           // matrix is singular
		dum = b[i];
		for (j = i + 1; j < n; j++) {
			dum -= A[i][j] * b[j];
		}
		b[i] = dum / A[i][i];
	}
	return 0;
}


}


#endif
