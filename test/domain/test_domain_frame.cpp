
#include "domain/domain.hpp"
#include "gtest/gtest.h"

using namespace carpio;

TEST(domain, initial){
    std::cout << "== Domain test ==" << std::endl;
    typedef SIndex_<1> Index;
    typedef SGrid_<1, Index> Grid;
    typedef SGhost_<1, Grid> Ghost;
    typedef SOrder_<1, Grid, Ghost> Order;

    Grid  grid;
    Ghost ghost;
    Order order;

    typedef SField_<1, Vt, Grid, Ghost, Order> Field;

    Field f;

    Laplacian(f, 3, "method");

}
