#include <iostream>
#include <memory> 
#include <string>
#include <thread>
#include <sstream>
#include <chrono>
#include <benchmark/benchmark.h>
#include <armadillo>

#include <Eigen/Dense>

#ifdef __x86_64__
#include <immintrin.h>
#elif defined(__arm64__)
#include <arm_neon.h>
#endif

#include <valarray>

#include "io/gnuplot.hpp"
#include "utility/tinyformat.hpp"
#include "algebra/algebra.hpp"

using namespace carpio;

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
        result = v1;
        // result = v2;
        // result = v1 - v2;

        benchmark::DoNotOptimize(v1);
        benchmark::DoNotOptimize(v2);
        benchmark::DoNotOptimize(result);
    } 
}



void BM_RawVectorAdd(benchmark::State& state) {
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
            result[i] = b[i];
        }
        for (decltype(n) i = 0; i < n; ++i) {
            result[i] = b[i];
        }

        benchmark::DoNotOptimize(result);
        delete[] a;
        delete[] b;
        delete[] result;
    }
}
template<typename ST>
void Add_(const ST& n, float32_t* v1, const float32_t* v2, float32_t* res){
	ST i = 0;
	#ifdef __arm64__
	for (; i <= n - 4; i += 4) {
        vst1q_f32(&res[i], vaddq_f32(vld1q_f32(&v1[i]), vld1q_f32(&v2[i])));
    }
	#endif
	for (; i < n; ++i) {
        res[i] = v1[i] + v2[i];
    }
}
template<typename ST>
void Add_(const ST& n, float64_t* v1, const float64_t* v2, float64_t* res){
	ST i = 0;
	#ifdef __arm64__
	for (; i <= n - 2; i += 2) {
        vst1q_f64(&res[i], vaddq_f64(vld1q_f64(&v1[i]), vld1q_f64(&v2[i])));
    }
	#endif
	for (; i < n; ++i) {
        res[i] = v1[i] + v2[i];
    }
}

void BM_SimdVectorAdd(benchmark::State& state) {
    for (auto _ : state) {
        auto n = state.range(0);
        ft* a = new ft[n];
        ft* b = new ft[n];
        ft* result = new ft[n];

        for (decltype(n) i = 0; i < n; ++i) {
            a[i] = i * 1.0;
            b[i] = 1.0;
        }

        Add_(n, a, b, result);
        // Add_(n, a, b, result);

        benchmark::DoNotOptimize(result);
        delete[] a;
        delete[] b;
        delete[] result;
    }
}
BENCHMARK(BM_RawVectorAdd)->Range(8, 8<<10); // 8 to 8192（2^13）
BENCHMARK(BM_VectorAdd<arma::vec>)->Range(8, 8<<10); // 8 to 8192（2^13)
BENCHMARK(BM_VectorAdd<Eigen::VectorXd>)->Range(8, 8<<10); // 8 to 8192（2^13)
BENCHMARK(BM_VectorAdd<std::valarray<ft> >)->Range(8, 8<<10); // 8 to 8192（2^13)
BENCHMARK(BM_VectorAdd<ArrayListV_<ft> >)->Range(8, 8<<10); // 8 to 8192（2^13)

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
    std::ofstream ofs("data/bm_vector_add.txt");
    if (!ofs.is_open()) {
        std::cerr << "Error: Unable to open file 'data/bm_vector_add.txt' for writing!" << std::endl;
        return 1;
    }
    reporter.SetOutputStream(&ofs);
    benchmark::RunSpecifiedBenchmarks(&reporter);                            
    benchmark::Shutdown();                                            
    return 0;
 }