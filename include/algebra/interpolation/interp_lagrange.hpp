#ifndef _INTERP_LAGRANGE_HPP_
#define _INTERP_LAGRANGE_HPP_

#include <cstdint>
#include <ratio>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <limits>

#include "type_define.hpp"
#include "algebra/algebra_define.hpp"
#include "interp_define.hpp"
#include "interp_linear.hpp"

namespace carpio {

    template <class ARRX, class ARRV>
    inline auto InterpLagrangeN(
        const typename ARRX::size_type &n,
        const ARRX &arrx,
        const ARRV &arrv,
        const typename ARRX::value_type &x)
    {

        typedef typename ARRX::size_type st;
        typedef typename ARRV::value_type vt;

        vt res = 0.0;
        auto iter_xi = arrx.begin();
        auto iter_vi = arrv.begin();
        for (st i = 0; i < n; ++i)
        {
            vt term = *iter_vi;
            auto iter_xj = arrx.begin();
            auto xi = *(iter_xi);
            for (st j = 0; j < n; ++j)
            {
                auto xj = *(iter_xj);
                if (i != j)
                {
                    term *= (x - xj) / (xi - xj);
                }
                ++iter_xj;
            }
            res += term;
            ++iter_xi;
            ++iter_vi;
        }
        return res;
    }

template<class ARRX, class ARRV>
inline auto InterpLagrange(const ARRX& arrx, const ARRV& arrv, const typename ARRX::value_type& x)  {
    typedef typename ARRV::value_type vt;
    typedef typename ARRX::value_type cvt;
    auto n = ArraySize(arrx, arrv);
    switch (n){
    case 1:
        throw std::invalid_argument("Error: Input data must be greater than 1.");
        return vt(1.0);
        break;
    case 2: {  // linear
        vt x1 = *(arrx.begin());
        vt x2 = *(std::next(arrx.begin()));
        cvt y1 = *(arrv.begin());
        cvt y2 = *(std::next(arrx.begin()));
        return InterpLinear(x, x1, x2, y1, y2);
        break;
    }
    default:
        break;
    }
    return InterpLagrangeN(n, arrx, arrv, x);
}



}

#endif