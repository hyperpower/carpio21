#include "gtest/gtest.h"
#include "algebra/array/array_list.hpp"
#include "algebra/array/multi_array.hpp"
#include "algebra/misc/linear_polynomial.hpp"

using namespace carpio;

TEST(linear_polynomial, lp){
    typedef LinearPolynomial_<double, std::string> Poly;
    Poly poly;
    poly["a"] = 1;
    poly["b"] = 2;
    poly["b"] +=3;

    Poly pb;
    pb = poly;

    pb += 3;
    EXPECT_EQ(1, pb["a"]);
    EXPECT_EQ(5, pb["b"]);
    EXPECT_EQ(3, pb.value());

    pb -= 5;
    EXPECT_EQ(1,  pb["a"]);
    EXPECT_EQ(5,  pb["b"]);
    EXPECT_EQ(-2, pb.value());

    pb *= 2;
    EXPECT_EQ(2,  pb["a"]);
    EXPECT_EQ(10, pb["b"]);
    EXPECT_EQ(-4, pb.value());

    pb /= 2;
    EXPECT_EQ(1,  pb["a"]);
    EXPECT_EQ(5,  pb["b"]);
    EXPECT_EQ(-2, pb.value());
}


TEST(linear_polynomial, op_add2){
    typedef LinearPolynomial_<double, std::string> Poly;
    Poly poly;
    poly["a"] = 1;
    poly["b"] = 2;
    poly["c"] = 3;
    std::cout << "poly :\n";
    std::cout << poly << std::endl;
    Poly pb;
    pb = poly;
    pb += 3;
    pb["c"] = 5;
    pb["d"] =-2;
    std::cout << "pb   :\n";
    std::cout << pb << std::endl;

    std::cout << " pb + poly \n";
    std::cout << pb + poly << std::endl;

    std::cout << " pb - poly \n";
    std::cout << pb - poly << std::endl;
    std::cout << " poly - pb \n";
    std::cout << poly - pb << std::endl;

    std::cout << " pb - 3 \n";
    std::cout << pb - 300.0 << std::endl;
    std::cout << " 100 - pb \n";
    std::cout << 100.0 - pb << std::endl;
}


TEST(linear_polynomial, op_multi){
    typedef LinearPolynomial_<double, std::string> Poly;
    Poly poly;
    poly["a"] = 1;
    poly["b"] = 2;
    poly["c"] = 3;
    std::cout << "poly :\n";
    std::cout << poly << std::endl;
    Poly pb;
    pb = poly;
    pb += 3;
    pb["c"] = 5;
    pb["d"] =-2;
    std::cout << "pb   :\n";
    std::cout << pb << std::endl;

    std::cout << " pb * 2\n";
    std::cout << pb * 2.0 << std::endl;

    std::cout << " 3 * pb\n";
    std::cout << 3.0 * pb << std::endl;

    std::cout << " pb / 2\n";
    std::cout << pb / 2.0 << std::endl;
}

TEST(linear_polynomial, op_add_term){
    typedef LinearPolynomial_<double, std::string> Poly;
    Poly poly;
    poly["a"] = 1;
    poly["b"] = 2;
    poly["c"] = 3;
    std::cout << "poly :\n";
    std::cout << poly << std::endl;
    Poly pb;
    pb = poly;
    pb += 3;
    pb["c"] = 5;
    pb["d"] =-2;
    std::cout << "pb   :\n";
    std::cout << pb << std::endl;

    std::cout << " pb + \"c\"\n";
    std::string str = "c";
    std::cout << pb + str << std::endl;

    std::cout << " pb + \"e\"\n";
    str = "e";
    std::cout << pb + str << std::endl;

    std::cout << " \"c\" + pb\n";
    str = "c";
    std::cout << str + pb << std::endl;

    std::cout << " \"e\" + pb\n";
    str = "e";
    std::cout << str + pb << std::endl;
}

TEST(linear_polynomial, arr){
    typedef LinearPolynomial_<double, std::string> Poly;

	ArrayListV_<double> arrd(5);
	ArrayListV_<Poly> arrexp(5);
    
    arrd.assign(1.0);

    Poly poly;
    poly["a"] = 1;
    poly["b"] = 2;
    poly["c"] = 3;

    arrexp.assign(poly);

	auto res =  arrexp + arrd;

    std::cout << "arrd[1] = " << arrd[1] << std::endl;
	std::cout << "arrexp[1]  = "<< arrexp[1] << std::endl;
	std::cout << "res[1]  = " << res[1] << std::endl;
}
TEST(linear_polynomial, multiarry){
    typedef LinearPolynomial_<double, std::string> Poly;

	MultiArrayV_<double, 2> arrd(5,5);
	MultiArrayV_<Poly,   2> arrexp(5,5);
    
    arrd.assign(1.0);

    Poly poly;
    poly["a"] = 1;
    poly["b"] = 2;
    poly["c"] = 3;

    arrexp.assign(poly);

	auto res =  arrexp + arrd;

    std::cout << "arrd[1] = " << arrd(1,1) << std::endl;
	std::cout << "arrexp[1]  = "<< arrexp(1,1) << std::endl;
	std::cout << "res[1]  = " << res(1,1) << std::endl;
}