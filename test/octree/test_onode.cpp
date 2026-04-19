#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/octree/grid/onode.hpp"
#include "domain/octree/io/ognuplot_actor_label.hpp"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"

#include <iterator>
#include <random>
#include <type_traits>
#include <vector>


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

    const double expected_x[Node::NumChildren] = {0.0, 2.0, 0.0, 2.0};
    const double expected_y[Node::NumChildren] = {0.0, 0.0, 2.0, 2.0};

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
        EXPECT_DOUBLE_EQ(child->cell.get(_C_, _X_), expected_x[i]);
        EXPECT_DOUBLE_EQ(child->cell.get(_C_, _Y_), expected_y[i]);
        EXPECT_DOUBLE_EQ(child->cell.get_hd(_X_), 1.0);
        EXPECT_DOUBLE_EQ(child->cell.get_hd(_Y_), 1.0);
    }
}

TEST(onode, find_root_returns_self_for_root){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;

    EXPECT_EQ(&root.find_root(), &root);
}

TEST(onode, find_root_returns_root_for_direct_child){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_PP_, new Node());

    ASSERT_NE(root.child[Node::Idx::_PP_], nullptr);
    EXPECT_EQ(&root.child[Node::Idx::_PP_]->find_root(), &root);
}

TEST(onode, find_root_returns_topmost_root_for_deep_descendant){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    ASSERT_NE(root.child[Node::Idx::_MM_], nullptr);
    root.child[Node::Idx::_MM_]->set_child(Node::Idx::_PP_, new Node());

    auto grandchild = root.child[Node::Idx::_MM_]->child[Node::Idx::_PP_];
    ASSERT_NE(grandchild, nullptr);
    EXPECT_EQ(&grandchild->find_root(), &root);
}

TEST(onode, find_root_const_returns_const_root_reference){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MP_, new Node());
    ASSERT_NE(root.child[Node::Idx::_MP_], nullptr);

    const Node& const_child = *root.child[Node::Idx::_MP_];
    static_assert(std::is_same_v<decltype(const_child.find_root()), const Node&>);
    EXPECT_EQ(&const_child.find_root(), &root);
}

TEST(onode, iterator_visits_single_node_subtree){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;

    static_assert(std::is_same_v<
        typename std::iterator_traits<Node::iterator>::iterator_category,
        std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<
        typename std::iterator_traits<Node::const_iterator>::iterator_category,
        std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<decltype(*node.begin()), Node&>);

    auto it = node.begin();
    ASSERT_NE(it, node.end());
    EXPECT_EQ(&(*it), &node);
    ++it;
    EXPECT_EQ(it, node.end());

    auto last = node.end();
    --last;
    EXPECT_EQ(&(*last), &node);
}

TEST(onode, iterator_visits_2d_subtree_in_preorder){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_PP_, new Node());
    root.child[Node::Idx::_MM_]->set_child(Node::Idx::_MP_, new Node());
    root.child[Node::Idx::_MM_]->set_child(Node::Idx::_PM_, new Node());
    root.child[Node::Idx::_PP_]->set_child(Node::Idx::_MM_, new Node());

    std::vector<Node*> visited;
    for (auto& node : root) {
        visited.push_back(&node);
    }

    std::vector<Node*> expected = {
        &root,
        root.child[Node::Idx::_MM_],
        root.child[Node::Idx::_MM_]->child[Node::Idx::_MP_],
        root.child[Node::Idx::_MM_]->child[Node::Idx::_PM_],
        root.child[Node::Idx::_PP_],
        root.child[Node::Idx::_PP_]->child[Node::Idx::_MM_],
    };
    EXPECT_EQ(visited, expected);
}

TEST(onode, iterator_from_middle_node_stays_inside_that_subtree){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());
    root.set_child(Node::Idx::_PP_, new Node());
    auto subtree = root.child[Node::Idx::_MP_];
    subtree->set_child(Node::Idx::_MM_, new Node());
    subtree->set_child(Node::Idx::_PP_, new Node());
    root.child[Node::Idx::_PP_]->set_child(Node::Idx::_MM_, new Node());

    std::vector<Node*> visited;
    for (auto& node : *subtree) {
        visited.push_back(&node);
    }

    std::vector<Node*> expected = {
        subtree,
        subtree->child[Node::Idx::_MM_],
        subtree->child[Node::Idx::_PP_],
    };
    EXPECT_EQ(visited, expected);
}

TEST(onode, for_each_visits_current_node_subtree_in_preorder){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());
    root.set_child(Node::Idx::_PP_, new Node());
    auto subtree = root.child[Node::Idx::_MP_];
    subtree->set_child(Node::Idx::_MM_, new Node());
    subtree->set_child(Node::Idx::_PP_, new Node());
    root.child[Node::Idx::_PP_]->set_child(Node::Idx::_MM_, new Node());

    std::vector<Node*> visited;
    subtree->for_each([&](Node::ref_Node node) {
        visited.push_back(&node);
    });

    std::vector<Node*> expected = {
        subtree,
        subtree->child[Node::Idx::_MM_],
        subtree->child[Node::Idx::_PP_],
    };
    EXPECT_EQ(visited, expected);

    const Node& const_subtree = *subtree;
    std::vector<const Node*> const_visited;
    const_subtree.for_each([&](Node::const_ref_Node node) {
        const_visited.push_back(&node);
    });

    std::vector<const Node*> const_expected = {
        subtree,
        subtree->child[Node::Idx::_MM_],
        subtree->child[Node::Idx::_PP_],
    };
    EXPECT_EQ(const_visited, const_expected);
}

TEST(onode, iterator_reverse_order_matches_forward_order_reversed){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());
    root.child[Node::Idx::_MM_]->set_child(Node::Idx::_PM_, new Node());
    root.child[Node::Idx::_MP_]->set_child(Node::Idx::_PP_, new Node());

    std::vector<Node*> forward;
    for (auto it = root.begin(); it != root.end(); ++it) {
        forward.push_back(&(*it));
    }

    std::vector<Node*> backward;
    for (auto it = root.end(); it != root.begin();) {
        --it;
        backward.push_back(&(*it));
    }

    ASSERT_EQ(backward.size(), forward.size());
    for (St i = 0; i < forward.size(); ++i) {
        EXPECT_EQ(backward[i], forward[forward.size() - 1 - i]);
    }
}

TEST(onode, const_iterator_dereferences_to_const_node){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node node;
    const Node& const_node = node;

    static_assert(std::is_same_v<decltype(*const_node.begin()), const Node&>);
    static_assert(std::is_same_v<decltype(*const_node.cbegin()), const Node&>);
    EXPECT_EQ(&(*const_node.begin()), &const_node);
    EXPECT_EQ(&(*const_node.cbegin()), &const_node);
}

TEST(onode, plots_random_subtree_2d){
    using Cell = OCellUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    std::mt19937 rng(20260419);
    std::uniform_real_distribution<double> center_dist(-1.0, 1.0);
    std::uniform_real_distribution<double> half_dist(0.8, 1.6);
    std::uniform_real_distribution<double> probability(0.0, 1.0);

    Node root;
    root.cell = Cell(1.0, 0.0, 0.0);

    const St max_level = 4;
    auto refine_randomly = [&](auto&& self, Node* node) -> void {
        ASSERT(node != nullptr);
        if (node->level() >= max_level) {
            return;
        }

        const double threshold = node->level() == 0 ? 1.0 : 0.45;
        if (probability(rng) >= threshold) {
            return;
        }

        node->new_full_children();

        for (St i = 0; i < Node::NumChildren; ++i) {
            if (node->child[i] != nullptr) {
                self(self, node->child[i]);
            }
        }
    };

    refine_randomly(refine_randomly, &root);

    Gnuplot gnu;
    const auto xmin = root.cell.get(_M_, _X_);
    const auto xmax = root.cell.get(_P_, _X_);
    const auto ymin = root.cell.get(_M_, _Y_);
    const auto ymax = root.cell.get(_P_, _Y_);
    const auto margin_x = 0.05 * root.cell.get_d(_X_);
    const auto margin_y = 0.05 * root.cell.get_d(_Y_);

    gnu.set_xrange(xmin - margin_x, xmax + margin_x);
    gnu.set_yrange(ymin - margin_y, ymax + margin_y);
    gnu.set_equal_aspect_ratio();

    auto root_frame = ToGnuplotActorWireFrame(root);
    root_frame.line_width(2);
    root_frame.line_color("red");
    gnu.add(root_frame);

    St leaf_count = 0;
    for (const auto& node : root) {
        if (!node.is_leaf()) {
            continue;
        }
        auto acount = ToGnuplotActorLabel(node, std::to_string(leaf_count));
        auto leaf_frame = ToGnuplotActorWireFrame(node);
        leaf_frame.line_width(1);
        leaf_frame.line_color("black");
        gnu.add(leaf_frame);
        gnu.add(acount);
        leaf_count++;
    }

    gnu.set_terminal_png(FIG_PATH + "OctreeONodeRandomSubtree2D", fig_width, fig_height);
    gnu.plot();
}

TEST(onode, find_face_neighbor_same_parent_siblings_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());
    root.set_child(Node::Idx::_PM_, new Node());

    EXPECT_EQ(root.child[Node::Idx::_MM_]->find_face_neighbor(_XP_),
              root.child[Node::Idx::_MP_]);
    EXPECT_EQ(root.child[Node::Idx::_MM_]->find_face_neighbor(_YP_),
              root.child[Node::Idx::_PM_]);
}

TEST(onode, find_face_neighbor_climbs_to_common_ancestor_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MM_, new Node());
    root.set_child(Node::Idx::_MP_, new Node());

    auto left_parent = root.child[Node::Idx::_MM_];
    auto right_parent = root.child[Node::Idx::_MP_];
    left_parent->set_child(Node::Idx::_MP_, new Node());
    right_parent->set_child(Node::Idx::_MM_, new Node());

    EXPECT_EQ(left_parent->child[Node::Idx::_MP_]->find_face_neighbor(_XP_),
              right_parent->child[Node::Idx::_MM_]);
}

TEST(onode, find_face_neighbor_crosses_root_neighbor_and_descends_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node left_root;
    Node right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(Node::Idx::_MP_, new Node());
    right_root.set_child(Node::Idx::_MM_, new Node());

    EXPECT_EQ(left_root.child[Node::Idx::_MP_]->find_face_neighbor(_XP_),
              right_root.child[Node::Idx::_MM_]);
}

TEST(onode, find_face_neighbor_returns_coarser_neighbor_when_child_is_missing_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node left_root;
    Node right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(Node::Idx::_MP_, new Node());

    EXPECT_EQ(left_root.child[Node::Idx::_MP_]->find_face_neighbor(_XP_),
              &right_root);
}

TEST(onode, find_face_neighbor_returns_nullptr_for_missing_boundary_neighbor_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;
    root.set_child(Node::Idx::_MP_, new Node());

    EXPECT_EQ(root.child[Node::Idx::_MP_]->find_face_neighbor(_XP_), nullptr);
}

TEST(onode, find_face_neighbor_rejects_direction_outside_dimension_2d){
    using Cell = OCellNonUniform_<double, 2>;
    using Node = ONode_<double, Cell, 2>;

    Node root;

    EXPECT_EQ(root.find_face_neighbor(_ZP_), nullptr);
}

TEST(onode, find_face_neighbor_climbs_to_common_ancestor_3d_z_direction){
    using Cell = OCellNonUniform_<double, 3>;
    using Node = ONode_<double, Cell, 3>;

    Node root;
    root.set_child(Node::Idx::_MMM_, new Node());
    root.set_child(Node::Idx::_PMM_, new Node());

    auto lower_parent = root.child[Node::Idx::_MMM_];
    auto upper_parent = root.child[Node::Idx::_PMM_];
    lower_parent->set_child(Node::Idx::_PMM_, new Node());
    upper_parent->set_child(Node::Idx::_MMM_, new Node());

    EXPECT_EQ(lower_parent->child[Node::Idx::_PMM_]->find_face_neighbor(_ZP_),
              upper_parent->child[Node::Idx::_MMM_]);
}
