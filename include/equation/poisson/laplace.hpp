#ifndef _LAPLACE_HPP_
#define _LAPLACE_HPP_

#include "type_define.hpp"
#include "equation/equation_base.hpp"
#include "equation/event/event.hpp"
#include <vector>

#include "laplace_fv.hpp"
#include "laplace_fd.hpp"

namespace carpio {
//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = 0   2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = 0   3D --version

template<class D>
using Laplace_ = LaplaceImpl_<D, typename D::LocationTag>;

}

#endif