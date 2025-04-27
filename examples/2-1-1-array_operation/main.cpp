#include <iostream>
#include <memory> 
#include <string>
#include <thread>
#include <sstream>
#include <chrono>
#include <benchmark/benchmark.h>
#include <armadillo>

#include <Eigen/Dense>

// #ifdef __x86_64__
// #include <immintrin.h>
// #elif defined(__arm64__)
// #include <arm_neon.h>
// #endif

#include <valarray>

#include "io/gnuplot.hpp"
#include "utility/tinyformat.hpp"
#include "algebra/algebra.hpp"
// #include "blas0_add_bm.hpp"

using namespace carpio;

template<class VECTORTYPE>
void Fun_VectorAdd(VECTORTYPE& a, VECTORTYPE& b, VECTORTYPE& c){
    c = a + b;
    c = b + a;
    c = a + b;
    // c = a + b + b + a;
    auto v = c[0];

    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c); 
    benchmark::DoNotOptimize(v); 
}

typedef double ft;

template<class VECTORTYPE>
void BM_VectorAdd(benchmark::State& state){
    for (auto _ : state){
        auto n = state.range(0);
        VECTORTYPE v1(n);
        VECTORTYPE v2(n);
        VECTORTYPE result(n);

        for (decltype(n) i = 0; i < n; ++i){
            v1[i] = i * 1.0;
            v2[i] = 1.0;
        }
        Fun_VectorAdd(v1, v2, result);
        // result = v2;
        // result = v1 - v2;

        benchmark::DoNotOptimize(v1);
        benchmark::DoNotOptimize(v2);
        benchmark::DoNotOptimize(result);
    } 
}

void BM_VectorAddRaw(benchmark::State& state) {
    for (auto _ : state) {
        auto n = state.range(0);
        ft* a = new ft[n];
        ft* b = new ft[n];
        ft* result = new ft[n];

        for (decltype(n) i = 0; i < n; ++i) {
            a[i] = i * 1.0;
            b[i] = 1.0;
        }

        for (decltype(n) i = 0; i < n; ++i) {
            result[i] = a[i] + b[i];
            result[i] = b[i] + a[i];
            result[i] = a[i] + b[i];
        }
        // for (decltype(n) i = 0; i < n; ++i) {
        //     result[i] = b[i] + a[i];
        // }
        // for (decltype(n) i = 0; i < n; ++i) {
        //     result[i] = a[i] + b[i];
        // }
        // for (decltype(n) i = 0; i < n; ++i) {
        //     result[i] = b[i] + a[i] + b[i] + a[i];
        // }
        benchmark::DoNotOptimize(result);
        delete[] a;
        delete[] b;
        delete[] result;
    }
}


double start = 2.;
double end   = 256.;
// double end   = 32768.;
double multiplier = 2;

BENCHMARK(BM_VectorAddRaw)->RangeMultiplier(multiplier)->Range(start, end); 
BENCHMARK(BM_VectorAdd<ArrayListV_<ft> >)->RangeMultiplier(multiplier)->Range(start, end); 
BENCHMARK(BM_VectorAdd<arma::vec>)->RangeMultiplier(multiplier)->Range(start, end); 
BENCHMARK(BM_VectorAdd<Eigen::VectorXd>)->RangeMultiplier(multiplier)->Range(start, end); 
BENCHMARK(BM_VectorAdd<std::valarray<ft> >)->RangeMultiplier(multiplier)->Range(start, end); 

// BENCHMARK_MAIN();


 int main(int argc, char** argv) {
    char arg0_default[] = "benchmark";                                 
    char* args_default = reinterpret_cast<char*>(arg0_default);         
    if (!argv) {                                                        
      argc = 1;                                                         
      argv = &args_default;                                             
    }                                         
    benchmark::Initialize(&argc, argv);                      
    if (benchmark::ReportUnrecognizedArguments(argc, argv)){
        return 1;
    } 
    benchmark::SetDefaultTimeUnit(benchmark::kMicrosecond);
    // benchmark::ConsoleReporter console_reporter;
    // std::cout << "Running benchmarks for console output...\n";
    // benchmark::RunSpecifiedBenchmarks(&console_reporter);

    benchmark::JSONReporter reporter;
    std::ofstream ofs("data/bm_result.txt");
    if (!ofs.is_open()) {
        std::cerr << "Error: Unable to open file 'data/bm_result.txt' for writing!" << std::endl;
        return 1;
    }
    reporter.SetOutputStream(&ofs);
    benchmark::RunSpecifiedBenchmarks(&reporter);                            
    benchmark::Shutdown();                                            
    return 0;
 }