#ifndef _BLAS_0_HPP_
#define _BLAS_0_HPP_

#include "type_define.hpp"

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#ifdef OPENMP
#include <omp.h>
#endif
// #ifdef __x86_64__
// #include <immintrin.h>
// #elif defined(__arm64__)
// #include <arm_neon.h>
// #endif

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

// template<typename ST>
// void AddEqual_(const ST& n, float32_t* src, const float32_t* dst){
// 	ST i = 0;
// 	#ifdef __arm64__
// 	for (; i <= n - 4; i += 4) {
//         auto vr = vaddq_f32(vld1q_f32(&src[i]), vld1q_f32(&dst[i]));
//         vst1q_f32(&src[i], vr);
//     }
// 	#endif
// 	for (; i < n; ++i) {
//         src[i] += dst[i];
//     }
// }
// template<typename ST>
// void AddEqual_(const ST& n, float64_t* src, const float64_t* dst){
// 	ST i = 0;
// 	#ifdef __arm64__
// 	for (; i <= n - 2; i += 2) {
//         auto vr = vaddq_f64(vld1q_f64(&src[i]), vld1q_f64(&dst[i]));
//         vst1q_f64(&src[i], vr);
//     }
// 	#endif
// 	for (; i < n; ++i) {
//         src[i] += dst[i];
//     }
// }
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
#ifdef __ARM_NEON_H
template<typename ST>
void AddUnrollNeon(const ST& n, const float32_t* a, const float32_t* b, float32_t* res){
	ST i = 0;
	ST ns = n - 3;
	for (; i < ns; i += 4) {
        auto vr = vaddq_f32(vld1q_f32(&a[i]), vld1q_f32(&b[i]));
        vst1q_f32(&res[i], vr);
    }
	for (; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}
template<typename ST>
void AddUnrollNeon(const ST& n, const float64_t* a, const float64_t* b, float64_t* res){
	ST i = 0;
	ST ns = n - 1;
	for (; i < ns; i += 2) {
        auto vr = vaddq_f64(vld1q_f64(&a[i]), vld1q_f64(&b[i]));
        vst1q_f64(&res[i], vr);
    }
	for (; i < n; ++i) {
        res[i] = a[i] + b[i];
    }
}
#endif
template<typename ST, typename VT>
void AddUnroll(const ST& n, const VT* a, const VT* b, VT* res) {
#ifdef __ARM_NEON_H
	AddUnrollNeon(n, a, b, res);
#else 	
	ST i  = 0;
	ST ns = n - 3;
#pragma omp parallel for
	for (i = 0; i < ns; i += 4) {
		ST i1 = i + 1; 
		ST i2 = i + 2; 
		ST i3 = i + 3; 
		res[i ] = a[i ] + b[i ];   
		res[i1] = a[i1] + b[i1];
		res[i2] = a[i2] + b[i2];
		res[i3] = a[i3] + b[i3];
	}
	for (; i < n; ++i) {
		res[i] = a[i] + b[i];
	}
#endif
}
template<typename ST, typename VT>
void Minus(const ST& n, const VT* a, const VT* b, VT* res) {
#pragma omp parallel for
    for (ST i = 0; i < n; ++i) {
        res[i] = a[i] - b[i];
    }
}


}

#endif
