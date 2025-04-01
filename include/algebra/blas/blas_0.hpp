#ifndef _BLAS_0_HPP_
#define _BLAS_0_HPP_

#include "type_define.hpp"

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#ifdef OPENMP
#include <omp.h>
#endif
#ifdef __x86_64__
#include <immintrin.h>
#elif defined(__arm64__)
#include <arm_neon.h>
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
#pragma omp parallel for
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
void AddEqual(const ST& n, VT* a, const VT2* b) {
	// std::cout << "Add Equal 2T" << std::endl;
#pragma omp parallel for
    for (ST i = 0; i < n; ++i) {
        a[i] += b[i];
    }
}

template<typename ST>
void AddEqual_(const ST& n, float32_t* src, const float32_t* dst){
	ST i = 0;
	#ifdef __arm64__
	for (; i <= n - 4; i += 4) {
        auto vr = vaddq_f32(vld1q_f32(&src[i]), vld1q_f32(&dst[i]));
        vst1q_f32(&src[i], vr);
    }
	#endif
	for (; i < n; ++i) {
        src[i] += dst[i];
    }
}
template<typename ST>
void AddEqual_(const ST& n, float64_t* src, const float64_t* dst){
	ST i = 0;
	#ifdef __arm64__
	for (; i <= n - 2; i += 2) {
        auto vr = vaddq_f64(vld1q_f64(&src[i]), vld1q_f64(&dst[i]));
        vst1q_f64(&src[i], vr);
    }
	#endif
	for (; i < n; ++i) {
        src[i] += dst[i];
    }
}
template<typename ST, typename VT>
void MinusEqual(const ST& n, VT* src, const VT* dst) {
#pragma omp parallel for
	for (ST i = 0; i < n; ++i) {
		src[i] -= dst[i];
	}
}
template<typename ST, typename VT, typename VT2>
void MinusEqual(const ST& n, VT* src, const VT2* dst) {
#pragma omp parallel for
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


template<typename ST, typename VT>
void Add(const ST& n, const VT* a, const VT* b, VT* res) {
#pragma omp parallel for
    for (ST i = 0; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}

}

#endif
