#include "gtest/gtest.h"
#include "algebra/number/number.hpp"

using namespace carpio;

TEST(number, epsilon){

    std::cout << "long double epsilon = " << std::numeric_limits<long double>::epsilon() << std::endl;
    std::cout << "double epsilon = " << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "float epsilon = " << std::numeric_limits<float>::epsilon() << std::endl;

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
    


}

