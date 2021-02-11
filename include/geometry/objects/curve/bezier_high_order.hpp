#ifndef _BEZIER_HIGHORDER_HPP_
#define _BEZIER_HIGHORDER_HPP_

#include "bezier.hpp"
#include "algebra/algebra.hpp"
#include <array>
#include <vector>
#include <math.h>

namespace carpio {

// The method is
// de Casteljau's Algorithm
// https://en.wikipedia.org/wiki/De_Casteljau's_algorithm
//

template<class TYPE, St DIM>
class BezierHighOrder_: public BezierCurve_<TYPE, DIM>{
public:
	static const St Dim = DIM;
	typedef TYPE Cvt;

	typedef BezierCurve_<TYPE, DIM> Base;

	typedef Point_<TYPE, 2> Point;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;

	typedef std::vector<Point> VectorPoints;

	typedef typename VectorPoints::iterator iterator;
	typedef typename VectorPoints::const_iterator const_iterator;
public:
	template<class Container_Point>
	BezierHighOrder_(
			const Container_Point& ver,
			St precision = 10,
			std::string method = "decasteljau") :
	    Base(0, precision){
		std::copy(ver.begin(), ver.end(), std::back_inserter(this->_control_points));
		cal_curve_points(method);
	}

	void cal_curve_points(const std::string& method) {
		if (method == "decasteljau") {
			_calculate_decasteljau();
		} else if (method == "bernstein") {
			_calculate_bernstein();
		} else {
			SHOULD_NOT_REACH;
		}
	}

    ~BezierHighOrder_(){};

protected:
    void _calculate_decasteljau(){
    	St size = this->_curve_points.size();
    	Cvt dt  = 1.0 / (size - 1.0);
    	for(St i = 0; i < size; i++){
    		Cvt t  = dt * i;
    		this->_curve_points[i] = _decasteljau(this->_control_points, t);
    	}
    }

    void _calculate_bernstein() {
    	St size = this->_curve_points.size();
		St ncp  = this->_control_points.size();
    	Cvt dt  = 1.0 / (size - 1.0);
		for (int i = 0; i < size; i++) {
    	    Cvt t  = dt * i;
			Point pres;
			for (St m = 0; m < ncp; m++) {
				auto bm = _bernstein(m, ncp - 1, t);
				for(St d = 0; d < Dim; d++){
					pres[d] += bm * this->_control_points[m][d];
				}
			}
			this->_curve_points[i] = pres;
		}
	}
    // Bernstein basis polynomial
    //            n            n-i    i
    //b   (t)=  (   ) * (1 - t)    * t
    // i,n        i
    //   n
    //  ( ) is a binomial coefficient.
    //   t
    Cvt _bernstein(St i, St n, Cvt t) {
    	// Binomial coefficient
		Cvt r = (Cvt) Factorial(n) / (Cvt) (Factorial(i) * Factorial(n - i));
		r *= std::pow(t, i);
		r *= std::pow(1.0 - t, n - i);
		return r;
	}

	Point _decasteljau(const VectorPoints& cp, Cvt t) {
		// cp : control points
		// Number of control points must be >= 3
		VectorPoints res;
		std::copy(cp.begin(), cp.end(),
				std::back_inserter(res));
        St degree = cp.size();
		for (St r = 0; r < degree - 1; ++r) {
			for (St i = 0; i < degree - 1 - r; ++i) {
				res[i] = _inter_t(res[i], res[i + 1], t);
			}
		}
		return res[0];

	}

	Point _inter_t(const Point& p0, const Point& p1, Cvt t) {
		Point res;
		for (St d = 0; d < Dim; ++d) {
			res[d] = (1.0 - t) * p0[d] + t * p1[d];
		}
		return res;
	}

};


}

#endif
