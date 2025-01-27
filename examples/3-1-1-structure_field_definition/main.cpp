#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/grid/nonuniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/base/base_io.hpp"
#include "domain/structure/io/s_stringify.hpp"

#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "dim1.h"
#include "dim2.h"
#include "dim3.h"

using namespace carpio;



int main(int argc, char** argv) {
    std::cout << "-------- Main ------" << std::endl;
    Uniform1();
    Uniform2();
    Uniform3();
    
    NonUniform1();
    NonUniform2();
    NonUniform3();
}