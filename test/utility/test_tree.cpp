#ifndef _TEST_TREE_HPP_
#define _TEST_TREE_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"

#include <math.h>
#include <string>
#include <memory>
// #include "mpi.h"

#include "utility/binary_tree.hpp"

namespace carpio {

TEST(binary_tree, basic){
    typedef TreeNode_<int, 2> Node;
    typedef BinaryTree_<Node> BinaryTree;

    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    std::cout << "tree is empty " << tree.empty() << std::endl;
    ASSERT_EQ(tree.empty(), 1);
}

TEST(binary_tree, sorted){
    typedef TreeNode_<int, 2> Node;
    typedef SortedBinaryTree_<Node> BinaryTree;

    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    std::cout << "tree is empty " << tree.empty() << std::endl;
    ASSERT_EQ(tree.empty(), 0);

}


}

#endif