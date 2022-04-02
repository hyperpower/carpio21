#include "gtest/gtest.h"
#include <math.h>
#include "algebra/array/array_list.hpp"
#include "io/python_interpreter.hpp"
// #include "io/plotly.hpp"

using namespace carpio;

TEST(test_p1, test1) {
    std::cout << "test1" << std::endl;
    auto p = PythonInterpreter::Get();
    // Plotly plotly;
}

TEST(test_plotly, test2) {
    std::cout << "aa" << std::endl;
}