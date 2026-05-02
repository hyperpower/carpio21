#include "gtest/gtest.h"
#include "test_octree.h"
#include "domain/octree/data/odata_idx.hpp"
#include "domain/octree/order/oorder.hpp"
#include "domain/octree/order/oorder_morton.hpp"

#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

using namespace carpio;
using namespace carpio::octree_test;

namespace {

typedef OGrid_<ODataIdx, OCellUniform_<double, 2>, 2> MortonOrderGrid2;
typedef OGhostRegular_<MortonOrderGrid2> MortonOrderGhost2;
typedef OOrderMorton_<MortonOrderGrid2, MortonOrderGhost2, CenterTag> MortonOrder2;

template<class GRID, class GHOST>
class TestOrder : public OOrder_<GRID, GHOST, CenterTag> {
public:
    typedef OOrder_<GRID, GHOST, CenterTag> Base;
    typedef typename Base::spGrid spGrid;
    typedef typename Base::spGhost spGhost;

    TestOrder(spGrid spgrid, spGhost spghost) :
        Base(spgrid, spghost) {
    }
};

std::vector<MortonOrderGrid2::Indices> RootIndicesInOrder(const MortonOrder2& order) {
    std::vector<MortonOrderGrid2::Indices> res;
    for (auto node : order) {
        const auto& root = node->find_root();
        res.push_back(order.grid().storage_1d_idx_to_indices(root.root_idx()));
    }
    return res;
}

}

TEST(oorder, aliases_instantiate_for_octree_grid_and_ghost) {
    typedef TestOrder<UniformGrid2, RegularGhost<UniformGrid2>> Order;

    static_assert(Order::Dim == 2);
    static_assert(std::is_same<typename Order::Grid, UniformGrid2>::value);
    static_assert(std::is_same<typename Order::Ghost, RegularGhost<UniformGrid2>>::value);
    static_assert(std::is_same<typename Order::Tag, OOrderTag>::value);
    static_assert(std::is_same<typename Order::LocationTag, CenterTag>::value);
    static_assert(std::is_same<typename Order::Indices, UniformGrid2::Indices>::value);
    static_assert(std::is_same<typename Order::Node, UniformGrid2::Node>::value);
    static_assert(std::is_same<typename Order::pNode, UniformGrid2::pNode>::value);
    static_assert(std::is_same<typename Order::const_pNode, UniformGrid2::const_pNode>::value);
    static_assert(std::is_same<typename Order::OrderVector, std::vector<UniformGrid2::const_pNode>>::value);

    SUCCEED();
}

TEST(oorder, stores_grid_and_ghost_shared_pointers) {
    auto grid = std::make_shared<UniformGrid2>(UniformGrid2::Point(0.0, 0.0), 1.0, 3, 4);
    auto ghost = std::make_shared<RegularGhost<UniformGrid2>>(grid);
    TestOrder<UniformGrid2, RegularGhost<UniformGrid2>> order(grid, ghost);

    EXPECT_EQ(order.spgrid(), grid);
    EXPECT_EQ(order.spghost(), ghost);
    EXPECT_EQ(&order.grid(), grid.get());
    EXPECT_EQ(&order.ghost(), ghost.get());
}

TEST(oorder_morton, orders_root_leaf_nodes_by_grid_morton_code) {
    auto grid = std::make_shared<MortonOrderGrid2>(4, 6);
    auto ghost = std::make_shared<MortonOrderGhost2>(grid);
    MortonOrder2 order(grid, ghost);

    std::vector<MortonOrderGrid2::Indices> expected;
    for (auto it = grid->cbegin(); it != grid->cend(); ++it) {
        expected.push_back(it.indices());
    }
    std::sort(expected.begin(), expected.end(), [](const auto& lhs, const auto& rhs) {
        const auto lhs_code = EncodeGridMortonCode<2>(St(lhs.i()), St(lhs.j()), 0);
        const auto rhs_code = EncodeGridMortonCode<2>(St(rhs.i()), St(rhs.j()), 0);
        if (lhs_code != rhs_code) {
            return lhs_code < rhs_code;
        }
        if (lhs.j() != rhs.j()) {
            return lhs.j() < rhs.j();
        }
        return lhs.i() < rhs.i();
    });

    EXPECT_EQ(order.size(), grid->size());
    EXPECT_EQ(RootIndicesInOrder(order), expected);
}

TEST(oorder_morton, stores_only_leaf_nodes_in_each_root_morton_order) {
    auto grid = std::make_shared<MortonOrderGrid2>(2, 2);
    auto ghost = std::make_shared<MortonOrderGhost2>(grid);
    auto root = grid->root_node(0, 0);
    root->cell = MortonOrderGrid2::Cell(0.5, 0.5, 0.5);
    root->new_full_children();

    MortonOrder2 order(grid, ghost);

    ASSERT_EQ(order.size(), 7u);
    auto it = order.begin();
    for (St idx = 0; idx < MortonOrderGrid2::Node::NumChildren; ++idx, ++it) {
        ASSERT_NE(it, order.end());
        EXPECT_TRUE((*it)->is_leaf());
        EXPECT_EQ((*it)->find_root().root_idx(), root->root_idx());
        EXPECT_EQ((*it)->idx(), idx);
    }
    for (; it != order.end(); ++it) {
        EXPECT_TRUE((*it)->is_leaf());
        EXPECT_FALSE(ghost->is_ghost(*it));
    }
}
