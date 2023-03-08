#include "gtest/gtest.h"
#include "utility/big_int.hpp"


TEST(Bigint, construct) {
    BigInt num1;        // should be 0 by default
    ASSERT_EQ(num1, 0);

    BigInt num2(0);     // 0 passed as an integer
    ASSERT_TRUE(num2 == 0);

    BigInt num3("0");   // 0 passed as a string
    ASSERT_TRUE(num3 == 0);

    BigInt num4(num3);  // object with value 0 passed to copy constructor
    ASSERT_TRUE(num4 == 0);
}

TEST(Bigint, construct_random) {
    std::random_device generator;
    // uniform distribution of numbers from LLONG_MIN to LLONG_MAX:
    std::uniform_int_distribution<long long> distribution((LLONG_MIN), (LLONG_MAX));
    for (size_t i = 0; i < 30; i++) {
        long long rand_num = distribution(generator);

        ASSERT_TRUE(BigInt(rand_num) == rand_num);
        ASSERT_TRUE(BigInt(std::to_string(rand_num)) == rand_num);
    }

    // should throw exception when invalid arguments are passed:
    try {
        BigInt trouble("123BigInt");    // without sign
    }catch (std::logic_error &e) {
        ASSERT_TRUE(e.what() == std::string("Expected an integer, got \'123BigInt\'"));
    }

    try {
        BigInt trouble("-4a5b6c");      // with sign
                                        // looks like hex, but is still not allowed
    }
    catch (std::logic_error &e) {
        ASSERT_TRUE(e.what() == std::string("Expected an integer, got \'-4a5b6c\'"));
    }
}

TEST(convert, to_string) {
    std::random_device generator;
    // uniform distribution of numbers from LLONG_MIN to LLONG_MAX:
    std::uniform_int_distribution<long long> distribution((LLONG_MIN), (LLONG_MAX));
    for (size_t i = 0; i < 20; i++) {
        long long rand_num = distribution(generator);

        EXPECT_TRUE(BigInt(rand_num).to_string() == std::to_string(rand_num));
    }
}

TEST(convert, to_int) {
    std::random_device generator;
    // uniform distribution of numbers from INT_MIN to INT_MAX:
    std::uniform_int_distribution<int> distribution((INT_MIN), (INT_MAX));
    for (size_t i = 0; i < 20; i++) {
        int rand_num = distribution(generator);

        EXPECT_TRUE(BigInt(rand_num).to_int() == rand_num);
    }
}

TEST(convert, to_long) {
    std::random_device generator;
    // uniform distribution of numbers from LONG_MIN to LONG_MAX:
    std::uniform_int_distribution<long> distribution((LONG_MIN), (LONG_MAX));
    for (size_t i = 0; i < 20; i++) {
        long rand_num = distribution(generator);

        ASSERT_TRUE(BigInt(rand_num).to_long() == rand_num);
    }
}

TEST(convert, to_long_long) {
    std::random_device generator;
    // uniform distribution of numbers from LLONG_MIN to LLONG_MAX:
    std::uniform_int_distribution<long long> distribution((LLONG_MIN), (LLONG_MAX));
    for (size_t i = 0; i < 20; i++) {
        long long rand_num = distribution(generator);

        ASSERT_TRUE(BigInt(rand_num).to_long_long() == rand_num);
    }
}