#ifndef _TEST_TREE_HPP_
#define _TEST_TREE_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"

#include <math.h>
#include <string>
#include <memory>
#include <iostream>

#include "utility/binary_tree.hpp"
#include "utility/avl_tree.hpp"
#include "utility/random.hpp"
#include "geometry/geometry.hpp"
#include "io/gnuplot.hpp"

namespace carpio {

typedef TreeNode_<int, 2> Node;
typedef SortedBinaryTree_<Node> BinaryTree;


typedef AvlTree_<Node> AvlTree;

void AddNode(
    int order, Gnuplot& gnu,
    const double& x,  const double& y, 
    const std::string& str){
    // plot a circle
    std::list<double> lx, ly;
    int n = 20;
    double r = 0.8;
    double da = 2 * _PI_ / n;
    for (int i = 0; i < n; i++) {
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
    gnu.set_label(order, str, x, y, "center noenhanced front");
}
void AddNodeEmphi(
    int order, Gnuplot& gnu,
    const double& x,  const double& y, 
    const std::string& str){
    // plot a circle
    std::list<double> lx, ly;
    int n = 20;
    double r = 0.8;
    double da = 2 * _PI_ / n;
    for (int i = 0; i < n; i++) {
        Vt rx = r * cos(i * da) + x;
        Vt ry = r * sin(i * da) + y;
        lx.push_back(rx);
        ly.push_back(ry);
    }
    lx.push_back(r + x);
    ly.push_back(y);
    auto actor = ToGnuplotActor(lx, ly);
    actor.style("with lines lc rgb \"#22F25022\" lw 4");
    gnu.add(actor);
    // label
}
void AddLink(Gnuplot& gnu, 
            const double& x, const double& y,
            const double& fx, const double& fy){
    typedef Point_<double, 2> Point;
    Point_<double, 2> s(x,   y);
    Point_<double, 2> e(fx, fy);
    // radius = 0.8
    double r = 0.8;
    auto ns = NewPointFromStart(s,e,r);
    auto ne = NewPointFromEnd(s,e,r);
    // std::list<Point>  lp{ns, ne}; 
    std::list<double> lx{ns.x(), ne.x()};
    std::list<double> ly{ns.y(), ne.y()};
    auto actor = ToGnuplotActor(lx, ly);
    actor.style("with lines lc rgb \"#00A4EF\" lw 3");
    gnu.add(actor);
}


struct PlotInfo{
    double value;
    double x;
    double y;

    PlotInfo():value(0.0), x(0.0), y(0.0){
    }
    PlotInfo(const double& _v,
             const double& _x,
             const double& _y):value(_v), x(_x), y(_y){
    }

    PlotInfo(const double& _v):value(_v), x(0.0), y(0.0){}

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
typedef AvlTree_<PlotNode> PlotAvlTree;

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
        if((d == PlotTree::Node::_RIGHT_ && nei->value.x <= pn->value.x) ||
           (d == PlotTree::Node::_LEFT_  && nei->value.x >= pn->value.x)){
            auto ca = tree.common_ancestor(pn, d);
            if(ca != nullptr){
                ca->right_child->pre_order(visit_plus_x, 1.0);
                ca->left_child->pre_order(visit_plus_x, -1.0);
            }
            return true;
        }
    }
    return false;
}
void UpdateXY(PlotTree& tree, PlotTree::pNode pn){
    // increase y by father
    if(pn->father != nullptr || pn != tree.root()){
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
        // std::cout << *pn << std::endl;
        // std::cout << "--------------" << std::endl;
    }
}

void Update(PlotTree& tree, PlotTree::pNode pn){
    // increase y by father
    if (pn == nullptr){
        return;
    }
    if(pn->father != nullptr && pn != tree.root()){
        pn->value.y = pn->father->value.y - 2;
        // pn->value.x = pn->father->value.x;
    }else{
        pn->value.y = 0;
        pn->value.x = 0;
    }
    // increase x by neighbor
    bool ur, ul;
    do{
        ur = IncreaseX(tree, pn, PlotTree::Node::_RIGHT_);
        ul = IncreaseX(tree, pn, PlotTree::Node::_LEFT_);
    }while(ur || ul);
    Update(tree, pn->left_child);
    Update(tree, pn->right_child);
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
    UpdateXY(tree, tree.insert({3.65, 0, 0}));

    // find
    auto pf = tree.find(3.1);
    std::cout << "find = " << *pf << std::endl;
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/plot_tree");
    // gnu.set_xrange(-10, 10);
    // gnu.set_yrange(-10, 1);
    gnu.set_equal_aspect_ratio();
    int order = 1;
    tree.pre_order(visit_add_node, gnu, order);
    AddNodeEmphi(100, gnu, pf->value.x, pf->value.y, "aa"); 
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

void visit_print(PlotAvlTree::pNode pn, int& count){
    std::cout << pn->value << std::endl;
    count++;
}

void visit_to_array(PlotAvlTree::cpNode pn, std::list<double>& l){
    l.push_back(pn->value.value);
}

bool IsOrderIncrease(const PlotAvlTree& tree){
    std::list<double> lv;
    tree.in_order(visit_to_array, lv);

    for(auto iter = lv.begin(); iter != lv.end(); iter++){
        auto next = std::next(iter);
        if(next != lv.end() && *next < *iter){
            return false;
        }
    }
    return true;
}

void output_node(const PlotAvlTree::pNode pn){
    if(pn == nullptr){
        std::cout << " cur  =  null" << std::endl;
        return;
    }
    std::cout << "cur.value   = " << pn->value << std::endl;
    if (pn->father == nullptr){
        std::cout << "father      = null \n";
    }else{
        std::cout << "father.value= " << pn->father->value << std::endl;
    }
    if (pn->left_child == nullptr){
        std::cout << "left        = null \n";
    }else{
        std::cout << "left.value  = " << pn->left_child->value << std::endl;
    }
    if (pn->right_child == nullptr){
        std::cout << "right       = null \n";
    }else{
        std::cout << "right.value = " << pn->right_child->value << std::endl;
    }
}


TEST(avl_tree, basic){

    PlotAvlTree tree;
    tree.insert({4, 0, 0});
    tree.insert({2, 0, 0});
    tree.insert({3, 0, 0});
    tree.insert({1, 0, 0});
    // tree.insert({1.1, 0, 0});
    tree.insert({0.5, 0, 0});
    tree.insert({0.11, 0, 0});
    tree.insert({5, 0, 0});
    tree.insert({0.15, 0, 0});
    tree.insert({4.14, 0, 0});
    tree.insert({4.1, 0, 0});
    tree.insert({4.16, 0, 0});
    tree.insert({0.21, 0, 0});
    tree.insert({0.2, 0, 0});
    tree.insert({0.3, 0, 0});
    // output_node(tree.root()->left_child->left_child);
    // std::cout << tree.root()->value << std::endl;
    // std::cout << tree.root()->left_child->value << std::endl;
    // std::cout << tree.root()->right_child->value << std::endl;
    // std::cout << tree.root()->left_child->left_child->value << std::endl;
    // std::cout << tree.root()->left_child->right_child->value << std::endl;
    // tree.erase({1.1, 0, 0});
    // tree.erase({4, 0, 0});
    // tree.erase({5, 0, 0});
    tree.erase({0.21, 0, 0});

    Update(tree, tree.root());
    Update(tree, tree.root());
    bool incr = IsOrderIncrease(tree);
    std::cout << "Is Increase = " << incr << std::endl;
    ASSERT_EQ(incr, 1);
    // find
    auto fp = tree.find(4.1);
    output_node(fp);


    int count = 0;
    // tree.in_order(visit_print, count);
    // UpdateXY(tree, tree.insert({2.5, 0, 0}));
    // UpdateXY(tree, tree.insert({3.5, 0, 0}));
    // UpdateXY(tree, tree.insert({3.1, 0, 0}));
    // UpdateXY(tree, tree.insert({3.6, 0, 0}));
    // UpdateXY(tree, tree.insert({2.4, 0, 0}));
    // UpdateXY(tree, tree.insert({2.6, 0, 0}));
    // UpdateXY(tree, tree.insert({2.7, 0, 0}));
    // UpdateXY(tree, tree.insert({3.65, 0, 0}));

    // find
    // auto pf = tree.find(3.1);
    // std::cout << "find = " << *pf << std::endl;
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/plot_avl_tree");
    // gnu.set_xrange(-10, 10);
    // gnu.set_yrange(-10, 1);
    gnu.set_equal_aspect_ratio();
    int order = 1;
    tree.pre_order(visit_add_node, gnu, order);
    // AddNodeEmphi(100, gnu, pf->value.x, pf->value.y, "aa"); 
    gnu.plot();


    // test find
}
TEST(avl_tree, random){
    
    PlotAvlTree tree;
    int num = 20;
    double vmin = -100;
    double vmax = 100;
    for (int i = 0; i< num ; i++){
        double v = Random::nextDouble(vmin, vmax);
        tree.insert({v, 0, 0});
    }
    Update(tree, tree.root());
    Update(tree, tree.root());
    Update(tree, tree.root());

    bool incr = IsOrderIncrease(tree);
    std::cout << "Is Increase = " << incr << std::endl;
    ASSERT_EQ(incr, 1);
    
    bool isb  = tree.is_balanced();
    std::cout << "Is balanced = " << isb << std::endl;
    ASSERT_EQ(isb, 1);
    

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/plot_avl_tree_r");
    // gnu.set_xrange(-10, 10);
    // gnu.set_yrange(-10, 1);
    gnu.set_equal_aspect_ratio();
    int order = 1;
    tree.pre_order(visit_add_node, gnu, order);
    // AddNodeEmphi(100, gnu, pf->value.x, pf->value.y, "aa"); 
    gnu.plot();
}

}

#endif