#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/octree/grid/otree.hpp"


using namespace carpio;

TEST(otree, initial){
    using Cell = OCellNonUniform_<double, 2>;
    using Tree = OTree_<double, Cell, 2>;

    Tree tree;

    ASSERT_NE(tree.root, nullptr);
    EXPECT_TRUE(tree.root->is_root());
    EXPECT_TRUE(tree.root->is_leaf());
}

TEST(otree, owns_root_children){
    using Cell = OCellNonUniform_<double, 2>;
    using Tree = OTree_<double, Cell, 2>;
    using Node = Tree::Node;

    Tree tree;
    tree.root->set_child(Node::Idx::_PP_, new Node());

    ASSERT_TRUE(tree.root->has_child(Node::Idx::_PP_));
    EXPECT_EQ(tree.root->child[Node::Idx::_PP_]->father, tree.root);
}
