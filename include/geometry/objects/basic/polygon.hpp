#ifndef _POINT_CHAIN_HPP_
#define _POINT_CHAIN_HPP_

#include "geometry/geometry_define.hpp"
#include "point_chain.hpp"
namespace carpio{
//Polygon is closed PointChain
template<class TYPE, St DIM>
class Polygon_ : public PointChain_<TYPE, DIM>{
public:
	static const St Dim = DIM;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;
	typedef Segment_<TYPE, DIM> Segment;
	typedef Segment& ref_Segment;
	typedef Box_<TYPE, DIM> Box;
	typedef PointChain_<TYPE, DIM>     PointChain;
	typedef PointChain                 Base;
	typedef PointChain_<TYPE, DIM>& refBase;

	typedef Polygon_<TYPE, DIM>        Polygon;
	typedef Polygon                    Self;
	typedef Polygon&                refSelf;

	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reference reference;
	typedef typename Base::const_reference const_reference;
	typedef Operation_<TYPE, DIM> Op;
	typedef Point value_type;


	Polygon_() :
				Base() {
		this->_closed = true;
	}

	Polygon_(const Point& p1, const Point& p2, const Point& p3) :
				Base(p1, p2, p3) {
		this->_closed = true;
	}
	Polygon_(const Point& p1,
			 const Point& p2,
			 const Point& p3,
			 const Point& p4) :
				Base(p1, p2, p3, p4) {
		this->_closed = true;
	}

	template<class Container_Point>
	Polygon_(const Container_Point& ver) : Base(ver){
		ASSERT(ver.size() >= 3);
		this->_closed = true;
	}



};
}

#endif
