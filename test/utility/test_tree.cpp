#ifndef _TEST_TREE_HPP_
#define _TEST_TREE_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"

#include <math.h>
#include <string>
#include <memory>
#include <iostream>
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

typedef TreeNode_<int, 2> Node;
typedef SortedBinaryTree_<Node> BinaryTree;

void visit(BinaryTree::pNode pn){
    std::cout << "value  : " <<  pn->value   << "  " << std::endl;
    std::cout << " height : " <<  pn->height() << std::endl;
    std::cout << " level  : " <<  pn->level() << std::endl;
    if (pn->father() != nullptr){
        std::cout << " f : "   <<  pn->father()->value << std::endl;
    }else{
        std::cout << " f : null" << std::endl;
    }
    if (pn->left_child() != nullptr){
        std::cout << " left : "   <<  pn->left_child()->value << std::endl;
    }else{
        std::cout << " left : null" << std::endl;
    }
    if (pn->right_child() != nullptr){
        std::cout << " right : "   <<  pn->right_child()->value << std::endl;
    }else{
        std::cout << " right : null" << std::endl;
    }
}


TEST(binary_tree, sorted){

    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(5);
    tree.insert(3);
    // tree.insert(3);
    tree.insert(4);

    std::cout << "tree is empty " << tree.empty() << std::endl;
    ASSERT_EQ(tree.empty(), 0);

    tree.pre_order(visit);

}


}

#endif