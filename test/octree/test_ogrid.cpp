#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/octree/grid/ogrid.hpp"
#include "domain/octree/io/ognuplot_actor_label.hpp"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"

#include <type_traits>


using namespace carpio;

TEST(ogrid, constructs_padded_tree_array_1d){
    using Cell = OCellNonUniform_<double, 1>;
    using Grid = OGrid_<double, Cell, 1>;

    Grid grid(5);

    EXPECT_EQ(grid.size_i(), 5);
    EXPECT_EQ(grid.size_j(), 0);
    EXPECT_EQ(grid.size_k(), 0);
    EXPECT_EQ(grid.size(), 5);
    ASSERT_NE(grid.root_node(0), nullptr);
    EXPECT_TRUE(grid.root_node(0)->is_root());
}

TEST(ogrid, constructs_padded_tree_array_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGrid_<double, Cell, 2>;

    Grid grid(3, 4);

    EXPECT_EQ(grid.size_i(), 3);
    EXPECT_EQ(grid.size_j(), 4);
    EXPECT_EQ(grid.size_k(), 0);
    EXPECT_EQ(grid.size(), 12);
    ASSERT_NE(grid.root_node(0, 0), nullptr);
    EXPECT_TRUE(grid.root_node(0, 0)->is_root());
}

TEST(ogrid, accesses_tree_by_1d_index_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGrid_<double, Cell, 2>;

    Grid grid(3, 4);

    EXPECT_EQ(&grid.tree_1d_index(0), &grid.tree(0, 0));
    EXPECT_EQ(&grid.tree_1d_index(1), &grid.tree(1, 0));
    EXPECT_EQ(&grid.tree_1d_index(3), &grid.tree(0, 1));
    EXPECT_EQ(&grid.tree_1d_index(11), &grid.tree(2, 3));

    const Grid& const_grid = grid;
    EXPECT_EQ(&const_grid.tree_1d_index(0), &const_grid.tree(0, 0));
    EXPECT_EQ(&const_grid.tree_1d_index(1), &const_grid.tree(1, 0));
    EXPECT_EQ(&const_grid.tree_1d_index(3), &const_grid.tree(0, 1));
    EXPECT_EQ(&const_grid.tree_1d_index(11), &const_grid.tree(2, 3));
}

TEST(ogrid, uses_x_then_y_then_z_storage_order){
    using Cell2 = OCellNonUniform_<double, 2>;
    using Grid2 = OGrid_<double, Cell2, 2>;

    Grid2 grid2(3, 4);

    EXPECT_EQ(grid2.to_1d_idx(0, 0), 0);
    EXPECT_EQ(grid2.to_1d_idx(1, 0), 1);
    EXPECT_EQ(grid2.to_1d_idx(0, 1), 3);
    EXPECT_EQ(grid2.to_1d_idx(2, 3), 11);

    EXPECT_EQ(grid2.to_1d_storage_idx(-1, -1), 0);
    EXPECT_EQ(grid2.to_1d_storage_idx(0, -1), 1);
    EXPECT_EQ(grid2.to_1d_storage_idx(-1, 0), 5);
    EXPECT_EQ(grid2.to_1d_storage_idx(0, 0), 6);
    EXPECT_EQ(grid2.to_1d_storage_idx(3, 4), 29);

    using Cell3 = OCellNonUniform_<double, 3>;
    using Grid3 = OGrid_<double, Cell3, 3>;

    Grid3 grid3(2, 3, 4);

    EXPECT_EQ(grid3.to_1d_idx(0, 0, 0), 0);
    EXPECT_EQ(grid3.to_1d_idx(1, 0, 0), 1);
    EXPECT_EQ(grid3.to_1d_idx(0, 1, 0), 2);
    EXPECT_EQ(grid3.to_1d_idx(0, 0, 1), 6);
    EXPECT_EQ(grid3.to_1d_idx(1, 2, 3), 23);

    EXPECT_EQ(grid3.to_1d_storage_idx(-1, -1, -1), 0);
    EXPECT_EQ(grid3.to_1d_storage_idx(0, -1, -1), 1);
    EXPECT_EQ(grid3.to_1d_storage_idx(-1, 0, -1), 4);
    EXPECT_EQ(grid3.to_1d_storage_idx(-1, -1, 0), 20);
    EXPECT_EQ(grid3.to_1d_storage_idx(0, 0, 0), 25);
    EXPECT_EQ(grid3.to_1d_storage_idx(2, 3, 4), 119);
}

TEST(ogrid, constructs_square_geometry_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;
    using Point = Grid::Point;

    Grid grid(Point(0.0, 0.0), 0.25, 4, 4);

    EXPECT_EQ(grid.size_i(), 4);
    EXPECT_EQ(grid.size_j(), 4);
    EXPECT_EQ(grid.size_k(), 0);
    EXPECT_EQ(grid.size(), 16);
    EXPECT_EQ(grid.ghost_layer(), 1);
    EXPECT_DOUBLE_EQ(grid.length(_X_), 1.0);
    EXPECT_DOUBLE_EQ(grid.length(_Y_), 1.0);
    EXPECT_DOUBLE_EQ(grid.length()[_X_], 1.0);
    EXPECT_DOUBLE_EQ(grid.length()[_Y_], 1.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_X_), 0.25);
    EXPECT_DOUBLE_EQ(grid.cell_size(_Y_), 0.25);

    const auto& interior = grid.root_node(0, 0)->cell;
    EXPECT_DOUBLE_EQ(interior.get(_C_, _X_), 0.125);
    EXPECT_DOUBLE_EQ(interior.get(_C_, _Y_), 0.125);
    EXPECT_DOUBLE_EQ(interior.get_hd(_X_), 0.125);
    EXPECT_DOUBLE_EQ(interior.get_hd(_Y_), 0.125);

    const auto& negative_ghost = grid.root_node(-1, -1)->cell;
    EXPECT_DOUBLE_EQ(negative_ghost.get(_C_, _X_), -0.125);
    EXPECT_DOUBLE_EQ(negative_ghost.get(_C_, _Y_), -0.125);

    const auto& last = grid.root_node(3, 3)->cell;
    EXPECT_DOUBLE_EQ(last.get(_C_, _X_), 0.875);
    EXPECT_DOUBLE_EQ(last.get(_C_, _Y_), 0.875);

    const auto& positive_ghost = grid.root_node(4, 4)->cell;
    EXPECT_DOUBLE_EQ(positive_ghost.get(_C_, _X_), 1.125);
    EXPECT_DOUBLE_EQ(positive_ghost.get(_C_, _Y_), 1.125);
}

TEST(ogrid, gnuplot_simple_grid_2d){
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;
    using Point = Grid::Point;

    Grid grid(Point(0.0, 0.0), 0.25, 3, 5);
    auto root = grid.root_node(1, 1);
    ASSERT_NE(root, nullptr);
    for (St i = 0; i < Grid::Node::NumChildren; ++i) {
        root->new_child(i);
    }

    Gnuplot gnu;
    gnu.set_xrange(-0.25, 1.35);
    gnu.set_yrange(-0.25, 1.25);
    gnu.set_equal_aspect_ratio();
    auto agrid = ToGnuplotActorWireFrame(grid);
    auto aindex = ToGnuplotActorLabel(grid, "root_index");
    agrid.line_width(2);
    gnu.add(agrid);
    gnu.add(aindex);
    gnu.set_terminal_png(FIG_PATH + "OctreeOGrid", fig_width, fig_height);
    gnu.plot();
}

TEST(ogrid, constructs_by_cell_length_and_direction_counts_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;
    using Point = Grid::Point;

    Grid grid(Point(1.0, 2.0), 0.5, 3, 4);

    EXPECT_EQ(grid.size_i(), 3);
    EXPECT_EQ(grid.size_j(), 4);
    EXPECT_EQ(grid.size(), 12);
    EXPECT_DOUBLE_EQ(grid.length(_X_), 1.5);
    EXPECT_DOUBLE_EQ(grid.length(_Y_), 2.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_X_), 0.5);
    EXPECT_DOUBLE_EQ(grid.cell_size(_Y_), 0.5);

    const auto& first = grid.root_node(0, 0)->cell;
    EXPECT_DOUBLE_EQ(first.get(_C_, _X_), 1.25);
    EXPECT_DOUBLE_EQ(first.get(_C_, _Y_), 2.25);

    const auto& last = grid.root_node(2, 3)->cell;
    EXPECT_DOUBLE_EQ(last.get(_C_, _X_), 2.25);
    EXPECT_DOUBLE_EQ(last.get(_C_, _Y_), 3.75);
}

TEST(ogrid, constructs_padded_tree_array_3d){
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGrid_<double, Cell, 3>;

    Grid grid(2, 3, 4);

    EXPECT_EQ(grid.size_i(), 2);
    EXPECT_EQ(grid.size_j(), 3);
    EXPECT_EQ(grid.size_k(), 4);
    EXPECT_EQ(grid.size(), 24);
    ASSERT_NE(grid.root_node(0, 0, 0), nullptr);
    EXPECT_TRUE(grid.root_node(0, 0, 0)->is_root());
}

TEST(ogrid, accesses_tree_by_1d_index_3d){
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGrid_<double, Cell, 3>;

    Grid grid(2, 3, 4);

    EXPECT_EQ(&grid.tree_1d_index(0), &grid.tree(0, 0, 0));
    EXPECT_EQ(&grid.tree_1d_index(1), &grid.tree(1, 0, 0));
    EXPECT_EQ(&grid.tree_1d_index(2), &grid.tree(0, 1, 0));
    EXPECT_EQ(&grid.tree_1d_index(6), &grid.tree(0, 0, 1));
    EXPECT_EQ(&grid.tree_1d_index(23), &grid.tree(1, 2, 3));
}

TEST(ogrid, constructs_cube_geometry_3d){
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGridNonUniform_<double, Cell, 3>;
    using Point = Grid::Point;

    Grid grid(Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);

    EXPECT_EQ(grid.size_i(), 2);
    EXPECT_EQ(grid.size_j(), 2);
    EXPECT_EQ(grid.size_k(), 2);
    EXPECT_EQ(grid.size(), 8);
    EXPECT_DOUBLE_EQ(grid.length(_X_), 2.0);
    EXPECT_DOUBLE_EQ(grid.length(_Y_), 2.0);
    EXPECT_DOUBLE_EQ(grid.length(_Z_), 2.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_X_), 1.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_Y_), 1.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_Z_), 1.0);
    EXPECT_DOUBLE_EQ(grid.root_node(0, 0, 0)->cell.volume(), 1.0);
}

TEST(ogrid, is_not_copyable){
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGrid_<double, Cell, 2>;
    using RectGrid = OGridNonUniform_<double, Cell, 2>;

    EXPECT_FALSE(std::is_copy_constructible_v<Grid>);
    EXPECT_FALSE(std::is_copy_assignable_v<Grid>);
    EXPECT_FALSE(std::is_copy_constructible_v<RectGrid>);
    EXPECT_FALSE(std::is_copy_assignable_v<RectGrid>);
}
