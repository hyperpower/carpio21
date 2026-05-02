#include "gtest/gtest.h"
#include "domain/octree/data/odata_idx.hpp"

using namespace carpio;

TEST(odata_idx, default_constructor_sets_zero_index) {
    ODataIdx data;

    EXPECT_EQ(data.idx(), 0);
}

TEST(odata_idx, constructor_stores_index) {
    ODataIdx data(12);

    EXPECT_EQ(data.idx(), 12);
}

TEST(odata_idx, set_idx_updates_index) {
    ODataIdx data;

    data.set_idx(7);

    EXPECT_EQ(data.idx(), 7);
}
