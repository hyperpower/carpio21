#include "gtest/gtest.h"
#include "test_define.hpp"
#include "test_octree.h"
#include "domain/octree/io/ognuplot_actor_label.hpp"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"

#include <iterator>
#include <type_traits>

using namespace carpio;
using namespace carpio::octree_test;

TEST(ogrid, converts_face_direction_to_orientation){
    EXPECT_EQ(FaceDirectionToOrientation(_XM_), _M_);
    EXPECT_EQ(FaceDirectionToOrientation(_XP_), _P_);
    EXPECT_EQ(FaceDirectionToOrientation(_YM_), _M_);
    EXPECT_EQ(FaceDirectionToOrientation(_YP_), _P_);
    EXPECT_EQ(FaceDirectionToOrientation(_ZM_), _M_);
    EXPECT_EQ(FaceDirectionToOrientation(_ZP_), _P_);
}

TEST(ogrid, constructs_padded_tree_array_1d){

    OctreeGrid1 grid(5);

    EXPECT_EQ(grid.size_i(), 5);
    EXPECT_EQ(grid.size_j(), 0);
    EXPECT_EQ(grid.size_k(), 0);
    EXPECT_EQ(grid.size(), 5);
    ASSERT_NE(grid.root_node(0), nullptr);
    EXPECT_TRUE(grid.root_node(0)->is_root());
    EXPECT_EQ(grid.root_node(0)->root_idx(), grid.to_1d_storage_idx(0));
}

TEST(ogrid, constructs_padded_tree_array_2d){

    OctreeGrid2 grid(3, 4);

    EXPECT_EQ(grid.size_i(), 3);
    EXPECT_EQ(grid.size_j(), 4);
    EXPECT_EQ(grid.size_k(), 0);
    EXPECT_EQ(grid.size(), 12);
    ASSERT_NE(grid.root_node(0, 0), nullptr);
    EXPECT_TRUE(grid.root_node(0, 0)->is_root());
    EXPECT_EQ(grid.root_node(0, 0)->root_idx(), grid.to_1d_storage_idx(0, 0));
}

TEST(ogrid, accesses_root_node_by_1d_index_2d){

    OctreeGrid2 grid(3, 4);

    EXPECT_EQ(grid.root_node_1d_index(0), grid.root_node(0, 0));
    EXPECT_EQ(grid.root_node_1d_index(1), grid.root_node(1, 0));
    EXPECT_EQ(grid.root_node_1d_index(3), grid.root_node(0, 1));
    EXPECT_EQ(grid.root_node_1d_index(11), grid.root_node(2, 3));

    const OctreeGrid2& const_grid = grid;
    EXPECT_EQ(const_grid.root_node_1d_index(0), const_grid.root_node(0, 0));
    EXPECT_EQ(const_grid.root_node_1d_index(1), const_grid.root_node(1, 0));
    EXPECT_EQ(const_grid.root_node_1d_index(3), const_grid.root_node(0, 1));
    EXPECT_EQ(const_grid.root_node_1d_index(11), const_grid.root_node(2, 3));
}

TEST(ogrid, accesses_root_node_by_1d_storage_index_2d){

    OctreeGrid2 grid(3, 4);

    EXPECT_EQ(grid.root_node_1d_storage_index(grid.to_1d_storage_idx(-1, -1)),
              grid.root_node(-1, -1));
    EXPECT_EQ(grid.root_node_1d_storage_index(grid.to_1d_storage_idx(0, 0)),
              grid.root_node(0, 0));
    EXPECT_EQ(grid.root_node_1d_storage_index(grid.to_1d_storage_idx(3, 4)),
              grid.root_node(3, 4));

    const OctreeGrid2& const_grid = grid;
    EXPECT_EQ(const_grid.root_node_1d_storage_index(grid.to_1d_storage_idx(-1, -1)),
              const_grid.root_node(-1, -1));
    EXPECT_EQ(const_grid.root_node_1d_storage_index(grid.to_1d_storage_idx(0, 0)),
              const_grid.root_node(0, 0));
    EXPECT_EQ(const_grid.root_node_1d_storage_index(grid.to_1d_storage_idx(3, 4)),
              const_grid.root_node(3, 4));
}

TEST(ogrid, accesses_root_node_by_indices){

    OctreeGrid2 grid(3, 4);

    EXPECT_EQ(grid.root_node(OctreeGrid2::Indices(0, 0)), grid.root_node(0, 0));
    EXPECT_EQ(grid.root_node(OctreeGrid2::Indices(2, 3)), grid.root_node(2, 3));
    EXPECT_EQ(grid.root_node(OctreeGrid2::Indices(-1, -1)), grid.root_node(-1, -1));
    EXPECT_EQ(grid.root_node(OctreeGrid2::Indices(3, 4)), grid.root_node(3, 4));

    const OctreeGrid2& const_grid = grid;
    EXPECT_EQ(const_grid.root_node(OctreeGrid2::Indices(1, 2)), const_grid.root_node(1, 2));
}

TEST(ogrid, iterates_interior_root_nodes_2d){

    OctreeGrid2 grid(3, 4);

    static_assert(std::is_same_v<
        typename std::iterator_traits<OctreeGrid2::iterator>::iterator_category,
        std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<
        typename std::iterator_traits<OctreeGrid2::const_iterator>::iterator_category,
        std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<decltype(*grid.begin()), OctreeGrid2::Node&>);

    St count = 0;
    for (auto it = grid.begin(); it != grid.end(); ++it) {
        auto& node = *it;
        const auto i = Int(count % grid.size_i());
        const auto j = Int(count / grid.size_i());
        EXPECT_EQ(it.indices(), OctreeGrid2::Indices(i, j));
        EXPECT_EQ(&node, grid.root_node_1d_index(count));
        EXPECT_EQ(node.root_idx(), grid.to_1d_storage_idx(i, j));
        EXPECT_NE(node.root_idx(), grid.to_1d_storage_idx(-1, -1));
        ++count;
    }
    EXPECT_EQ(count, grid.size());

    auto last = grid.end();
    --last;
    EXPECT_EQ(&(*last), grid.root_node_1d_index(grid.size() - 1));
    EXPECT_EQ(last.indices(), OctreeGrid2::Indices(2, 3));

    count = 0;
    for (auto it = grid.end(); it != grid.begin();) {
        --it;
        const auto expected = grid.size() - 1 - count;
        const auto i = Int(expected % grid.size_i());
        const auto j = Int(expected / grid.size_i());
        EXPECT_EQ(it.indices(), OctreeGrid2::Indices(i, j));
        EXPECT_EQ(&(*it), grid.root_node_1d_index(expected));
        ++count;
    }
    EXPECT_EQ(count, grid.size());
}

TEST(ogrid, iterates_const_interior_root_nodes_2d){

    OctreeGrid2 grid(3, 4);
    const OctreeGrid2& const_grid = grid;

    static_assert(std::is_same_v<decltype(*const_grid.begin()), const OctreeGrid2::Node&>);
    static_assert(std::is_same_v<decltype(*const_grid.cbegin()), const OctreeGrid2::Node&>);

    St count = 0;
    for (auto it = const_grid.cbegin(); it != const_grid.cend(); ++it) {
        const auto i = Int(count % const_grid.size_i());
        const auto j = Int(count / const_grid.size_i());
        EXPECT_EQ(it.indices(), OctreeGrid2::Indices(i, j));
        EXPECT_EQ(&(*it), const_grid.root_node_1d_index(count));
        ++count;
    }
    EXPECT_EQ(count, const_grid.size());

    auto last = std::prev(const_grid.end());
    EXPECT_EQ(&(*last), const_grid.root_node_1d_index(const_grid.size() - 1));
}

TEST(ogrid, for_each_root_visits_only_interior_root_nodes){

    OctreeGrid2 grid(3, 4);

    St count = 0;
    grid.for_each_root([&](OctreeGrid2::ref_Node node) {
        const auto expected = grid.root_node_1d_index(count);
        EXPECT_EQ(&node, expected);
        EXPECT_GE(node.root_idx(), grid.to_1d_storage_idx(0, 0));
        EXPECT_LE(node.root_idx(), grid.to_1d_storage_idx(2, 3));
        EXPECT_NE(&node, grid.root_node(-1, -1));
        EXPECT_NE(&node, grid.root_node(3, 4));
        ++count;
    });
    EXPECT_EQ(count, grid.size());

    const OctreeGrid2& const_grid = grid;
    St const_count = 0;
    const_grid.for_each_root([&](OctreeGrid2::const_ref_Node node) {
        EXPECT_EQ(&node, const_grid.root_node_1d_index(const_count));
        ++const_count;
    });
    EXPECT_EQ(const_count, const_grid.size());
}

TEST(ogrid, iterates_interior_root_nodes_3d){

    OctreeGrid3 grid(2, 3, 4);

    St count = 0;
    for (auto it = grid.begin(); it != grid.end(); ++it) {
        const auto ij_size = grid.size_i() * grid.size_j();
        const auto rem = count % ij_size;
        const auto i = Int(rem % grid.size_i());
        const auto j = Int(rem / grid.size_i());
        const auto k = Int(count / ij_size);
        EXPECT_EQ(it.indices(), OctreeGrid3::Indices(i, j, k));
        EXPECT_EQ(&(*it), grid.root_node_1d_index(count));
        EXPECT_EQ(it->root_idx(), grid.to_1d_storage_idx(i, j, k));
        ++count;
    }
    EXPECT_EQ(count, grid.size());

    count = 0;
    for (auto it = grid.end(); it != grid.begin();) {
        --it;
        const auto expected = grid.size() - 1 - count;
        const auto ij_size = grid.size_i() * grid.size_j();
        const auto rem = expected % ij_size;
        const auto i = Int(rem % grid.size_i());
        const auto j = Int(rem / grid.size_i());
        const auto k = Int(expected / ij_size);
        EXPECT_EQ(it.indices(), OctreeGrid3::Indices(i, j, k));
        EXPECT_EQ(&(*it), grid.root_node_1d_index(expected));
        ++count;
    }
    EXPECT_EQ(count, grid.size());
}

TEST(ogrid, uses_x_then_y_then_z_storage_order){

    OctreeGrid2 grid2(3, 4);

    EXPECT_EQ(grid2.to_1d_idx(0, 0), 0);
    EXPECT_EQ(grid2.to_1d_idx(1, 0), 1);
    EXPECT_EQ(grid2.to_1d_idx(0, 1), 3);
    EXPECT_EQ(grid2.to_1d_idx(2, 3), 11);
    EXPECT_EQ(grid2.to_indices(11), OctreeGrid2::Indices(2, 3));

    EXPECT_EQ(grid2.to_1d_storage_idx(-1, -1), 0);
    EXPECT_EQ(grid2.to_1d_storage_idx(0, -1), 1);
    EXPECT_EQ(grid2.to_1d_storage_idx(-1, 0), 5);
    EXPECT_EQ(grid2.to_1d_storage_idx(0, 0), 6);
    EXPECT_EQ(grid2.to_1d_storage_idx(3, 4), 29);
    EXPECT_EQ(grid2.storage_1d_idx_to_indices(0), OctreeGrid2::Indices(-1, -1));
    EXPECT_EQ(grid2.storage_1d_idx_to_indices(6), OctreeGrid2::Indices(0, 0));
    EXPECT_EQ(grid2.storage_1d_idx_to_indices(29), OctreeGrid2::Indices(3, 4));


    OctreeGrid3 grid3(2, 3, 4);

    EXPECT_EQ(grid3.to_1d_idx(0, 0, 0), 0);
    EXPECT_EQ(grid3.to_1d_idx(1, 0, 0), 1);
    EXPECT_EQ(grid3.to_1d_idx(0, 1, 0), 2);
    EXPECT_EQ(grid3.to_1d_idx(0, 0, 1), 6);
    EXPECT_EQ(grid3.to_1d_idx(1, 2, 3), 23);
    EXPECT_EQ(grid3.to_indices(23), OctreeGrid3::Indices(1, 2, 3));

    EXPECT_EQ(grid3.to_1d_storage_idx(-1, -1, -1), 0);
    EXPECT_EQ(grid3.to_1d_storage_idx(0, -1, -1), 1);
    EXPECT_EQ(grid3.to_1d_storage_idx(-1, 0, -1), 4);
    EXPECT_EQ(grid3.to_1d_storage_idx(-1, -1, 0), 20);
    EXPECT_EQ(grid3.to_1d_storage_idx(0, 0, 0), 25);
    EXPECT_EQ(grid3.to_1d_storage_idx(2, 3, 4), 119);
    EXPECT_EQ(grid3.storage_1d_idx_to_indices(0), OctreeGrid3::Indices(-1, -1, -1));
    EXPECT_EQ(grid3.storage_1d_idx_to_indices(25), OctreeGrid3::Indices(0, 0, 0));
    EXPECT_EQ(grid3.storage_1d_idx_to_indices(119), OctreeGrid3::Indices(2, 3, 4));
}

TEST(ogrid, connects_root_neighbors_2d){

    OctreeGrid2 grid(3, 4);
    auto root = grid.root_node(0, 0);
    ASSERT_NE(root, nullptr);
    for (St i = 0; i < OctreeGrid2::Node::NumNeighbors; ++i) {
        EXPECT_EQ(root->neighbor[i], nullptr);
    }

    root->set_child(OctreeGrid2::Node::Idx::_PP_, new OctreeGrid2::Node());
    auto child = root->child[OctreeGrid2::Node::Idx::_PP_];
    ASSERT_NE(child, nullptr);
    EXPECT_EQ(child->root_idx(), root->root_idx());

    grid.connect_neighbors();

    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_XP_)], grid.root_node(1, 0));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_XM_)], grid.root_node(-1, 0));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_YP_)], grid.root_node(0, 1));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_YM_)], grid.root_node(0, -1));
    EXPECT_EQ(grid.find_face_neighbor(root, _XP_), grid.root_node(1, 0));
    EXPECT_EQ(grid.find_face_neighbor(nullptr, _XP_), nullptr);

    EXPECT_EQ(grid.root_node(-1, 0)->neighbor[FaceDirectionInOrder(_XP_)],
              grid.root_node(0, 0));
    EXPECT_EQ(grid.root_node(-1, 0)->neighbor[FaceDirectionInOrder(_XM_)],
              nullptr);

    for (St i = 0; i < OctreeGrid2::Node::NumNeighbors; ++i) {
        EXPECT_EQ(child->neighbor[i], nullptr);
    }
}

TEST(ogrid, connects_root_neighbors_3d){

    OctreeGrid3 grid(2, 3, 4);
    auto root = grid.root_node(0, 0, 0);
    ASSERT_NE(root, nullptr);

    grid.connect_neighbors();

    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_XP_)], grid.root_node(1, 0, 0));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_YP_)], grid.root_node(0, 1, 0));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_ZP_)], grid.root_node(0, 0, 1));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_XM_)], grid.root_node(-1, 0, 0));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_YM_)], grid.root_node(0, -1, 0));
    EXPECT_EQ(root->neighbor[FaceDirectionInOrder(_ZM_)], grid.root_node(0, 0, -1));
    EXPECT_EQ(grid.find_face_neighbor(root, _ZP_), grid.root_node(0, 0, 1));

    EXPECT_EQ(grid.root_node(-1, 0, 0)->neighbor[FaceDirectionInOrder(_XP_)],
              grid.root_node(0, 0, 0));
    EXPECT_EQ(grid.root_node(-1, 0, 0)->neighbor[FaceDirectionInOrder(_XM_)],
              nullptr);
    EXPECT_EQ(grid.root_node(0, 0, -1)->neighbor[FaceDirectionInOrder(_ZP_)],
              grid.root_node(0, 0, 0));
    EXPECT_EQ(grid.root_node(0, 0, -1)->neighbor[FaceDirectionInOrder(_ZM_)],
              nullptr);
}

TEST(ogrid, detects_node_ownership){

    OctreeGrid2 grid(3, 4);
    auto root = grid.root_node(1, 2);
    ASSERT_NE(root, nullptr);
    root->set_child(OctreeGrid2::Node::Idx::_PP_, new OctreeGrid2::Node());
    ASSERT_NE(root->child[OctreeGrid2::Node::Idx::_PP_], nullptr);

    EXPECT_TRUE(IsNodeInGrid(*root, grid));
    EXPECT_TRUE(IsNodeInGrid(*root->child[OctreeGrid2::Node::Idx::_PP_], grid));
    EXPECT_TRUE(IsNodeInGrid(*grid.root_node(-1, 0), grid));

    OctreeGrid2::Node standalone;
    EXPECT_FALSE(IsNodeInGrid(standalone, grid));

    OctreeGrid2 other_grid(3, 4);
    EXPECT_FALSE(IsNodeInGrid(*other_grid.root_node(1, 2), grid));
}

TEST(ogrid, constructs_square_geometry_2d){

    NonUniformGrid2 grid(NonUniformGrid2::Point(0.0, 0.0), 0.25, 4, 4);

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

    UniformGrid2 grid(UniformGrid2::Point(0.0, 0.0), 0.25, 3, 5);
    auto root = grid.root_node(1, 1);
    ASSERT_NE(root, nullptr);
    for (St i = 0; i < UniformGrid2::Node::NumChildren; ++i) {
        root->new_child(i);
    }
    auto c = root->child[3];
    ASSERT_NE(c, nullptr);
    c->new_full_children();

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

TEST(ogrid, gnuplot_random_grid_4x5_2d){

    auto grid = MakeRandomUniformOctree2(0.0, 0.0, 0.25, 4, 5);

    Gnuplot gnu;
    gnu.set_xrange(-0.05, 1.05);
    gnu.set_yrange(-0.05, 1.30);
    gnu.set_equal_aspect_ratio();
    auto agrid = ToGnuplotActorWireFrame(*grid);
    auto& n1 = *grid->locate(UniformGrid2::Point(0.4, 0.36));
    auto anode = ToGnuplotActorWireFrame(n1);
    anode.line_width(2);
    anode.line_color("red");
    agrid.line_width(1);
    auto nn = n1.find_face_neighbor(_YM_);
    if (nn != nullptr) {
        auto anode2 = ToGnuplotActorWireFrame(*nn);
        auto al = ToGnuplotActorLabel(*nn, "level");
        anode2.line_width(2);
        anode2.line_color("blue");
        gnu.add(al);
        gnu.add(anode2);
    }else{
        std::cerr << "No neighbor in direction\n";
    }

    gnu.add(agrid);
    gnu.add(anode);
    gnu.set_terminal_png(FIG_PATH + "OctreeOGridRandom4x5", fig_width, fig_height);
    gnu.plot();
}

TEST(ogrid, constructs_by_cell_length_and_direction_counts_2d){

    NonUniformGrid2 grid(NonUniformGrid2::Point(1.0, 2.0), 0.5, 3, 4);

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

TEST(ogrid, constructs_nonuniform_geometry_from_cell_lengths_2d){

    NonUniformGrid2::ArrCellSize cell_lengths;
    cell_lengths[_X_] = {0.2, 0.3, 0.5};
    cell_lengths[_Y_] = {1.0, 2.0};
    NonUniformGrid2 grid(NonUniformGrid2::Point(10.0, 20.0), cell_lengths);

    EXPECT_EQ(grid.size_i(), 3);
    EXPECT_EQ(grid.size_j(), 2);
    EXPECT_DOUBLE_EQ(grid.length(_X_), 1.0);
    EXPECT_DOUBLE_EQ(grid.length(_Y_), 3.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_X_), 0.2);
    EXPECT_DOUBLE_EQ(grid.cell_size(_Y_), 1.0);

    const auto& sizes = grid.cell_size();
    EXPECT_DOUBLE_EQ(sizes[_X_][0], 0.2);
    EXPECT_DOUBLE_EQ(sizes[_X_][1], 0.2);
    EXPECT_DOUBLE_EQ(sizes[_X_][2], 0.3);
    EXPECT_DOUBLE_EQ(sizes[_X_][3], 0.5);
    EXPECT_DOUBLE_EQ(sizes[_X_][4], 0.5);
    EXPECT_DOUBLE_EQ(grid.cell_size(St(_X_), -1), 0.2);
    EXPECT_DOUBLE_EQ(grid.cell_size(St(_X_), 0), 0.2);
    EXPECT_DOUBLE_EQ(grid.cell_size(St(_X_), 1), 0.3);
    EXPECT_DOUBLE_EQ(grid.cell_size(St(_X_), 2), 0.5);
    EXPECT_DOUBLE_EQ(grid.cell_size(St(_X_), 3), 0.5);

    const auto& negative_ghost = grid.root_node(-1, -1)->cell;
    EXPECT_DOUBLE_EQ(negative_ghost.get(_C_, _X_), 9.9);
    EXPECT_DOUBLE_EQ(negative_ghost.get(_C_, _Y_), 19.5);
    EXPECT_DOUBLE_EQ(negative_ghost.get_hd(_X_), 0.1);
    EXPECT_DOUBLE_EQ(negative_ghost.get_hd(_Y_), 0.5);

    const auto& first = grid.root_node(0, 0)->cell;
    EXPECT_DOUBLE_EQ(first.get(_C_, _X_), 10.1);
    EXPECT_DOUBLE_EQ(first.get(_C_, _Y_), 20.5);
    EXPECT_DOUBLE_EQ(first.get_hd(_X_), 0.1);
    EXPECT_DOUBLE_EQ(first.get_hd(_Y_), 0.5);

    const auto& middle = grid.root_node(1, 0)->cell;
    EXPECT_DOUBLE_EQ(middle.get(_C_, _X_), 10.35);
    EXPECT_DOUBLE_EQ(middle.get_hd(_X_), 0.15);

    const auto& last = grid.root_node(2, 1)->cell;
    EXPECT_DOUBLE_EQ(last.get(_C_, _X_), 10.75);
    EXPECT_DOUBLE_EQ(last.get(_C_, _Y_), 22.0);
    EXPECT_DOUBLE_EQ(last.get_hd(_X_), 0.25);
    EXPECT_DOUBLE_EQ(last.get_hd(_Y_), 1.0);

    const auto& positive_ghost = grid.root_node(3, 2)->cell;
    EXPECT_DOUBLE_EQ(positive_ghost.get(_C_, _X_), 11.25);
    EXPECT_DOUBLE_EQ(positive_ghost.get(_C_, _Y_), 24.0);
    EXPECT_DOUBLE_EQ(positive_ghost.get_hd(_X_), 0.25);
    EXPECT_DOUBLE_EQ(positive_ghost.get_hd(_Y_), 1.0);
}

TEST(ogrid, constructs_uniform_geometry_2d){

    UniformGrid2 grid(UniformGrid2::Point(1.0, 2.0), 0.5, 3, 4);

    EXPECT_EQ(grid.size_i(), 3);
    EXPECT_EQ(grid.size_j(), 4);
    EXPECT_EQ(grid.size(), 12);
    EXPECT_DOUBLE_EQ(grid.length(_X_), 1.5);
    EXPECT_DOUBLE_EQ(grid.length(_Y_), 2.0);
    EXPECT_DOUBLE_EQ(grid.length()[_X_], 1.5);
    EXPECT_DOUBLE_EQ(grid.length()[_Y_], 2.0);
    EXPECT_DOUBLE_EQ(grid.cell_size(_X_), 0.5);
    EXPECT_DOUBLE_EQ(grid.cell_size(_Y_), 0.5);
    EXPECT_DOUBLE_EQ(grid.origin().x(), 1.0);
    EXPECT_DOUBLE_EQ(grid.origin().y(), 2.0);

    const auto& first = grid.root_node(0, 0)->cell;
    EXPECT_DOUBLE_EQ(first.get(_C_, _X_), 1.25);
    EXPECT_DOUBLE_EQ(first.get(_C_, _Y_), 2.25);
    EXPECT_DOUBLE_EQ(first.get_hd(_X_), 0.25);
    EXPECT_DOUBLE_EQ(first.get_hd(_Y_), 0.25);

    const auto& negative_ghost = grid.root_node(-1, -1)->cell;
    EXPECT_DOUBLE_EQ(negative_ghost.get(_C_, _X_), 0.75);
    EXPECT_DOUBLE_EQ(negative_ghost.get(_C_, _Y_), 1.75);

    const auto& positive_ghost = grid.root_node(3, 4)->cell;
    EXPECT_DOUBLE_EQ(positive_ghost.get(_C_, _X_), 2.75);
    EXPECT_DOUBLE_EQ(positive_ghost.get(_C_, _Y_), 4.25);
}

TEST(ogrid, constructs_padded_tree_array_3d){

    OctreeGrid3 grid(2, 3, 4);

    EXPECT_EQ(grid.size_i(), 2);
    EXPECT_EQ(grid.size_j(), 3);
    EXPECT_EQ(grid.size_k(), 4);
    EXPECT_EQ(grid.size(), 24);
    ASSERT_NE(grid.root_node(0, 0, 0), nullptr);
    EXPECT_TRUE(grid.root_node(0, 0, 0)->is_root());
    EXPECT_EQ(grid.root_node(0, 0, 0)->root_idx(), grid.to_1d_storage_idx(0, 0, 0));
}

TEST(ogrid, accesses_root_node_by_1d_index_3d){

    OctreeGrid3 grid(2, 3, 4);

    EXPECT_EQ(grid.root_node_1d_index(0), grid.root_node(0, 0, 0));
    EXPECT_EQ(grid.root_node_1d_index(1), grid.root_node(1, 0, 0));
    EXPECT_EQ(grid.root_node_1d_index(2), grid.root_node(0, 1, 0));
    EXPECT_EQ(grid.root_node_1d_index(6), grid.root_node(0, 0, 1));
    EXPECT_EQ(grid.root_node_1d_index(23), grid.root_node(1, 2, 3));
}

TEST(ogrid, constructs_cube_geometry_3d){

    NonUniformGrid3 grid(NonUniformGrid3::Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);

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

TEST(ogrid, constructs_nonuniform_geometry_from_cell_lengths_3d){

    NonUniformGrid3::ArrCellSize cell_lengths;
    cell_lengths[_X_] = {1.0, 2.0};
    cell_lengths[_Y_] = {0.5, 1.5};
    cell_lengths[_Z_] = {0.25, 0.75, 1.0};
    NonUniformGrid3 grid(NonUniformGrid3::Point(0.0, 0.0, 0.0), cell_lengths);

    EXPECT_EQ(grid.size_i(), 2);
    EXPECT_EQ(grid.size_j(), 2);
    EXPECT_EQ(grid.size_k(), 3);
    EXPECT_DOUBLE_EQ(grid.length(_X_), 3.0);
    EXPECT_DOUBLE_EQ(grid.length(_Y_), 2.0);
    EXPECT_DOUBLE_EQ(grid.length(_Z_), 2.0);

    const auto& sizes = grid.cell_size();
    EXPECT_DOUBLE_EQ(sizes[_Z_][0], 0.25);
    EXPECT_DOUBLE_EQ(sizes[_Z_][3], 1.0);
    EXPECT_DOUBLE_EQ(sizes[_Z_][4], 1.0);
    EXPECT_DOUBLE_EQ(grid.root_node(1, 1, 2)->cell.volume(), 3.0);
}

TEST(ogrid, locates_root_by_point_3d){

    NonUniformGrid3 grid(NonUniformGrid3::Point(0.0, 0.0, 0.0), 1.0, 2, 2, 2);
    const NonUniformGrid3& const_grid = grid;

    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(0.25, 0.25, 0.25)),
              grid.root_node(0, 0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(1.25, 1.25, 0.25)),
              grid.root_node(1, 1, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(0.0, 0.0, 0.0)),
              grid.root_node(0, 0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(2.0, 2.0, 2.0)),
              grid.root_node(1, 1, 1));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(1.0, 0.25, 0.25)),
              grid.root_node(1, 0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(-0.25, 0.25, 0.25)),
              grid.root_node(-1, 0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(2.25, 1.25, 1.25)),
              grid.root_node(2, 1, 1));
    EXPECT_EQ(grid.locate_root(NonUniformGrid3::Point(3.01, 1.25, 1.25)), nullptr);
    EXPECT_EQ(const_grid.locate_root(NonUniformGrid3::Point(1.25, 0.25, 1.25)),
              const_grid.root_node(1, 0, 1));
}

TEST(ogrid, locates_root_by_point_2d){

    NonUniformGrid2 grid(NonUniformGrid2::Point(0.0, 0.0), 0.5, 3, 4);

    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(0.25, 0.25)), grid.root_node(0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(0.5, 0.25)), grid.root_node(1, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(1.5, 2.0)), grid.root_node(2, 3));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(-0.25, 0.25)), grid.root_node(-1, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(1.75, 0.25)), grid.root_node(3, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(2.01, 0.25)), nullptr);

    Gnuplot gnu;
    gnu.set_xrange(-0.05, 2.05);
    gnu.set_yrange(-0.05, 2.30);
    gnu.set_equal_aspect_ratio();
    auto agrid = ToGnuplotActorWireFrame(grid);
    agrid.line_width(1);
    auto anode = ToGnuplotActorWireFrame(*grid.locate_root(NonUniformGrid2::Point(0.5, 2.0)));
    anode.line_width(2);
    gnu.add(agrid);
    gnu.add(anode);

    gnu.set_terminal_png(FIG_PATH + "OctreeLocateRoot2D", fig_width, fig_height);
    gnu.plot();
}

TEST(ogrid, locates_root_by_point_on_nonuniform_geometry_2d){

    NonUniformGrid2::ArrCellSize cell_lengths;
    cell_lengths[_X_] = {0.2, 0.3, 0.5};
    cell_lengths[_Y_] = {1.0, 2.0};
    NonUniformGrid2 grid(NonUniformGrid2::Point(0.0, 0.0), cell_lengths);

    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(0.1, 0.5)), grid.root_node(0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(0.0, 0.0)), grid.root_node(0, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(0.2, 0.5)), grid.root_node(1, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(0.5, 2.0)), grid.root_node(2, 1));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(1.0, 3.0)), grid.root_node(2, 1));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(-0.1, 0.5)), grid.root_node(-1, 0));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(1.5, 5.0)), grid.root_node(3, 2));
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(1.51, 0.5)), nullptr);
    EXPECT_EQ(grid.locate_root(NonUniformGrid2::Point(-0.21, 0.5)), nullptr);
}

TEST(ogrid, uniform_locates_root_by_point_2d){

    UniformGrid2 grid(UniformGrid2::Point(0.0, 0.0), 0.5, 3, 4);
    const UniformGrid2& const_grid = grid;

    EXPECT_EQ(grid.locate_root(UniformGrid2::Point(0.25, 0.25)), grid.root_node(0, 0));
    EXPECT_EQ(grid.locate_root(UniformGrid2::Point(0.5, 0.25)), grid.root_node(1, 0));
    EXPECT_EQ(grid.locate_root(UniformGrid2::Point(1.5, 2.0)), grid.root_node(2, 3));
    EXPECT_EQ(grid.locate_root(UniformGrid2::Point(-0.25, 0.25)), grid.root_node(-1, 0));
    EXPECT_EQ(grid.locate_root(UniformGrid2::Point(1.75, 0.25)), grid.root_node(3, 0));
    EXPECT_EQ(grid.locate_root(UniformGrid2::Point(2.01, 0.25)), nullptr);
    EXPECT_EQ(const_grid.locate_root(UniformGrid2::Point(0.25, 1.25)),
              const_grid.root_node(0, 2));
}

TEST(ogrid, locates_deepest_existing_node_by_point_2d){

    UniformGrid2 grid(UniformGrid2::Point(0.0, 0.0), 1.0, 2, 2);
    auto root = grid.root_node(0, 0);
    ASSERT_NE(root, nullptr);
    const UniformGrid2& const_grid = grid;

    EXPECT_EQ(grid.locate(UniformGrid2::Point(0.25, 0.25)), root);

    root->new_full_children();
    EXPECT_EQ(grid.locate(UniformGrid2::Point(0.75, 0.75)), root->child[UniformGrid2::Node::Idx::_PP_]);
    EXPECT_EQ(grid.locate(UniformGrid2::Point(0.5, 0.5)), root->child[UniformGrid2::Node::Idx::_PP_]);

    auto child = root->child[UniformGrid2::Node::Idx::_PP_];
    ASSERT_NE(child, nullptr);
    child->new_full_children();
    EXPECT_EQ(grid.locate(UniformGrid2::Point(0.875, 0.625)),
              child->child[UniformGrid2::Node::Idx::_MP_]);
    EXPECT_EQ(const_grid.locate(UniformGrid2::Point(0.875, 0.625)),
              child->child[UniformGrid2::Node::Idx::_MP_]);
}

TEST(ogrid, locates_parent_when_target_child_is_missing_2d){

    UniformGrid2 grid(UniformGrid2::Point(0.0, 0.0), 1.0, 1, 1);
    auto root = grid.root_node(0, 0);
    ASSERT_NE(root, nullptr);

    root->new_child(UniformGrid2::Node::Idx::_MM_);

    EXPECT_EQ(grid.locate(UniformGrid2::Point(0.25, 0.25)), root->child[UniformGrid2::Node::Idx::_MM_]);
    EXPECT_EQ(grid.locate(UniformGrid2::Point(0.75, 0.75)), root);
    EXPECT_EQ(grid.locate(UniformGrid2::Point(2.01, 0.25)), nullptr);
}

TEST(ogrid, locates_deepest_existing_node_by_point_3d){

    UniformGrid3 grid(UniformGrid3::Point(0.0, 0.0, 0.0), 1.0, 1, 1, 1);
    auto root = grid.root_node(0, 0, 0);
    ASSERT_NE(root, nullptr);

    root->new_full_children();

    EXPECT_EQ(grid.locate(UniformGrid3::Point(0.75, 0.25, 0.75)),
              root->child[UniformGrid3::Node::Idx::_PMP_]);
}

TEST(ogrid, is_not_copyable){

    EXPECT_FALSE(std::is_copy_constructible_v<OctreeGrid2>);
    EXPECT_FALSE(std::is_copy_assignable_v<OctreeGrid2>);
    EXPECT_FALSE(std::is_copy_constructible_v<NonUniformGrid2>);
    EXPECT_FALSE(std::is_copy_assignable_v<NonUniformGrid2>);
}
