#include "gtest/gtest.h"
#include "domain/octree/grid/ocell.hpp"
#include "domain/octree/grid/ogrid.hpp"


using namespace carpio;

TEST(ocell_uniform, initial){
    using Cell = OCellUniform_<double, 2>;

    Cell cell;

    EXPECT_DOUBLE_EQ(cell.get(_C_, _X_), 0.0);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Y_), 0.0);
    EXPECT_DOUBLE_EQ(cell.get_hd(_X_), 0.0);
    EXPECT_DOUBLE_EQ(cell.get_hd(_Y_), 0.0);
    EXPECT_DOUBLE_EQ(cell.volume(), 0.0);
}

TEST(ocell_uniform, geometry_2d){
    using Cell = OCellUniform_<double, 2>;

    Cell cell(0.25, 1.0, 2.0);

    EXPECT_DOUBLE_EQ(cell.get(_M_, _X_), 0.75);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _X_), 1.0);
    EXPECT_DOUBLE_EQ(cell.get(_P_, _X_), 1.25);
    EXPECT_DOUBLE_EQ(cell.get(_M_, _Y_), 1.75);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Y_), 2.0);
    EXPECT_DOUBLE_EQ(cell.get(_P_, _Y_), 2.25);
    EXPECT_DOUBLE_EQ(cell.get_d(_X_), 0.5);
    EXPECT_DOUBLE_EQ(cell.get_d(_Y_), 0.5);
    EXPECT_DOUBLE_EQ(cell.get_hd(_X_), 0.25);
    EXPECT_DOUBLE_EQ(cell.get_hd(_Y_), 0.25);
    EXPECT_DOUBLE_EQ(cell.volume(), 0.25);

    const auto origin = cell.get_origin();
    EXPECT_DOUBLE_EQ(origin.x(), 0.75);
    EXPECT_DOUBLE_EQ(origin.y(), 1.75);

    const auto maxium = cell.get_maxium();
    EXPECT_DOUBLE_EQ(maxium.x(), 1.25);
    EXPECT_DOUBLE_EQ(maxium.y(), 2.25);
}

TEST(ocell_uniform, accepts_nonuniform_style_constructor){
    using Cell = OCellUniform_<double, 2>;

    Cell cell(1.0, 0.25, 2.0, 0.25, 0.0, 0.25);

    EXPECT_DOUBLE_EQ(cell.get(_C_, _X_), 1.0);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Y_), 2.0);
    EXPECT_DOUBLE_EQ(cell.get_hd(_X_), 0.25);
    EXPECT_DOUBLE_EQ(cell.get_hd(_Y_), 0.25);
}

TEST(ocell_uniform, in_on_2d){
    using Cell = OCellUniform_<double, 2>;
    using Point = Cell::Point;

    Cell cell(0.25, 1.0, 2.0);

    EXPECT_TRUE(cell.is_in_on(1.0, 2.0));
    EXPECT_TRUE(cell.is_in_on(0.75, 1.75));
    EXPECT_TRUE(cell.is_in_on(Point(1.25, 2.25)));
    EXPECT_TRUE(cell.is_in_on(_X_, 1.25));
    EXPECT_FALSE(cell.is_in_on(1.26, 2.0));
    EXPECT_FALSE(cell.is_in_on(_Y_, 2.26));
}

TEST(ocell_uniform, transfer_2d){
    using Cell = OCellUniform_<double, 2>;

    Cell cell(0.25, 1.0, 2.0);
    cell.transfer(0.5, -1.0);

    EXPECT_DOUBLE_EQ(cell.get(_C_, _X_), 1.5);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Y_), 1.0);
    EXPECT_DOUBLE_EQ(cell.get_hd(_X_), 0.25);
    EXPECT_DOUBLE_EQ(cell.get_hd(_Y_), 0.25);
}

TEST(ocell_uniform, geometry_3d){
    using Cell = OCellUniform_<double, 3>;

    Cell cell(0.5, 1.0, 2.0, 3.0);

    EXPECT_DOUBLE_EQ(cell.get(_C_, _X_), 1.0);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Y_), 2.0);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Z_), 3.0);
    EXPECT_DOUBLE_EQ(cell.get_d(_X_), 1.0);
    EXPECT_DOUBLE_EQ(cell.get_d(_Y_), 1.0);
    EXPECT_DOUBLE_EQ(cell.get_d(_Z_), 1.0);
    EXPECT_DOUBLE_EQ(cell.volume(), 1.0);
    EXPECT_TRUE(cell.is_in_on(1.5, 2.5, 3.5));
    EXPECT_FALSE(cell.is_in_on(1.51, 2.0, 3.0));
}

TEST(ocell_uniform, make_sub_cell_1d){
    using Cell = OCellUniform_<double, 1>;

    Cell cell(1.0, 0.0);

    const auto m = cell.make_sub_cell(0);
    EXPECT_DOUBLE_EQ(m.get(_C_, _X_), -0.5);
    EXPECT_DOUBLE_EQ(m.get_hd(_X_), 0.5);

    const auto p = cell.make_sub_cell(1);
    EXPECT_DOUBLE_EQ(p.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(p.get_hd(_X_), 0.5);
}

TEST(ocell_uniform, make_sub_cell_2d){
    using Cell = OCellUniform_<double, 2>;

    Cell cell(1.0, 0.0, 0.0);

    const auto mm = cell.make_sub_cell(0);
    EXPECT_DOUBLE_EQ(mm.get(_C_, _X_), -0.5);
    EXPECT_DOUBLE_EQ(mm.get(_C_, _Y_), -0.5);
    EXPECT_DOUBLE_EQ(mm.get_hd(_X_), 0.5);
    EXPECT_DOUBLE_EQ(mm.get_hd(_Y_), 0.5);

    const auto mp = cell.make_sub_cell(1);
    EXPECT_DOUBLE_EQ(mp.get(_C_, _X_), -0.5);
    EXPECT_DOUBLE_EQ(mp.get(_C_, _Y_), 0.5);

    const auto pm = cell.make_sub_cell(2);
    EXPECT_DOUBLE_EQ(pm.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(pm.get(_C_, _Y_), -0.5);

    const auto pp = cell.make_sub_cell(3);
    EXPECT_DOUBLE_EQ(pp.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(pp.get(_C_, _Y_), 0.5);
}

TEST(ocell_uniform, make_sub_cell_3d){
    using Cell = OCellUniform_<double, 3>;

    Cell cell(1.0, 0.0, 0.0, 0.0);

    const auto mmm = cell.make_sub_cell(0);
    EXPECT_DOUBLE_EQ(mmm.get(_C_, _X_), -0.5);
    EXPECT_DOUBLE_EQ(mmm.get(_C_, _Y_), -0.5);
    EXPECT_DOUBLE_EQ(mmm.get(_C_, _Z_), -0.5);
    EXPECT_DOUBLE_EQ(mmm.get_hd(_X_), 0.5);
    EXPECT_DOUBLE_EQ(mmm.get_hd(_Y_), 0.5);
    EXPECT_DOUBLE_EQ(mmm.get_hd(_Z_), 0.5);

    const auto pmp = cell.make_sub_cell(5);
    EXPECT_DOUBLE_EQ(pmp.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(pmp.get(_C_, _Y_), -0.5);
    EXPECT_DOUBLE_EQ(pmp.get(_C_, _Z_), 0.5);

    const auto ppp = cell.make_sub_cell(7);
    EXPECT_DOUBLE_EQ(ppp.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(ppp.get(_C_, _Y_), 0.5);
    EXPECT_DOUBLE_EQ(ppp.get(_C_, _Z_), 0.5);
}

TEST(ocell_uniform, works_as_ogrid_cell){
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.25, 4, 4);
    const auto& cell = grid.root_node(0, 0)->cell;

    EXPECT_DOUBLE_EQ(cell.get(_C_, _X_), 0.125);
    EXPECT_DOUBLE_EQ(cell.get(_C_, _Y_), 0.125);
    EXPECT_DOUBLE_EQ(cell.get_hd(_X_), 0.125);
    EXPECT_DOUBLE_EQ(cell.get_hd(_Y_), 0.125);
}

TEST(ocell_nonuniform, point_in_on_checks_z_in_3d){
    using Cell = OCellNonUniform_<double, 3>;
    using Point = Cell::Point;

    Cell cell(1.0, 0.5, 2.0, 0.5, 3.0, 0.5);

    EXPECT_TRUE(cell.is_in_on(Point(1.0, 2.0, 3.0)));
    EXPECT_TRUE(cell.is_in_on(Point(1.5, 2.5, 3.5)));
    EXPECT_FALSE(cell.is_in_on(Point(1.0, 2.0, 3.51)));
}
