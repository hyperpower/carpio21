#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include <array>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <ostream>

#include "algebra/algebra_define.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"

namespace carpio {

struct SphereTag: public GeometryTag {};

template<typename NUM>
inline NUM CalculateSphereX2(
        const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& y, const NUM& z) {
    const NUM dy = y - yc;
    const NUM dz = z - zc;
    return r * r - dy * dy - dz * dz;
}

template<typename NUM>
inline NUM CalculateSphereY2(
        const NUM& xc, const NUM& zc, const NUM& r,
        const NUM& x, const NUM& z) {
    const NUM dx = x - xc;
    const NUM dz = z - zc;
    return r * r - dx * dx - dz * dz;
}

template<typename NUM>
inline NUM CalculateSphereZ2(
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& x, const NUM& y) {
    const NUM dx = x - xc;
    const NUM dy = y - yc;
    return r * r - dx * dx - dy * dy;
}

template<typename NUM>
inline std::array<NUM, 2> CalculateSphereX(
        const NUM& xc, const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& y, const NUM& z,
        const NUM& s = 1e-10) {
    const NUM dx2 = CalculateSphereX2(yc, zc, r, y, z);
    if (dx2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dx = std::sqrt((std::max)(NUM(0), dx2));
    return {xc - dx, xc + dx};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateSphereY(
        const NUM& xc, const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& x, const NUM& z,
        const NUM& s = 1e-10) {
    const NUM dy2 = CalculateSphereY2(xc, zc, r, x, z);
    if (dy2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dy = std::sqrt((std::max)(NUM(0), dy2));
    return {yc - dy, yc + dy};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateSphereZ(
        const NUM& xc, const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& x, const NUM& y,
        const NUM& s = 1e-10) {
    const NUM dz2 = CalculateSphereZ2(xc, yc, r, x, y);
    if (dz2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dz = std::sqrt((std::max)(NUM(0), dz2));
    return {zc - dz, zc + dz};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateSphereXInRange(
        const NUM& xc, const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& y, const NUM& z,
        const NUM& x_min, const NUM& x_max, const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateSphereX(xc, yc, zc, r, y, z, s);
    const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
    if (!IsInRange(x_min, res[0], x_max, type, s)) {
        res[0] = nan;
    }
    if (!IsInRange(x_min, res[1], x_max, type, s)) {
        res[1] = nan;
    }
    return res;
}

template<typename NUM>
inline std::array<NUM, 2> CalculateSphereYInRange(
        const NUM& xc, const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& x, const NUM& z,
        const NUM& y_min, const NUM& y_max, const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateSphereY(xc, yc, zc, r, x, z, s);
    const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
    if (!IsInRange(y_min, res[0], y_max, type, s)) {
        res[0] = nan;
    }
    if (!IsInRange(y_min, res[1], y_max, type, s)) {
        res[1] = nan;
    }
    return res;
}

template<typename NUM>
inline std::array<NUM, 2> CalculateSphereZInRange(
        const NUM& xc, const NUM& yc, const NUM& zc, const NUM& r,
        const NUM& x, const NUM& y,
        const NUM& z_min, const NUM& z_max, const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateSphereZ(xc, yc, zc, r, x, y, s);
    const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
    if (!IsInRange(z_min, res[0], z_max, type, s)) {
        res[0] = nan;
    }
    if (!IsInRange(z_min, res[1], z_max, type, s)) {
        res[1] = nan;
    }
    return res;
}

template<typename TYPE>
class Sphere_: public std::array<TYPE, 4> {
public:
    static const St Dim = 3;
    typedef St size_type;
    typedef TYPE Vt;
    typedef TYPE& reference;
    typedef const TYPE& const_reference;
    typedef Sphere_<TYPE> Sphere;
    typedef Sphere_<TYPE> Self;
    typedef SphereTag Tag;

    typedef Point_<Vt, 3> Point;
    typedef TYPE value_type;

public:
    Sphere_() :
            std::array<Vt, 4>() {
    }

    Sphere_(const Vt& xc, const Vt& yc, const Vt& zc, const Vt& r) {
        this->at(0) = xc;
        this->at(1) = yc;
        this->at(2) = zc;
        this->at(3) = r;
    }

    Sphere_(const Point& c, const Vt& r) {
        this->at(0) = c.x();
        this->at(1) = c.y();
        this->at(2) = c.z();
        this->at(3) = r;
    }

    Sphere_(const Sphere& s) {
        this->at(0) = s[0];
        this->at(1) = s[1];
        this->at(2) = s[2];
        this->at(3) = s[3];
    }

    Sphere_(std::initializer_list<TYPE> l) {
        auto b = l.begin();
        if (l.size() == 4) {
            this->reconstruct(*b, *(std::next(b)), *(std::next(b, 2)),
                              *(std::next(b, 3)));
        }
    }

    void reconstruct(const Vt& xc, const Vt& yc, const Vt& zc, const Vt& r) {
        this->xc() = xc;
        this->yc() = yc;
        this->zc() = zc;
        this->r()  = r;
    }

    inline reference xc() {
        return this->at(0);
    }

    inline reference yc() {
        return this->at(1);
    }

    inline reference zc() {
        return this->at(2);
    }

    inline reference r() {
        return this->at(3);
    }

    inline const_reference xc() const {
        return this->at(0);
    }

    inline const_reference yc() const {
        return this->at(1);
    }

    inline const_reference zc() const {
        return this->at(2);
    }

    inline const_reference r() const {
        return this->at(3);
    }

    Point center() const {
        return Point(this->xc(), this->yc(), this->zc());
    }

    std::array<Vt, 2> cal_x(const Vt& y, const Vt& z) const {
        return CalculateSphereX(this->xc(), this->yc(), this->zc(), this->r(), y, z);
    }

    std::array<Vt, 2> cal_y(const Vt& x, const Vt& z) const {
        return CalculateSphereY(this->xc(), this->yc(), this->zc(), this->r(), x, z);
    }

    std::array<Vt, 2> cal_z(const Vt& x, const Vt& y) const {
        return CalculateSphereZ(this->xc(), this->yc(), this->zc(), this->r(), x, y);
    }

    Vt area() const {
        return Vt(4) * Vt(_PI_) * this->r() * this->r();
    }

    Vt volume() const {
        return Vt(4.0 / 3.0) * Vt(_PI_) * this->r() * this->r() * this->r();
    }

    bool empty() const {
        return this->r() <= 0.0;
    }
};

template<typename TYPE>
std::ostream& operator<<(std::ostream& stream, const Sphere_<TYPE>& sphere) {
    stream << std::setprecision(4)
           << "(X - " << sphere.xc() << ")^2 + "
           << "(Y - " << sphere.yc() << ")^2 + "
           << "(Z - " << sphere.zc() << ")^2 = "
           << sphere.r() * sphere.r();
    return stream;
}

template<class TYPE, class TYPE2>
bool IsInOnSphere(
    const TYPE&  xc, const TYPE& yc, const TYPE& zc, const TYPE& r,
    const TYPE2& px, const TYPE2& py, const TYPE2& pz) {
    const auto dx = px - xc;
    const auto dy = py - yc;
    const auto dz = pz - zc;
    return dx * dx + dy * dy + dz * dz <= r * r;
}

template<class TYPE, class TYPE2>
bool IsInOnSphere(const Sphere_<TYPE>& sphere, const Point_<TYPE2, 3>& p) {
    return IsInOnSphere(sphere.xc(), sphere.yc(), sphere.zc(), sphere.r(),
                        p.x(), p.y(), p.z());
}

template<class TYPE, class TYPE2>
bool IsInSphere(
    const TYPE&  xc, const TYPE& yc, const TYPE& zc, const TYPE& r,
    const TYPE2& px, const TYPE2& py, const TYPE2& pz) {
    const auto dx = px - xc;
    const auto dy = py - yc;
    const auto dz = pz - zc;
    return dx * dx + dy * dy + dz * dz < r * r;
}

template<class TYPE, class TYPE2>
bool IsInSphere(const Sphere_<TYPE>& sphere, const Point_<TYPE2, 3>& p) {
    return IsInSphere(sphere.xc(), sphere.yc(), sphere.zc(), sphere.r(),
                      p.x(), p.y(), p.z());
}

}
#endif
