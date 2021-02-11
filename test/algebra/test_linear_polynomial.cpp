#include "gtest/gtest.h"
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
	std::cout << "pb += 3\n";
	std::cout << pb << std::endl;

	pb -= 5;
	std::cout << "pb -= 5\n";
	std::cout << pb << std::endl;

	pb *= 2;
	std::cout << "pb *= 2\n";
	std::cout << pb << std::endl;

	pb /= 3;
	std::cout << "pb /= 3\n";
	std::cout << pb << std::endl;

}


TEST(linear_polynomial, op_add){
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

	std::cout << " pb += poly \n";
	pb += poly;
	std::cout << pb << std::endl;

	std::cout << " pb -= poly \n";
	pb -= poly;
	pb -= poly;
	std::cout << pb << std::endl;
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