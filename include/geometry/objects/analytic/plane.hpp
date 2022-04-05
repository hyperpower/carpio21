#ifndef _PLANE_HPP_
#define _PLANE_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/analytic/line.hpp"
#include <array>

namespace carpio {
template<typename NUM>
inline NUM CalculateX(const NUM& a, const NUM& b, const NUM& c, const NUM& alpha,  // The line
               const NUM& y, const NUM& z,                                            // Y = value
               const NUM& s = 1e-10){
    if(std::abs(a) < s){
        return (alpha - y * b - z * c) / s;
    } else {
        return (alpha - y * b - z * c) / a;
    }
}
template<typename NUM>
inline NUM CalculateY(const NUM& a, const NUM& b, const NUM& c, const NUM& alpha,  // The line
               const NUM& z, const NUM& x,                                            // Y = value
               const NUM& s = 1e-10){
    if(std::abs(b) < s){
        return (alpha - x * a - z * c) / s;
    } else {
        return (alpha - x * a - z * c) / b;
    }
}
template<typename NUM>
inline NUM CalculateZ(const NUM& a, const NUM& b, const NUM& c, const NUM& alpha,  // The line
               const NUM& x, const NUM& y,                                            // Y = value
               const NUM& s = 1e-10){
    if(std::abs(c) < s){
        return (alpha - x * a - y * b) / s;
    } else {
        return (alpha - x * a - y * b) / c;
    }
}
template<typename NUM>
inline NUM Calculate(const NUM& a, const NUM& b, const NUM& c, const NUM& alpha,  // The line
               const Axes& axe, const NUM& v1, const NUM& v2,                                            // Y = value
               const NUM& s = 1e-10){
    if(axe == _X_){
        return CalculateX(a, b, c, alpha, v1, v2, s);
    } else if(axe == _Y_){
        return CalculateY(a, b, c, alpha, v1, v2, s);
    } else if(axe == _Z_){
        return CalculateZ(a, b, c, alpha, v1, v2, s);
    } 
    return 0.0;
}
class PlaneTag: public GeometryTag{};

template<typename TYPE>
class Plane_: public std::array<TYPE, 4> {
    //The Line function defined as ax+by+cz=alpha
public:
    static const St Dim = 3;
    typedef St size_type;
    typedef TYPE vt;
    typedef TYPE& reference;
    typedef const TYPE& const_reference;
    typedef PlaneTag Tag;
public:
    Plane_() :
            std::array<vt, 4>() {
    }
    Plane_(const vt& a, const vt& b, const vt& c, const vt& alpha) {
        this->at(0) = a;
        this->at(1) = b;
        this->at(2) = c;
        this->at(3) = alpha;
    }
    Plane_(const vt& x, const vt& y, const vt& z, const vt& nx, const vt& ny,
            const vt& nz) {
        //assert(!isEqual(ax, bx) || !isEqual(ay,by));
        this->a() = nx;
        this->b() = ny;
        this->c() = nz;
        this->alpha() = (nx * x + ny * y + nz * z);
    }
    void reconstruct(const vt& a, const vt& b, const vt& c, const vt& alpha) {
        if (a == 0.0 && b == 0.0) {
            a = _SMALL_;
            b = _SMALL_;
        }
        this->a() = a;
        this->b() = b;
        this->c() = c;
        this->alpha() = alpha;
    }
    inline reference a() {
        return this->at(0);
    }
    inline reference b() {
        return this->at(1);
    }
    inline reference c() {
        return this->at(2);
    }
    inline reference alpha() {
        return this->at(3);
    }
    inline const_reference a() const {
        return this->at(0);
    }
    inline const_reference b() const {
        return this->at(1);
    }
    inline const_reference c() const {
        return this->at(2);
    }
    inline const_reference alpha() const {
        return this->at(2);
    }
    vt cal_x(const vt& y, const vt& z) const {
        return CalculateX(this->a(), this->b(), this->c(), this->alpha(),
                          y, z, _SMALL_);       
    }
    vt cal_y(const vt& z, const vt& x) const {
        return CalculateY(this->a(), this->b(), this->c(), this->alpha(),
                          z, x, _SMALL_);       
    }
    vt cal_z(const vt& x, const vt& y) const {
        return CalculateZ(this->a(), this->b(), this->c(), this->alpha(),
                          x, y, _SMALL_);       
    }
    vt cal(Axes a, vt v1, vt v2) const{
        return Calculate(this->a(), this->b(), this->c(), this->alpha(),
                          a, v1, v2, _SMALL_);       
    }
    vt intersept_x() const {
        return this->cal_x(0, 0);
    }
    vt intersept_y() const {
        return this->cal_y(0, 0);
    }
    vt intersept_z() const {
        return this->cal_z(0, 0);
    }
    vt norm_x() const {
        return this->a();
    }
    vt norm_y() const {
        return this->b();
    }
    vt norm_z() const {
        return this->c();
    }
    bool empty() const {
        if (this->a() != 0.0 && this->b() != 0.0 && this->c() != 0.0) {
            return true;
        } else {
            return false;
        }
    }
    void show() const {
        std::cout << this->a() << " X + " << this->b() << " Y + " << this->c()
                << " Z = " << this->alpha() << "\n";
    }
};

}

#endif
