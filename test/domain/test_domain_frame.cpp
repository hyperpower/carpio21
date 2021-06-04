
#include "domain/domain.hpp"
#include "gtest/gtest.h"

using namespace carpio;

TEST(domain, initial){
    const int dim = 1;
    std::cout << "[   INFO   ] Dim = " << 1 << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid> spgrid(new Grid(p,10, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef SField_<1, Vt, Grid, Ghost, Order> Field;

    Field f;

    Laplacian(f, 3, "method");

}
