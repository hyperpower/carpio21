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
void AddEqual(     const ST& n, VT*, const VT*);
template<typename ST, typename VT>
void MinusEqual(   const ST& n, VT*, const VT*);
template<typename ST, typename VT>
void MultiplyEqual(const ST& n, VT*, const VT*);
template<typename ST, typename VT>
void DivideEqual(  const ST& n, VT*, const VT*);

template<typename ST, typename VT>
void AddEqual(     const ST& n, VT*, const VT&);
template<typename ST, typename VT>
void MinusEqual(   const ST& n, VT*, const VT&);
template<typename ST, typename VT>
void MultiplyEqual(const ST& n, VT*, const VT&);
template<typename ST, typename VT>
void DivideEqual(  const ST& n, VT*, const VT&);


template<typename ST, typename VT>
void Assign(const ST& n, VT* src, const VT& value){
    for (ST i = 0; i < n; ++i) {
        src[i] = value;
    }
}

template<typename ST, typename VT>
void AddEqual(const ST& n, VT* src, const VT* dst) {
    for (ST i = 0; i < n; ++i) {
        src[i] += dst[i];
    }
}
template<typename ST, typename VT>
void MinusEqual(const ST& n, VT* src, const VT* dst) {
	for (ST i = 0; i < n; ++i) {
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
	for (ST i = 0; i < n; ++i) {
		src[i] /= dst[i];
	}
}

template<typename ST, typename VT>
void AddEqual(const ST& n, VT* src, const VT& dst) {
    for (ST i = 0; i < n; ++i) {
        src[i] += dst;
    }
}
template<typename ST, typename VT>
void MinusEqual(const ST& n, VT* src, const VT& dst) {
	for (ST i = 0; i < n; ++i) {
		src[i] -= dst;
	}
}
template<typename ST, typename VT>
void MultiplyEqual(const ST& n, VT* src, const VT& dst) {
	for (ST i = 0; i < n; ++i) {
		src[i] *= dst;
	}
}
template<typename ST, typename VT>
void DivideEqual(const ST& n, VT* src, const VT& dst) {
	for (ST i = 0; i < n; ++i) {
		src[i] /= dst;
	}
}

}

#endif
