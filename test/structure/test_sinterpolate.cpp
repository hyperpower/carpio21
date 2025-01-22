#include "gtest/gtest.h"
#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/sinterpolate.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 1200;
const int fig_height = 900;

TEST(s_interpolate, dim2){

    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<2, double, Grid, Ghost, Order> Field;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field a(spgrid, spghost, sporder);

    a.assign([](typename Field::ValueType x,
                typename Field::ValueType y,
                typename Field::ValueType z
                ){
        return std::sin(2 * _PI_ * x) * std::sin(2 * _PI_ * y);
    });

    auto fa = InterpolateCenterToFace(a, _X_);
    
    Gnuplot gnu;
    gnu.set_xrange(-0.2, 4.0);
    gnu.set_yrange(-0.2, 4.0);
    gnu.set_xlabel("X");
    gnu.set_ylabel("Y");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    gnu.set_palette_blue_red();
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    gnu.add(agrid);

    gnu.set_terminal_png(OUTPUTPATH + "Iner_CenterToFace_2d", 
                        fig_width, fig_height);
    
    auto aloc = ToGnuplotActorPointContour(fa);
    aloc.title("value on x face");
    gnu.add(aloc);
    auto ac = ToGnuplotActorPointContour(a);
    ac.title("value on x center");
    gnu.add(ac);
    gnu.plot();
}

TEST(s_interpolate, dim1){
    typedef SGridUniform_<1> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<1, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<1, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<1, double, Grid, Ghost, Order> Field;
    typedef typename Field::Index Index;
    typedef SFieldFace_<1, double, Grid, Ghost, Order> FieldFace;

    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));
    spGhost spghost(new Ghost(spgrid));

    Field f(spgrid, spghost);
    // FieldFace ff(spgrid, spghost, _X_);

    f.assign([](typename FieldFace::ValueType x,
                typename FieldFace::ValueType y,
                typename FieldFace::ValueType z
            ){
        return 1.0 * std::sin(x) + 1.0;
    });

    BoundaryIndex bi;
    auto spbcym = std::make_shared<BoundaryConditionValue>(1, 0.5);
    auto spbcyp = std::make_shared<BoundaryConditionValue>(1, 1.5);
    bi.insert(0, spbcym);
    bi.insert(1, spbcyp);
    // bi.insert(0, );;
    bi.show();

    auto ff = InterpolateCenterToFace(f, bi, _X_);

    Gnuplot gnu;
	gnu.set_xrange(-0.2, 4.0);
	gnu.set_yrange(-0.2, 3.0);
	gnu.set_xlabel("X");
	gnu.set_ylabel("Value");
	gnu.set_equal_aspect_ratio();
	gnu.set_key_spacing(1.5);
	gnu.set_palette_blue_red();
	
	auto agrid = ToGnuplotActorWireFrame(*spgrid);
	agrid.title("Grid");
	agrid.line_width(2);
	gnu.add(agrid);

	auto aidx = ToGnuplotActorLabel(f, "index");
	gnu.add(aidx);

	gnu.set_terminal_png(OUTPUTPATH + "Iner_CenterToFace_1d", 
	                    fig_width, fig_height);
	
	auto aloc = ToGnuplotActorPointContour(ff);
	aloc.title("value on x face");
	gnu.add(aloc);
	auto ac = ToGnuplotActorPointContour(f);
    ac.title("value on x center");
    gnu.add(ac);
	gnu.plot();
}