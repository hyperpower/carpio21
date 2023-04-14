#include "algebra/number/number.hpp"
#include "io/io_define.hpp"

#include "gtest/gtest.h"

using namespace carpio;


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

typedef Rational_<int> Rational;

TEST(num, count_digits){
    double x = 3.14159268923;
    Rational r(x, 8);
    std::cout << r.num() << "/" << r.denom() << std::endl;
    std::cout << "double = " << std::setprecision(10) << r.to_double() << std::endl;
}