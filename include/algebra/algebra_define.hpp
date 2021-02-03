/************************
 //  \file   Arithmetic.h
 //  \brief
 //
 //  \author czhou
 //  \date   12 févr. 2014
 ***********************/
#ifndef ALGEBRA_DEFINE_HPP_
#define ALGEBRA_DEFINE_HPP_

#include "type_define.hpp"

#include <math.h>
#include <iostream>
#include <cmath>

namespace carpio {

inline int StepFun(Float x) {
	return (x <= 0) ? 0 : 1;
}

inline int StepFun(int x) {
	return (x <= 0) ? 0 : 1;
}

template<class TYPE>
inline bool IsSameSign(const TYPE& x, const TYPE& y){
	if(Sign(x) == Sign(y)){
		return true;
	}else{
		return false;
	}
}
enum Range {
	_oo_, _oc_, _co_, _cc_,
};

template<typename TYPE>
inline bool IsInRange(TYPE down, TYPE value, TYPE up, Range range) {
	switch (range) {
	case _oo_:
		return (down < value && value < up) ? true : false;
	case _oc_:
		return (down < value && value <= up) ? true : false;
	case _co_:
		return (down <= value && value < up) ? true : false;
	case _cc_:
		return (down <= value && value <= up) ? true : false;
	}
	return false;
}

// a number is prime or not
inline bool IsPrime(St n) {
	if (n == 1) {
		return false;
	}
	if (n == 2 || n == 3) {
		return true;
	} else if ((n % 2 == 0) || (n % 3 == 0)) {
		return false;
	} else {
		St p = 5;
		St w = 2;
		while (p * p <= n) {
			if (n % p == 0) {
				return false;
			}
			p += w;
			w = 6 - w;
		}
		return true;
	}
}


// this function return a^2+b^2
template<typename TYPE>
TYPE SquareSum(const TYPE &a, const TYPE &b){
	return a * a + b * b;
}

// this function return (a + b)*(a + b)
template<typename TYPE>
TYPE SumSquare(const TYPE &a, const TYPE &b) {
	return (a + b) * (a + b);
}

// round to n digit of a
// roundto(3.145,1)=3.1    roundto(3.145,2)=3.15
inline long double RoundTo(long double a, int n) {
	return std::round(a * std::pow(10.0, n)) / std::pow(10.0, n);
}

inline int CountSignificanceDigit(Float a) {
	for (int i = 0; i < 30; i++) {
		if (RoundTo(a, i) == a) {
			return i;
		}
	}
	return -1;
}


Float volume_of_circular_truncated_cone(Float R, Float r, Float h);

template<class TYPE>
inline TYPE max(TYPE a, TYPE b, bool (*Comp_ge)(const TYPE&, const TYPE&)) {
	return Comp_ge(a, b) ? a : b;
}

template<class TYPE>
inline TYPE max(TYPE a, TYPE b, TYPE c,
		bool (*Comp_ge)(const TYPE&, const TYPE&)) {
	TYPE tmp = Comp_ge(a, b) ? a : b;
	return Comp_ge(tmp, c) ? tmp : c;
}

template<class TYPE>
inline TYPE min(TYPE a, TYPE b, bool (*Comp_le)(const TYPE&, const TYPE&)) {
	return Comp_le(a, b) ? a : b;
}

template<class TYPE>
inline TYPE min(TYPE a, TYPE b, TYPE c,
		bool (*Comp_le)(const TYPE&, const TYPE&)) {
	TYPE tmp = Comp_le(a, b) ? a : b;
	return Comp_le(tmp, c) ? tmp : c;
}

template<class TYPE>
inline TYPE mid(TYPE a, TYPE b, TYPE c,
		bool (*Comp_ge)(const TYPE&, const TYPE&)) {
	int idx = Comp_ge(a, b) ? 1 : 2;
	if (idx == 1)
		idx = Comp_ge(a, c) ? 1 : 3;
	else
		idx = Comp_ge(b, c) ? 2 : 3;
	if (idx == 1)
		return Comp_ge(b, c) ? b : c;
	else if (idx == 2)
		return Comp_ge(a, c) ? a : c;
	else
		return Comp_ge(a, b) ? a : b;
}

template<class TYPE>
inline void sort(const TYPE& a, const TYPE& b, const TYPE& c,  //
		bool (*Comp)(const TYPE&, const TYPE&), //
		TYPE& big, TYPE& mid, TYPE& small) {
	int idx = Comp(a, b) ? 1 : 2;
	if (idx == 1)
		idx = Comp(a, c) ? 1 : 3;
	else
		idx = Comp(b, c) ? 2 : 3;
	if (idx == 1) {
		big = a;
		if (Comp(b, c)) {
			mid = b;
			small = c;
		} else {
			mid = c;
			small = b;
		}
		return;
	}
	if (idx == 2) {
		big = b;
		if (Comp(a, c)) {
			mid = a;
			small = c;
		} else {
			mid = c;
			small = a;
		}
		return;
	}
	big = c;
	if (Comp(a, b)) {
		mid = a;
		small = b;
	} else {
		mid = b;
		small = a;
	}
}

template<class TYPE>
bool Comp_great(const TYPE& a, const TYPE& b) {
	return a > b;
}

template<class TYPE>
bool Comp_less(const TYPE& a, const TYPE& b) {
	return a < b;
}

template<class TYPE>
inline void sort(const TYPE& a, const TYPE& b, const TYPE& c,  //
		bool (*Comp)(const TYPE&, const TYPE&), //
		int& big, int& mid, int& small) {
	int idx = Comp(a, b) ? 0 : 1;
	if (idx == 0)
		idx = Comp(a, c) ? 0 : 2;
	else
		idx = Comp(b, c) ? 1 : 2;
	if (idx == 0) {
		big = 0;
		if (Comp(b, c)) {
			mid = 1;
			small = 2;
		} else {
			mid = 2;
			small = 1;
		}
		return;
	}
	if (idx == 1) {
		big = 1;
		if (Comp(a, c)) {
			mid = 0;
			small = 2;
		} else {
			mid = 2;
			small = 0;
		}
		return;
	}
	big = 2;
	if (Comp(a, b)) {
		mid = 0;
		small = 1;
	} else {
		mid = 1;
		small = 0;
	}
}
template<class TYPE>
inline void Swap(TYPE& a, TYPE& b) //
		{
	TYPE tmp = a;
	a = b;
	b = tmp;
}

template<class TYPE>
inline void SortIncr(TYPE& a, TYPE& b, TYPE& c) //
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
int _QuadraticDiscriminant(const TYPE& a, const TYPE& b, const TYPE& c,
		Float& discri) {
	discri = b * b - 4.0 * a * c;
	if (discri == 0) {
		return 1;
	} else if (discri > 0) {
		return 2;
	} else {
		return 0;
	}
}

// n!
inline double Factorial(St n) {
	double r = 1;
	for (St i = n; i > 0; i--) {
		r *= i;
	}
	return r;
}




} //namespace Larus

#endif /* ARITHMETIC_H_ */
