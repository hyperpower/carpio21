#ifndef _BLAS_0_HPP_
#define _BLAS_0_HPP_

#include "type_define.hpp"

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#ifdef OPENMP
#include <omp.h>
#endif


namespace carpio {
template<typename ST, typename VT>
void Assign(const ST& n, VT*, const VT&);

template<typename ST, typename VT>
void AddEqual(     const ST& , VT*, const VT*);
template<typename ST, typename VT>
void AddEqual(     const ST& , VT*, const VT&);
template<typename ST, typename VT, typename VT2>
void AddEqual(     const ST& , VT*, const VT2*);
// template<typename ST, typename VT, typename VT2>
// void AddEqual(     const ST& , VT*, const VT2&);

template<typename ST, typename VT>
void MinusEqual(   const ST& , VT*, const VT*);
template<typename ST, typename VT>
void MinusEqual(   const ST& , VT*, const VT&);
template<typename ST, typename VT, typename VT2>
void MinusEqual(   const ST& , VT*, const VT2*);

template<typename ST, typename VT>
void MultiplyEqual(const ST& , VT*, const VT*);
template<typename ST, typename VT>
void MultiplyEqual(const ST& , VT*, const VT&);
template<typename ST, typename VT, typename VT2>
void MultiplyEqual(const ST& n, VT* src, const VT2*);
template<typename ST, typename VT, typename VT2>
void MultiplyEqual(const ST& n, VT* src, const VT2&);

template<typename ST, typename VT>
void DivideEqual(  const ST& , VT*, const VT*);
template<typename ST, typename VT>
void DivideEqual(  const ST& , VT*, const VT&);



template<typename ST, typename VT>
void Assign(const ST& len, VT* src, const VT& value){
    for (int i = 0; i < len; ++i) {
        src[i] = value;
    }
}

template<typename ST, typename VT>
void AddEqual(const ST& n, VT* src, const VT* dst) {
#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        src[i] += dst[i];
    }
}
template<typename ST, typename VT, typename VT2>
void AddEqual(const ST& n, VT* src, const VT2* dst) {
#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        src[i] += dst[i];
    }
}
template<typename ST, typename VT>
void MinusEqual(const ST& n, VT* src, const VT* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] -= dst[i];
	}
}
template<typename ST, typename VT, typename VT2>
void MinusEqual(const ST& n, VT* src, const VT2* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] -= dst[i];
	}
}
template<typename ST, typename VT>
void MultiplyEqual(const ST& n, VT* src, const VT* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] *= dst[i];
	}
}
template<typename ST, typename VT>
void DivideEqual(const ST& n, VT* src, const VT* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] /= dst[i];
	}
}
template<typename ST, typename VT>
void AddEqual(const ST& n, VT* src, const VT& dst) {
#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        src[i] += dst;
    }
}
template<typename ST, typename VT>
void MinusEqual(const ST& n, VT* src, const VT& dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] -= dst;
	}
}
template<typename ST, typename VT>
void MultiplyEqual(const ST& n, VT* src, const VT& dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] *= dst;
	}
}
template<typename ST, typename VT, typename VT2>
void MultiplyEqual(const ST& n, VT* src, const VT2* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] *= dst[i];
	}
}
template<typename ST, typename VT, typename VT2>
void MultiplyEqual(const ST& n, VT* src, const VT2& dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] *= dst;
	}
}
template<typename ST, typename VT>
void DivideEqual(const ST& n, VT* src, const VT& dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] /= dst;
	}
}
template<typename ST, typename VT, typename VT2>
void DivideEqual(const ST& n, VT* src, const VT2* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] /= dst[i];
	}
}
template<typename ST, typename VT>
void Nagative(const ST& n, VT* src) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		src[i] =  -(src[i]);
	}
}
template<typename ST, typename VT>
void Nagative(const ST& n, const VT* src,  VT* dst) {
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		dst[i] =  -(src[i]);
	}
}
}

#endif
