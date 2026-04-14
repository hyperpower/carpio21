#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/octree/grid/onode.hpp"


using namespace carpio;

TEST(onode, initial){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;

    EXPECT_EQ(node.father, nullptr);

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
    node.set_child(Node::Idx::_PP_, new Node());

    ASSERT_TRUE(node.has_child(Node::Idx::_PP_));
    EXPECT_TRUE(node.has_child());
    EXPECT_FALSE(node.is_leaf());
    EXPECT_EQ(node.child[Node::Idx::_PP_]->father, &node);

    node.delete_child(Node::Idx::_PP_);

    EXPECT_FALSE(node.has_child(Node::Idx::_PP_));
    EXPECT_FALSE(node.has_child());
    EXPECT_TRUE(node.is_leaf());
}

TEST(onode, construct_child_node){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    Cell cell(1.0, 0.25, 2.0, 0.25);
    Node child(Node::Idx::_PP_, &root, cell, 3.0);

    EXPECT_EQ(child.father, &root);
    EXPECT_FALSE(child.is_root());
    EXPECT_TRUE(child.is_leaf());
    EXPECT_DOUBLE_EQ(child.cell.get(_C_, _X_), 1.0);
    EXPECT_DOUBLE_EQ(child.cell.get(_C_, _Y_), 2.0);
    EXPECT_DOUBLE_EQ(child.data, 3.0);
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
    node.cell = Cell(1.0, 0.0, 0.0);
    node.data = 7.0;

    node.new_child(Node::Idx::_PP_);

    ASSERT_TRUE(node.has_child(Node::Idx::_PP_));
    auto child = node.child[Node::Idx::_PP_];
    EXPECT_EQ(child->father, &node);
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
        EXPECT_TRUE(child->is_leaf());
        EXPECT_DOUBLE_EQ(child->data, node.data);
        EXPECT_DOUBLE_EQ(child->cell.get_hd(_X_), 1.0);
        EXPECT_DOUBLE_EQ(child->cell.get_hd(_Y_), 1.0);
    }
}
