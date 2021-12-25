#ifndef _S_BOUNDARY_CONDITION_FUNXYZ_HPP_
#define _S_BOUNDARY_CONDITION_FUNXYZ_HPP_

#include "domain/domain_define.hpp"
#include "boundary_condition.hpp"

#include <functional>

namespace carpio{

class BoundaryConditionFunXYZ :public BoundaryCondition {
public:
	typedef BoundaryCondition       Base;
	typedef BoundaryConditionFunXYZ Self;

    typedef std::function<Vt(Vt, Vt, Vt)> FunXYZ_Value;
protected:
	// data
	// 1 Bondary conditon type
	int _type;
	FunXYZ_Value _fun;
public:
	// Constructor
	BoundaryConditionFunXYZ() : Base(){
		// default boundary condition is symmetric boundary condition
		_type = BoundaryCondition::_BC2_;
		_fun = [](Vt, Vt, Vt){
			return 0.0;
		};
	}
	BoundaryConditionFunXYZ(int type, Vt v) : Base(),
		_type(type) {
		_fun = [v](Vt, Vt, Vt){
			return v;
		};
	}
	BoundaryConditionFunXYZ(int type, FunXYZ_Value fun) : Base(),
		_type(type), _fun(fun) {
	}

	// get
	int type() const {
		return _type;
	}

	Vt value(Vt x, Vt y, Vt z, Vt t) const {
		// t will not be used
		return _fun(x, y, z);
	}

};




}

#endif
