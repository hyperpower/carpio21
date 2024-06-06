#include "gtest/gtest.h"
#include "algebra/number/number.hpp"

#include <iomanip>

using namespace carpio;

TEST(number, epsilon){

    std::cout << "long double epsilon = " << std::numeric_limits<long double>::epsilon() << std::endl;
    std::cout << "double epsilon = " << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "float epsilon = " << std::numeric_limits<float>::epsilon() << std::endl;

}
TEST(number, compare_int){
    int a = 12;
    int b = 12;
    auto res = IsEqual(a, b);
    EXPECT_TRUE(res);
}

TEST(number, compare){
    float a = 12.0;
    float b = 12.1;
    auto res = IsEqual(a, b);
    EXPECT_FALSE(res);

    b = a + 1e-7;
    EXPECT_TRUE(IsEqual(a,b));
    b = a + 1e-5;
    EXPECT_FALSE(IsEqual(a,b));

    a = a * 10000;
    b = a + 1e-3;
    std::cout << "a = " << std::setprecision(20) << a << std::endl;
    std::cout << "b = " << std::setprecision(20) << b << std::endl;
    std::cout << "epsilon = " << EpsilonForComparison(a,b) << b << std::endl;
    EXPECT_TRUE(IsEqual(a,b));
    EXPECT_TRUE(a==b);
    
    b = a + 1e-1;
    EXPECT_FALSE(IsEqual(a,b));
    EXPECT_FALSE(a==b);

}

