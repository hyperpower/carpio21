#include "gtest/gtest.h"
#include "test_define.hpp"
#include "test_octree.h"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"

#include <algorithm>
#include <memory>
#include <vector>

using namespace carpio;
using namespace carpio::octree_test;

TEST(oghost_regular, constructs_with_grid) {

    auto grid = std::make_shared<OctreeGrid2>(3, 4);
    RegularGhost<OctreeGrid2> ghost(grid);

    EXPECT_TRUE(ghost.has_grid());
    EXPECT_EQ(ghost.ghost_layer(), 1);
    EXPECT_EQ(&ghost.grid(), grid.get());
    EXPECT_EQ(ghost.type(), "OGhostRegular");
}

TEST(oghost_regular, classifies_root_and_descendant_ghost_nodes_2d) {

    auto grid = std::make_shared<UniformGrid2>(UniformGrid2::Point(0.0, 0.0), 1.0, 3, 4);
    RegularGhost<UniformGrid2> ghost(grid);

    EXPECT_TRUE(ghost.is_ghost(grid->root_node(-1, 0)));
    EXPECT_TRUE(ghost.is_ghost(grid->root_node(3, 2)));
    EXPECT_TRUE(ghost.is_ghost(grid->root_node(1, -1)));
    EXPECT_TRUE(ghost.is_ghost(grid->root_node(1, 4)));

    EXPECT_FALSE(ghost.is_ghost(grid->root_node(0, 0)));
    EXPECT_FALSE(ghost.is_ghost(grid->root_node(2, 3)));
    EXPECT_FALSE(ghost.is_ghost(nullptr));

    UniformGrid2::Node standalone;
    EXPECT_FALSE(ghost.is_ghost(&standalone));

    auto ghost_root = grid->root_node(-1, 0);
    ghost_root->new_child(UniformGrid2::Node::Idx::_PP_);
    ASSERT_NE(ghost_root->child[UniformGrid2::Node::Idx::_PP_], nullptr);
    EXPECT_TRUE(ghost.is_ghost(ghost_root->child[UniformGrid2::Node::Idx::_PP_]));

    auto normal_root = grid->root_node(1, 1);
    normal_root->new_child(UniformGrid2::Node::Idx::_MM_);
    ASSERT_NE(normal_root->child[UniformGrid2::Node::Idx::_MM_], nullptr);
    EXPECT_FALSE(ghost.is_ghost(normal_root->child[UniformGrid2::Node::Idx::_MM_]));
}

TEST(oghost_regular, foreach_root_visits_all_ghost_roots_2d) {

    auto grid = std::make_shared<OctreeGrid2>(3, 4);
    RegularGhost<OctreeGrid2> ghost(grid);

    std::vector<OctreeGrid2::Indices> visited;
    ghost.for_each_root([&visited, &grid](OctreeGrid2::ref_Node node) {
        visited.push_back(grid->storage_1d_idx_to_indices(node.root_idx()));
    });

    EXPECT_EQ(visited.size(), 18);
    EXPECT_NE(std::find(visited.begin(), visited.end(), OctreeGrid2::Indices(-1, -1)), visited.end());
    EXPECT_NE(std::find(visited.begin(), visited.end(), OctreeGrid2::Indices(-1, 2)), visited.end());
    EXPECT_NE(std::find(visited.begin(), visited.end(), OctreeGrid2::Indices(1, -1)), visited.end());
    EXPECT_NE(std::find(visited.begin(), visited.end(), OctreeGrid2::Indices(3, 1)), visited.end());
    EXPECT_NE(std::find(visited.begin(), visited.end(), OctreeGrid2::Indices(2, 4)), visited.end());
    EXPECT_EQ(std::find(visited.begin(), visited.end(), OctreeGrid2::Indices(1, 1)), visited.end());

    const RegularGhost<OctreeGrid2>& const_ghost = ghost;
    St const_count = 0;
    const_ghost.for_each_root([&const_count](OctreeGrid2::const_ref_Node) {
        ++const_count;
    });
    EXPECT_EQ(const_count, 18);
}

TEST(oghost_regular, detects_normal_root_boundaries_with_connected_neighbors_2d) {

    auto grid = std::make_shared<OctreeGrid2>(3, 4);
    grid->connect_neighbors();
    RegularGhost<OctreeGrid2> ghost(grid);

    EXPECT_TRUE(ghost.is_boundary(grid->root_node(0, 2), _XM_));
    EXPECT_TRUE(ghost.is_boundary(grid->root_node(2, 1), _XP_));
    EXPECT_TRUE(ghost.is_boundary(grid->root_node(1, 0), _YM_));
    EXPECT_TRUE(ghost.is_boundary(grid->root_node(1, 3), _YP_));

    EXPECT_FALSE(ghost.is_boundary(grid->root_node(1, 1), _XM_));
    EXPECT_FALSE(ghost.is_boundary(grid->root_node(1, 1), _XP_));
    EXPECT_FALSE(ghost.is_boundary(grid->root_node(-1, 1), _XP_));
    EXPECT_FALSE(ghost.is_boundary(nullptr, _XM_));
    EXPECT_FALSE(ghost.is_boundary(grid->root_node(1, 1), _ZP_));
}

TEST(oghost_regular, detects_z_boundaries_with_connected_neighbors_3d) {

    auto grid = std::make_shared<OctreeGrid3>(2, 2, 2);
    grid->connect_neighbors();
    RegularGhost<OctreeGrid3> ghost(grid);

    EXPECT_TRUE(ghost.is_boundary(grid->root_node(0, 0, 0), _ZM_));
    EXPECT_TRUE(ghost.is_boundary(grid->root_node(1, 1, 1), _ZP_));
    EXPECT_FALSE(ghost.is_boundary(grid->root_node(0, 0, -1), _ZP_));
}

TEST(oghost_regular, detects_refined_child_boundaries_with_connected_neighbors) {

    auto grid = std::make_shared<UniformGrid2>(UniformGrid2::Point(0.0, 0.0), 1.0, 2, 2);
    grid->connect_neighbors();
    RegularGhost<UniformGrid2> ghost(grid);

    auto normal_root = grid->root_node(0, 0);
    auto ghost_root = grid->root_node(-1, 0);
    normal_root->new_full_children();
    ghost_root->new_child(UniformGrid2::Node::Idx::_MP_);

    auto normal_child = normal_root->child[UniformGrid2::Node::Idx::_MM_];
    ASSERT_NE(normal_child, nullptr);

    EXPECT_TRUE(ghost.is_boundary(normal_child, _XM_));
    EXPECT_FALSE(ghost.is_boundary(normal_child, _XP_));
}

TEST(oghost_regular, gnuplot_ghost_roots_2d) {

    auto grid = std::make_shared<UniformGrid2>(UniformGrid2::Point(0.0, 0.0), 0.25, 3, 4);
    auto left_ghost = grid->root_node(-1, 1);
    auto bottom_ghost = grid->root_node(1, -1);
    auto corner_ghost = grid->root_node(-1, -1);

    left_ghost->new_full_children();
    ASSERT_NE(left_ghost->child[UniformGrid2::Node::Idx::_PP_], nullptr);
    left_ghost->child[UniformGrid2::Node::Idx::_PP_]->new_full_children();

    bottom_ghost->new_child(UniformGrid2::Node::Idx::_PM_);
    corner_ghost->new_child(UniformGrid2::Node::Idx::_PP_);

    RegularGhost<UniformGrid2> ghost(grid);

    Gnuplot gnu;
    gnu.set_xrange(-0.35, 1.05);
    gnu.set_yrange(-0.35, 1.30);
    gnu.set_equal_aspect_ratio();

    auto agrid = ToGnuplotActorWireFrame(*grid);
    agrid.line_width(1);
    agrid.line_color("black");

    auto aghost = ToGnuplotActorWireFrame(ghost);
    aghost.title("ghost");
    aghost.line_width(2);
    aghost.line_color("blue");

    gnu.add(agrid);
    gnu.add(aghost);
    gnu.set_terminal_png(FIG_PATH + "OctreeOGhostRegular2D", fig_width, fig_height);
    gnu.plot();
}
