#include "gtest/gtest.h"
#include <iostream>
// #include "type_define.hpp"

// #include <benchmark/benchmark.h>

// using namespace carpio;

TEST(test, first_try){
    std::cout << "test_first_try" << std::endl;
    int a = 1;
    ASSERT_EQ(a, 1);
}
// TEST(test, test_define){
//     std::cout << "GetTypeName int   = " << GetTypeName<int>() << std::endl;
//     std::cout << "GetTypeName float = " << GetTypeName<float>() << std::endl;
// }
// void BM_SimpleFunction(benchmark::State& state) {
//     for (auto _ : state) {
//         for (int i = 0; i < 1000; ++i) {
//             volatile int x = i * i; // 
//         }
//     }
// }

// BENCHMARK(BM_SimpleFunction);

// BENCHMARK_MAIN();