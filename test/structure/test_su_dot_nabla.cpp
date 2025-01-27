#include "gtest/gtest.h"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "domain/structure/operator/su_dot_nabla.hpp"

#include "test_define.hpp"

using namespace carpio;

TEST(su_dot_nabla, dim2){
    typedef SGridUniform_<2> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SGhostRegular_<2, Grid> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef SOrderXYZ_<2, Grid, Ghost> Order;
	typedef std::shared_ptr<Order> spOrder;

    typedef SFieldFace_<2, double, Grid, Ghost, Order> FieldFace;
    typedef std::shared_ptr<FieldFace> spFieldFace;
    typedef SVectorFace_<2, double, Grid, Ghost, Order> VectorFace;
    typedef SFieldCenter_<2, double, Grid, Ghost, Order> FieldCenter;
    typedef std::shared_ptr<FieldCenter> spFieldCenter;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));

	spGhost spghost(new Ghost(spgrid));

    spFieldFace spfx = std::make_shared<FieldFace>(spgrid, spghost, _X_);
    spFieldFace spfy = std::make_shared<FieldFace>(spgrid, spghost, _Y_);

    spfx->assign([](typename FieldFace::ValueType x,
	                typename FieldFace::ValueType y,
				    typename FieldFace::ValueType z
				){
		return 0.1 * x + 0.1 * y;
	});

    spfy->assign([](typename FieldFace::ValueType x,
	             typename FieldFace::ValueType y,
				 typename FieldFace::ValueType z
				){
		return 0.1 * x + 0.1 * y;
	});

    VectorFace vf(spfx, spfy);
    FieldCenter phi(spgrid, spghost);

    BoundaryIndex bi;
    auto spbcym = std::make_shared<BoundaryConditionValue>(1, 0.5);
    auto spbcyp = std::make_shared<BoundaryConditionValue>(1, 1.5);
    bi.insert(0, spbcym);
    bi.insert(1, spbcyp);
    // bi.insert(0, );;
    bi.show();

    std::cout << vf(_X_, _M_, 2, 2 ) << std::endl;
	EXPECT_EQ(vf(_X_, _M_, 2, 2), 0.225);
	std::cout << vf(_Y_, _M_, 3, 3 ) << std::endl;
	EXPECT_EQ(vf(_Y_, _M_, 3, 3), 0.325);

    UdotNabla(vf, phi, bi);

    
} 