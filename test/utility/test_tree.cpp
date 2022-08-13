#ifndef _TEST_TREE_HPP_
#define _TEST_TREE_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"

#include <math.h>
#include <string>
#include <memory>
#include <iostream>

#include "utility/binary_tree.hpp"
#include "io/gnuplot.hpp"

namespace carpio {

typedef TreeNode_<int, 2> Node;
typedef SortedBinaryTree_<Node> BinaryTree;

void AddNode(
    int order, Gnuplot& gnu,
    const double& x, const double& y, 
    const std::string& str){
    // plot a circle
    std::list<double> lx, ly;
    int n = 10;
    double r = 0.8;
    double da = 2 * _PI_ / 10;
    for (uInt i = 0; i < n; i++) {
        Vt rx = r * cos(i * da) + x;
        Vt ry = r * sin(i * da) + y;
        lx.push_back(rx);
        ly.push_back(ry);
    }
    lx.push_back(r + x);
    ly.push_back(y);
    auto actor = ToGnuplotActor(lx, ly);
    gnu.add(actor);
    gnu.set_label(order, str, x, y, "center");
}

struct PlotInfo{
    int value;
    double x;
    double y;

    bool operator<(const PlotInfo& rhs) const {
		return (this->value < rhs.value);
	}
};

std::ostream& operator<<(std::ostream& stream, const PlotInfo& v) {
    stream << v.value << "  " << v.x << "  " << v.y ;
    return stream;
}

typedef TreeNode_<PlotInfo, 2> PlotNode;
typedef SortedBinaryTree_<PlotNode> PlotTree;

void UpdateXY(PlotTree::pNode pn){
    // increase y by father
    if(pn->father != nullptr){
        pn->value.y = pn->father->value.y + 1;
    }
    // increase x by neighbor
    auto nei = pn->neighbor(PlotTree::Node::_LEFT_);
    if(nei != nullptr){
        if(nei->value.x == pn->value.x){

        }
    }
    // output
    if(pn != nullptr){
        std::cout << *pn << std::endl;
        std::cout << "--------------" << std::endl;
    }
    // auto pnn = pn->neighbor(0);
    // if(pnn != nullptr){
    //     std::cout << *pnn << std::endl;
    // }else{
    //     std::cout << "null" << std::endl;
    // }

    // std::cout << pnn << std::endl;
}

TEST(binary_tree, plot){
    PlotTree tree;
    tree.insert({3, 0, 0});
    auto pn = tree.insert({4, 0, 0});
    UpdateXY(pn);
    UpdateXY(tree.insert({2, 0, 0}));

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/plot_tree");
    gnu.set_xrange(-5, 5);
    gnu.set_yrange(-5, 5);
    gnu.set_equal_aspect_ratio();
    AddNode(1, gnu, 0, 0, "aa");
    gnu.plot();
}


TEST(binary_tree, basic){
    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    std::cout << "tree is empty " << tree.empty() << std::endl;
    ASSERT_EQ(tree.empty(), 1);
}

void visit(BinaryTree::pNode pn, int& count){
    std::cout << pn->value << "  " << std::endl;
    std::cout << "c = " << count << std::endl;
    count++;
}


TEST(binary_tree, sorted){

    BinaryTree tree;
    BinaryTree t2;

    t2 = tree;

    tree.insert(3);
    tree.insert(5);
    tree.insert(2);
    tree.insert(1);
    // tree.insert(3);
    tree.insert(4);

    std::cout << "tree is empty " << tree.empty() << std::endl;
    std::cout << "height = " << tree.height() << std::endl;
    ASSERT_EQ(tree.empty(), 0);
    int c =0;
    tree.pre_order(visit, c);
    auto pn = tree.root();
    pn = pn->child(1);
    std::cout << "pn child(1) = " << pn->value << std::endl;
    std::cout << "pn child(1) has sibling = " << pn->has_sibling() << std::endl;
    std::cout << "pn child(1) neighbor  = " << pn->neighbor(0)->value << std::endl;


}


}

#endif