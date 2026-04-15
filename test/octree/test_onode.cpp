#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/octree/grid/onode.hpp"


using namespace carpio;

TEST(onode, initial){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;

    EXPECT_EQ(node.father, nullptr);
    EXPECT_EQ(node.root_idx(), 0);

    for (St i = 0; i < Node::NumChildren; ++i) {
        EXPECT_EQ(node.child[i], nullptr);
    }
    for (St i = 0; i < Node::NumNeighbors; ++i) {
        EXPECT_EQ(node.neighbor[i], nullptr);
    }
}

TEST(onode, child_ownership){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;
    node.set_root_idx(12);
    node.set_child(Node::Idx::_PP_, new Node());

    ASSERT_TRUE(node.has_child(Node::Idx::_PP_));
    EXPECT_TRUE(node.has_child());
    EXPECT_FALSE(node.is_leaf());
    EXPECT_EQ(node.child[Node::Idx::_PP_]->father, &node);
    EXPECT_EQ(node.child[Node::Idx::_PP_]->root_idx(), 12);

    node.delete_child(Node::Idx::_PP_);

    EXPECT_FALSE(node.has_child(Node::Idx::_PP_));
    EXPECT_FALSE(node.has_child());
    EXPECT_TRUE(node.is_leaf());
}

TEST(onode, construct_child_node){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_root_idx(7);
    Cell cell(1.0, 0.25, 2.0, 0.25);
    Node child(Node::Idx::_PP_, &root, cell, 3.0);

    EXPECT_EQ(child.father, &root);
    EXPECT_FALSE(child.is_root());
    EXPECT_TRUE(child.is_leaf());
    EXPECT_DOUBLE_EQ(child.cell.get(_C_, _X_), 1.0);
    EXPECT_DOUBLE_EQ(child.cell.get(_C_, _Y_), 2.0);
    EXPECT_DOUBLE_EQ(child.data, 3.0);
    EXPECT_EQ(child.root_idx(), 7);
    for (St i = 0; i < Node::NumChildren; ++i) {
        EXPECT_EQ(child.child[i], nullptr);
    }
    for (St i = 0; i < Node::NumNeighbors; ++i) {
        EXPECT_EQ(child.neighbor[i], nullptr);
    }
}

TEST(onode, new_child_from_parent_cell){
    using Cell = OCellUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;
    node.set_root_idx(21);
    node.cell = Cell(1.0, 0.0, 0.0);
    node.data = 7.0;

    node.new_child(Node::Idx::_PP_);

    ASSERT_TRUE(node.has_child(Node::Idx::_PP_));
    auto child = node.child[Node::Idx::_PP_];
    EXPECT_EQ(child->father, &node);
    EXPECT_EQ(child->root_idx(), 21);
    EXPECT_DOUBLE_EQ(child->data, node.data);
    EXPECT_DOUBLE_EQ(child->cell.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(child->cell.get(_C_, _Y_), 0.5);
    EXPECT_DOUBLE_EQ(child->cell.get_hd(_X_), 0.5);
    EXPECT_DOUBLE_EQ(child->cell.get_hd(_Y_), 0.5);

    child->new_child(Node::Idx::_MM_);
    ASSERT_TRUE(child->has_child(Node::Idx::_MM_));

    node.new_child(Node::Idx::_PP_);

    ASSERT_TRUE(node.has_child(Node::Idx::_PP_));
    EXPECT_EQ(node.child[Node::Idx::_PP_]->father, &node);
    EXPECT_TRUE(node.child[Node::Idx::_PP_]->is_leaf());
}

TEST(onode, new_full_children_from_parent_cell){
    using Cell = OCellUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;
    node.set_root_idx(33);
    node.cell = Cell(2.0, 1.0, 1.0);
    node.data = 7.0;

    node.new_full_children();

    EXPECT_TRUE(node.is_full_child());
    EXPECT_FALSE(node.is_leaf());
    EXPECT_EQ(node.height(), 1);
    for (St i = 0; i < Node::NumChildren; ++i) {
        ASSERT_TRUE(node.has_child(i));
        auto child = node.child[i];
        EXPECT_EQ(child->father, &node);
        EXPECT_EQ(child->root_idx(), 33);
        EXPECT_TRUE(child->is_leaf());
        EXPECT_DOUBLE_EQ(child->data, node.data);
        EXPECT_DOUBLE_EQ(child->cell.get_hd(_X_), 1.0);
        EXPECT_DOUBLE_EQ(child->cell.get_hd(_Y_), 1.0);
    }
}

TEST(onode, find_neighbor_same_parent_siblings_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());
    root.set_child(Node::Idx::_PM_, new Node());

    EXPECT_EQ(root.find_neighbor(*root.child[Node::Idx::_MM_], _XP_),
              root.child[Node::Idx::_MP_]);
    EXPECT_EQ(root.find_neighbor(*root.child[Node::Idx::_MM_], _YP_),
              root.child[Node::Idx::_PM_]);
}

TEST(onode, find_neighbor_climbs_to_common_ancestor_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());

    auto left_parent = root.child[Node::Idx::_MM_];
    auto right_parent = root.child[Node::Idx::_MP_];
    left_parent->set_child(Node::Idx::_MP_, new Node());
    right_parent->set_child(Node::Idx::_MM_, new Node());

    EXPECT_EQ(root.find_neighbor(*left_parent->child[Node::Idx::_MP_], _XP_),
              right_parent->child[Node::Idx::_MM_]);
}

TEST(onode, find_neighbor_crosses_root_neighbor_and_descends_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node left_root;
    Node right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(Node::Idx::_MP_, new Node());
    right_root.set_child(Node::Idx::_MM_, new Node());

    EXPECT_EQ(left_root.find_neighbor(*left_root.child[Node::Idx::_MP_], _XP_),
              right_root.child[Node::Idx::_MM_]);
}

TEST(onode, find_neighbor_returns_coarser_neighbor_when_child_is_missing_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node left_root;
    Node right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(Node::Idx::_MP_, new Node());

    EXPECT_EQ(left_root.find_neighbor(*left_root.child[Node::Idx::_MP_], _XP_),
              &right_root);
}

TEST(onode, find_neighbor_returns_nullptr_for_missing_boundary_neighbor_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MP_, new Node());

    EXPECT_EQ(root.find_neighbor(*root.child[Node::Idx::_MP_], _XP_), nullptr);
}

TEST(onode, find_neighbor_rejects_direction_outside_dimension_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;

    EXPECT_EQ(root.find_neighbor(root, _ZP_), nullptr);
}

TEST(onode, find_neighbor_climbs_to_common_ancestor_3d_z_direction){
    using Cell = OCellNonUniform_<double, 3>;
    using Node = ONode_<double, Cell, 3>;

    Node root;
    root.set_child(Node::Idx::_MMM_, new Node());
    root.set_child(Node::Idx::_PMM_, new Node());

    auto lower_parent = root.child[Node::Idx::_MMM_];
    auto upper_parent = root.child[Node::Idx::_PMM_];
    lower_parent->set_child(Node::Idx::_PMM_, new Node());
    upper_parent->set_child(Node::Idx::_MMM_, new Node());

    EXPECT_EQ(root.find_neighbor(*lower_parent->child[Node::Idx::_PMM_], _ZP_),
              upper_parent->child[Node::Idx::_MMM_]);
}
