#ifndef _BBOX_HPP_
#define _BBOX_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include "segment.hpp"
#include <array>
#include <map>
#include "utility/any.hpp"


namespace carpio {

template<typename TYPE, St DIM>
class Operation_;

template<typename TYPE, St DIM>
class Segment_;

struct TagBox: public TagGeometry {
	TagBox() {
	}
};

template<typename TYPE, St DIM>
class Box_ {
public:
	static const St Dim = DIM;
	typedef Point_<TYPE, DIM> Point;
	typedef Box_<TYPE, DIM> Self;
	typedef St size_type;
	typedef TYPE Vt;
	typedef TagBox Tag;
	typedef TYPE& reference;
	typedef TYPE* pointer;
	typedef const TYPE* const_pointer;
	typedef const TYPE& const_reference;

	typedef Operation_<TYPE, DIM> Op;

protected:
	Point _min, _max;
	//TYPE _xmin,_ymin,_xmax,_ymax;
public:
	Box_() {
	}
	Box_(Vt min, Vt max) {
		for (St i = 0; i < Dim; i++) {
			_min[i] = min;
			_max[i] = max;
		}
	}
	Box_(const Self& o) {
		_min = o._min;
		_max = o._max;
	}
	Box_(Vt x_min, Vt y_min, Vt x_max, Vt y_max) {
		ASSERT(Dim == 2);
		_min[0] = x_min;
		_min[1] = y_min;

		_max[0] = x_max;
		_max[1] = y_max;
	}

	Box_(const Point& point) {
		_min = point;
		_max = point;
	}
	Box_(const Point& p1, const Point& p2) {
		_min = Min(p1, p2);
		_max = Max(p1, p2);
	}

	Box_(const Self& a, const Self& b) { // union to one box without data
		Self& self = (*this);
		self._min = Min(a._min, b._min);
		self._max = Max(a._max, b._max);
	}

	bool empty() const{
		return _min == _max;
	}

	Vt min(int a) const {
		ASSERT(a < Dim);
		return _min[a];
	}
	Vt max(int a) const {
		ASSERT(a < Dim);
		return _max[a];
	}
	Point& min() {
		return _min;
	}
	const Point& min() const {
		return _min;
	}
	Point& max() {
		return _max;
	}
	const Point& max() const {
		return _max;
	}

	Vt xmin() const {
		return _min[_X_];
	}
	Vt xmax() const {
		return _max[_X_];;
	}
	Vt ymin() const {
		return _min[_Y_];;
	}
	Vt ymax() const {
		return _max[_Y_];;
	}

	Vt center(int a) const {
		ASSERT(a < Dim);
		return (_min[a] + _max[a]) * 0.5;
	}

	Point center() const {
		Point res;
		for (St i = 0; i < Dim; i++) {
			res[i] = center(i);
		}
		return res;
	}

	Self& operator=(const Self& other) {
		if (this != &other) { // self-assignment check expected
			this->_min = other._min;
			this->_max = other._max;
		}
		return *this;
	}

	bool operator==(const Self& other) {
		return (this->_min == other._min) &&
				(this->_max == other._max);
	}
	bool operator!=(const Self& other){
		return !(this->operator ==(other));
	}

	bool operator<(const Self& rhs) const {
		// compare order:
		// center point x -> y -> z -> point address
		const Self& self = (*this);
		if (this->center(_X_) < rhs.center(_X_)) {
			return true;
		} else if (this->center(_X_) == rhs.center(_X_)) {
			if (DIM < 2)
				return (this < &rhs);
			if (this->center(_Y_) < rhs.center(_Y_)) {
				return true;
			} else if (this->center(_Y_) == rhs.center(_Y_)) {
				if (DIM < 3)
					return (this < &rhs);
				if (this->center(_Z_) < rhs.center(_Z_)) {
					return true;
				}
			}
		}
		return (this < &rhs);
	}

	Self operator+(const Self& b) const {
		Point min = Op::Min(_min, b._min);
		Point max = Op::Max(_max, b._max);
		return Self(min, max);
	}

	Vt get(Axes aix, Orientation loc) const {
		if (aix == _Z_ && DIM == 2) {
			return 0;
		}
		Vt xv = 0;
		const Self& self = (*this);
		switch (loc) {
		case _M_: {
			xv = _min[aix];
			break;
		}
		case _C_: {
			xv = (_min[aix] + _max[aix]) * 0.5;
			break;
		}
		case _P_: {
			xv = _max[aix];
			break;
		}
		default: {
			break;
		}
		}
		return xv;
	}

	Vt get(Orientation loc, Axes aix) const {
		return get(aix, loc);
	}

	Point d() const {
		Point res;
		for (St i = 0; i < Dim; i++) {
			res[i] = _max[i] - _min[i];
		}
		return res;
	}

	Vt get_d(Axes aix) const {
		const Self& self = (*this);
		return _max[aix] - _min[aix];
	}

	Vt get_dh(Axes aix) const {
		const Self& self = (*this);
		return (_max[aix] - _min[aix]) / 2.0;
	}

	Point get_point(Orientation lx, Orientation ly, Orientation lz) const {
		return Point(get(_X_, lx), get(_Y_, ly), get(_Z_, lz));
	}

	template<typename T>
	void transfer(const T&dx, const T& dy = 0, const T& dz = 0) {
		this->_min.transfer(dx, dy, dz);
		this->_max.transfer(dx, dy, dz);
	}

};

template<typename TYPE, St DIM>
std::ostream& operator<<(std::ostream& stream, const Box_<TYPE, DIM>& box) {
	stream << "max = " << box.max();
	stream << "  min = " << box.min();
	return stream;
}



// Point location relative to a box
// It has 25 possibilities in 2D
//
// 51    52    53    54    55
//        |           |
// 41 -- 42 == 43 == 44 -- 45
//        |           |
// 31    32    33    34    35
//        |           |
// 21 -- 22 == 23 == 24 -- 25
//        |           |
// 11    12    13    14    15
//
inline int _ToInt(const PointToSegmentPosition& ps){
//	_PS_IN_        = 0  --> 3
//	_PS_LEFT_      = 1  --> 6 == NOT USE
//	_PS_RIGHT_     = 2  --> 7 == NOT USE
//	_PS_ON_START_  = 3  --> 2
//	_PS_ON_END_    = 4  --> 4
//	_PS_OUT_START_ = 5  --> 1
//	_PS_OUT_END_   = 6  --> 5
	int _ARES[] = {3, 6, 7, 2, 4, 1, 5};
	return _ARES[int(ps)];
}

template<class TYPE>
int PointToBoxPositionCode(
		const TYPE& minx, const TYPE& miny,
		const TYPE& maxx, const TYPE& maxy,
		const TYPE& vx,   const TYPE& vy){
	auto positionx =  OnWhichSide5(minx, maxx, vx);
	auto positiony =  OnWhichSide5(miny, maxy, vy);
	auto nx        = _ToInt(positionx);
	auto ny        = _ToInt(positiony);
	return (ny * 10) + nx;
}

template<class TYPE>
int PointToBoxPositionCode(
		const TYPE& minx, const TYPE& miny, const TYPE& minz,
		const TYPE& maxx, const TYPE& maxy, const TYPE& maxz,
		const TYPE& vx,   const TYPE& vy,   const TYPE& vz){
	auto positionx =  OnWhichSide5(minx, maxx, vx);
	auto positiony =  OnWhichSide5(miny, maxy, vy);
	auto positionz =  OnWhichSide5(minz, maxz, vz);
	auto nx        = _ToInt(positionx);
	auto ny        = _ToInt(positiony);
	auto nz        = _ToInt(positionz);
	return (nz * 100) + (ny * 10) + nx;
}

template<class TYPE, St DIM>
int PointToBoxPositionCode(
		const Point_<TYPE, DIM>& pmin,
		const Point_<TYPE, DIM>& pmax,
		const Point_<TYPE, DIM>& p){
	if(DIM == 1){
		auto position = OnWhichSide5(pmin.x(), pmax.x(), p.x());
		return _ToInt(position);
	}else if(DIM == 2){
		return PointToBoxPositionCode(
				   pmin.x(), pmin.y(),
				   pmax.x(), pmax.y(),
				   p.x(),    p.y());
	}else{ //Dim == 3
		return PointToBoxPositionCode(
	               pmin.x(), pmin.y(), pmin.z(),
	               pmax.x(), pmax.y(), pmax.z(),
				   p.x(),    p.y(),    p.z());
	}
}

template<class TYPE, St DIM>
int PointToBoxPositionCode(
		const Box_<TYPE, DIM>& box,
		const Point_<TYPE, DIM>& p){
	return PointToBoxPositionCode(box.min(), box.max(), p);
}

template<typename TYPE, St DIM>
bool IsInOn(
		const Box_<TYPE, DIM>& box,
		const Point_<TYPE, DIM>& p){
	auto pos =  OnWhichSide5(box.min(_X_), box.max(_X_), p.x());
	if(pos == _PS_OUT_START_ || pos == _PS_OUT_END_){
		return false;
	}
	if (DIM >= 2) {
		pos = OnWhichSide5(box.min(_Y_), box.max(_Y_), p.y());
		if (pos == _PS_OUT_START_ || pos == _PS_OUT_END_) {
			return false;
		}
		if (DIM == 3) {
			pos = OnWhichSide5(box.min(_Z_), box.max(_Z_), p.z());
			if (pos == _PS_OUT_START_ || pos == _PS_OUT_END_) {
				return false;
			}
		}
	}
	return true;
}

template<typename TYPE, St DIM>
bool IsInOn(
		const Box_<TYPE, DIM>& b1,
		const Box_<TYPE, DIM>& b2){
	for (St d = 0; d < DIM; ++d) {
		if (b2.min(d) > b1.max(d)) {
			return false;
		}
		if (b2.max(d) < b1.min(d)) {
			return false;
		}
	}
	return true;
}

}

#endif
