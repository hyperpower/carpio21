#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";
const int fig_width  = 800;
const int fig_height = 600;
const std::size_t dim = 2;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost> Order;
typedef std::shared_ptr<Order> spOrder;

typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;

void ExactSolution(){

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 100;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x()/double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field a(spgrid, spghost, sporder);

    a.assign([](typename Field::ValueType x,
                typename Field::ValueType y,
                typename Field::ValueType z,
                double t){
        return std::sin(2 * _PI_ * x) * std::sin(2 * _PI_ * y);
    });

    Gnuplot gnu;
    gnu.set_xrange(-0.1, 1.1);
    gnu.set_yrange(-0.1, 1.1);
    gnu.set_ylabel("y");
    gnu.set_xlabel("x");
    // gnu.set_zrange(-1.0, 1.0);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_blue_red();
    // gnu.set_ticslevel(0.1);
    // gnu.set_view(55, 25 ,1.2 ,1.0);
    gnu.add(ToGnuplotActorContour(a));
    gnu.set_terminal_png(OUTPUTPATH + "ExactSolutionContour", 
                        fig_width, fig_height);
    gnu.plot();
}



int main(int argc, char** argv) {
    ExactSolution();
}