#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/equation.hpp"

#include "fv.h"
#include "fd.h"


using namespace carpio;


int main(int argc, char** argv) {
    fv::ExactSolution();
    // // fv::AScheme("finite_volume_2");
    fd::AScheme("finite_difference_2");
    // fd::AScheme("HOC4");
}