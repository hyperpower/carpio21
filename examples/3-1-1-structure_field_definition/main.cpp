#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"

using namespace carpio;

typedef SGridUniform_<1>                     Grid1;
typedef std::shared_ptr<SGridUniform_<1> > spGrid1;

typedef SGhostRegular_<1, Grid1> Ghost1;
typedef std::shared_ptr<Ghost1> spGhost1;

typedef SOrderXYZ_<1, Grid1, Ghost1> Order1;
typedef std::shared_ptr<Order1> spOrder1;

typedef SFieldCenter_<1, double, Grid1, Ghost1, Order1> Field1;

const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 900;
const int fig_height = 900;

void Uniform1(){

	Point_<Vt, 1> pmin(0, 0, 0);
	Point_<Vt, 1> pmax(1, 1, 1);
	spGrid1 spgrid(new Grid1(pmin, {10, 10}, 0.3, 2));
    spGhost1 spghost(new Ghost1(spgrid));

	spOrder1 sporder(new Order1(spgrid,spghost));

    Field1 a(spgrid, spghost, sporder);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 3.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_aspect_ratio();
	gnu.add(ToGnuplotActorLines(*spgrid));
    gnu.set_terminal_png(OUTPUTPATH + "UniformSturctureGrid1", fig_width, fig_height);
	gnu.plot();
}

int main(int argc, char** argv) {
    Uniform1();
    
}