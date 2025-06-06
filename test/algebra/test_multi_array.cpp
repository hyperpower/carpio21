#include "algebra/array/multi_array.hpp"
#include "algebra/array/multi_array_v.hpp"
#include "gtest/gtest.h"
#include <valarray>
#include <chrono>
#include <omp.h>

#include <valarray>

using namespace carpio;
typedef std::chrono::milliseconds ms;

typedef ArrayListV_<double> Array;
// typedef std::valarray<double> Array;
TEST(array, add_1T){
	Array::size_type n = 6;
	Array arr1(n);
	Array arr2(n);
	Array res(n);

	arr1.assign(1.0);
	arr2.assign(2.0);

	std::cout << "res = arrd + arri -->" << std::endl;
	res = arr1 + arr2;
	// res.show();
	std::cout << "<--" << std::endl;

	std::cout << "arr1[1] = " << arr1[1] << std::endl;
	ASSERT_EQ(arr1[1], 1.0);
	std::cout << "arr2[1] = " << arr2[1] << std::endl;
	ASSERT_EQ(arr2[1], 2);
	std::cout << "res[1]  = " << res[1] << std::endl;
	ASSERT_EQ(res[1], 3);
	res += arr1;
	ASSERT_EQ(res[1], 4);
	res = 5.0 + arr1;
	ASSERT_EQ(res[1], 6);
	res = arr1 + 5.0 + 1.0;
	ASSERT_EQ(res[1], 7);
	res = arr1 + 5.0 + arr2;
	ASSERT_EQ(res[1], 8);
	res = arr1 + arr2 + arr2 + arr1;
	ASSERT_EQ(res[1], 6);
}


TEST(array, unroll){
	int n = 100;
	Array arr1(n);
	Array arr2(n);
	Array res(n);

	for(int i = 0; i<n ; i++){
		arr1[i] = i * i;
	}
	arr2.assign(2.0);
	
	AddUnroll256(n, arr1.data(), arr2.data(), res.data());

	// res.show();

	ASSERT_EQ(res[1], 3);	
	ASSERT_EQ(res[2], 6);	
	ASSERT_EQ(res[n- 1], (n-1) * (n-1) + 2.0);	

	Add(n, arr1.data(), arr2.data(), res.data());
	ASSERT_EQ(res[1], 3);
}

TEST(array, minus_1T){
	Array arr1(5);
	Array arr2(5);
	Array res(5);

	arr1.assign(1.0);
	arr2.assign(2.0);

	std::cout << "res = arrd + arri -->" << std::endl;
	res = arr1 - arr2;
	std::cout << "<--" << std::endl;

	std::cout << "arr1[1] = " << arr1[1] << std::endl;
	ASSERT_EQ(arr1[1], 1.0);
	std::cout << "arr2[1] = " << arr2[1] << std::endl;
	ASSERT_EQ(arr2[1], 2);
	std::cout << "res[1]  = " << res[1] << std::endl;
	ASSERT_EQ(res[1], -1);
}
TEST(array, add_2T){
	ArrayListV_<double> arrd(5);
	ArrayListV_<int> arri(5);

	arrd.assign(1.0);
	arri.assign(2);

	std::cout << "res = arrd + arri -->" << std::endl;
	auto res = arrd + arri;
	std::cout << "<--" << std::endl;

	std::cout << "arrd[1] = " << arrd[1] << std::endl;
	ASSERT_EQ(arrd[1], 1.0);
	std::cout << "arri[1]  = "<< arri[1] << std::endl;
	ASSERT_EQ(arri[1], 2);
	std::cout << "res[1]  = " << res[1] << std::endl;
	ASSERT_EQ(res[1], 3);
}

TEST(multiarray, multiarray){
	MultiArrayV_<double, 2> ma;
	ma.reconstruct(3,3);
	ma.assign(1);
	ASSERT_EQ(ma(0,0), 1);

	MultiArrayV_<int, 2> mb;
	mb.reconstruct(3,3);
	mb.assign(2);
	ASSERT_EQ(mb(0,0), 2);

	std::cout << "a = ma + mb -->" << std::endl;
	auto a = ma + mb;
	std::cout << "<-- " << std::endl;

	ASSERT_EQ(a(0,0), 3);
	std::cout << "ma(0,0) = " << ma(0,0) << std::endl;
	std::cout << "mb(0,0) = " << mb(0,0) << std::endl;
	std::cout << "a(0,0)  = " << a(0,0) << std::endl;
}

TEST(array, openmp){
	ArrayListV_<double> a(1e7), b(1e7);
	a.assign(2);
	b.assign(1);
	// a.show();
	// b.show();
	auto start = std::chrono::steady_clock::now();
	// omp_set_num_threads(1);
//#pragma omp parallel
	b = a + b - a * b + 5.0 * a;
//  b.show();
	auto end = std::chrono::steady_clock::now();
 	std::cout << "Time = " << std::chrono::duration_cast<ms>(end - start).count() << "ms" << std::endl;
//  a.show();
}

TEST(array, std_valarray){
	// test std::array 
	// std::array is slower for debug mode
	std::cout << "test std::valarray" << std::endl;
	std::valarray<double> a(2.0, 1e7), b(1.0, 1e7);
	auto start = std::chrono::steady_clock::now();
	b = a + b - a * b + 5.0 * a;
	auto end = std::chrono::steady_clock::now();
 	std::cout << "Time = " << std::chrono::duration_cast<ms>(end - start).count() << "ms" << std::endl;
}

