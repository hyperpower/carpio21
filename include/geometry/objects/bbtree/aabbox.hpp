#ifndef _AABBOX_HPP_
#define _AABBOX_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/box.hpp"
#include <array>
#include <map>
#include "utility/any.hpp"


namespace carpio {

template<typename TYPE, St DIM, class OBJ>
class AABBox_ : public Box_<TYPE, DIM> {
public:
	static const St Dim = DIM;
	typedef Point_<TYPE, DIM> Point;
	typedef AABBox_<TYPE, DIM, OBJ> Self;
	typedef Box_<TYPE, DIM> Base;
	typedef St size_type;
	typedef TYPE Vt;
	typedef TYPE& reference;
	typedef TYPE* pointer;
	typedef const TYPE* const_pointer;
	typedef const TYPE& const_reference;
	typedef OBJ Object;
	typedef Object* pObject;

protected:
	Object* _pobj;
public:
	AABBox_():
		Base(), _pobj(nullptr){
	}
	AABBox_(pObject po):
		_pobj(po), Base(po->box()){
		std::cout << "constructor pObject\n";
	}
	AABBox_(const Self& o):
		_pobj(o._pobj), Base(o._min, o._max){
	}
	AABBox_(const Self& a, const Self& b) :
		_pobj(nullptr), Base(a, b){
	}
	const Object& object() const{
		return *_pobj;
	}
	Object& object(){
		return *_pobj;
	}
	void free_object(){
		this->min() = Point(0.0, 0.0, 0.0);
		this->max() = Point(0.0, 0.0, 0.0);
		if(_pobj != nullptr){
			delete _pobj;
		}
		_pobj = nullptr;
	}
	Self& operator=(const Self& other) {
		_pobj= other._pobj;
		this->_max = other._max;
		this->_min = other._min;
		return *this;
	}
};

template<typename TYPE, St DIM, class OBJ>
class AABBoxCompareLess_{
	typedef AABBox_<TYPE, DIM, OBJ> Box;
public:
	bool operator()(const Box& a, const Box& b){
		for(St d = 0; d < DIM; d++){
			if(a.min(d) < b.min(d)){
				return true;
			}
		}
		return false;
	}
};

}
#endif
