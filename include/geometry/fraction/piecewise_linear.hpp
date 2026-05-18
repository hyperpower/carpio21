#ifndef _GEOMETRY_FRACTION_PIECEWISE_LINEAR_HPP_
#define _GEOMETRY_FRACTION_PIECEWISE_LINEAR_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/box.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/analytic/plane.hpp"
#include "geometry/objects/basic/point_chain.hpp"
#include "geometry/affine.hpp"

#include "algebra/algebra.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <list>
#include <memory>

namespace carpio {

/**
 * \brief Calculate the volume fraction cut by an oriented plane in the unit box.
 *
 * This is the 3D forward VOF problem for
 * m1 * x + m2 * y + m3 * z <= alpha in [0, 1]^3.
 * The coefficients are expected to be non-negative and sorted as
 * m1 <= m2 <= m3. This helper handles the lower-half cases; the caller uses
 * symmetry for alpha larger than one half of m1 + m2 + m3.
 */
template<class NUM>
NUM _FractionVolumeOrientedUnitBox3D(
        const NUM& m1, const NUM& m2, const NUM& m3,
        const NUM& alpha){
    const NUM m12 = m1 + m2;
    const NUM max_alpha = m1 + m2 + m3;

    if (NUM(0) <= alpha && alpha < m1){
        return alpha * alpha * alpha / (NUM(6) * m1 * m2 * m3);
    }else if (m1 <= alpha && alpha < m2){
        return (alpha * alpha - alpha * m1) / (NUM(2) * m2 * m3)
             + m1 * m1 / (NUM(6) * m2 * m3);
    }else if (m2 <= alpha && alpha < std::min(m12, m3)){
        return (alpha * alpha * (NUM(3) * m12 - alpha)
             + m1 * m1 * (m1 - NUM(3) * alpha)
             + m2 * m2 * (m2 - NUM(3) * alpha))
             / (NUM(6) * m1 * m2 * m3);
    }else if (std::min(m12, m3) <= alpha && alpha <= NUM(0.5) * max_alpha){
        if (m3 < m12){
            return (alpha * alpha * (NUM(3) * m12 + NUM(3) * m3 - NUM(2) * alpha)
                 + m1 * m1 * (m1 - NUM(3) * alpha)
                 + m2 * m2 * (m2 - NUM(3) * alpha)
                 + m3 * m3 * (m3 - NUM(3) * alpha))
                 / (NUM(6) * m1 * m2 * m3);
        }else{
            return (NUM(2) * alpha - m12) / (NUM(2) * m3);
        }
    }
    return NUM(0);
}

/**
 * \brief Calculate the area fraction cut by a line in the unit box.
 *
 * The line is a * x + b * y = alpha, and the returned value is the area of
 * a * x + b * y <= alpha inside [0, 1]^2. The coefficients may have any sign;
 * they are oriented internally by reflecting the unit box, following the same
 * idea as line-box intersection tools.
 */
template<class NUM>
NUM _FractionVolumeUnitBox(const NUM& a, const NUM& b, const NUM& alpha){
    const NUM tol = DefaultTolerance<NUM>();
    NUM na = std::abs(a);
    NUM nb = std::abs(b);
    NUM nalpha = alpha;
    if (a < NUM(0)){
        nalpha -= a;
    }
    if (b < NUM(0)){
        nalpha -= b;
    }

    const NUM max_alpha = na + nb;
    if (nalpha < NUM(0)){
        return NUM(0);
    }
    if (nalpha >= max_alpha){
        return NUM(1);
    }
    if (na <= tol){
        return nalpha / nb;
    }
    if (nb <= tol){
        return nalpha / na;
    }

    NUM r1 = NUM(0);
    NUM r2 = NUM(0);
    if (nalpha - na > NUM(0)){
        r1 = (nalpha - na) / nalpha;
        r1 *= r1;
    }
    if (nalpha - nb > NUM(0)){
        r2 = (nalpha - nb) / nalpha;
        r2 *= r2;
    }
    return NUM(0.5) * nalpha * nalpha / na / nb * (NUM(1) - r1 - r2);
}

/**
 * \brief Calculate the volume fraction cut by a plane in the unit box.
 *
 * The plane is a * x + b * y + c * z = alpha, and the returned value is the
 * volume of a * x + b * y + c * z <= alpha inside [0, 1]^3. The coefficients
 * may have any sign; they are oriented internally, then sorted before the
 * oriented 3D formula is applied.
 */
template<class NUM>
NUM _FractionVolumeUnitBox(
        const NUM& a, const NUM& b, const NUM& c, const NUM& alpha){
    const NUM tol = DefaultTolerance<NUM>();
    std::array<NUM, 3> m{{std::abs(a), std::abs(b), std::abs(c)}};
    NUM nalpha = alpha;
    if (a < NUM(0)){
        nalpha -= a;
    }
    if (b < NUM(0)){
        nalpha -= b;
    }
    if (c < NUM(0)){
        nalpha -= c;
    }

    const NUM max_alpha = m[0] + m[1] + m[2];
    if (nalpha < NUM(0)){
        return NUM(0);
    }
    if (nalpha >= max_alpha){
        return NUM(1);
    }
    if (m[0] <= tol && m[1] <= tol){
        return nalpha / m[2];
    }
    if (m[0] <= tol && m[2] <= tol){
        return nalpha / m[1];
    }
    if (m[1] <= tol && m[2] <= tol){
        return nalpha / m[0];
    }
    if (m[0] <= tol){
        return _FractionVolumeUnitBox(m[1], m[2], nalpha);
    }
    if (m[1] <= tol){
        return _FractionVolumeUnitBox(m[0], m[2], nalpha);
    }
    if (m[2] <= tol){
        return _FractionVolumeUnitBox(m[0], m[1], nalpha);
    }

    std::sort(m.begin(), m.end());

    if (nalpha <= max_alpha * NUM(0.5)){
        return _FractionVolumeOrientedUnitBox3D(m[0], m[1], m[2], nalpha);
    }else{
        return NUM(1) - _FractionVolumeOrientedUnitBox3D(m[0], m[1], m[2], max_alpha - nalpha);
    }
}

/**
 * \brief Calculate the cut area in a rectangle.
 *
 * Computes the area of a * x + b * y <= alpha in the rectangle
 * [0, dx] x [0, dy]. The edge lengths may differ; the implementation scales
 * the problem to the unit box first and then multiplies by dx * dy.
 */
template<class NUM>
NUM FractionVolume(const NUM& a, const NUM& b, const NUM& alpha, 
                   const NUM& dx, const NUM& dy){
    const NUM sa = a * dx;
    const NUM sb = b * dy;
    return _FractionVolumeUnitBox(sa, sb, alpha) * dx * dy;
}

/**
 * \brief Calculate the cut volume in a rectangular box.
 *
 * Computes the volume of a * x + b * y + c * z <= alpha in the box
 * [0, dx] x [0, dy] x [0, dz]. The edge lengths may differ; the implementation
 * scales the problem to the unit box first and then multiplies by dx * dy * dz.
 */
template<class NUM>
NUM FractionVolume(const NUM& a, const NUM& b, const NUM& c, const NUM& alpha, 
                   const NUM& dx, const NUM& dy, const NUM& dz){
    const NUM sa = a * dx;
    const NUM sb = b * dy;
    const NUM sc = c * dz;
    return _FractionVolumeUnitBox(sa, sb, sc, alpha) * dx * dy * dz;
}

/**
 * \brief Calculate alpha from a known area fraction in the unit box.
 *
 * This is the 2D inverse VOF problem for a * x + b * y = alpha. The input
 * fraction is a normalized area in [0, 1], not an absolute area. Coefficients
 * may have any sign; the returned alpha is for the original signed line.
 */
template<class NUM>
NUM _CalAlphaUnitBox(const NUM& a, const NUM& b, const NUM& fraction){
    const NUM tol = DefaultTolerance<NUM>();
    const NUM na = std::abs(a);
    const NUM nb = std::abs(b);
    const NUM max_alpha = na + nb;
    NUM nalpha = NUM(0);

    if (fraction <= NUM(0)){
        nalpha = NUM(0);
    }else if (fraction >= NUM(1)){
        nalpha = max_alpha;
    }else if (na <= tol && nb <= tol){
        nalpha = NUM(0);
    }else if (na <= tol){
        nalpha = fraction * nb;
    }else if (nb <= tol){
        nalpha = fraction * na;
    }else{
        const NUM m = std::min(na, nb);
        const NUM n = std::max(na, nb);
        const NUM c1 = m / (NUM(2) * n);
        const NUM c2 = NUM(1) - c1;
        if (fraction <= c1){
            nalpha = std::sqrt(NUM(2) * fraction * m * n);
        }else if (fraction < c2){
            nalpha = (NUM(2) * fraction * n + m) / NUM(2);
        }else{
            nalpha = m + n - std::sqrt(NUM(2) * (NUM(1) - fraction) * m * n);
        }
    }

    if (a < NUM(0) && na > tol){
        nalpha += a;
    }
    if (b < NUM(0) && nb > tol){
        nalpha += b;
    }
    return nalpha;
}

template<class NUM>
NUM _ChooseCubicRootInRange(
        const double& x1, const double& x2, const double& x3,
        const NUM& lower, const NUM& upper){
    const NUM tol = DefaultTolerance<NUM>();
    const std::array<double, 3> roots{{x1, x2, x3}};
    for (auto root : roots){
        if (IsInRange(lower, NUM(root), upper, _cc_, tol)){
            return NUM(root);
        }
    }
    ASSERT(false);
    return lower;
}

/**
 * \brief Calculate alpha from a known volume fraction in the oriented unit box.
 *
 * This is the 3D inverse VOF problem for sorted, non-negative coefficients.
 * The input fraction is a normalized volume in [0, 1]. Fractions greater than
 * one half are handled by symmetry; the lower half is inverted by analytic
 * formulas and the cubic solver.
 */
template<class NUM>
NUM _CalAlphaOrientedUnitBox3D(
        const NUM& m1, const NUM& m2, const NUM& m3,
        const NUM& fraction){
    const NUM max_alpha = m1 + m2 + m3;
    const NUM m12       = m1 + m2;
    if (fraction <= NUM(0)){
        return NUM(0);
    }
    if (fraction >= NUM(1)){
        return max_alpha;
    }
    if (fraction > NUM(0.5)){
        return max_alpha - _CalAlphaOrientedUnitBox3D(m1, m2, m3, NUM(1) - fraction);
    }

    const NUM v1  = m1 * m1 / (NUM(6) * m2 * m3);
    const NUM v2  = v1 + (m2 - m1) / (NUM(2) * m3);
    const NUM v31 = (m3 * m3 * (NUM(3) * m12 - m3)
                  + m1 * m1 * (m1 - NUM(3) * m3)
                  + m2 * m2 * (m2 - NUM(3) * m3))
                  / (NUM(6) * m1 * m2 * m3);
    const NUM v32 = m12 / (NUM(2) * m3);

    if (fraction < v1){
        return std::pow(NUM(6) * m1 * m2 * m3 * fraction, NUM(1.0 / 3.0));
    }else if (fraction < v2){
        return (m1 + std::sqrt(NUM(8) * m2 * m3 * fraction
                            - m1 * m1 / NUM(3))) / NUM(2);
    }else if (fraction < std::min(v31, v32)){
        double x1 = 0.0;
        double x2 = 0.0;
        double x3 = 0.0;
        SolveCubicEquation(
                NUM(-1),
                NUM(3) * m12,
                NUM(-3) * (m1 * m1 + m2 * m2),
                m1 * m1 * m1 + m2 * m2 * m2
                    - NUM(6) * m1 * m2 * m3 * fraction,
                x1, x2, x3);
        return _ChooseCubicRootInRange(x1, x2, x3, m2, std::min(m12, m3));
    }else if (fraction <= NUM(0.5)){
        if (v31 <= v32){
            double x1 = 0.0;
            double x2 = 0.0;
            double x3 = 0.0;
            SolveCubicEquation(
                    NUM(-2),
                    NUM(3) * max_alpha,
                    NUM(-3) * (m1 * m1 + m2 * m2 + m3 * m3),
                    m1 * m1 * m1 + m2 * m2 * m2 + m3 * m3 * m3
                        - NUM(6) * m1 * m2 * m3 * fraction,
                    x1, x2, x3);
            return _ChooseCubicRootInRange(
                    x1, x2, x3, std::min(m12, m3), max_alpha * NUM(0.5));
        }else{
            return (m12 + NUM(2) * m3 * fraction) / NUM(2);
        }
    }
    return max_alpha * NUM(0.5);
}

/**
 * \brief Calculate alpha from a known volume fraction in the unit box.
 *
 * This is the 3D inverse VOF problem for a * x + b * y + c * z = alpha.
 * The input fraction is a normalized volume in [0, 1], not an absolute volume.
 * Coefficients may have any sign; the returned alpha is for the original
 * signed plane.
 */
template<class NUM>
NUM _CalAlphaUnitBox(
        const NUM& a, const NUM& b, const NUM& c, const NUM& fraction){
    const NUM tol = DefaultTolerance<NUM>();
    std::array<NUM, 3> m{{std::abs(a), std::abs(b), std::abs(c)}};
    NUM nalpha = NUM(0);

    if (fraction <= NUM(0)){
        nalpha = NUM(0);
    }else if (fraction >= NUM(1)){
        nalpha = m[0] + m[1] + m[2];
    }else if (m[0] <= tol && m[1] <= tol && m[2] <= tol){
        nalpha = NUM(0);
    }else if (m[0] <= tol && m[1] <= tol){
        nalpha = fraction * m[2];
    }else if (m[0] <= tol && m[2] <= tol){
        nalpha = fraction * m[1];
    }else if (m[1] <= tol && m[2] <= tol){
        nalpha = fraction * m[0];
    }else if (m[0] <= tol){
        nalpha = _CalAlphaUnitBox(m[1], m[2], fraction);
    }else if (m[1] <= tol){
        nalpha = _CalAlphaUnitBox(m[0], m[2], fraction);
    }else if (m[2] <= tol){
        nalpha = _CalAlphaUnitBox(m[0], m[1], fraction);
    }else{
        std::sort(m.begin(), m.end());
        nalpha = _CalAlphaOrientedUnitBox3D(m[0], m[1], m[2], fraction);
    }

    if (a < NUM(0) && std::abs(a) > tol){
        nalpha += a;
    }
    if (b < NUM(0) && std::abs(b) > tol){
        nalpha += b;
    }
    if (c < NUM(0) && std::abs(c) > tol){
        nalpha += c;
    }
    return nalpha;
}

/**
 * \brief Calculate alpha from a known area fraction in a rectangle.
 *
 * The fraction is normalized to [0, 1], not an absolute area. The rectangle is
 * [0, dx] x [0, dy]. The returned alpha satisfies
 * FractionVolume(a, b, alpha, dx, dy) / (dx * dy) == fraction, up to numerical
 * tolerance.
 */
template<class NUM>
NUM CalAlpha(const NUM& fraction, const NUM& a, const NUM& b, const NUM& dx, const NUM& dy){
    const NUM sa = a * dx;
    const NUM sb = b * dy;
    return _CalAlphaUnitBox(sa, sb, fraction);
}

/**
 * \brief Calculate alpha from a known volume fraction in a rectangular box.
 *
 * The fraction is normalized to [0, 1], not an absolute volume. The box is
 * [0, dx] x [0, dy] x [0, dz]. The returned alpha satisfies
 * FractionVolume(a, b, c, alpha, dx, dy, dz) / (dx * dy * dz) == fraction,
 * up to numerical tolerance.
 */
template<class NUM>
NUM CalAlpha(const NUM& fraction, const NUM& a, const NUM& b, const NUM& c, 
             const NUM& dx, const NUM& dy, const NUM& dz){
    const NUM sa = a * dx;
    const NUM sb = b * dy;
    const NUM sc = c * dz;
    return _CalAlphaUnitBox(sa, sb, sc, fraction);
}

}

#endif
