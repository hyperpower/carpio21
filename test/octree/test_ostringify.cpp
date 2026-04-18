#include "gtest/gtest.h"
#include "domain/octree/io/ognuplot_actor_label.hpp"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"
#include "domain/octree/io/ostringify.hpp"

#include <iterator>

using namespace carpio;

TEST(ostringify, stringify_nonuniform_cell_2d) {
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.25, 4, 4);

    const auto lines = StringifyCell(grid, Grid::Indices(0, 0));

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

    const auto lines = StringifyCell(grid, Grid::Indices(0, 0, 0));

    ASSERT_EQ(lines.size(), 9);
    EXPECT_EQ(lines.front(), "0 0 0");
    EXPECT_EQ(lines.back(), lines.front());
}

TEST(ostringify, stringify_uniform_cell_2d) {
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.25, 4, 4);

    const auto lines = StringifyCell(grid, Grid::Indices(0, 0));

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

TEST(ostringify, wire_frame_actor_node_2d) {
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.5, 2, 2);
    const auto actor = ToGnuplotActorWireFrame(*grid.root_node(0, 0));

    EXPECT_EQ(actor.command(), "using 1:2 title \"\" ");
    EXPECT_EQ(actor.style(), "with line lc 0");
    EXPECT_EQ(actor.data().size(), 6);
    EXPECT_EQ(actor.data().front(), "0 0 0");
}

TEST(ostringify, wire_frame_actor_refined_node_2d) {
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.5, 2, 2);
    auto root = grid.root_node(0, 0);
    root->new_full_children();

    const auto actor = ToGnuplotActorWireFrame(*root);

    EXPECT_EQ(actor.command(), "using 1:2 title \"\" ");
    EXPECT_EQ(actor.style(), "with line lc 0");
    EXPECT_EQ(actor.data().size(), 6);
    EXPECT_EQ(actor.data().front(), "0 0 0");
}

TEST(ostringify, wire_frame_actor_node_3d) {
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGridNonUniform_<double, Cell, 3>;

    Grid grid(Grid::Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);
    const auto actor = ToGnuplotActorWireFrame(*grid.root_node(0, 0, 0));

    EXPECT_EQ(actor.command(), "using 1:2:3 title \"\" ");
    EXPECT_EQ(actor.style(), "with line lc 0");
    EXPECT_FALSE(actor.data().empty());
    EXPECT_EQ(actor.data().front(), "0 0 0");
}

TEST(ostringify, label_actor_root_index_2d) {
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.5, 2, 2);

    const auto actor = ToGnuplotActorLabel(grid, "root_index");

    EXPECT_EQ(actor.command(), "using 1:2:3 title \"\" ");
    EXPECT_EQ(actor.style(), "with labels center textcolor lt 2");
    ASSERT_EQ(actor.data().size(), 4);
    EXPECT_EQ(actor.data().front(), "0.25 0.25 \"(0, 0)\"");
}

TEST(ostringify, label_actor_root_index_1d) {
    using Cell = OCellNonUniform_<double, 1>;
    using Grid = OGridNonUniform_<double, Cell, 1>;

    Grid grid(Grid::Point(0.0), 0.5, 2);

    const auto actor = ToGnuplotActorLabel(grid, "root_index");

    EXPECT_EQ(actor.command(), "using 1:2:3 title \"\" ");
    EXPECT_EQ(actor.style(), "with labels center textcolor lt 2");
    ASSERT_EQ(actor.data().size(), 2);
    auto it = actor.data().begin();
    EXPECT_EQ(*it++, "0.25 0 \"(0)\"");
    EXPECT_EQ(*it++, "0.75 0 \"(1)\"");
}

TEST(ostringify, label_actor_root_index1d_2d) {
    using Cell = OCellNonUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 0.5, 2, 2);

    const auto actor = ToGnuplotActorLabel(grid, "root_index1d");

    ASSERT_EQ(actor.data().size(), 4);
    auto it = actor.data().begin();
    EXPECT_EQ(*it++, "0.25 0.25 \"0\"");
    EXPECT_EQ(*it++, "0.75 0.25 \"1\"");
    EXPECT_EQ(*it++, "0.25 0.75 \"2\"");
    EXPECT_EQ(*it++, "0.75 0.75 \"3\"");
}

TEST(ostringify, label_actor_root_index1d_3d) {
    using Cell = OCellNonUniform_<double, 3>;
    using Grid = OGridNonUniform_<double, Cell, 3>;

    Grid grid(Grid::Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);

    const auto actor = ToGnuplotActorLabel(grid, "root_index1d");

    EXPECT_EQ(actor.command(), "using 1:2:3:4 title \"\" ");
    EXPECT_EQ(actor.style(), "with labels center textcolor lt 2");
    ASSERT_EQ(actor.data().size(), 8);
    auto it = actor.data().begin();
    EXPECT_EQ(*it++, "0.5 0.5 0.5 \"0\"");
    EXPECT_EQ(*it++, "1.5 0.5 0.5 \"1\"");
    EXPECT_EQ(*it++, "0.5 1.5 0.5 \"2\"");
    EXPECT_EQ(*it++, "1.5 1.5 0.5 \"3\"");
    EXPECT_EQ(*it++, "0.5 0.5 1.5 \"4\"");
}

TEST(ostringify, label_actor_node_level_2d) {
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;

    Grid grid(Grid::Point(0.0, 0.0), 1.0, 1, 1);
    auto root = grid.root_node(0, 0);

    const auto actor = ToGnuplotActorLabel(*root, "level");

    EXPECT_EQ(actor.command(), "using 1:2:3 title \"\" ");
    EXPECT_EQ(actor.style(), "with labels center textcolor lt 2");
    ASSERT_EQ(actor.data().size(), 1);
    EXPECT_EQ(actor.data().front(), "0.5 0.5 \"0\"");
}

TEST(ostringify, label_actor_child_node_level_idx_and_custom_text_2d) {
    using Cell = OCellUniform_<double, 2>;
    using Grid = OGridNonUniform_<double, Cell, 2>;
    using Node = Grid::Node;

    Grid grid(Grid::Point(0.0, 0.0), 1.0, 1, 1);
    auto root = grid.root_node(0, 0);
    root->new_full_children();
    const auto child = root->child[Node::Idx::_MP_];

    const auto level_actor = ToGnuplotActorLabel(*child, "level");
    ASSERT_EQ(level_actor.data().size(), 1);
    EXPECT_EQ(level_actor.data().front(), "0.75 0.25 \"1\"");

    const auto idx_actor = ToGnuplotActorLabel(*child, "idx");
    ASSERT_EQ(idx_actor.data().size(), 1);
    EXPECT_EQ(idx_actor.data().front(), "0.75 0.25 \"1\"");

    const auto custom_actor = ToGnuplotActorLabel(*child, "leaf");
    ASSERT_EQ(custom_actor.data().size(), 1);
    EXPECT_EQ(custom_actor.data().front(), "0.75 0.25 \"leaf\"");
}
