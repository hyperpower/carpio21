#ifndef _TEST_TREE_HPP_
#define _TEST_TREE_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"

#include <math.h>
#include <string>
#include <memory>
#include <iostream>

#include "utility/binary_tree.hpp"

namespace carpio {

typedef TreeNode_<int, 2> Node;
typedef SortedBinaryTree_<Node> BinaryTree;


TEST(binary_tree, basic){
    typedef TreeNode_<int, 2> Node;
    typedef BinaryTree_<Node> BinaryTree;

    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    std::cout << "tree is empty " << tree.empty() << std::endl;
    ASSERT_EQ(tree.empty(), 1);
}

void visit(BinaryTree::pNode pn){
    std::cout << pn->value << "  " << std::endl;
}


TEST(binary_tree, sorted){

    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(5);

    std::cout << "tree is empty " << tree.empty() << std::endl;
    ASSERT_EQ(tree.empty(), 0);

    tree.pre_order(visit);

}


}

#endif