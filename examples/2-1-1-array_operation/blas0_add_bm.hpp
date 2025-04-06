#ifndef _BLAS0_ADD_BM_HPP_
#define _BLAS0_ADD_BM_HPP_

#include <benchmark/benchmark.h>
#include "io/gnuplot.hpp"
#include "utility/tinyformat.hpp"
#include "algebra/algebra.hpp"
#include "algebra/blas/blas_0.hpp"

using namespace carpio;

template<class FT>
void BM_BLAS0Add_Normal(benchmark::State& state) {
    for (auto _ : state) {
        auto n = state.range(0);
        FT* a = new FT[n];
        FT* b = new FT[n];
        FT* result = new FT[n];

        Add(n, a, b, result);
        auto v = result[0];  

        benchmark::DoNotOptimize(result);
        delete[] a;
        delete[] b;
        delete[] result;
    }
}

template<class FT>
void BM_BLAS0Add_Unroll(benchmark::State& state) {
    for (auto _ : state) {
        auto n = state.range(0);
        FT* a = new FT[n];
        FT* b = new FT[n];
        FT* result = new FT[n];

        AddUnroll(n, a, b, result);
        auto v = result[0];  

        benchmark::DoNotOptimize(result);
        delete[] a;
        delete[] b;
        delete[] result;
    }
}
namespace _local{
double start = 2.;
double end   = 256.;
double multiplier = 2;
}

// BENCHMARK(BM_BLAS0Add_Normal<double>)->RangeMultiplier(_local::multiplier)->Range(_local::start, _local::end); 
// BENCHMARK(BM_BLAS0Add_Unroll<double>)->RangeMultiplier(_local::multiplier)->Range(_local::start, _local::end); 


template<class FT>
void BM_BLAS0Add_FixUnroll(benchmark::State& state) {
    for (auto _ : state) {
        auto n = state.range(0);
        FT* a = new FT[n];
        FT* b = new FT[n];
        FT* result = new FT[n];

        AddUnroll256(n, a, b, result);
        auto v = result[0];  

        benchmark::DoNotOptimize(result);
        delete[] a;
        delete[] b;
        delete[] result;
    }
}

BENCHMARK(BM_BLAS0Add_FixUnroll<double>)->RangeMultiplier(_local::multiplier)->Range(_local::start, _local::end); 
BENCHMARK(BM_BLAS0Add_Normal<double>)->RangeMultiplier(_local::multiplier)->Range(_local::start, _local::end); 

#endif