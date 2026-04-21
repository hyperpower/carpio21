#ifndef _O_MORTON_HPP_
#define _O_MORTON_HPP_

#include "domain/octree/octree_define.hpp"

#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace carpio {

struct MortonCodeTag : public OctreeTag {};

//_code
// std::uint64_t [ level: 6 bits ][ path: 58 bits ]
//  1D: max level = 58 
//  2D: max level = 29 
//  3D: max level = 19 
// std::uint32_t [ level: 6 bits ][ path: 26 bits ]
//  1D: max level = 26 
//  2D: max level = 13 
//  3D: max level = 8 
// std::uint16_t [ level: 6 bits ][ path: 10 bits ]
//  1D: max level = 10 
//  2D: max level = 5 
//  3D: max level = 3 
//  

template<St DIM, typename CODE = std::uint64_t>
class MortonCode_ {
public:
    static_assert(DIM >= 1 && DIM <= 3, "MortonCode_ only supports dimensions 1, 2, and 3");
    static_assert(std::is_integral_v<CODE>, "Morton code type must be integral");
    static_assert(std::is_unsigned_v<CODE>, "Morton code type must be unsigned");

    typedef MortonCodeTag Tag;
    typedef CODE Code;

    static constexpr St Dim = DIM;
    static constexpr St CodeBits = std::numeric_limits<Code>::digits;
    static constexpr St LevelBits = 6;
    static_assert(CodeBits > LevelBits, "Morton code type must have more than 6 bits");

    static constexpr St PathBits = CodeBits - LevelBits;
    static constexpr St LevelShift = PathBits;
    static constexpr St BitsPerLevel = DIM;
    static constexpr St NumChildren = St(1) << DIM;
    static constexpr St MaxLevels = PathBits / DIM;
    static constexpr Code PathMask = static_cast<Code>((Code(1) << PathBits) - Code(1));
    static constexpr Code ChildMask = static_cast<Code>(NumChildren - 1);

protected:
    Code _code;

    static constexpr St ChildShift(St level) {
        return PathBits - (level + 1) * DIM;
    }

public:
    MortonCode_() :
        _code(0) {
    }

    explicit MortonCode_(Code code) :
        _code(code) {
        ASSERT(level() <= MaxLevels);
    }

    Code code() const {
        return _code;
    }

    void set_code(Code code) {
        _code = code;
        ASSERT(level() <= MaxLevels);
    }

    St level() const {
        return St(_code >> LevelShift);
    }

    Code path() const {
        return _code & PathMask;
    }

    void set_level(St level) {
        ASSERT(level <= MaxLevels);
        _code = path() | (static_cast<Code>(level) << LevelShift);
    }

    void set_path_code(Code path) {
        ASSERT((path & ~PathMask) == 0);
        _code = (_code & ~PathMask) | path;
    }

    void append(St child_idx) {
        const St current_level = level();
        ASSERT(current_level < MaxLevels);
        set_child(current_level, child_idx);
        set_level(current_level + 1);
    }

    void set_child(St level, St child_idx) {
        ASSERT(level < MaxLevels);
        ASSERT(child_idx < NumChildren);

        const St shift = ChildShift(level);
        const Code mask = static_cast<Code>(ChildMask << shift);
        const Code child = static_cast<Code>(child_idx) << shift;
        set_path_code((path() & ~mask) | child);
    }

    St child_at(St level) const {
        ASSERT(level < this->level());

        const St shift = ChildShift(level);
        return St((path() >> shift) & ChildMask);
    }

    std::vector<St> path_as_vector() const {
        std::vector<St> res;
        res.reserve(level());
        for (St l = 0; l < level(); ++l) {
            res.push_back(child_at(l));
        }
        return res;
    }

    void set_path(const std::vector<St>& child_indices) {
        ASSERT(child_indices.size() <= MaxLevels);

        _code = 0;
        for (St l = 0; l < child_indices.size(); ++l) {
            set_child(l, child_indices[l]);
        }
        set_level(child_indices.size());
    }

};

using MortonCode1 = MortonCode_<1>;
using MortonCode2 = MortonCode_<2>;
using MortonCode3 = MortonCode_<3>;

template<St DIM, typename CODE = std::uint64_t>
inline CODE EncodeGridMortonCode(St i, St j, St k) {
    static_assert(DIM >= 1 && DIM <= 3, "EncodeGridMortonCode only supports dimensions 1, 2, and 3");
    static_assert(std::is_integral_v<CODE>, "Morton code type must be integral");
    static_assert(std::is_unsigned_v<CODE>, "Morton code type must be unsigned");

    constexpr St CoordinateBits = std::numeric_limits<CODE>::digits / DIM;
    ASSERT((i >> CoordinateBits) == 0);
    if constexpr (DIM >= 2) {
        ASSERT((j >> CoordinateBits) == 0);
    }
    if constexpr (DIM >= 3) {
        ASSERT((k >> CoordinateBits) == 0);
    }

    CODE code = 0;
    for (St bit = 0; bit < CoordinateBits; ++bit) {
        code |= CODE((i >> bit) & St(1)) << (DIM * bit);
        if constexpr (DIM >= 2) {
            code |= CODE((j >> bit) & St(1)) << (DIM * bit + 1);
        }
        if constexpr (DIM >= 3) {
            code |= CODE((k >> bit) & St(1)) << (DIM * bit + 2);
        }
    }
    return code;
}

}

#endif
