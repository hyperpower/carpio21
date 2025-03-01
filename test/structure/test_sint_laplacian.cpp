#include "gtest/gtest.h"
#include "domain/base/base_operator.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/slaplacian_integral.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "test_define.hpp"
using namespace carpio;



TEST(s_lap, a_test){
    if(TestDir() == false){
        std::abort();
    }
}

TEST(s_lap, dim2){
    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost, CenterTag> Order;
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

    BoundaryIndex bi;
    auto spbcym = std::make_shared<BoundaryConditionValue>(1, 0.5);
    auto spbcyp = std::make_shared<BoundaryConditionValue>(1, 1.5);
    bi.insert(0, spbcym);
    bi.insert(1, spbcyp);
    // bi.insert(0, );;
    bi.show();

    auto res = IntegralLaplacian(a, bi);
    
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

    gnu.set_terminal_png(FIG_PATH + "Int_Laplacian_2d", 
                        fig_width, fig_height);
    
    auto aloc = ToGnuplotActorContourPoints(res);
    aloc.title("value on x face");
    gnu.add(aloc);
    auto ac = ToGnuplotActorContourPoints(a);
    ac.title("value on x center");
    gnu.add(ac);
    gnu.plot();
}

TEST(s_lap, HOC4_op){
    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost, CenterTag> Order;
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

    BoundaryIndex bi;
    auto spbcym = std::make_shared<BoundaryConditionValue>(1, 0.5);
    auto spbcyp = std::make_shared<BoundaryConditionValue>(1, 1.5);
    bi.insert(0, spbcym);
    bi.insert(1, spbcyp);
    // bi.insert(0, );;
    bi.show();

    auto res = IntegralLaplacian(a, bi);
    
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

    gnu.set_terminal_png(FIG_PATH + "Int_Laplacian_2d", 
                        fig_width, fig_height);
    
    auto aloc = ToGnuplotActorContourPoints(res);
    aloc.title("value on x face");
    gnu.add(aloc);
    auto ac = ToGnuplotActorContourPoints(a);
    ac.title("value on x center");
    gnu.add(ac);
    gnu.plot();
}