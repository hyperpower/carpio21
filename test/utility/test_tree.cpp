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
    const double& x,  const double& y, 
    const std::string& str){
    // plot a circle
    std::list<double> lx, ly;
    int n = 20;
    double r = 0.8;
    double da = 2 * _PI_ / n;
    for (uInt i = 0; i < n; i++) {
        Vt rx = r * cos(i * da) + x;
        Vt ry = r * sin(i * da) + y;
        lx.push_back(rx);
        ly.push_back(ry);
    }
    lx.push_back(r + x);
    ly.push_back(y);
    auto actor = ToGnuplotActor(lx, ly);
    actor.style("with filledcurves closed lc rgb \"#557FBA00\"");
    gnu.add(actor);
    // label
    gnu.set_label(order, str, x, y, "center front");
}

void AddLink(Gnuplot& gnu, 
            const double& x, const double& y,
            const double& fx, const double& fy){
    std::list<double> lx{x, fx};
    std::list<double> ly{y, fy};
    auto actor = ToGnuplotActor(lx, ly);
    actor.style("with lines lc rgb \"#00A4EF\" lw 2");
    gnu.add(actor);
}


struct PlotInfo{
    double value;
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

void visit_plus_x(PlotTree::pNode pn, double v){
    pn->value.x += v;
}

void visit_add_node(PlotTree::pNode pn, Gnuplot& gnu, int& order){
    if(pn->father != nullptr){
        AddLink(gnu, 
            pn->value.x, pn->value.y, 
            pn->father->value.x, pn->father->value.y
        );
    }
    AddNode(order, gnu, 
            pn->value.x, pn->value.y, 
            ToString(pn->value.value));

    order++;
}

bool IncreaseX(PlotTree& tree, PlotTree::pNode pn, int d){
    auto nei = tree.neighbor(pn, d);
    if(nei != nullptr){
        if(nei->value.x == pn->value.x){
            auto ca = tree.common_ancestor(pn, d);
            ca->right_child->pre_order(visit_plus_x, 1.0);
            ca->left_child->pre_order(visit_plus_x, -1.0);
            return true;
        }
    }
    return false;
}
void UpdateXY(PlotTree& tree, PlotTree::pNode pn){
    // increase y by father
    if(pn->father != nullptr){
        pn->value.y = pn->father->value.y - 2;
        pn->value.x = pn->father->value.x;
    }
    // increase x by neighbor
    bool ur, ul;
    do{
        ur = IncreaseX(tree, pn, PlotTree::Node::_RIGHT_);
        ul = IncreaseX(tree, pn, PlotTree::Node::_LEFT_);
    }while(ur || ul);
    
    // output
    if(pn != nullptr){
        std::cout << *pn << std::endl;
        std::cout << "--------------" << std::endl;
    }
}

TEST(binary_tree, plot){
    PlotTree tree;
    tree.insert({3, 0, 0});
    auto pn = tree.insert({4, 0, 0});
    std::cout << "is right = " << pn->is_right_child() << std::endl;
    UpdateXY(tree, pn);
    UpdateXY(tree, tree.insert({2, 0, 0}));
    UpdateXY(tree, tree.insert({1, 0, 0}));
    UpdateXY(tree, tree.insert({5, 0, 0}));
    UpdateXY(tree, tree.insert({2.5, 0, 0}));
    UpdateXY(tree, tree.insert({3.5, 0, 0}));
    UpdateXY(tree, tree.insert({3.1, 0, 0}));
    UpdateXY(tree, tree.insert({3.6, 0, 0}));
    UpdateXY(tree, tree.insert({2.4, 0, 0}));
    UpdateXY(tree, tree.insert({2.6, 0, 0}));
    UpdateXY(tree, tree.insert({2.7, 0, 0}));
    UpdateXY(tree, tree.insert({3.05, 0, 0}));

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/plot_tree");
    gnu.set_xrange(-10, 10);
    gnu.set_yrange(-10, 1);
    gnu.set_equal_aspect_ratio();
    int order = 1;
    tree.pre_order(visit_add_node, gnu, order);
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
    std::cout << "pn child(1) neighbor  = " << tree.neighbor(pn,0)->value << std::endl;


}


}

#endif