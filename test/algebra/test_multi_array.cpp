#include "algebra/array/multi_array.hpp"
#include "gtest/gtest.h"
#include "utility/clock.hpp"
#include <valarray>

using namespace carpio;

TEST(multiarray, multiarray){
	MultiArray_<double, 2> ma;
	ma.reconstruct(3,3);
	ma.assign(1);
	ASSERT_EQ(ma(0,0), 1);

	std::cout << ma(0,0) << std::endl;
}

TEST(array, openmp){
	ArrayListV_<double> a(1e7), b(1e7);
	a.assign(2);
	b.assign(1);
	// a.show();
	// b.show();
	tick_t start = Clock::Tick();
//	omp_set_num_threads(1);
//#pragma omp parallel
	b = a + b - a * b + 5.0 * a;
//	b.show();
	tick_t end = Clock::Tick();
 	std::cout << "Time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;
	// a.show();
}

TEST(multi_array, openmp){
	// test std::array 
	// std::array is slower for debug mode
	std::valarray<double> a(2.0, 1e7), b(1.0, 1e7);
	tick_t start = Clock::Tick();
	b = a + b - a * b + 5.0 * a;
	tick_t end = Clock::Tick();
 	std::cout << "Time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;
}

