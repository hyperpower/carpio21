#include "algebra/number/number.hpp"
#include "io/io_define.hpp"

#include "gtest/gtest.h"

using namespace carpio;

TEST(num, rational){
    using rational = Rational_<int>;

    constexpr rational r1{};
    static_assert(r1.num() == 0, "");
    static_assert(r1.denom() == 1, "");
    std::cout << r1 << std::endl;

    rational r2;
    ASSERT_EQ(r2.num(), 0);
    ASSERT_EQ(r2.denom(), 1);

    rational r3 = 3; 
    std::cout << r3 << std::endl;
    r2 = 2;
    auto r4 = r2/ r3;
    std::cout << r4 << std::endl;
}


TEST(numtype, int_relation){
    using value_type = decltype(int() + long());
    std::cout << std::numeric_limits<int>::max() << std::endl;
    std::cout << std::numeric_limits<long>::max() << std::endl;
    std::cout << std::numeric_limits<long long>::max() << std::endl;
    std::cout << std::numeric_limits<value_type>::max() << std::endl;

    std::cout << "digits10 of long long = "
              << std::numeric_limits<long long>::digits10 << std::endl;
    std::cout << "digits10 of int = "
              << std::numeric_limits<int>::digits10 << std::endl;

    std::cout << "digits10 of float = "
              << std::numeric_limits<float>::digits10 << std::endl;
    std::cout << "digits10 of long double = "
              << std::numeric_limits<double>::digits10 << std::endl;

    std::cout << ToString(detail::is_nonnarrowing_assignable_v<long, int>);
    std::cout << ToString(detail::is_nonnarrowing_assignable_v<int, long>);

    // typical usage
    std::cout << "pow(2, 10) = " << std::pow(2, 10) << '\n'
              << "pow(2, 0.5) = " << std::pow(2, 0.5) << '\n'
              << "pow(-2, -3) = " << std::pow(10, -3) << '\n';
}

TEST(numtype, overflow){
    int b = 10000;
    int a = std::numeric_limits<int>::max() - 10;
    std::cout << "a + b is overflow = " << ToString(CheckAddOverFlow(a,b)) << std::endl;
    auto min = std::numeric_limits<int>::min();
    b = -10;
    a = std::numeric_limits<int>::min() + 3;
    ASSERT_TRUE(CheckMultiOverFlow(a, b));
    a = -10;
    b = std::numeric_limits<int>::min();
    ASSERT_TRUE(CheckMultiOverFlow(a, b));
    a = 10;
    b = std::numeric_limits<int>::max() - 3;
    ASSERT_TRUE(CheckMultiOverFlow(a, b));
    a = -10;
    b = std::numeric_limits<int>::max() - 3;
    ASSERT_TRUE(CheckMultiOverFlow(a, b));
}

typedef Rational_<long> Rational;

TEST(num, count_digits){
    double x = 3.16890100101;
    try{
        Rational r(x, 8);
        auto sd = CountSignificanceDigit(x);
        std::cout << "Sig Digit = " << sd << std::endl;
        std::cout << r.num() << "/" << r.denom() << std::endl;
        std::cout << "double = " << std::setprecision(10) << r.to_double() << std::endl;
    }catch(std::overflow_error e){
        std::cerr<< e.what()<< std::endl;
    }
}

TEST(num, conversion_digits){
    double x = 3.1415;
    Rational r(x, 6);
    std::cout << r.num() << "/" << r.denom() << std::endl;
    std::cout << "double = " << std::setprecision(6) << float(r) << std::endl;
}


