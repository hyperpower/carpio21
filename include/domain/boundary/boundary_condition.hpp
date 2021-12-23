#ifndef _S_BOUNDARY_CONDITION_HPP_
#define _S_BOUNDARY_CONDITION_HPP_

#include "domain/domian_define.hpp"

namespace carpio{

class BoundaryCondition {
public:
	static const int _BC1_ = 1;
	static const int _BC2_ = 2;
	static const int _BC3_ = 3; // periodic
public:
	// Constructor
	BoundaryCondition() {
		// default boundary condition is symmetric boundary condition
	}
	virtual ~BoundaryCondition(){}

	// get
	virtual int type() const {}
	virtual Vt value(Vt x, Vt y, Vt z, Vt t) const {}

};




}

#endif
