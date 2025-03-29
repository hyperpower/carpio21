#include "algebra/blas/blas_0.hpp"
#include "gtest/gtest.h"
#include <valarray>
#include <chrono>
#include <armadillo>

using namespace carpio;
typedef std::chrono::milliseconds ms;

typedef float ft;

auto test_vector_add(const long& n){
    std::cout << "n : " << n << "  ";
    auto start = std::chrono::steady_clock::now();
    ft* a = new ft[n];
    ft* b = new ft[n];
    ft* result = new ft[n];

    for (int i = 0; i < n; ++i) {
        a[i] = i * 1.0;
        b[i] = 1.0;
    }

    AddEqual(n, a, b);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "     Time: " << duration.count() << " us" << std::endl;

    delete[] a;
    delete[] b;
    delete[] result;
    return duration; 
}

auto test_vector_add_simd(const long& n){
    std::cout << "n : " << n << "  ";
    auto start = std::chrono::steady_clock::now();
    ft* a = new ft[n];
    ft* b = new ft[n];
    ft* result = new ft[n];

    for (int i = 0; i < n; ++i) {
        a[i] = i * 1.0;
        b[i] = 1.0;
    }

    AddEqual_(n, a, b);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "SIMD Time: " << duration.count() << " us" << std::endl;

    delete[] a;
    delete[] b;
    delete[] result;
    return duration; 
}

auto test_vector_add_arma(const long& n){
    std::cout << "n : " << n << "  ";
    auto start = std::chrono::steady_clock::now();
    arma::vec v1(n); 
    arma::vec v2(n); 

    for (int i = 0; i < n; ++i) {
        v1[i] = i * 1.0;
        v2[i] = 1.0;
    }

    auto res = v1 + v2;
    
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "arma Time: " << duration.count() << " us" << std::endl;
    return duration;
}

TEST(array, add_correct){
    ft a[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    ft b[8] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    AddEqual(8, a, b);
    ASSERT_EQ(a[0], 2);
    ASSERT_EQ(a[5], 7);

    AddEqual_(8, a, b);
    ASSERT_EQ(a[0], 3);
    ASSERT_EQ(a[5], 8);
}

TEST(array, add){
	auto dt  = test_vector_add(9e7);
	auto dt2 = test_vector_add_simd(9e7);
	auto dta = test_vector_add_arma(9e7);
}