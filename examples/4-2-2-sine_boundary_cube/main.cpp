#include "fv.h"
#include "fd.h"

using namespace carpio;


int main(int argc, char** argv) {
    fv::ExactSolution();
    fv::AScheme("finite_volume_2");
    fd::AScheme("finite_difference_2");

    // std::list<double> l1,l2,li;
    // std::list<std::list<double> > lr;
    // for(auto& n : vn){
        // fd::LaplaceSolver("finite_difference_2", 8, l1, l2, li, lr);
    // }
}