#include "domain/domain.hpp"
#include "equation/equation.hpp"
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

using namespace carpio;

TEST(equation, equation_base){
    std::cout << "Equation Test"<<std::endl;
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

    typedef SField_<dim, Vt, Grid, Ghost, Order> Field;

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef EquationBase_<dim, Domain> EquationBase;

    // EquationBase equ(spgrid, spghost, sporder);
    // auto s = Stringify(*spgrid);
    // for(auto& row : s){
        // std::cout << row << std::endl;
    // }
    // OutputFile("./test_output/grid.txt", *spgrid);

    // TextFile f("./test_output/grid.txt");
    // f.read();
    // f.parse_config();
    // f.show_config();
}

TEST(equation, laplace){
    std::cout << "[  Laplace ] Test"<<std::endl;
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

    typedef SField_<dim, Vt, Grid, Ghost, Order> Field;

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;

    typedef Laplace_<dim, Domain> Laplace;

    Laplace equ(spgrid, spghost, sporder);

    equ.run();


}