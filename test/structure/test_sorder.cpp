#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"



using namespace carpio;

// const std::string OUTPUTPATH = "./fig/";

// const int fig_width  = 900;
// const int fig_height = 900;

TEST(order, a_test){
    if(TestDir() == false){
        std::abort();
    }
}
TEST(order, initial){
	typedef SGridUniform_<2> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<2, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<2, Grid, Ghost, CenterTag> Order;
	typedef std::shared_ptr<Order> spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGrid  spgrid(new Grid(pmin, {10, 10}, 0.3, 2));

	spGhost spghost(new Ghost(spgrid));


	spOrder sporder(new Order(spgrid, spghost));

	std::cout << "here" << std::endl;

	std::cout << "Size = " << sporder->size() << std::endl;
	ASSERT_EQ(sporder->size(), 100);

    // get order is most important function in Order Class
	typename Grid::Index idx1(1,1);
	std::cout << idx1 << " order = " << sporder->get_order(idx1) << std::endl;
	ASSERT_EQ(sporder->get_order(idx1), 11);
}

TEST(order, plot_order){
	typedef SGridUniform_<2> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<2, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<2, Grid, Ghost, CenterTag> Order;
	typedef std::shared_ptr<Order> spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGrid  spgrid(new Grid(pmin, {5, 5}, 0.3, 2));

	spGhost spghost(new Ghost(spgrid));

	spOrder sporder(new Order(spgrid, spghost));

	// typedef SGnuplotActor_<2> GA;
	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png(FIG_PATH + "GridOrder", fig_width, fig_height);
	gnu.set_xrange(-0.2, 1.7);
	gnu.set_yrange(-0.2, 1.7);
	gnu.add(ToGnuplotActorWireFrame(*spgrid));
	auto a = ToGnuplotActorLabel(*sporder, "order");
	gnu.add(a);
	gnu.plot();
}

TEST(order, order_vertex){
	typedef SGridUniform_<2> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<2, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<2, Grid, Ghost, VertexTag> Order;
	typedef std::shared_ptr<Order> spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGrid  spgrid(new Grid(pmin, {5, 5}, 0.3, 2));

	spGhost spghost(new Ghost(spgrid));

	spOrder sporder(new Order(spgrid,spghost));

	ASSERT_EQ(sporder->get_order(3, 3), 15);
	ASSERT_EQ(sporder->get_order(0, 0), 0);

	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png(FIG_PATH + "GridOrderVertex", fig_width, fig_height);
	gnu.set_xrange(-0.2, 1.7);
	gnu.set_yrange(-0.2, 1.7);
	gnu.add(ToGnuplotActorWireFrame(*spgrid));
	auto a = ToGnuplotActorLabel(*sporder, "order");
	gnu.add(a);
	gnu.plot();
}


