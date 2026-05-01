#include "gtest/gtest.h"
#include "test_define.hpp"
#include "test_octree.h"
#include "domain/octree/io/ognuplot_actor_label.hpp"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"

#include <iterator>
#include <random>
#include <type_traits>
#include <vector>


using namespace carpio;
using namespace carpio::octree_test;

TEST(onode, initial){

    NonUniformNode2 node;

    EXPECT_EQ(node.father, nullptr);
    EXPECT_EQ(node.root_idx(), 0);

    for (St i = 0; i < NonUniformNode2::NumChildren; ++i) {
        EXPECT_EQ(node.child[i], nullptr);
    }
    for (St i = 0; i < NonUniformNode2::NumNeighbors; ++i) {
        EXPECT_EQ(node.neighbor[i], nullptr);
    }
}

TEST(onode, child_ownership){

    NonUniformNode2 node;
    node.set_root_idx(12);
    node.set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());

    ASSERT_TRUE(node.has_child(NonUniformNode2::Idx::_PP_));
    EXPECT_TRUE(node.has_child());
    EXPECT_FALSE(node.is_leaf());
    EXPECT_EQ(node.child[NonUniformNode2::Idx::_PP_]->father, &node);
    EXPECT_EQ(node.child[NonUniformNode2::Idx::_PP_]->root_idx(), 12);

    node.delete_child(NonUniformNode2::Idx::_PP_);

    EXPECT_FALSE(node.has_child(NonUniformNode2::Idx::_PP_));
    EXPECT_FALSE(node.has_child());
    EXPECT_TRUE(node.is_leaf());
}

TEST(onode, construct_child_node){

    NonUniformNode2 root;
    root.set_root_idx(7);
    NonUniformCell2 cell(1.0, 0.25, 2.0, 0.25);
    NonUniformNode2 child(NonUniformNode2::Idx::_PP_, &root, cell, 3.0);

    EXPECT_EQ(child.father, &root);
    EXPECT_FALSE(child.is_root());
    EXPECT_TRUE(child.is_leaf());
    EXPECT_DOUBLE_EQ(child.cell.get(_C_, _X_), 1.0);
    EXPECT_DOUBLE_EQ(child.cell.get(_C_, _Y_), 2.0);
    EXPECT_DOUBLE_EQ(child.data, 3.0);
    EXPECT_EQ(child.root_idx(), 7);
    for (St i = 0; i < NonUniformNode2::NumChildren; ++i) {
        EXPECT_EQ(child.child[i], nullptr);
    }
    for (St i = 0; i < NonUniformNode2::NumNeighbors; ++i) {
        EXPECT_EQ(child.neighbor[i], nullptr);
    }
}

TEST(onode, exposes_morton_code){

    UniformNode2 root;

    EXPECT_EQ(root.code(), UniformNode2::MortonCode::Code{0});

    root.set_child(UniformNode2::Idx::_PP_, new UniformNode2());
    auto child = root.child[UniformNode2::Idx::_PP_];
    ASSERT_NE(child, nullptr);

    EXPECT_EQ(child->code(),
              (UniformNode2::MortonCode::Code{1} << UniformNode2::MortonCode::LevelShift)
              | (UniformNode2::MortonCode::Code{UniformNode2::Idx::_PP_}
                 << (UniformNode2::MortonCode::PathBits - UniformNode2::MortonCode::BitsPerLevel)));

    child->set_child(UniformNode2::Idx::_MP_, new UniformNode2());
    auto grandchild = child->child[UniformNode2::Idx::_MP_];
    ASSERT_NE(grandchild, nullptr);

    const auto expected_path =
        (UniformNode2::MortonCode::Code{UniformNode2::Idx::_PP_}
         << (UniformNode2::MortonCode::PathBits - UniformNode2::MortonCode::BitsPerLevel))
        | (UniformNode2::MortonCode::Code{UniformNode2::Idx::_MP_}
           << (UniformNode2::MortonCode::PathBits - 2 * UniformNode2::MortonCode::BitsPerLevel));
    EXPECT_EQ(grandchild->code(),
              (UniformNode2::MortonCode::Code{2} << UniformNode2::MortonCode::LevelShift)
              | expected_path);
}

TEST(onode, new_child_from_parent_cell){

    UniformNode2 node;
    node.set_root_idx(21);
    node.cell = UniformCell2(1.0, 0.0, 0.0);
    node.data = 7.0;

    node.new_child(UniformNode2::Idx::_PP_);

    ASSERT_TRUE(node.has_child(UniformNode2::Idx::_PP_));
    auto child = node.child[UniformNode2::Idx::_PP_];
    EXPECT_EQ(child->father, &node);
    EXPECT_EQ(child->root_idx(), 21);
    EXPECT_DOUBLE_EQ(child->data, node.data);
    EXPECT_DOUBLE_EQ(child->cell.get(_C_, _X_), 0.5);
    EXPECT_DOUBLE_EQ(child->cell.get(_C_, _Y_), 0.5);
    EXPECT_DOUBLE_EQ(child->cell.get_hd(_X_), 0.5);
    EXPECT_DOUBLE_EQ(child->cell.get_hd(_Y_), 0.5);

    child->new_child(UniformNode2::Idx::_MM_);
    ASSERT_TRUE(child->has_child(UniformNode2::Idx::_MM_));

    node.new_child(UniformNode2::Idx::_PP_);

    ASSERT_TRUE(node.has_child(UniformNode2::Idx::_PP_));
    EXPECT_EQ(node.child[UniformNode2::Idx::_PP_]->father, &node);
    EXPECT_TRUE(node.child[UniformNode2::Idx::_PP_]->is_leaf());
}

TEST(onode, new_full_children_from_parent_cell){

    UniformNode2 node;
    node.set_root_idx(33);
    node.cell = UniformCell2(2.0, 1.0, 1.0);
    node.data = 7.0;

    node.new_full_children();

    const double expected_x[UniformNode2::NumChildren] = {0.0, 2.0, 0.0, 2.0};
    const double expected_y[UniformNode2::NumChildren] = {0.0, 0.0, 2.0, 2.0};

    EXPECT_TRUE(node.is_full_child());
    EXPECT_FALSE(node.is_leaf());
    EXPECT_EQ(node.height(), 1);
    for (St i = 0; i < UniformNode2::NumChildren; ++i) {
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

    NonUniformNode2 root;

    EXPECT_EQ(&root.find_root(), &root);
}

TEST(onode, find_root_returns_root_for_direct_child){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());

    ASSERT_NE(root.child[NonUniformNode2::Idx::_PP_], nullptr);
    EXPECT_EQ(&root.child[NonUniformNode2::Idx::_PP_]->find_root(), &root);
}

TEST(onode, find_root_returns_topmost_root_for_deep_descendant){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    ASSERT_NE(root.child[NonUniformNode2::Idx::_MM_], nullptr);
    root.child[NonUniformNode2::Idx::_MM_]->set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());

    auto grandchild = root.child[NonUniformNode2::Idx::_MM_]->child[NonUniformNode2::Idx::_PP_];
    ASSERT_NE(grandchild, nullptr);
    EXPECT_EQ(&grandchild->find_root(), &root);
}

TEST(onode, find_root_const_returns_const_root_reference){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    ASSERT_NE(root.child[NonUniformNode2::Idx::_MP_], nullptr);

    const NonUniformNode2& const_child = *root.child[NonUniformNode2::Idx::_MP_];
    static_assert(std::is_same_v<decltype(const_child.find_root()), const NonUniformNode2&>);
    EXPECT_EQ(&const_child.find_root(), &root);
}

TEST(onode, iterator_visits_single_node_subtree){

    NonUniformNode2 node;

    static_assert(std::is_same_v<
        typename std::iterator_traits<NonUniformNode2::iterator>::iterator_category,
        std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<
        typename std::iterator_traits<NonUniformNode2::const_iterator>::iterator_category,
        std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<decltype(*node.begin()), NonUniformNode2&>);

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

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_MM_]->set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_MM_]->set_child(NonUniformNode2::Idx::_PM_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_PP_]->set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    std::vector<NonUniformNode2*> visited;
    for (auto& node : root) {
        visited.push_back(&node);
    }

    std::vector<NonUniformNode2*> expected = {
        &root,
        root.child[NonUniformNode2::Idx::_MM_],
        root.child[NonUniformNode2::Idx::_MM_]->child[NonUniformNode2::Idx::_MP_],
        root.child[NonUniformNode2::Idx::_MM_]->child[NonUniformNode2::Idx::_PM_],
        root.child[NonUniformNode2::Idx::_PP_],
        root.child[NonUniformNode2::Idx::_PP_]->child[NonUniformNode2::Idx::_MM_],
    };
    EXPECT_EQ(visited, expected);
}

TEST(onode, iterator_from_middle_node_stays_inside_that_subtree){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());
    auto subtree = root.child[NonUniformNode2::Idx::_MP_];
    subtree->set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    subtree->set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_PP_]->set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    std::vector<NonUniformNode2*> visited;
    for (auto& node : *subtree) {
        visited.push_back(&node);
    }

    std::vector<NonUniformNode2*> expected = {
        subtree,
        subtree->child[NonUniformNode2::Idx::_MM_],
        subtree->child[NonUniformNode2::Idx::_PP_],
    };
    EXPECT_EQ(visited, expected);
}

TEST(onode, for_each_visits_current_node_subtree_in_preorder){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());
    auto subtree = root.child[NonUniformNode2::Idx::_MP_];
    subtree->set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    subtree->set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_PP_]->set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    std::vector<NonUniformNode2*> visited;
    subtree->for_each([&](NonUniformNode2::ref_Node node) {
        visited.push_back(&node);
    });

    std::vector<NonUniformNode2*> expected = {
        subtree,
        subtree->child[NonUniformNode2::Idx::_MM_],
        subtree->child[NonUniformNode2::Idx::_PP_],
    };
    EXPECT_EQ(visited, expected);

    const NonUniformNode2& const_subtree = *subtree;
    std::vector<const NonUniformNode2*> const_visited;
    const_subtree.for_each([&](NonUniformNode2::const_ref_Node node) {
        const_visited.push_back(&node);
    });

    std::vector<const NonUniformNode2*> const_expected = {
        subtree,
        subtree->child[NonUniformNode2::Idx::_MM_],
        subtree->child[NonUniformNode2::Idx::_PP_],
    };
    EXPECT_EQ(const_visited, const_expected);
}

TEST(onode, iterator_reverse_order_matches_forward_order_reversed){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_MM_]->set_child(NonUniformNode2::Idx::_PM_, new NonUniformNode2());
    root.child[NonUniformNode2::Idx::_MP_]->set_child(NonUniformNode2::Idx::_PP_, new NonUniformNode2());

    std::vector<NonUniformNode2*> forward;
    for (auto it = root.begin(); it != root.end(); ++it) {
        forward.push_back(&(*it));
    }

    std::vector<NonUniformNode2*> backward;
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

    NonUniformNode2 node;
    const NonUniformNode2& const_node = node;

    static_assert(std::is_same_v<decltype(*const_node.begin()), const NonUniformNode2&>);
    static_assert(std::is_same_v<decltype(*const_node.cbegin()), const NonUniformNode2&>);
    EXPECT_EQ(&(*const_node.begin()), &const_node);
    EXPECT_EQ(&(*const_node.cbegin()), &const_node);
}

TEST(onode, plots_random_subtree_2d){

    std::mt19937 rng(20260419);
    std::uniform_real_distribution<double> center_dist(-1.0, 1.0);
    std::uniform_real_distribution<double> half_dist(0.8, 1.6);
    std::uniform_real_distribution<double> probability(0.0, 1.0);

    UniformNode2 root;
    root.cell = UniformCell2(1.0, 0.0, 0.0);

    const St max_level = 4;
    auto refine_randomly = [&](auto&& self, UniformNode2* node) -> void {
        ASSERT(node != nullptr);
        if (node->level() >= max_level) {
            return;
        }

        const double threshold = node->level() == 0 ? 1.0 : 0.45;
        if (probability(rng) >= threshold) {
            return;
        }

        node->new_full_children();

        for (St i = 0; i < UniformNode2::NumChildren; ++i) {
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

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_PM_, new NonUniformNode2());

    EXPECT_EQ(root.child[NonUniformNode2::Idx::_MM_]->find_neighbor(_XP_),
              root.child[NonUniformNode2::Idx::_MP_]);
    EXPECT_EQ(root.child[NonUniformNode2::Idx::_MM_]->find_neighbor(_YP_),
              root.child[NonUniformNode2::Idx::_PM_]);
}

TEST(onode, find_face_neighbor_climbs_to_common_ancestor_2d){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());

    auto left_parent = root.child[NonUniformNode2::Idx::_MM_];
    auto right_parent = root.child[NonUniformNode2::Idx::_MP_];
    left_parent->set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    right_parent->set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    EXPECT_EQ(left_parent->child[NonUniformNode2::Idx::_MP_]->find_neighbor(_XP_),
              right_parent->child[NonUniformNode2::Idx::_MM_]);
}

TEST(onode, find_face_neighbor_crosses_root_neighbor_and_descends_2d){

    NonUniformNode2 left_root;
    NonUniformNode2 right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    right_root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    EXPECT_EQ(left_root.child[NonUniformNode2::Idx::_MP_]->find_neighbor(_XP_),
              right_root.child[NonUniformNode2::Idx::_MM_]);
}

TEST(onode, get_neighbor_returns_direct_neighbor_slot_without_search_2d){

    NonUniformNode2 left_root;
    NonUniformNode2 right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    right_root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    EXPECT_EQ(left_root.get_neighbor(_XP_), &right_root);
    EXPECT_EQ(left_root.child[NonUniformNode2::Idx::_MP_]->get_neighbor(_XP_), nullptr);
    EXPECT_EQ(left_root.child[NonUniformNode2::Idx::_MP_]->find_neighbor(_XP_),
              right_root.child[NonUniformNode2::Idx::_MM_]);
    EXPECT_EQ(left_root.get_neighbor(_ZP_), nullptr);
}

TEST(onode, connect_neighbors_fills_direct_neighbor_slots_2d){

    NonUniformNode2 left_root;
    NonUniformNode2 right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());
    right_root.set_child(NonUniformNode2::Idx::_MM_, new NonUniformNode2());

    auto child = left_root.child[NonUniformNode2::Idx::_MP_];
    ASSERT_NE(child, nullptr);
    EXPECT_EQ(child->get_neighbor(_XP_), nullptr);

    child->connect_neighbors();

    EXPECT_EQ(child->get_neighbor(_XP_), child->find_neighbor(_XP_));
    EXPECT_EQ(child->get_neighbor(_XP_),
              right_root.child[NonUniformNode2::Idx::_MM_]);
}

TEST(onode, find_face_neighbor_returns_coarser_neighbor_when_child_is_missing_2d){

    NonUniformNode2 left_root;
    NonUniformNode2 right_root;
    left_root.neighbor[FaceDirectionInOrder(_XP_)] = &right_root;

    left_root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());

    EXPECT_EQ(left_root.child[NonUniformNode2::Idx::_MP_]->find_neighbor(_XP_),
              &right_root);
}

TEST(onode, find_face_neighbor_returns_nullptr_for_missing_boundary_neighbor_2d){

    NonUniformNode2 root;
    root.set_child(NonUniformNode2::Idx::_MP_, new NonUniformNode2());

    EXPECT_EQ(root.child[NonUniformNode2::Idx::_MP_]->find_neighbor(_XP_), nullptr);
}

TEST(onode, find_face_neighbor_rejects_direction_outside_dimension_2d){

    NonUniformNode2 root;

    EXPECT_EQ(root.find_neighbor(_ZP_), nullptr);
}

TEST(onode, find_face_neighbor_climbs_to_common_ancestor_3d_z_direction){

    NonUniformNode3 root;
    root.set_child(NonUniformNode3::Idx::_MMM_, new NonUniformNode3());
    root.set_child(NonUniformNode3::Idx::_PMM_, new NonUniformNode3());

    auto lower_parent = root.child[NonUniformNode3::Idx::_MMM_];
    auto upper_parent = root.child[NonUniformNode3::Idx::_PMM_];
    lower_parent->set_child(NonUniformNode3::Idx::_PMM_, new NonUniformNode3());
    upper_parent->set_child(NonUniformNode3::Idx::_MMM_, new NonUniformNode3());

    EXPECT_EQ(lower_parent->child[NonUniformNode3::Idx::_PMM_]->find_neighbor(_ZP_),
              upper_parent->child[NonUniformNode3::Idx::_MMM_]);
}
