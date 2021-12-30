
#include "domain/domain.hpp"
#include "gtest/gtest.h"

using namespace carpio;

TEST(domain, initial){
    const int dim = 2;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    typedef Point_<double,dim> Point;
    Point p(0,0,0);
    typedef SGridUniform_<dim> Grid;
    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef SOrderXYZ_<dim, Grid, Ghost> Order;

    std::shared_ptr<Grid>  spgrid(new Grid(p,10, 1, 2));
    std::shared_ptr<Ghost> spghost(new Ghost(spgrid));
    std::shared_ptr<Order> sporder(new Order(spgrid, spghost));

    typedef LinearPolynomial_<Vt, typename Grid::Index> Poly;

    typedef SFieldCenter_<dim, Poly, Grid, Ghost, Order> Field;

    Field f(spgrid, spghost, sporder);

    // auto g = f.ghost();

    BoundaryIndex bi;

    bi.show();

    auto lap = Laplacian(f, bi);
    for(auto& idx : lap.order()){
        std::cout << idx << " --------" << std::endl;
        std::cout << lap(idx) << " --------" << std::endl;
    }

}
