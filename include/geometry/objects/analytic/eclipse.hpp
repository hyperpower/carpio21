#ifndef _ECLIPSE_HPP_
#define _ECLIPSE_HPP_

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

struct EclipseTag: public GeometryTag {};

template<typename NUM>
inline NUM CalculateEclipseY2(
        const NUM& xc, const NUM& a, const NUM& b,
        const NUM& x) {
    const NUM dx = x - xc;
    return b * b * (NUM(1) - dx * dx / (a * a));
}

template<typename NUM>
inline NUM CalculateEclipseX2(
        const NUM& yc, const NUM& a, const NUM& b,
        const NUM& y) {
    const NUM dy = y - yc;
    return a * a * (NUM(1) - dy * dy / (b * b));
}

template<typename NUM>
inline std::array<NUM, 2> CalculateEclipseY(
        const NUM& xc, const NUM& yc, const NUM& a, const NUM& b,
        const NUM& x,
        const NUM& s = 1e-10) {
    const NUM dy2 = CalculateEclipseY2(xc, a, b, x);
    if (dy2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dy = std::sqrt((std::max)(NUM(0), dy2));
    return {yc - dy, yc + dy};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateEclipseX(
        const NUM& xc, const NUM& yc, const NUM& a, const NUM& b,
        const NUM& y,
        const NUM& s = 1e-10) {
    const NUM dx2 = CalculateEclipseX2(yc, a, b, y);
    if (dx2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dx = std::sqrt((std::max)(NUM(0), dx2));
    return {xc - dx, xc + dx};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateEclipseXInRange(
        const NUM& xc, const NUM& yc, const NUM& a, const NUM& b,
        const NUM& y, const NUM& x_min, const NUM& x_max,
        const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateEclipseX(xc, yc, a, b, y, s);
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
inline std::array<NUM, 2> CalculateEclipseYInRange(
        const NUM& xc, const NUM& yc, const NUM& a, const NUM& b,
        const NUM& x, const NUM& y_min, const NUM& y_max,
        const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateEclipseY(xc, yc, a, b, x, s);
    const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
    if (!IsInRange(y_min, res[0], y_max, type, s)) {
        res[0] = nan;
    }
    if (!IsInRange(y_min, res[1], y_max, type, s)) {
        res[1] = nan;
    }
    return res;
}

template<typename TYPE>
class Eclipse_: public std::array<TYPE, 4> {
public:
    static const St Dim = 2;
    typedef St size_type;
    typedef TYPE Vt;
    typedef TYPE& reference;
    typedef const TYPE& const_reference;
    typedef Eclipse_<TYPE> Eclipse;
    typedef Eclipse_<TYPE> Self;
    typedef EclipseTag Tag;

    typedef Point_<Vt, 2> Point;
    typedef TYPE value_type;

public:
    Eclipse_() :
            std::array<Vt, 4>() {
    }

    Eclipse_(const Vt& xc, const Vt& yc, const Vt& a, const Vt& b) {
        this->at(0) = xc;
        this->at(1) = yc;
        this->at(2) = a;
        this->at(3) = b;
    }

    Eclipse_(const Point& c, const Vt& a, const Vt& b) {
        this->at(0) = c.x();
        this->at(1) = c.y();
        this->at(2) = a;
        this->at(3) = b;
    }

    Eclipse_(const Eclipse& e) {
        this->at(0) = e[0];
        this->at(1) = e[1];
        this->at(2) = e[2];
        this->at(3) = e[3];
    }

    Eclipse_(std::initializer_list<TYPE> l) {
        auto b = l.begin();
        if (l.size() == 4) {
            this->reconstruct(*b, *(std::next(b)), *(std::next(b, 2)),
                              *(std::next(b, 3)));
        }
    }

    void reconstruct(const Vt& xc, const Vt& yc, const Vt& a, const Vt& b) {
        this->xc() = xc;
        this->yc() = yc;
        this->a()  = a;
        this->b()  = b;
    }

    inline reference xc() {
        return this->at(0);
    }

    inline reference yc() {
        return this->at(1);
    }

    inline reference a() {
        return this->at(2);
    }

    inline reference b() {
        return this->at(3);
    }

    inline const_reference xc() const {
        return this->at(0);
    }

    inline const_reference yc() const {
        return this->at(1);
    }

    inline const_reference a() const {
        return this->at(2);
    }

    inline const_reference b() const {
        return this->at(3);
    }

    Point center() const {
        return Point(this->xc(), this->yc());
    }

    std::array<Vt, 2> cal_x(const Vt& y) const {
        return CalculateEclipseX(this->xc(), this->yc(), this->a(), this->b(), y);
    }

    std::array<Vt, 2> cal_y(const Vt& x) const {
        return CalculateEclipseY(this->xc(), this->yc(), this->a(), this->b(), x);
    }

    Vt area() const {
        return Vt(_PI_) * this->a() * this->b();
    }

    bool empty() const {
        return this->a() <= 0.0 || this->b() <= 0.0;
    }
};

template<typename TYPE>
std::ostream& operator<<(std::ostream& stream, const Eclipse_<TYPE>& eclipse) {
    stream << std::setprecision(4)
           << "(X - " << eclipse.xc() << ")^2 / "
           << eclipse.a() * eclipse.a() << " + "
           << "(Y - " << eclipse.yc() << ")^2 / "
           << eclipse.b() * eclipse.b() << " = 1";
    return stream;
}

template<class TYPE, class TYPE2>
bool IsInOnEclipse(
    const TYPE&  xc, const TYPE& yc, const TYPE& a, const TYPE& b,
    const TYPE2& px, const TYPE2& py) {
    const auto dx = px - xc;
    const auto dy = py - yc;
    return dx * dx / (a * a) + dy * dy / (b * b) <= 1.0;
}

template<class TYPE, class TYPE2>
bool IsInOnEclipse(const Eclipse_<TYPE>& eclipse, const Point_<TYPE2, 2>& p) {
    return IsInOnEclipse(eclipse.xc(), eclipse.yc(), eclipse.a(), eclipse.b(),
                         p.x(), p.y());
}

template<class TYPE, class TYPE2>
bool IsInEclipse(
    const TYPE&  xc, const TYPE& yc, const TYPE& a, const TYPE& b,
    const TYPE2& px, const TYPE2& py) {
    const auto dx = px - xc;
    const auto dy = py - yc;
    return dx * dx / (a * a) + dy * dy / (b * b) < 1.0;
}

template<class TYPE, class TYPE2>
bool IsInEclipse(const Eclipse_<TYPE>& eclipse, const Point_<TYPE2, 2>& p) {
    return IsInEclipse(eclipse.xc(), eclipse.yc(), eclipse.a(), eclipse.b(),
                       p.x(), p.y());
}

}
#endif
