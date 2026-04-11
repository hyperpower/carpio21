#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/octree/grid/onode.hpp"


using namespace carpio;

TEST(onode, initial){
    using Cell = OCellRect_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;

    EXPECT_EQ(node.father, nullptr);
    EXPECT_EQ(node.cell, nullptr);
    EXPECT_EQ(node.data, nullptr);

    for (St i = 0; i < Node::NumChildren; ++i) {
        EXPECT_EQ(node.child[i], nullptr);
    }
    for (St i = 0; i < Node::NumNeighbors; ++i) {
        EXPECT_EQ(node.neighbor[i], nullptr);
    }
}
