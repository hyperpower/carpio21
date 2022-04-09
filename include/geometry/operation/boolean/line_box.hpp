#ifndef _BOOLEAN_LINE_BOX_HPP_
#define _BOOLEAN_LINE_BOX_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point_chain.hpp"
#include "geometry/operation/affine.hpp"

#include "algebra/algebra.hpp"

#include <memory>
#include <array>

namespace carpio {

// Line : ax + by = alpha
// Box  : Pmin Pmax



// Point:min is (0, 0)
//       max is (1, 1)
// Line : a >= b
template<class POINT>
class _FunctorIntersectOrientLineUnitBox_{
protected:
    typedef std::list<POINT> ListPoint;
    typedef _FunctorIntersectOrientLineUnitBox_<POINT> Self;
    typedef POINT Point;
    double on_y;
    double on_x;
    bool   is_trivial;

    void _cal_on_y(const double& b, const double& alpha, const double& tol){
        // x = 0
        on_y = alpha / (b + tol);
    }    
    void _cal_on_x(const double& a, const double& alpha, const double& tol){
        // y = 0
        on_x = alpha / (a + tol);
    }    
public:
    _FunctorIntersectOrientLineUnitBox_():on_x(0), on_y(0), is_trivial(false){};
    _FunctorIntersectOrientLineUnitBox_(const Self& other):
        on_x(other.on_x), on_y(other.on_y), is_trivial(other.is_trivial){};

    ListPoint trivial(const double& a, const double& b, const double& alpha, // Point max=(1,1)
                      const double& tol = 1e-10){
        // line pass (0,0)
        ListPoint res;
        bool zero_a     = a == 0.0;
        bool zero_b     = b == 0.0;
        if (zero_a && zero_b){
            is_trivial = true;
            return res;
        }
        if (zero_a && alpha/b == 0.0){ // Y = 0
            res.emplace_back(Point(0.0, 0.0));
            res.emplace_back(Point(1.0, 0.0));
            is_trivial = true;
            return res;
        }else if (zero_b && alpha/a == 0.0){ // X = 0
            res.emplace_back(Point(0.0, 0.0));
            res.emplace_back(Point(0.0, 1.0));
            is_trivial = true;
            return res;
        }else if (alpha == 0.0){
            res.emplace_back(Point(0.0, 0.0));
            is_trivial = true;
            return res;
        }
        if ( zero_a && (alpha/b - 1.0) == 0.0){ // Y = 1
            res.emplace_back(Point(0.0, 1.0));
            res.emplace_back(Point(1.0, 1.0));
            is_trivial = true;
            return res;
        }else if (zero_b && (alpha/a - 1.0) == 0.0){ // X = 1
            res.emplace_back(Point(1.0, 0.0));
            res.emplace_back(Point(1.0, 1.0));
            is_trivial = true;
            return res;
        }else if (a + b - 1.0 == 0.0){
            res.emplace_back(Point(1.0, 1.0));
            is_trivial = true;
            return res;
        }
        return res;
    }

    ListPoint operator()(const double& a, const double& b, const double& alpha,// Point max=(1,1)
                        const double& tol = 1e-10){
        ListPoint res = this->trivial(a, b, alpha, tol);
        // no intersection or on intersection
        if ((a + b - alpha < 0) || (-alpha > 0) || this->is_trivial)
        {
            return res;
        }
        // point on Y, that is X = 0
        this->_cal_on_y(b, alpha, tol);
        if (this->on_y <= 1.0 && this->on_y > 0)
        {
            res.emplace_back(Point(0.0, on_y));
        }
        else if (on_y > 1.0)
        {
            double on_y1 = (alpha - b) / (a + tol);
            res.emplace_back(Point(on_y1, 1.0));
        }
        // point on X, Y = 0
        this->_cal_on_x(a, alpha, tol);
        if (this->on_x <= 1.0 && this->on_x > 0)
        {
            res.emplace_back(Point(on_x, 0.0));
        }
        else if (this->on_x > 1.0)
        {
            double on_x1 = (alpha - a) / (b + tol);
            res.emplace_back(Point(1.0, on_x1));
        }
        return res;
    } 
};
template<class POINT>
class _FunctorIntersectOrientPlaneUnitBox_{
protected:
    typedef std::list<POINT> ListPoint;
    typedef _FunctorIntersectOrientPlaneUnitBox_<POINT> Self;
    typedef _FunctorIntersectOrientLineUnitBox_<POINT>  FunctorLine;
    typedef POINT Point;
    double on_y;
    double on_x;
    double on_z;
    bool   is_trivial;

    void _cal_on_x(const double& a, const double& alpha, const double& tol){
        // y = 0, z = 0
        on_x = alpha / (a + tol);
    }    
    void _cal_on_y(const double& b, const double& alpha, const double& tol){
        // x = 0, z = 0
        on_y = alpha / (b + tol);
    }    
    void _cal_on_z(const double& c, const double& alpha, const double& tol){
        // x = 0, y = 0
        on_z = alpha / (c + tol);
    }    
public:
    _FunctorIntersectOrientPlaneUnitBox_():on_x(0), on_y(0), on_z(0), is_trivial(false){};
    _FunctorIntersectOrientPlaneUnitBox_(const Self& other):
        on_x(other.on_x), on_y(other.on_y), on_z(other.on_z), is_trivial(other.is_trivial){};

    ListPoint trivial(const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
                      const double& tol = 1e-10){
        // line pass (0,0)
        ListPoint res;
        bool zero_a = (a == 0.0);
        bool zero_b = (b == 0.0);
        bool zero_c = (c == 0.0);
        if (zero_a && zero_b && zero_c){
            is_trivial = true;
            return res;
        }
        if (zero_a && zero_c){ // Y =  alpha / b
            double adb = alpha/b;
            if ( adb >= 0.0 && adb <= 1.0 ){ 
                res.emplace_back(Point(0.0, adb, 0.0));
                res.emplace_back(Point(0.0, adb, 1.0));
                res.emplace_back(Point(1.0, adb, 1.0));
                res.emplace_back(Point(1.0, adb, 0.0));
                is_trivial = true;
                return res;
            }
        }else if (zero_b && zero_c ){ // X = 0
            double ada = alpha/a;
            if ( ada >= 0.0 && ada <= 1.0 ){ 
                res.emplace_back(Point(ada, 0.0, 0.0));
                res.emplace_back(Point(ada, 1.0, 0.0));
                res.emplace_back(Point(ada, 1.0, 1.0));
                res.emplace_back(Point(ada, 0.0, 1.0));
                is_trivial = true;
                return res;
            }
        }else if (zero_a && zero_b){ // z = 0
            double adc = alpha/a;
            if ( adc >= 0.0 && adc <= 1.0 ){ 
                res.emplace_back(Point(0.0, 0.0, adc));
                res.emplace_back(Point(1.0, 0.0, adc));
                res.emplace_back(Point(1.0, 1.0, adc));
                res.emplace_back(Point(1.0, 1.0, adc));
                is_trivial = true;
                return res;
            }
        }else if (alpha == 0.0){
            res.emplace_back(Point(0.0, 0.0, 0.0));
            is_trivial = true;
            return res;
        }else if (a + b + c - 1.0 == 0.0){
            res.emplace_back(Point(1.0, 1.0, 1.0));
            is_trivial = true;
            return res;
        }
        return res;
    }

    ListPoint operator()(const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
                        const double& tol = 1e-10){
        ListPoint res = this->trivial(a, b, c, alpha, tol);
        // no intersection or on intersection
        if ((a + b + c - alpha < 0) || (-alpha > 0) || this->is_trivial){
            return res;
        }
        this->_cal_on_z(c, alpha, tol);
        if (0 < this->on_z && this->on_z <= 1.0 ){
            res.emplace_back(Point(0.0, 0.0, on_z));
        } 
        this->_cal_on_x(a, alpha, tol);
        double on_x1y1 = (alpha - a - b) / (c + tol);
        if (0 < this->on_x && this->on_x <=1.0){
            res.emplace_back(Point(this->on_x, 0.0, 0.0));
        } else if (this->on_x > 1.0){
            double on_x1y0 = (alpha - a) / (c + tol);
            if( 0 < on_x1y0 && on_x1y0 <= 1.0){
                res.emplace_back(Point(1.0, 0.0, on_x1y0));
            }
            if (on_x1y1 < 0){
                double on_x1z0 = (alpha - a) / (b + tol);
                res.emplace_back(Point(1.0, on_x1z0, 0.0));
            } else { 
                res.emplace_back(Point(1.0, 1.0, on_x1y1));
            }
        }
        this->_cal_on_y(b, alpha, tol);
        if (0 < this->on_y && this->on_y <= 1.0) {
            res.emplace_back(Point(0.0, this->on_y, 0.0));
        } else if (this->on_y > 1.0) {
            if (on_x1y1 < 0){
                double on_y1z0 = (alpha - b) / (a + tol);
                res.emplace_back(Point(on_y1z0, 1.0, 0.0));
            }
            double on_x0y1 = (alpha - b) / (c + tol);
            if( 0 < on_x0y1 && on_x0y1 <= 1.0){
                res.emplace_back(Point(0.0, 1.0, on_x0y1));
            }
        }
        if ( this->on_z > 1.0){
            double on_y0z1 = (alpha - c) / (a + tol);
            if (0 < on_y0z1 && on_y0z1 <= 1.0){
                res.emplace_front(Point(on_y0z1, 0.0, 1.0));
            } else {
                double on_x1z1 = (alpha - a - c) / (b + tol);
                res.emplace_front(Point(1.0, on_x1z1, 1.0));
            }
            double on_x0z1 = (alpha - c) / (b + tol);
            if (0 < on_x0z1 && on_x0z1 <= 1.0){
                res.emplace_back(Point(0.0, on_x0z1, 1.0));
            } else {
                double on_y1z1 = (alpha - b - c) / (a + tol);
                res.emplace_back(Point(on_y1z1, 1.0, 1.0));
            }
        }
            // double on_y0z1 = (alpha - c) / (a + tol);
            // if ( 0 < on_y0z1 && on_y0z1 <= 1){
            //     res.emplace_back(Point(on_y0z1, 0.0, 1.0));
            // }else{
            //     double on_x1z1 = (alpha - a - c) / (b + tol);
            //     res.emplace_back(Point(1.0, on_x1z1, 1.0));
            // }
        
        return res;
    } 
};
template<class POINT>
class _FunctorNegativeOrientLineUnitBox_:public _FunctorIntersectOrientLineUnitBox_<POINT>{
protected:
    typedef std::list<POINT> ListPoint;
    typedef _FunctorIntersectOrientLineUnitBox_<POINT> Base;
    typedef _FunctorNegativeOrientLineUnitBox_<POINT> Self;
    typedef POINT Point;
public:
    _FunctorNegativeOrientLineUnitBox_():Base(){};
    _FunctorNegativeOrientLineUnitBox_(const Self& other):Base(other){};
    
    ListPoint operator()(const double& a, const double& b, const double& alpha,// Point max=(1,1)
                        const double& tol = 1e-14){
        ListPoint res = this->Base::operator()(a, b, alpha, tol);
        if (res.size() <= 1){
            return res;
        }
        if (this->on_y <= 1.0 && this->on_y > 0) {
            res.emplace_back(Point(0.0, 0.0));
        }else if (this->on_y > 1.0) {
            res.emplace_front(Point(0.0, 1.0));
            res.emplace_front(Point(0.0, 0.0));
        }
        // point on X, Y = 0
        if (this->on_x > 1.0) {
            res.emplace_back(Point(1.0, 0.0));
        }
        return res;
    } 
};


template<class POINT>
class _FunctorPositiveOrientLineUnitBox_:public _FunctorIntersectOrientLineUnitBox_<POINT>{
protected:
    typedef std::list<POINT> ListPoint;
    typedef _FunctorIntersectOrientLineUnitBox_<POINT> Base;
    typedef _FunctorPositiveOrientLineUnitBox_<POINT> Self;
    typedef POINT Point;
public:
    _FunctorPositiveOrientLineUnitBox_():Base(){};
    _FunctorPositiveOrientLineUnitBox_(const Self& other):Base(other){};
    
    ListPoint operator()(const double& a, const double& b, const double& alpha,// Point max=(1,1)
                        const double& tol = 1e-14){
        ListPoint res = this->Base::operator()(a, b, alpha, tol);
        if (res.size() <= 1){
            return res;
        }
        // additional points
        if (this->on_y <= 1.0 && this->on_y > 0){
            res.emplace_front(Point(0.0, 1.0));
        }
        // point on X, Y = 0
        if (this->on_x <= 1.0 && this->on_x > 0){
            res.emplace_back(Point(1.0, 0.0));
            res.emplace_back(Point(1.0, 1.0));
        }else if (this->on_x > 1.0){
            res.emplace_back(Point(1.0, 1.0));
        }
        return res;
    }  
};

template<class POINT, class FUNCTOR>
std::list<POINT>
_OpOrientLineUnitBox(const double& a, const double& b, const double& alpha,// Point max=(1,1)
                      const double& tol = 1e-14){     
    double na = std::abs(a);
    double nb = std::abs(b);
    double nalpha = alpha;
    if (a == -na){
        nalpha = alpha - a;
    }
    if (b == -nb){
        nalpha = nalpha - b;
    }
    FUNCTOR fun;
    auto res = fun(na, nb, nalpha, tol);

    for(auto& p : res){
        if (a == -na){
            Reflect(p, _X_, 1.0);
        }
        if (b == -nb){
            Reflect(p, _Y_, 1.0);
        }
    }
    
    return res;
}
template<class POINT, class FUNCTOR>
std::list<POINT>
_OpOrientLineUnitBox(const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
                     const double& tol = 1e-14){
    double na = std::abs(a);
    double nb = std::abs(b);
    double nc = std::abs(c);
    double nalpha = alpha;
    if (a == -na){
        nalpha = alpha - a;
    }
    if (b == -nb){
        nalpha = nalpha - b;
    }
    if (c == -nc){
        nalpha = nalpha - c;
    }
    FUNCTOR fun;
    auto res = fun(na, nb, nalpha, tol);

    for(auto& p : res){
        if (a == -na){
            Reflect(p, _X_, 1.0);
        }
        if (b == -nb){
            Reflect(p, _Y_, 1.0);
        }
        if (c == -nc){
            Reflect(p, _Z_, 1.0);
        }
    }
    
    return res;
}
template<class POINT, class FUNCTOR>
std::list<POINT>
_OpLineBox(const POINT& min, const POINT& max, 
            const double& a, const double& b, const double& alpha,// Point max=(1,1)
            const double& tol = 1e-14){ 
    Line_<double> line(a, b, alpha);
    auto arr_scale = max - min;
    auto inv_scale(arr_scale);
    for(auto& d : inv_scale){
        d = 1.0/(d + tol);
    }
    Translate(line, -min);
    Scale(line, inv_scale);

    auto res = _OpOrientLineUnitBox<POINT, FUNCTOR>(line.a(), line.b(), line.alpha(), tol);
    
    Scale(res, arr_scale);
    Translate(res, min);
    
    return res;
}
template<class POINT, class FUNCTOR>
std::list<POINT>
_OpLineBox(const POINT& min, const POINT& max, 
            const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
            const double& tol = 1e-14){ 
    Plane_<double> plane(a, b, alpha);
    auto arr_scale = max - min;
    auto inv_scale(arr_scale);
    for(auto& d : inv_scale){
        d = 1.0/(d + tol);
    }
    Translate(plane,  -min);
    Scale(plane, inv_scale);

    auto res = _OpOrientLineUnitBox<POINT, FUNCTOR>(plane.a(), plane.b(), plane.c(), plane.alpha(), tol);
    
    Scale(res, arr_scale);
    Translate(res, min);
    
    return res;
}
template<class POINT>
std::list<POINT>
IntersectLineBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorIntersectOrientLineUnitBox_<POINT> Functor;
    return _OpLineBox<POINT, Functor>(min, max, a, b, alpha, tol);
}
template<class POINT>
std::list<POINT>
IntersectPlaneBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorIntersectOrientLineUnitBox_<POINT> Functor;
    return _OpLineBox<POINT, Functor>(min, max, a, b, alpha, tol);
}

template<class POINT>
std::list<POINT>
NegativeLineBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorNegativeOrientLineUnitBox_<POINT> Functor;
    return _OpLineBox<POINT, Functor>(min, max, a, b, alpha, tol);
}
template<class POINT>
std::list<POINT>
PositiveLineBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorPositiveOrientLineUnitBox_<POINT> Functor;
    return _OpLineBox<POINT, Functor>(min, max, a, b, alpha, tol);
}

// Tools for VOF

}

#endif
