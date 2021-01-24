#include "gtest/gtest.h"
#include <iostream>

TEST(test, first_try){
    std::cout << "test_first_try" << std::endl;
    int a = 1;
    ASSERT_EQ(a, 1);
}