#include "gtest/gtest.h"
#include <iostream>
// #include <benchmark/benchmark.h>

TEST(test, first_try){
    std::cout << "test_first_try" << std::endl;
    int a = 1;
    ASSERT_EQ(a, 1);
}

// void BM_SimpleFunction(benchmark::State& state) {
//     for (auto _ : state) {
//         for (int i = 0; i < 1000; ++i) {
//             volatile int x = i * i; // 模拟工作
//         }
//     }
// }

// BENCHMARK(BM_SimpleFunction);

// BENCHMARK_MAIN();