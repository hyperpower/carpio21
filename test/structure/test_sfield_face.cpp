#include "gtest/gtest.h"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "test_define.hpp"

using namespace carpio;

// const std::string FIG_PATH = "./fig/";

// const int fig_width  = 1200;
// const int fig_height = 900;

TEST(field_face, dim2){

    typedef SGridUniform_<2> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<2, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<2, Grid, Ghost> Order;
	typedef std::shared_ptr<Order> spOrder;

    typedef SFieldFace_<2, double, Grid, Ghost, Order> FieldFace;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));

	spGhost spghost(new Ghost(spgrid));

    FieldFace ff(spgrid, spghost, _X_);
    FieldFace fy(spgrid, spghost, _Y_);

	ff.assign([](typename FieldFace::ValueType x,
	             typename FieldFace::ValueType y,
				 typename FieldFace::ValueType z
				){
		return 0.1 * x + 0.1 * y;
	});
	
	fy.assign([](typename FieldFace::ValueType x,
	             typename FieldFace::ValueType y,
				 typename FieldFace::ValueType z
				){
		return 0.1 * x + 0.1 * y;
	});

	std::cout << ff(_M_, 2, 2 ) << std::endl;
	EXPECT_EQ(ff(_M_, 2, 2), 0.225);
	std::cout << ff(_M_, 3, 3 ) << std::endl;
	EXPECT_EQ(ff(_M_, 3, 3), 0.325);
	
	Gnuplot gnu;
	gnu.set_xrange(-0.2, 4.0);
	gnu.set_yrange(-0.2, 4.0);
	gnu.set_xlabel("X");
	gnu.set_ylabel("Y");
	gnu.set_equal_aspect_ratio();
	gnu.set_key_spacing(1.5);
	gnu.set_palette_red_blue();
	
	auto agrid = ToGnuplotActorWireFrame(*spgrid);
	agrid.title("Grid");
	agrid.line_width(2);
	gnu.add(agrid);

	gnu.set_terminal_png(FIG_PATH + "SturctureFieldFace", 
	                    fig_width, fig_height);
		
	
	auto aloc = ToGnuplotActorContourPoints(ff);
	aloc.title("value on x face");
	gnu.add(aloc);
	auto alocy = ToGnuplotActorContourPoints(fy);
	alocy.title("value on y face");
	alocy.point_type(9);
	gnu.add(alocy);
	gnu.plot();
	

}

TEST(field_face, dim1){

    typedef SGridUniform_<1> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<1, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<1, Grid, Ghost> Order;
	typedef std::shared_ptr<Order> spOrder;

    typedef SFieldFace_<1, double, Grid, Ghost, Order> FieldFace;

	Point_<Vt, 1> pmin(0, 0, 0);
	Point_<Vt, 1> pmax(1, 1, 1);
	spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));

	spGhost spghost(new Ghost(spgrid));

    FieldFace ff(spgrid, spghost, _X_);

	ff.assign([](typename FieldFace::ValueType x,
	             typename FieldFace::ValueType y,
				 typename FieldFace::ValueType z
				){
		return 1.0 * std::sin(x) + 1.0;
	});

	std::cout << "ff(_M_, 2) = " << ff(_M_, 2) << std::endl;
	
	Gnuplot gnu;
	gnu.set_xrange(-0.2, 4.0);
	gnu.set_yrange(-0.2, 3.0);
	gnu.set_xlabel("X");
	gnu.set_ylabel("Value");
	gnu.set_equal_aspect_ratio();
	gnu.set_key_spacing(1.5);
	gnu.set_palette_red_blue();
	
	auto agrid = ToGnuplotActorWireFrame(*spgrid);
	agrid.title("Grid");
	agrid.line_width(2);
	gnu.add(agrid);

	auto aidx = ToGnuplotActorLabel(ff, "index");
	gnu.add(aidx);

	gnu.set_terminal_png(FIG_PATH + "SturctureFieldFace1d", 
	                    fig_width, fig_height);
		
	
	auto aloc = ToGnuplotActorContourPoints(ff);
	aloc.title("value on x face");
	gnu.add(aloc);
	
	gnu.plot();
}

TEST(field_face, dim3){
	typedef SGridUniform_<3> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<3, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<3, Grid, Ghost> Order;
	typedef std::shared_ptr<Order> spOrder;

    typedef SFieldFace_<3, double, Grid, Ghost, Order> FieldFace;

	Point_<Vt, 3> pmin(0, 0, 0);
	Point_<Vt, 3> pmax(1, 1, 1);
	spGrid spgrid(new Grid(pmin, {7, 7, 7}, 0.5, 2));
	
	spGhost spghost(new Ghost(spgrid));

    FieldFace ff(spgrid, spghost, _X_);

	
}

