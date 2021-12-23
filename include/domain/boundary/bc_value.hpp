#ifndef _S_BOUNDARY_CONDITION_VALUE_HPP_
#define _S_BOUNDARY_CONDITION_VALUE_HPP_

#include "domain/domian_define.hpp"
#include "boundary_condition.hpp"

namespace carpio{

class BoundaryConditionValue :public BoundaryCondition {
public:
	typedef BoundaryCondition       Base;
	typedef BoundaryConditionValue Self;
protected:
	// data
	// 1 Bondary conditon type
	int _type;
	Vt  _value;
public:
	// Constructor
	BoundaryConditionValue() : Base(){
		// default boundary condition is symmetric boundary condition
		_type = BoundaryCondition::_BC2_;
		_value = 0.0;
	}
	/*
	 * this constructor should not used to BC2
	 */
	BoundaryConditionValue(int type, Vt v) : Base(),
		_type(type), _value(v) {
	}

	// get
	int type() const {
		return _type;
	}

	Vt value(Vt x, Vt y, Vt z, Vt t) const {
		return _value;
	}

};




}

#endif
