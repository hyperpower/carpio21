#include "gtest/gtest.h"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"
#include "domain/octree/io/ostringify.hpp"

#include <iterator>

using namespace carpio;

TEST(ostringify, stringify_nonuniform_cell_2d) {
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.25, 4, 4);

    const auto lines = StringifyCell(grid, Grid::Index(0, 0));

    ASSERT_EQ(lines.size(), 5);
    auto it = lines.begin();
    EXPECT_EQ(*it++, "0 0 0");
    EXPECT_EQ(*it++, "0.25 0 0");
    EXPECT_EQ(*it++, "0.25 0.25 0");
    EXPECT_EQ(*it++, "0 0.25 0");
    EXPECT_EQ(*it++, "0 0 0");
}

TEST(ostringify, stringify_nonuniform_cell_3d) {
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGridNonUniform_<double, Cell, 3>;

    Grid grid(Grid::Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);

    const auto lines = StringifyCell(grid, Grid::Index(0, 0, 0));

    ASSERT_EQ(lines.size(), 9);
    EXPECT_EQ(lines.front(), "0 0 0");
    EXPECT_EQ(lines.back(), lines.front());
}

TEST(ostringify, stringify_uniform_cell_2d) {
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.25, 4, 4);

    const auto lines = StringifyCell(grid, Grid::Index(0, 0));

    ASSERT_EQ(lines.size(), 5);
    auto it = lines.begin();
    EXPECT_EQ(*it++, "0 0 0");
    EXPECT_EQ(*it++, "0.25 0 0");
    EXPECT_EQ(*it++, "0.25 0.25 0");
    EXPECT_EQ(*it++, "0 0.25 0");
    EXPECT_EQ(*it++, "0 0 0");
}

TEST(ostringify, wire_frame_actor_2d) {
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.5, 2, 2);

    const auto actor = ToGnuplotActorWireFrame(grid);

    EXPECT_EQ(actor.command(), "using 1:2 title \"\" ");
    EXPECT_EQ(actor.style(), "with line lc 0");
    EXPECT_EQ(actor.data().size(), 24);
    EXPECT_EQ(actor.data().front(), "0 0 0");
}

TEST(ostringify, wire_frame_actor_3d) {
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGridNonUniform_<double, Cell, 3>;

    Grid grid(Grid::Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);

    const auto actor = ToGnuplotActorWireFrame(grid);

    EXPECT_EQ(actor.command(), "using 1:2:3 title \"\" ");
    EXPECT_EQ(actor.style(), "with line lc 0");
    EXPECT_EQ(actor.data().size(), 80);
    EXPECT_EQ(actor.data().front(), "0 0 0");
}
