#include "gtest/gtest.h"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
// #include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";

const int fig_width  = 900;
const int fig_height = 900;

TEST(field, initial){
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
	
	a = a * a;
	EXPECT_EQ (4.0, a(idx));

	a = a / 2.0;
	EXPECT_EQ (2.0, a(idx));

	a = 20 / a;
	EXPECT_EQ (10, a(idx));

	Field b(spgrid, spghost, sporder);
	b +=2;

	Field c(a + b);
	EXPECT_EQ (12.0, c(idx));
}