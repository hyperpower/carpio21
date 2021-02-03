#ifndef _CURVE_HPP_
#define _CURVE_HPP_

#include "geometry/geometry_define.hpp"
//#include "geometry/objects/point.hpp"
//#include "segment.hpp"
#include <array>
#include <vector>

namespace carpio {

template<class TYPE, St DIM>
class Curve_{
public:
	static const St Dim = DIM;
	typedef TYPE Cvt;

	typedef Point_<TYPE, DIM> Point;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;

	typedef std::vector<Point> VectorPoints;

	typedef typename VectorPoints::iterator iterator;
	typedef typename VectorPoints::const_iterator const_iterator;
protected:
	VectorPoints _curve_points;
public:
	Curve_(const St& size):
		_curve_points(size){
	}

	const Point& operator[](St i) const{
		return _curve_points[i];
	}

	iterator begin(){
		return _curve_points.begin();
	}

	const_iterator begin() const{
		return _curve_points.begin();
	}

	iterator end() {
		return _curve_points.end();
	}

	const_iterator end() const {
		return _curve_points.end();
	}

	St size() const{
		return _curve_points.size();
	}

	virtual void cal_curve_points(){
		SHOULD_NOT_REACH;
	}

    virtual ~Curve_(){};

protected:


};


}

#endif
