#ifndef _OCTREE_DIRECTION_HPP_
#define _OCTREE_DIRECTION_HPP_

#include "domain/octree/direction_code.hpp"

namespace carpio {

struct FaceDirection {

    typedef DirectionCode Code;

    Orientation orientation;
    Axes axes;

    FaceDirection() : orientation(_P_), axes(_X_) {}

    FaceDirection(const Code& c) {
        ASSERT(IsFaceDirection(c));
        FaceDirectionToOrientationAndAxes(c, orientation, axes);
    }

    operator Code() const { 
        return ToFaceDirection(orientation, axes);
    }

    std::string to_string() const {
        return ToString(static_cast<Code>(*this));
    }

    bool is_valid_in_dim(const St& dim) const {
        return St(axes) < dim && orientation != _C_;
    }
};

struct CornerDirection {

    typedef DirectionCode Code;

    std::array<Orientation, 2> orientations;
    std::array<Axes, 2> axes;

    CornerDirection() {
        for (St i = 0; i < 2; ++i) {
            orientations[i] = _P_;
            axes[i] = ArrAxes<2>()[i];
        }
    }

    CornerDirection(const Code& c) {
        ASSERT(IsCornerDirection(c));
        CornerDirectionToOrientationAndAxes(
            c,
            orientations[0], axes[0],
            orientations[1], axes[1]);
    }

    operator Code() const { 
        return ToCornerDirection(
            orientations[0], axes[0],
            orientations[1], axes[1]);
    }

    std::string to_string() const {
        return ToString(static_cast<Code>(*this));
    }

    Axes axes_first() const {
        return axes[0];
    }

    Axes axes_second() const {
        return axes[1];
    }
    
    Orientation orientation_first() const {
        return orientations[0];
    }

    Orientation orientation_second() const {
        return orientations[1];
    }

    Orientation orientation(const Axes& a) const {
        if (a == axes[0]) {
            return orientations[0];
        } else if (a == axes[1]) {
            return orientations[1];
        }
        SHOULD_NOT_REACH;
        return _P_;
    }

    bool is_valid_in_dim(const St& dim) const {
        return St(axes[0]) < dim && St(axes[1]) < dim
            && orientations[0] != _C_ && orientations[1] != _C_;
    }
};

struct VertexDirection {

    typedef DirectionCode Code;

    std::array<Orientation, 3> orientations;

    VertexDirection() {
        for (St i = 0; i < 3; ++i) {
            orientations[i] = _P_;
        }
    }

    VertexDirection(const Code& c) {
        ASSERT(IsVertexDirection(c));
        orientations[0] = ToOrientation(c, _X_);
        orientations[1] = ToOrientation(c, _Y_);
        orientations[2] = ToOrientation(c, _Z_);
    }

    operator Code() const {
        return ToDirectionCode(orientations[0], orientations[1], orientations[2]);
    }

    std::string to_string() const {
        return ToString(static_cast<Code>(*this));
    }

    Axes axes_first() const {
        return _X_;
    }

    Axes axes_second() const {
        return _Y_;
    }

    Axes axes_third() const {
        return _Z_;
    }

    Orientation orientation_first() const {
        return orientations[0];
    }

    Orientation orientation_second() const {
        return orientations[1];
    }

    Orientation orientation_third() const {
        return orientations[2];
    }

    Orientation orientation(const Axes& a) const {
        switch (a) {
        case _X_:
            return orientations[0];
        case _Y_:
            return orientations[1];
        case _Z_:
            return orientations[2];
        }
        SHOULD_NOT_REACH;
        return _P_;
    }

    bool is_valid_in_dim(const St& dim) const {
        return St(_Z_) < dim
            && orientations[0] != _C_
            && orientations[1] != _C_
            && orientations[2] != _C_;
    }

};


}

#endif
