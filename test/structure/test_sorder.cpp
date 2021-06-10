#include "gtest/gtest.h"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
// #include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";

const int fig_width  = 900;
const int fig_height = 900;

TEST(order, initial){
	typedef SGridUniform_<2> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<2, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<2, Grid, Ghost> Order;
	typedef std::shared_ptr<Order> spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGrid  spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

	spGhost spghost(new Ghost(spgrid));

	spOrder sporder(new Order(spgrid,spghost));


	std::cout << "Size = " << sporder->size() << std::endl;
	ASSERT_EQ(sporder->size(), 100);

    // get order is most important function in Order Class
	typename Grid::Index idx1(1,1);
	std::cout << idx1 << " order = " << sporder->get_order(idx1) << std::endl;
	ASSERT_EQ(sporder->get_order(idx1), 11);
	
	


}