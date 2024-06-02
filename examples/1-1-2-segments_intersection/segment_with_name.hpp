#ifndef _SEGMENT_WITH_NAME_HPP_
#define _SEGMENT_WITH_NAME_HPP_

#include "geometry/geometry.hpp"
#include <array>
#include <vector>
#include <type_traits>
#include <typeinfo>
#include "math.h"



using namespace carpio;

template<typename TYPE, St DIM>
class SegmentWithName_: public Segment_<TYPE, DIM> {
public:
	static const St Dim = DIM;
	typedef SegmentTag Tag;
	typedef TYPE Vt;
	typedef TYPE coord_value_type;
	typedef Vt& ref_Vt;
	typedef const Vt& const_ref_Vt;
	typedef SegmentWithName_<TYPE, DIM> Self;
	typedef SegmentWithName_<TYPE, DIM>& ref_Self;
	typedef const SegmentWithName_<TYPE, DIM>& const_ref_Self;
	typedef Segment_<TYPE, DIM> Base;
	typedef Segment_<TYPE, DIM>& ref_Base;
	typedef const Segment_<TYPE, DIM>& const_ref_Base;
	typedef Point_<TYPE, DIM> Point;
    typedef Point value_type;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;
protected:
    std::string name;
public:
	SegmentWithName_() :
			Base() {};
	
	SegmentWithName_(const Base& b): Base(b){};
    
	SegmentWithName_(const Point& s, const Point& e) :Base(s,e){
	}
	SegmentWithName_(const Vt& ax, const Vt& bx, //
			         const Vt& ay, const Vt& by, //
			         const Vt& az = 0, const Vt& bz = 0) :Base(ax, bx, ay, by, az, bz){
	}
    SegmentWithName_(const Point& s, const Point& e, const std::string& n):
        Base(s,e), name(n){
    }
    SegmentWithName_(const Self& o):
        Base(o.ps(), o.pe()), name(o.name){
    }
    void set_name(const std::string& n){
        name = n;
    }
    std::string get_name() const{
        return name;
    }
	void foo(){
		return;
	}
};



#endif