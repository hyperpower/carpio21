#ifndef _BEZIER_HPP_
#define _BEZIER_HPP_

#include "curve.hpp"
#include <array>
#include <vector>

namespace carpio {

// The method is
// de Casteljau's Algorithm
// https://en.wikipedia.org/wiki/De_Casteljau's_algorithm
//

template<class TYPE, St DIM>
class BezierCurve_: public Curve_<TYPE, DIM>{
public:
	static const St Dim = DIM;
	typedef TYPE Cvt;

	typedef Curve_<TYPE, DIM> Base;

	typedef Point_<TYPE, DIM>      Point;
	typedef Point*                pPoint;
	typedef Point&             ref_Point;
	typedef const Point& const_ref_Point;

	typedef std::vector<Point> VectorPoints;

	typedef typename VectorPoints::iterator iterator;
	typedef typename VectorPoints::const_iterator const_iterator;
protected:
	VectorPoints _control_points;
public:
	BezierCurve_(const St& num_control_points, const St& precision):
			Base(precision + 2), _control_points(num_control_points){
	}

	const Point& start_point() const{
		return _control_points[0];
	}
	const Point& end_point() const{
		return _control_points.back();
	}

	const VectorPoints& control_points() const{
		return _control_points;
	}

	virtual void cal_curve_points(){
		SHOULD_NOT_REACH;
	}

	virtual ~BezierCurve_(){
	}
};


template<class TYPE, St DIM>
class BezierCurveLinear_: public BezierCurve_<TYPE, DIM>{
public:
	static const St Dim = DIM;
	typedef TYPE Cvt;

	typedef BezierCurve_<TYPE, DIM> Base;

	typedef Point_<TYPE, DIM> Point;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;

	typedef std::vector<Point> VectorPoints;

	typedef typename VectorPoints::iterator iterator;
	typedef typename VectorPoints::const_iterator const_iterator;
protected:

public:
	BezierCurveLinear_(
			const Point& start,
			const Point& end,
			const St&    precision): Base(2, precision){
		this->_control_points[0] = start;
		this->_control_points[1] = end;
		cal_curve_points();
	}

	void cal_curve_points(){
		St  size = this->size();
		Cvt dt   = 1.0 / (size - 1.0);
		for(St i = 0; i < size; i++){
			Cvt t = i * dt;
			this->_curve_points[i] = _bezier_function(
					        t,
					        this->_control_points[0],
							this->_control_points[1]);
		}
	}
protected:
	Point _bezier_function(const Cvt&   t,
			               const Point& p0,
			               const Point& p1){
		Point res;
		for (St d = 0; d < DIM; ++d) {
			res[d] = (1.0 - t) * p0[d] + t * p1[d];
		}
		return res;
	}
};

}
#endif
