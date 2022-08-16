#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"

using namespace carpio;



const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 800;
const int fig_height = 600;

void Uniform1(){
	typedef SGridUniform_<1>                     Grid1;
	typedef std::shared_ptr<SGridUniform_<1> > spGrid1;

	typedef SGhostRegular_<1, Grid1> Ghost1;
	typedef std::shared_ptr<Ghost1> spGhost1;

	typedef SOrderXYZ_<1, Grid1, Ghost1> Order1;
	typedef std::shared_ptr<Order1> spOrder1;

	typedef SFieldCenter_<1, double, Grid1, Ghost1, Order1> Field1;

	Point_<Vt, 1> pmin(0, 0, 0);
	Point_<Vt, 1> pmax(1, 1, 1);
	spGrid1 spgrid(new Grid1(pmin, {5, 5}, 0.5, 2));
    spGhost1 spghost(new Ghost1(spgrid));

	spOrder1 sporder(new Order1(spgrid,spghost));

    Field1 field(spgrid, spghost, sporder);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 3.5);
	gnu.set_yrange(-0.5, 2.0);
	gnu.set_xlabel("Location");
	gnu.set_ylabel("Scalar");
	gnu.set_equal_aspect_ratio();
	gnu.set_key_spacing(1.5);
	
	auto agrid = ToGnuplotActorLines(*spgrid);
	agrid.title("Grid");
	agrid.line_width(2);
	gnu.add(agrid);

	//center location
	std::list<double> lx, ly;
	for(auto& index : field.order()){
		auto p = field.grid().c(index);
		lx.push_back(p.x());
		ly.push_back(0.0);
	}
	auto aloc = ToGnuplotActor(lx, ly);
	aloc.title("Center scalar location");
	aloc.style("with points ps 3 pt 7");
	gnu.add(aloc);

	//scalar on field
	std::list<double> vx, vy;
	for(auto& index : field.order()){
		auto p = field.grid().c(index);
		field(index) = std::sin(p.x());
		vx.push_back(p.x());
		vy.push_back(field(index));
	}
	auto ascalar = ToGnuplotActor(vx, vy);
	ascalar.title("Scalar");
	ascalar.style("with points ps 3 pt 9");
	gnu.add(ascalar);


    gnu.set_terminal_png(OUTPUTPATH + "UniformSturctureGrid1", fig_width, fig_height);
	gnu.plot();
}

int main(int argc, char** argv) {
    Uniform1();
    
}