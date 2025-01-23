#include "gtest/gtest.h"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 1200;
const int fig_height = 900;

TEST(field, DISABLED_init){
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

    typename Grid::Index idx(1,2);
    std::cout << "Access value    " << idx << "  = " << a(idx) << std::endl; 

    a += 1;
    EXPECT_EQ (1.0, a(idx));
    
    a += a;
    EXPECT_EQ (2.0, a(idx));

    a -= 1;
    EXPECT_EQ (1.0, a(idx));

    a = a + a;
    EXPECT_EQ (2.0, a(idx));
    
    a = a / 2.0;
    EXPECT_EQ (2.0, a(idx));

    a = 20 / a;
    EXPECT_EQ (10, a(idx));

    Field b(spgrid, spghost, sporder);
    b +=2;

    Field c(a + b);
    EXPECT_EQ (12.0, c(idx));
}


TEST(field, add){
    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef typename Grid::Index Index;

    typedef LinearPolynomial_<double, typename Grid::Index> Exp;
    typedef SFieldCenter_<2, double, Grid, Ghost, Order> FieldV;
    typedef SFieldCenter_<2, Exp, Grid, Ghost, Order>    FieldExp;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    FieldV a(spgrid, spghost, sporder);
    a.assign(2);

    FieldExp e(spgrid, spghost, sporder);
    e.assign([](const FieldExp::Index& idx){return FieldExp::ValueType(idx);});

    typename Grid::Index idx(1,2);
    std::cout << "Access value a   " << idx << "  = " << a(idx) << std::endl; 
    std::cout << "Access value aexp" << idx << "  = " << e(idx) << std::endl; 
    
    auto a2 = a + a;
    EXPECT_EQ (4.0, a2(idx));
    auto av3 = a + 3;
    EXPECT_EQ (av3(idx), 5);
    auto v3a = 3.0 + a;
    EXPECT_EQ (5.0, v3a(idx));
    std::cout << "exp field + value field" << std::endl;
    auto ea = e + a;
    std::cout << ea(idx) << std::endl;
    EXPECT_EQ (ea(idx), e(idx) + a(idx) );
    auto ae  = a + e;
    // std::cout << ae(idx) << std::endl;
    EXPECT_EQ (ae(idx), e(idx) + a(idx) );
    auto ev2  = e + 2.0;
    EXPECT_EQ (ev2(idx), e(idx) + 2.0 );
    auto v2e = 2 + e;
    EXPECT_EQ (v2e(idx), e(idx) + 2.0);
    auto e2 = e + e;
    EXPECT_EQ (e2(idx), e(idx) + e(idx));

    Exp vexp(Index(1, 1));
    auto eve = e + vexp;
    EXPECT_EQ (eve(idx), e(idx) + vexp);
    auto vee = vexp + e;
    EXPECT_EQ (vee(idx), e(idx) + vexp);

    auto ce = vexp + e + a + 3.0;
    std::cout << "ce(idx) = " << ce(idx) << std::endl;

}

TEST(field, minus){
    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef typename Grid::Index Index;

    typedef LinearPolynomial_<double, typename Grid::Index> Exp;
    typedef SFieldCenter_<2, double, Grid, Ghost, Order> FieldV;
    typedef SFieldCenter_<2, Exp, Grid, Ghost, Order>    FieldExp;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    FieldV a(spgrid, spghost, sporder);
    a.assign(2);

    FieldExp e(spgrid, spghost, sporder);
    e.assign([](const FieldExp::Index& idx){return FieldExp::ValueType(idx);});

    typename Grid::Index idx(1,2);
    std::cout << "Access value a   " << idx << "  = " << a(idx) << std::endl; 
    std::cout << "Access value aexp" << idx << "  = " << e(idx) << std::endl; 
    
    auto a2 = a - a;
    EXPECT_EQ (0.0, a2(idx));
    auto av3 = a - 3;
    EXPECT_EQ (av3(idx), -1);
    auto v3a = 3.0 - a;
    EXPECT_EQ (1, v3a(idx));
    std::cout << "exp field + value field" << std::endl;
    auto ea = e - a;
    std::cout << ea(idx) << std::endl;
    EXPECT_EQ (ea(idx), e(idx) - a(idx) );
    auto ae  = a - e;
    // std::cout << ae(idx) << std::endl;
    EXPECT_EQ (ae(idx), a(idx) - e(idx) );
    auto ev2  = e - 2.0;
    EXPECT_EQ (ev2(idx), e(idx) - 2.0 );
    auto v2e = 2 - e;
    EXPECT_EQ (v2e(idx), (2.0 - e(idx)));
    auto e2 = e - e;
    EXPECT_EQ (e2(idx), e(idx) - e(idx));

    Exp vexp(Index(1, 1));
    auto eve = e - vexp;
    EXPECT_EQ (eve(idx), e(idx) - vexp);
    auto vee = vexp - e;
    EXPECT_EQ (vee(idx), vexp - e(idx));

    auto ce = vexp + e + a + 3.0;
    std::cout << "ce(idx) = " << ce(idx) << std::endl;

}

TEST(field, mutiply){
    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef typename Grid::Index Index;

    typedef LinearPolynomial_<double, typename Grid::Index> Exp;
    typedef SFieldCenter_<2, double, Grid, Ghost, Order> FieldV;
    typedef SFieldCenter_<2, Exp, Grid, Ghost, Order>    FieldExp;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    FieldV a(spgrid, spghost, sporder);
    a.assign(2);

    FieldExp e(spgrid, spghost, sporder);
    e.assign([](const FieldExp::Index& idx){return FieldExp::ValueType(idx);});

    typename Grid::Index idx(1,2);
    std::cout << "Access value a   " << idx << "  = " << a(idx) << std::endl; 
    std::cout << "Access value aexp" << idx << "  = " << e(idx) << std::endl; 
    
    auto a2 = a * a;
    EXPECT_EQ (4.0, a2(idx));
    auto av3 = a * 3;
    EXPECT_EQ (6.0, av3(idx));
    auto v3a = 3.0 * a;
    EXPECT_EQ (6.0, v3a(idx));
    auto ea = e * a;
    // std::cout << ea(idx) << std::endl;
    EXPECT_EQ (ea(idx), e(idx) * a(idx) );
    auto ae  = a * e;
    // std::cout << ae(idx) << std::endl;
    EXPECT_EQ (ae(idx), e(idx) * a(idx) );
    auto e2  = e * 2.0;
    EXPECT_EQ (e2(idx), e(idx) * 2.0 );
    auto v2e = 2 * e;
    EXPECT_EQ (v2e(idx), e(idx) * 2.0);
    
}
TEST(field, coutour){
    const std::size_t dim = 2;
    typedef SGridUniform_<dim> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 50;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x()/double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    Field a(spgrid, spghost, sporder);

    a.assign([](typename Field::ValueType x,
                typename Field::ValueType y,
                typename Field::ValueType z
                ){
        return std::sin(2 * _PI_ * x) * std::sin(2 * _PI_ * y);
    });

    Gnuplot gnu;
    gnu.set_xrange(-0.0, 1.0);
    gnu.set_yrange(-0.0, 1.0);
    gnu.set_zrange(-1.0, 1.0);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_blue_red();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25 ,1.2 ,1.0);
    gnu.add(ToGnuplotActorContourWire(a));
    gnu.set_terminal_png(OUTPUTPATH + "FieldCenterContour", 
                        fig_width, fig_height);
    gnu.splot();
}

TEST(field, divide){
    typedef SGridUniform_<2> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<2, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<2, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef typename Grid::Index Index;

    typedef LinearPolynomial_<double, typename Grid::Index> Exp;
    typedef SFieldCenter_<2, double, Grid, Ghost, Order> FieldV;
    typedef SFieldCenter_<2, Exp, Grid, Ghost, Order>    FieldExp;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    FieldV a(spgrid, spghost, sporder);
    a.assign(2);

    FieldExp e(spgrid, spghost, sporder);
    e.assign([](const FieldExp::Index& idx){return FieldExp::ValueType(idx);});

    typename Grid::Index idx(1,2);
    std::cout << "Access value a   " << idx << "  = " << a(idx) << std::endl; 
    std::cout << "Access value aexp" << idx << "  = " << e(idx) << std::endl; 
    
    auto a2 = a / a;
    EXPECT_EQ (1, a2(idx));
    auto av3 = a / 3;
    EXPECT_EQ (av3(idx), 2.0 / 3.0);
    auto v3a = 3.0 / a;
    EXPECT_EQ (3 / 2.0, v3a(idx));
    std::cout << "exp field + value field" << std::endl;
    auto ea = e / a;
    std::cout << ea(idx) << std::endl;
    EXPECT_EQ (ea(idx), e(idx) / a(idx) );
    // auto ae  = a / e;
    // EXPECT_EQ (ae(idx), a(idx) / e(idx) );
    auto ev2  = e / 2.0;
    EXPECT_EQ (ev2(idx), e(idx) / 2.0 );
    // auto v2e = 2 / e;
    // EXPECT_EQ (v2e(idx), (2.0 / e(idx)));
    // auto e2 = e / e;
    // EXPECT_EQ (e2(idx), e(idx) / e(idx));

}


TEST(field, different_type){
    const std::size_t dim = 2;
    typedef SGridUniform_<dim> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef LinearPolynomial_<double, typename Grid::Index> Exp;
    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> FieldV;
    typedef SFieldCenter_<dim, Exp, Grid, Ghost, Order>    FieldExp;
    
    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 10;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x()/double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid,spghost));

    FieldV   fv(spgrid, spghost, sporder);
    fv.assign(2);
    FieldExp fexp(spgrid, spghost, sporder);
    
    typename Grid::Index idx(1,2);
    std::cout << "fv(" << idx << ")    = " << fv(idx) << std::endl; 
    std::cout << "fexp(" << idx << ")  = " << fexp(idx) << std::endl; 
    
    std::cout << "fexp + fv" << std::endl; 
    auto res  = fexp + fv;
    std::cout << "res(" << idx << ")  = " << res(idx) << std::endl; 
    std::cout << "fv + fexp " << std::endl; 
    auto res2 = fv + fexp;
    std::cout << "res(" << idx << ")  = " << res2(idx) << std::endl; 
}

