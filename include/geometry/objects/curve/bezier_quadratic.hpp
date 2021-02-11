#ifndef _BEZIER_QUADRATIC_HPP_
#define _BEZIER_QUADRATIC_HPP_

#include "bezier.hpp"
#include <array>
#include <vector>

namespace carpio {

// The method is
// de Casteljau's Algorithm
// https://en.wikipedia.org/wiki/De_Casteljau's_algorithm
//

template<class TYPE, St DIM>
class BezierCurveQuadratic_: public BezierCurve_<TYPE, DIM>{
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
	BezierCurveQuadratic_(
			const Point& start,
			const Point& control,
			const Point& end,
			const St&    precision): Base(3, precision){
		this->_control_points[0] = start;
		this->_control_points[1] = control;
		this->_control_points[2] = end;
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
							this->_control_points[1],
							this->_control_points[2]);
		}
	}

	void set_control_point(const Point& pc){
		this->_control_points[1] = pc;
		cal_curve_points();
	}

protected:
	Point _bezier_function(const Cvt&   t,
			               const Point& p0,
			               const Point& p1,
			               const Point& p2){
		Point res;
		for (St d = 0; d < DIM; ++d) {
			res[d] = p1[d]
				     + (1.0 - t) * (1.0 - t) * (p0[d] - p1[d])
					 + t * t * (p2[d] - p1[d]);
		}
		return res;
	}





};
}
#endif
