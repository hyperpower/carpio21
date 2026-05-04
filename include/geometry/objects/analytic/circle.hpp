#ifndef _CIRCLE_HPP_
#define _CIRCLE_HPP_

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

struct CircleTag: public GeometryTag {};

template<typename NUM>
inline NUM CalculateCircleY2(
        const NUM& xc, const NUM& r,
        const NUM& x) {
    const NUM dx = x - xc;
    return r * r - dx * dx;
}

template<typename NUM>
inline NUM CalculateCircleX2(
        const NUM& yc, const NUM& r,
        const NUM& y) {
    const NUM dy = y - yc;
    return r * r - dy * dy;
}

template<typename NUM>
inline std::array<NUM, 2> CalculateCircleY(
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& x,
        const NUM& s = 1e-10) {
    const NUM dy2 = CalculateCircleY2(xc, r, x);
    if (dy2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dy = std::sqrt((std::max)(NUM(0), dy2));
    return {yc - dy, yc + dy};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateCircleX(
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& y,
        const NUM& s = 1e-10) {
    const NUM dx2 = CalculateCircleX2(yc, r, y);
    if (dx2 < -s) {
        const NUM nan = std::numeric_limits<NUM>::quiet_NaN();
        return {nan, nan};
    }
    const NUM dx = std::sqrt((std::max)(NUM(0), dx2));
    return {xc - dx, xc + dx};
}

template<typename NUM>
inline std::array<NUM, 2> CalculateCircleXInRange(
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& y,  const NUM& x_min, const NUM& x_max, const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateCircleX(xc, yc, r, y, s);
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
inline std::array<NUM, 2> CalculateCircleYInRange(
        const NUM& xc, const NUM& yc, const NUM& r,
        const NUM& x, const NUM& y_min, const NUM& y_max, const IntervalType& type = _cc_,
        const NUM& s = 1e-10) {
    auto res = CalculateCircleY(xc, yc, r, x, s);
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
class Circle_: public std::array<TYPE, 3> {
public:
    static const St Dim = 2;
    typedef St size_type;
    typedef TYPE Vt;
    typedef TYPE& reference;
    typedef const TYPE& const_reference;
    typedef Circle_<TYPE> Circle;
    typedef Circle_<TYPE> Self;
    typedef CircleTag Tag;

    typedef Point_<Vt, 2> Point;
    typedef TYPE value_type;

public:
    Circle_() :
            std::array<Vt, 3>() {
    }

    Circle_(const Vt& xc, const Vt& yc, const Vt& r) {
        this->at(0) = xc;
        this->at(1) = yc;
        this->at(2) = r;
    }

    Circle_(const Point& c, const Vt& r) {
        this->at(0) = c.x();
        this->at(1) = c.y();
        this->at(2) = r;
    }

    Circle_(const Circle& c) {
        this->at(0) = c[0];
        this->at(1) = c[1];
        this->at(2) = c[2];
    }

    Circle_(std::initializer_list<TYPE> l) {
        auto b = l.begin();
        if (l.size() == 3) {
            this->reconstruct(*b, *(std::next(b)), *(std::next(b, 2)));
        }
    }

    void reconstruct(const Vt& xc, const Vt& yc, const Vt& r) {
        this->xc() = xc;
        this->yc() = yc;
        this->r()  = r;
    }

    inline reference xc() {
        return this->at(0);
    }

    inline reference yc() {
        return this->at(1);
    }

    inline reference r() {
        return this->at(2);
    }

    inline const_reference xc() const {
        return this->at(0);
    }

    inline const_reference yc() const {
        return this->at(1);
    }

    inline const_reference r() const {
        return this->at(2);
    }

    Point center() const {
        return Point(this->xc(), this->yc());
    }

    std::array<Vt, 2> cal_x(const Vt& y) const {
        return CalculateCircleX(this->xc(), this->yc(), this->r(), y);
    }

    std::array<Vt, 2> cal_y(const Vt& x) const {
        return CalculateCircleY(this->xc(), this->yc(), this->r(), x);
    }

    bool empty() const {
        return this->r() <= 0.0;
    }
};

template<typename TYPE>
std::ostream& operator<<(std::ostream& stream, const Circle_<TYPE>& circle) {
    stream << std::setprecision(4)
           << "(X - " << circle.xc() << ")^2 + "
           << "(Y - " << circle.yc() << ")^2 = "
           << circle.r() * circle.r();
    return stream;
}

template<class TYPE, class TYPE2>
bool IsInOnCircle(
    const TYPE&  xc, const TYPE& yc, const TYPE& r, 
    const TYPE2& px, const TYPE2& py) {
    const auto dx = px - xc;
    const auto dy = py - yc;
    return dx * dx + dy * dy <= r * r;
}

template<class TYPE, class TYPE2>
bool IsInOnCircle(const Circle_<TYPE>& circle, const Point_<TYPE2, 2>& p) {
    return IsInOnCircle(circle.xc(), circle.yc(), circle.r(), p.x(), p.y());
}

template<class TYPE, class TYPE2>
bool IsInCircle(
    const TYPE&  xc, const TYPE& yc, const TYPE& r,
    const TYPE2& px, const TYPE2& py) {
    const auto dx = px - xc;
    const auto dy = py - yc;
    return dx * dx + dy * dy < r * r;
}

template<class TYPE, class TYPE2>
bool IsInCircle(const Circle_<TYPE>& circle, const Point_<TYPE2, 2>& p) {
    return IsInCircle(circle.xc(), circle.yc(), circle.r(), p.x(), p.y());
}

}
#endif
