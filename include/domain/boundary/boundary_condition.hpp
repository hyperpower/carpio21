#ifndef _S_BOUNDARY_CONDITION_HPP_
#define _S_BOUNDARY_CONDITION_HPP_

#include "domain/domain_define.hpp"

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
	virtual int type() const {return 1;}
	virtual Vt value() const {return 0.0;}
	virtual Vt value(Vt , Vt, Vt , Vt) const {return 0.0;}

};




}

#endif
