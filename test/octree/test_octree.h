#ifndef TEST_OCTREE_H
#define TEST_OCTREE_H

#include "domain/octree/ghost/oregular.hpp"
#include "domain/octree/grid/ocell.hpp"
#include "domain/octree/grid/ogrid.hpp"
#include "domain/octree/grid/onode.hpp"

#include <memory>
#include <random>

namespace carpio::octree_test {

using Value = double;

template <St DIM>
using UniformCell = OCellUniform_<Value, DIM>;

using UniformCell1 = UniformCell<1>;
using UniformCell2 = UniformCell<2>;
using UniformCell3 = UniformCell<3>;

template <St DIM>
using NonUniformCell = OCellNonUniform_<Value, DIM>;

using NonUniformCell1 = NonUniformCell<1>;
using NonUniformCell2 = NonUniformCell<2>;
using NonUniformCell3 = NonUniformCell<3>;

template <St DIM>
using UniformNode = ONode_<Value, UniformCell<DIM>, DIM>;

using UniformNode1 = UniformNode<1>;
using UniformNode2 = UniformNode<2>;
using UniformNode3 = UniformNode<3>;

template <St DIM>
using NonUniformNode = ONode_<Value, NonUniformCell<DIM>, DIM>;

using NonUniformNode1 = NonUniformNode<1>;
using NonUniformNode2 = NonUniformNode<2>;
using NonUniformNode3 = NonUniformNode<3>;

template <St DIM>
using OctreeGrid = OGrid_<Value, NonUniformCell<DIM>, DIM>;

using OctreeGrid1 = OctreeGrid<1>;
using OctreeGrid2 = OctreeGrid<2>;
using OctreeGrid3 = OctreeGrid<3>;

template <St DIM>
using UniformGrid = OGridUniform_<Value, DIM>;

using UniformGrid1 = UniformGrid<1>;
using UniformGrid2 = UniformGrid<2>;
using UniformGrid3 = UniformGrid<3>;

template <St DIM>
using NonUniformGrid = OGridNonUniform_<Value, DIM>;

using NonUniformGrid1 = NonUniformGrid<1>;
using NonUniformGrid2 = NonUniformGrid<2>;
using NonUniformGrid3 = NonUniformGrid<3>;

template <typename GRID>
using RegularGhost = OGhostRegular_<GRID>;

inline std::shared_ptr<UniformGrid2> MakeRandomUniformOctree2(
    Value origin_x, Value origin_y, Value cell_len, St nx, St ny) {
    auto grid = std::make_shared<UniformGrid2>(
        UniformGrid2::Point(origin_x, origin_y), cell_len, nx, ny);

    std::mt19937 rng(20260415);
    std::uniform_real_distribution<Value> uniform(0.0, 1.0);

    auto refine_randomly = [&](auto&& self, UniformGrid2::Node* node, St depth) -> void {
        if (node == nullptr || depth >= 3) {
            return;
        }
        const Value probability = depth == 0 ? 0.35 : 0.55;
        if (uniform(rng) > probability) {
            return;
        }
        node->new_full_children();
        for (St i = 0; i < UniformGrid2::Node::NumChildren; ++i) {
            self(self, node->child[i], depth + 1);
        }
    };

    for (auto& root : *grid) {
        refine_randomly(refine_randomly, &root, 0);
    }

    grid->connect_neighbors();
    return grid;
}

} // namespace carpio::octree_test

#endif
