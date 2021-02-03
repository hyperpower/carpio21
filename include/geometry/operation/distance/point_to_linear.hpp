#ifndef GEOMETRY_POINT_TO_LINEAR_HPP_
#define GEOMETRY_POINT_TO_LINEAR_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/objects.hpp"


namespace carpio {
// The distance d from a point (x0, y0) to the line ax + by + c = 0
//
template <typename NUM>
NUM Distance_PointToLine(const NUM& x0, const NUM& y0,
		                 const NUM& a,  const NUM& b,  const NUM& c){
	double s = a * a + b * b;
	s = (s==0)?SMALL:s;
	return std::abs(a * x0 + b * y0 + c) / std::sqrt(s);
}
template <typename NUM>
NUM Distance2_PointToRay(const NUM&  x0, const NUM&   y0,
  	                     const NUM&  x,  const NUM&   y,
		                 const NUM& vx,  const NUM&  vy){
	// Ray is defined as
	// Ray origin point          P( x,  y)
	//     direction unit vector V(vx, vy), ||V||=1
	NUM dx  = x0 - x;
	NUM dy  = y0 - y;
	NUM dot = dx * vx + dy * vy;
	if(dot > 0){
		return Distance2_PointToPoint2d(x0,y0,x,y) - dot * dot;
	}else{
		return Distance2_PointToPoint2d(x0,y0,x,y);
	}
}

template <typename NUM>
NUM Distance2_PointToSegment(const NUM& x0,  const NUM&  y0,
		                     const NUM& sx0, const NUM& sy0,
							 const NUM& sx1, const NUM& sy1){
	NUM d[2]   = {sx1 - sx0, sy0 - sy1};
	NUM ps0[2] = {x0  - sx0,  y0 - sy0};
	NUM t      = Dot(d[_X_], d[_Y_], ps0[_X_], ps0[_Y_]);
	NUM dis2   = 0;

	// s0 is closest to p
	if (t <= 0 ){
		return Dot(ps0[_X_], ps0[_Y_], ps0[_X_], ps0[_Y_]);
	}
	// s1 is closest to p
	NUM dd = Dot(d[_X_], d[_Y_],d[_X_], d[_Y_]);
	if ( t >= dd){
		NUM ps1[2] = {x0  - sx1,  y0 - sy1};
		return Dot(ps1[_X_], ps1[_Y_], ps1[_X_], ps1[_Y_]);
	}
	// closest point is interior to segment
	return Dot(ps0[_X_], ps0[_Y_], ps0[_X_], ps0[_Y_] - t * t / dd);
}


// --------------------
// high level function
// --------------------
template <typename NUM>
NUM Distance2(const Point_<NUM, 2>& p, const Line_<NUM>& l){
	// Line equation a x + b y = alpha
	// Line equation change to ax + by - alpha = 0
	return Distance_PointToLine(p.x(), p.y(), l.a(), l.b(), -(l.alpha()));
}

template <typename NUM>
NUM Distance(const Point_<NUM, 2>& p, const Line_<NUM>& l){

	return std::sqrt(Distance2(p, l));
}

template <typename NUM>
NUM Distance2(const Point_<NUM, 2>& p,
		      const Ray_<NUM, 2>&   r){
	return Distance_PointToRay(p.x(),   p.y(),
			                   r.pox(), r.poy(),
							   r.pvx(), r.pvy());
}

template <typename NUM>
NUM Distance(const Point_<NUM, 2>& p, const Ray_<NUM, 2>& r){
	return std::sqrt(Distance2(p, r));
}


template <typename NUM>
NUM Distance2(const Point_<NUM, 2>& p, const Segment_<NUM, 2>& s){
	return Distance2_PointToSegment(p.x(),   p.y(),
			                        s.psx(), s.psy(),
									s.pex(), s.pey());
}

template <typename NUM>
NUM Distance(const Point_<NUM, 2>& p, const Segment_<NUM, 2>& s){
	return std::sqrt(Distance2(p, s));
}



}

#endif
