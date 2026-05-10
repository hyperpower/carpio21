#include "gtest/gtest.h"

#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/stencil/sstencil_2x2.hpp"

using namespace carpio;

namespace {

using Grid = SGridUniform_<2>;
using Ghost = SGhostRegular_<2, Grid>;
using Order = SOrderXYZ_<2, Grid, Ghost, CenterTag>;
using Field = SFieldCenter_<2, double, Grid, Ghost, Order>;
using Index = Grid::Index;

class TestStencil2x2 : public SStencil2x2_<double> {
public:
    using Base = SStencil2x2_<double>;
    using Base::Base;

    const StencilIndices& relative_idx() const {
        return this->_relative_idx;
    }

    const StencilValue& values() const {
        return this->_val;
    }
};

Field make_field() {
    using spGrid = std::shared_ptr<Grid>;
    using spGhost = std::shared_ptr<Ghost>;
    using spOrder = std::shared_ptr<Order>;

    Point_<Vt, 2> pmin(0, 0, 0);
    spGrid spgrid(new Grid(pmin, {4, 4}, 1.0, 1));
    spGhost spghost(new Ghost(spgrid));
    spOrder sporder(new Order(spgrid, spghost));

    Field field(spgrid, spghost, sporder);
    field.assign([](const Index& idx) {
        return 10.0 * idx.i() + idx.j();
    });
    return field;
}

} // namespace

TEST(sstencil_2x2, set_value_pp_corner) {
    auto field = make_field();
    BoundaryIndex bi;

    TestStencil2x2 stencil(CornerDirection(_X_, _P_, _Y_, _P_));
    stencil.set_value(field, Index(1, 1), bi);

    const auto& idx = stencil.relative_idx();
    EXPECT_EQ(Index(0, 0), idx[0]);
    EXPECT_EQ(Index(1, 0), idx[1]);
    EXPECT_EQ(Index(0, 1), idx[2]);
    EXPECT_EQ(Index(1, 1), idx[3]);

    const auto& val = stencil.values();
    EXPECT_DOUBLE_EQ(11.0, val[0]);
    EXPECT_DOUBLE_EQ(21.0, val[1]);
    EXPECT_DOUBLE_EQ(12.0, val[2]);
    EXPECT_DOUBLE_EQ(22.0, val[3]);
}

TEST(sstencil_2x2, set_value_mm_corner) {
    auto field = make_field();
    BoundaryIndex bi;

    TestStencil2x2 stencil(CornerDirection(_X_, _M_, _Y_, _M_));
    stencil.set_value(field, Index(2, 2), bi);

    const auto& idx = stencil.relative_idx();
    EXPECT_EQ(Index(0, 0), idx[0]);
    EXPECT_EQ(Index(-1, 0), idx[1]);
    EXPECT_EQ(Index(0, -1), idx[2]);
    EXPECT_EQ(Index(-1, -1), idx[3]);

    const auto& val = stencil.values();
    EXPECT_DOUBLE_EQ(22.0, val[0]);
    EXPECT_DOUBLE_EQ(12.0, val[1]);
    EXPECT_DOUBLE_EQ(21.0, val[2]);
    EXPECT_DOUBLE_EQ(11.0, val[3]);
}

TEST(sstencil_2x2, set_value_averages_diagonal_when_adjacent_points_are_ghost) {
    auto field = make_field();
    BoundaryIndex bi;
    bi.insert(1, std::make_shared<BoundaryConditionValue>(BoundaryCondition::_BC1_, 200.0));
    bi.insert(3, std::make_shared<BoundaryConditionValue>(BoundaryCondition::_BC1_, 100.0));

    TestStencil2x2 stencil(CornerDirection(_X_, _P_, _Y_, _P_));
    stencil.set_value(field, Index(3, 3), bi);

    const auto& idx = stencil.relative_idx();
    EXPECT_EQ(Index(0, 0), idx[0]);
    EXPECT_EQ(Index(1, 0), idx[1]);
    EXPECT_EQ(Index(0, 1), idx[2]);
    EXPECT_EQ(Index(1, 1), idx[3]);

    const auto& val = stencil.values();
    EXPECT_DOUBLE_EQ(33.0, val[0]);
    EXPECT_DOUBLE_EQ(367.0, val[1]);
    EXPECT_DOUBLE_EQ(167.0, val[2]);
    EXPECT_DOUBLE_EQ(267.0, val[3]);
}
