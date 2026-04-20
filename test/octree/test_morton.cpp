#include "gtest/gtest.h"
#include "test_define.hpp"
#include "test_octree.h"
#include "domain/octree/order/morton.hpp"
#include "domain/octree/io/ognuplot_actor_label.hpp"
#include "domain/octree/io/ognuplot_actor_wire_frame.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using namespace carpio;
using namespace carpio::octree_test;

namespace {

using MortonGrid2 = UniformGrid2;

struct RootMortonEntry {
    MortonGrid2::Indices indices;
    MortonCode2::Code morton_code;
    double x;
    double y;
};

MortonCode2::Code EncodeMorton2(St i, St j) {
    MortonCode2::Code code = 0;
    const St bits = std::numeric_limits<St>::digits;
    for (St bit = 0; bit < bits; ++bit) {
        code |= MortonCode2::Code((i >> bit) & St(1)) << (2 * bit);
        code |= MortonCode2::Code((j >> bit) & St(1)) << (2 * bit + 1);
    }
    return code;
}

std::vector<RootMortonEntry> CollectRootMortonEntries(const MortonGrid2& grid) {
    std::vector<RootMortonEntry> entries;
    entries.reserve(grid.size());
    for (auto it = grid.cbegin(); it != grid.cend(); ++it) {
        const auto idx = it.indices();
        entries.push_back({
            idx,
            EncodeMorton2(St(idx.i()), St(idx.j())),
            it->cell.get(_C_, _X_),
            it->cell.get(_C_, _Y_)
        });
    }
    std::sort(entries.begin(), entries.end(), [](const auto& lhs, const auto& rhs) {
        if (lhs.morton_code != rhs.morton_code) {
            return lhs.morton_code < rhs.morton_code;
        }
        if (lhs.indices.j() != rhs.indices.j()) {
            return lhs.indices.j() < rhs.indices.j();
        }
        return lhs.indices.i() < rhs.indices.i();
    });
    return entries;
}

GnuplotActor MakeCenterConnectionActor(const std::vector<RootMortonEntry>& entries) {
    GnuplotActor actor;
    actor.command("using 1:2 title \"\" ");
    actor.style("with linespoints pt 7 ps 1.5 lc rgb \"#F25022\" lw 2");
    for (const auto& entry : entries) {
        actor.data().push_back(ToString(entry.x, entry.y, " "));
    }
    return actor;
}

GnuplotActor MakeMortonLabelActor(const std::vector<RootMortonEntry>& entries) {
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 1");
    for (const auto& entry : entries) {
        const std::string text = "\""
            + ToString(entry.indices)
            + " / "
            + ToString(entry.morton_code)
            + "\"";
        actor.data().push_back(ToString(entry.x, entry.y, " ") + " " + text);
    }
    return actor;
}

}  // namespace

TEST(morton, default_code_is_zero) {
    MortonCode3 code;

    EXPECT_EQ(code.code(), MortonCode3::Code{0});
    EXPECT_EQ(code.level(), 0u);
    EXPECT_EQ(code.path(), MortonCode3::Code{0});
}

TEST(morton, raw_constructor_and_accessor_preserve_code) {
    const auto raw = (MortonCode3::Code{2} << MortonCode3::LevelShift) | MortonCode3::Code{5};
    MortonCode3 code(raw);

    EXPECT_EQ(code.code(), raw);
    EXPECT_EQ(code.level(), 2u);
    EXPECT_EQ(code.path(), MortonCode3::Code{5});
}

TEST(morton, high_six_bits_store_level_for_uint64) {
    MortonCode3 code;
    code.set_level(19);

    EXPECT_EQ(code.level(), 19u);
    EXPECT_EQ(code.code(), MortonCode3::Code{19} << MortonCode3::LevelShift);
}

TEST(morton, high_six_bits_store_level_for_uint32) {
    using Morton32 = MortonCode_<3, std::uint32_t>;

    Morton32 code;
    code.set_level(8);

    EXPECT_EQ(code.level(), 8u);
    EXPECT_EQ(code.code(), Morton32::Code{8} << Morton32::LevelShift);
}

TEST(morton, low_bits_store_path_only) {
    MortonCode3 code;
    code.set_level(3);
    code.set_path_code(0x1234u);

    EXPECT_EQ(code.level(), 3u);
    EXPECT_EQ(code.path(), MortonCode3::Code{0x1234u});
    EXPECT_EQ(code.code(), (MortonCode3::Code{3} << MortonCode3::LevelShift) | MortonCode3::Code{0x1234u});
}

TEST(morton, append_increments_level_and_stores_children) {
    MortonCode3 code;

    code.append(7);
    code.append(1);
    code.append(4);

    EXPECT_EQ(code.level(), 3u);
    EXPECT_EQ(code.child_at(0), 7u);
    EXPECT_EQ(code.child_at(1), 1u);
    EXPECT_EQ(code.child_at(2), 4u);
}

TEST(morton, round_trips_path_2d) {
    const std::vector<St> path = {3, 0, 2};
    MortonCode2 code;

    code.set_path(path);

    EXPECT_EQ(code.level(), path.size());
    EXPECT_EQ(code.path_as_vector(), path);
}

TEST(morton, round_trips_path_3d) {
    const std::vector<St> path = {7, 1, 4};
    MortonCode3 code;

    code.set_path(path);

    EXPECT_EQ(code.level(), path.size());
    EXPECT_EQ(code.path_as_vector(), path);
}

TEST(morton, set_child_overwrites_without_changing_level) {
    MortonCode3 code;
    code.set_path({7, 1, 4});

    code.set_child(1, 6);

    EXPECT_EQ(code.level(), 3u);
    EXPECT_EQ(code.child_at(0), 7u);
    EXPECT_EQ(code.child_at(1), 6u);
    EXPECT_EQ(code.child_at(2), 4u);
}

TEST(morton, max_levels_follow_code_bits_minus_level_bits) {
    EXPECT_EQ(MortonCode1::MaxLevels,
              (std::numeric_limits<MortonCode1::Code>::digits - MortonCode1::LevelBits) / MortonCode1::Dim);
    EXPECT_EQ(MortonCode2::MaxLevels,
              (std::numeric_limits<MortonCode2::Code>::digits - MortonCode2::LevelBits) / MortonCode2::Dim);
    EXPECT_EQ(MortonCode3::MaxLevels,
              (std::numeric_limits<MortonCode3::Code>::digits - MortonCode3::LevelBits) / MortonCode3::Dim);

    using Morton32 = MortonCode_<3, std::uint32_t>;
    EXPECT_EQ(Morton32::MaxLevels,
              (std::numeric_limits<Morton32::Code>::digits - Morton32::LevelBits) / Morton32::Dim);
}

TEST(morton, plots_root_nodes_in_morton_order_for_3x4_grid_2d) {
    MortonGrid2 grid(MortonGrid2::Point(0.0, 0.0), 1.0, 4, 6);

    const auto entries = CollectRootMortonEntries(grid);

    // ASSERT_EQ(entries.size(), 12u);
    EXPECT_EQ(entries.front().indices, MortonGrid2::Indices(0, 0));
    EXPECT_EQ(entries.front().morton_code, MortonCode2::Code{0});
    EXPECT_EQ(entries[1].indices, MortonGrid2::Indices(1, 0));
    EXPECT_EQ(entries[1].morton_code, MortonCode2::Code{1});
    EXPECT_EQ(entries[2].indices, MortonGrid2::Indices(0, 1));
    EXPECT_EQ(entries[2].morton_code, MortonCode2::Code{2});
    EXPECT_EQ(entries[3].indices, MortonGrid2::Indices(1, 1));
    EXPECT_EQ(entries[3].morton_code, MortonCode2::Code{3});
    // // EXPECT_EQ(entries.back().indices, MortonGrid2::Indices(2, 3));
    // EXPECT_EQ(entries.back().morton_code, EncodeMorton2(2, 3));

    Gnuplot gnu;
    gnu.set_xrange(-0.2, 5.2);
    gnu.set_yrange(-0.2, 6.2);
    gnu.set_equal_aspect_ratio();

    auto grid_actor = ToGnuplotActorWireFrame(grid);
    grid_actor.line_width(1);
    grid_actor.line_color_grey();

    auto line_actor = MakeCenterConnectionActor(entries);
    auto label_actor = MakeMortonLabelActor(entries);

    gnu.add(grid_actor);
    gnu.add(line_actor);
    gnu.add(label_actor);
    gnu.set_terminal_png(FIG_PATH + "OctreeMortonRootOrder3x4", fig_width, fig_height);
    gnu.plot();
}
