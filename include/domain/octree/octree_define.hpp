#ifndef _O_DEFINE_HPP
#define _O_DEFINE_HPP

#include "domain/octree/direction.hpp"

namespace carpio {

inline Axes VerticalAxes2D(const Axes& a) {
	ASSERT(a != _Z_);
	return a == _X_ ? _Y_ : _X_;
}

inline Axes VerticalAxes1(const Axes& a) {
	switch (a) {
	case _X_: {
		return _Y_;
	}
	case _Y_: {
		return _Z_;
	}
	case _Z_: {
		return _X_;
	}
	default: {
		SHOULD_NOT_REACH;
		return _X_;
	}
	}
}
inline Axes VerticalAxes2(const Axes& a) {
	switch (a) {
	case _X_: {
		return _Z_;
	}
	case _Y_: {
		return _X_;
	}
	case _Z_: {
		return _Y_;
	}
	default: {
		SHOULD_NOT_REACH;
		return _X_;
	}
	}
}
}

#endif
