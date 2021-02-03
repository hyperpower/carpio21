#include "algebra/array/multi_array.hpp"
#include "gtest/gtest.h"
#include "utility/clock.hpp"

using namespace carpio;

TEST(multiarray, multiarray){
	MultiArray_<double, 2> ma;
	ma.reconstruct(3,3);
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

class ObjNoData{
public:
    // Simple constructor that initializes the resource.
    explicit ObjNoData()
    {
		std::cout << "Constructor" << std::endl;
    }

    // Destructor.
    ~ObjNoData(){
		std::cout << "Destructor";
		std::cout << std::endl;
    }

	// Copy constructor.
    ObjNoData(const ObjNoData& other){
		std::cout << "Copy constructor" << std::endl;
    }

	// Copy assignment operator.
    ObjNoData& operator=(const ObjNoData& other)
    {
		std::cout << "Copy Assignment Operator" << std::endl;
		return *this;
    }

	// Move assignment operator.
    ObjNoData& operator=(ObjNoData&& other)
    {
        std::cout << "Move Assignment Operator" << std::endl;
       	return *this;
    }
    ObjNoData& operator+=(const ObjNoData& other)
    {
        std::cout << "Operator +=" << std::endl;
       	return *this;
    }

};

TEST(array, move_nodata){
	ObjNoData a;
	ObjNoData b = a;

	ObjNoData c;
	c += a;
}