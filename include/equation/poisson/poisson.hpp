#ifndef _PoissonImpl_HPP_
#define _PoissonImpl_HPP_

#include "type_define.hpp"
#include "equation/poisson/poisson_fv.hpp"
#include "equation/poisson/poisson_fd.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio {
//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = s  2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = s   3D --version

template<class D>
using Poisson_ = PoissonImpl_<D, typename D::LocationTag>;

}

#endif