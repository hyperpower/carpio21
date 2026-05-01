#ifndef _OCTREE_DIRECTION_CODE_HPP_
#define _OCTREE_DIRECTION_CODE_HPP_

#include "domain/domain_define.hpp"

#include <sstream>
#include <string>

namespace carpio {

typedef unsigned short DirectionCode;

static const DirectionCode _XM_ = 8;  //001 000
static const DirectionCode _XP_ = 9;  //001 001
static const DirectionCode _YM_ = 16; //010 000
static const DirectionCode _YP_ = 18; //010 010
static const DirectionCode _ZM_ = 32; //100 000
static const DirectionCode _ZP_ = 36; //100 100

inline unsigned short HI(const DirectionCode &d) {
	return d >> 3;
}

inline unsigned short LO(const DirectionCode &d) {
	return d & 7;
}
inline DirectionCode ToDirectionCode(
		const Orientation &x,
		const Orientation &y,
		const Orientation &z) {
	ASSERT(x != _C_);
	ASSERT(y != _C_);
	ASSERT(z != _C_);
	return 56 + x + (y << 1) + (z << 2);
}
inline DirectionCode Opposite(const DirectionCode& ori) {
	DirectionCode res = ori;
	unsigned short hi = HI(ori);
	//unsigned short hi = LO(ori);
	return (res ^ hi);
}

static const short COUNT_1[8] = { 0, 1, 1, 2, 1, 2, 2, 3 };

inline bool IsFaceDirection(const DirectionCode &d) {
	return COUNT_1[(d >> 3)] == 1;
}
inline bool IsCornerDirection(const DirectionCode &d) {
	return COUNT_1[(d >> 3)] == 2;
}
inline bool IsVertexDirection(const DirectionCode &d) {
	return COUNT_1[(d >> 3)] == 3;
}

inline bool GetBit(const DirectionCode &d, St i) {
	const DirectionCode ARR_BIT[6] = { 1, 2, 4, 8, 16, 32 };
	return (d & ARR_BIT[i]) == ARR_BIT[i];
}

inline DirectionCode ToFaceDirection(const Orientation &o, const Axes& a) {
	ASSERT(o != _C_);
	St lo = (o == _P_) ? 7 : 0;
	St hi = 0;
	switch (a) {
	case _X_: {
		hi = 1;
		break;
	}
	case _Y_: {
		hi = 2;
		break;
	}
	case _Z_: {
		hi = 4;
		break;
	}
	}
	St nlo = lo & hi;
	return (hi << 3) | nlo;
}

inline DirectionCode ToCornerDirection( //
		const Orientation &o1, const Axes& a1, //
		const Orientation &o2, const Axes& a2 //
		) {
	ASSERT(o1 != _C_);
	ASSERT(o2 != _C_);
	DirectionCode d1 = ToFaceDirection(o1, a1);
	DirectionCode d2 = ToFaceDirection(o2, a2);
	return d1 | d2;
}

inline Axes FaceDirectionToAxes(const DirectionCode &d) {
	ASSERT(IsFaceDirection(d));
	Axes a;
	unsigned short hi = d >> 3;
	if ((hi & 1) == 1) {
		a = _X_;
		return a;
	}
	if ((hi & 2) == 2) {
		a = _Y_;
		return a;
	}

	if ((hi & 4) == 4) {
		a = _Z_;
		return a;
	}
	SHOULD_NOT_REACH;
	return _X_;
}
inline Orientation FaceDirectionToOrientation(const DirectionCode &d) {
	ASSERT(IsFaceDirection(d));
	unsigned short hi = d >> 3;
	if ((hi & 1) == 1) {
		return (GetBit(d, 0)) ? _P_ : _M_;
	}
	if ((hi & 2) == 2) {
		return (GetBit(d, 1)) ? _P_ : _M_;
	}
	if ((hi & 4) == 4) {
		return (GetBit(d, 2)) ? _P_ : _M_;
	}
	SHOULD_NOT_REACH;
	return _C_;
}
inline void FaceDirectionToOrientationAndAxes(const DirectionCode &d,
		Orientation &o, Axes &a) {
	o = FaceDirectionToOrientation(d);
	a = FaceDirectionToAxes(d);
}
inline void CornerDirectionToOrientationAndAxes(const DirectionCode &d,
		Orientation &o1, Axes &a1, Orientation &o2, Axes& a2) {
	ASSERT(IsCornerDirection(d));
	unsigned short hi = d >> 3;
	if ((hi & 1) == 1 && (hi & 2) == 2) {
		a1 = _X_;
		a2 = _Y_;
		o1 = (GetBit(d, 0)) ? _P_ : _M_;
		o2 = (GetBit(d, 1)) ? _P_ : _M_;
		return;
	}
	if ((hi & 2) == 2 && (hi& 4)== 4) {
		a1 = _Y_;
		a2 = _Z_;
		o1 = (GetBit(d, 1)) ? _P_ : _M_;
		o2 = (GetBit(d, 2)) ? _P_ : _M_;
		return;
	}
	if ((hi & 4) == 4 && (hi & 1) == 1) {
		a1 = _Z_;
		a2 = _X_;
		o1 = (GetBit(d, 2)) ? _P_ : _M_;
		o2 = (GetBit(d, 0)) ? _P_ : _M_;
		return;
	}
	SHOULD_NOT_REACH;
}
/*
 * Does DirectionCode on axes active
 */
inline bool IsDirectionOn(const DirectionCode &d, const Axes& a) {
	unsigned short hi = d >> 3;
	switch (a) {
	case _X_:
		return ((hi & 1) == 1);
	case _Y_:
		return ((hi & 2) == 2);
	case _Z_:
		return ((hi & 4) == 4);
	}
	return false;
}

template<St DIM>
inline bool IsValidFaceDirection(const DirectionCode& d) {
	if (!IsFaceDirection(d)) {
		return false;
	}
	return St(FaceDirectionToAxes(d)) < DIM;
}

template<St DIM>
inline bool IsValidNeighborDirection(const DirectionCode& d) {
	static_assert(DIM >= 1 && DIM <= 3, "Neighbor directions only support dimensions 1, 2, and 3");
	const unsigned short hi = HI(d);
	if (hi == 0 || hi >= 8) {
		return false;
	}
	if (COUNT_1[hi] > DIM) {
		return false;
	}
	for (St a = DIM; a < 3; ++a) {
		if ((hi & (1 << a)) != 0) {
			return false;
		}
	}
	return true;
}

inline bool IsFacePDirection(const DirectionCode &d) {
	unsigned short hi = d >> 3;
	unsigned short low = d & 7;
	return (COUNT_1[hi] == 1) && (hi & low) != 0;
}

inline bool IsXYDirection(const DirectionCode &d) {
	return (d >> 3) == 3;
}

inline bool IsYZDirection(const DirectionCode &d) {
	return (d >> 3) == 6;
}

inline bool IsZXDirection(const DirectionCode &d) {
	return (d >> 3) == 5;
}


inline bool IsXYZDirection(const DirectionCode &d) {
	return (d >> 3) == 7;
}

inline Orientation ToOrientation(const DirectionCode &d, const Axes &a) {
	if (IsDirectionOn(d, a)) {
		switch (a) {
		case _X_:
			return (GetBit(d, 0) == true) ? _P_ : _M_;
		case _Y_:
			return (GetBit(d, 1) == true) ? _P_ : _M_;
		case _Z_:
			return (GetBit(d, 2) == true) ? _P_ : _M_;
		}
	} else {
		return _C_;
	}
	SHOULD_NOT_REACH;
	return _C_;
}

inline DirectionCode FaceDirectionInOrder(const St& i) {
	ASSERT(i < 6);
	static const DirectionCode ARR_FD[] = { 8, 9, 16, 18, 32, 36 };
	return ARR_FD[i];
}
inline St FaceDirectionInOrder(const DirectionCode& dir) {
	ASSERT(IsFaceDirection(dir));
	static const DirectionCode ARR_FD[] = { 8, 9, 16, 18, 32, 36 };
	for (St i = 0; i < 6; i++) {
		if (ARR_FD[i] == dir) {
			return i;
		}
	}
	SHOULD_NOT_REACH;
	return 1;
}
inline St FaceDirectionInOrder(const Axes& a, const Orientation& o){
	// axes and orientation construct a direction
	DirectionCode d = ToFaceDirection(o, a);
	return FaceDirectionInOrder(d);
}

inline DirectionCode CornerDirectionInOrderXY(const St& i) {
	ASSERT(i < 4);
	return 24 + i;
}

inline DirectionCode CornerDirectionInOrderYZ(const St& i) {
	ASSERT(i < 4);
	return 48 + i * 2;
}

inline DirectionCode CornerDirectionInOrderZX(const St& i) {
	ASSERT(i < 4);
	static const DirectionCode ARR_ZXD[] = {
			40 + 0, //101 000
	        40 + 1, //101 001
	        40 + 4, //101 100
	        40 + 5, //101 101
			};
	return ARR_ZXD[i];
}

inline DirectionCode VertexDirectionInOrder(const St& i) {
	ASSERT(i < 8);
	return 56 + i;
}

inline DirectionCode DirectionInOrder(const St& i) {
	ASSERT(i < 26);
	if (i < 6)
		return FaceDirectionInOrder(i);
	if (i < 10)
		return CornerDirectionInOrderXY(i - 6);
	if (i < 14)
		return CornerDirectionInOrderYZ(i - 10);
	if (i < 18)
		return CornerDirectionInOrderZX(i - 14);
	return VertexDirectionInOrder(i - 18);
}

template<St DIM>
inline constexpr St NumNeighborDirections() {
	static_assert(DIM >= 1 && DIM <= 3, "Neighbor directions only support dimensions 1, 2, and 3");
	if constexpr (DIM == 1) {
		return 2;
	} else if constexpr (DIM == 2) {
		return 8;
	} else {
		return 26;
	}
}

template<St DIM>
inline DirectionCode NeighborDirectionInOrder(const St& i) {
	ASSERT(i < NumNeighborDirections<DIM>());
	if constexpr (DIM == 1) {
		return FaceDirectionInOrder(i);
	} else if constexpr (DIM == 2) {
		if (i < 4) {
			return FaceDirectionInOrder(i);
		}
		return CornerDirectionInOrderXY(i - 4);
	} else {
		return DirectionInOrder(i);
	}
}

template<St DIM>
inline St NeighborDirectionInOrder(const DirectionCode& dir) {
	ASSERT(IsValidNeighborDirection<DIM>(dir));
	for (St i = 0; i < NumNeighborDirections<DIM>(); ++i) {
		if (NeighborDirectionInOrder<DIM>(i) == dir) {
			return i;
		}
	}
	SHOULD_NOT_REACH;
	return 0;
}

inline std::string ToString(const DirectionCode& d) {
	std::stringstream sst;
	std::stringstream sst2;
	unsigned short hi = HI(d);
	unsigned short lo = LO(d);
	if ((hi & 1) == 1) {
		sst << "X";
		sst2 << (((lo & 1) == 1) ? "P" : "M");
	}
	if ((hi & 2) == 2) {
		sst << "Y";
		sst2 << (((lo & 2) == 2) ? "P" : "M");
	}
	if ((hi & 4) == 4) {
		sst << "Z";
		sst2 << (((lo & 4) == 4) ? "P" : "M");
	}
	sst << "_" << sst2.str();
	return sst.str();
}

}

#endif
