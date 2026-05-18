#ifndef _BOOLEAN_LINE_BOX_HPP_
#define _BOOLEAN_LINE_BOX_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/box.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point_chain.hpp"

#include "algebra/algebra_define.hpp"
#include "algebra/algebra.hpp"

#include <algorithm>
#include <cmath>
#include <list>
#include <vector>

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

    static void _emplace_unique(ListPoint& res, const Point& p) {
        for (const auto& op : res) {
            if (IsSame(op, p)) {
                return;
            }
        }
        res.emplace_back(p);
    }
    static void _append_edge_intersections(
            ListPoint& res,
            const double& a, const double& b, const double& alpha,
            const Point& p0, const Point& p1) {
        const double f0 = a * p0[0] + b * p0[1] - alpha;
        const double f1 = a * p1[0] + b * p1[1] - alpha;
        const bool z0 = IsZero(f0);
        const bool z1 = IsZero(f1);

        if (z0 && z1) {
            _emplace_unique(res, p0);
            _emplace_unique(res, p1);
            return;
        }
        if (z0) {
            _emplace_unique(res, p0);
            return;
        }
        if (z1) {
            _emplace_unique(res, p1);
            return;
        }
        if ((f0 < 0.0 && f1 > 0.0) || (f0 > 0.0 && f1 < 0.0)) {
            const double t = f0 / (f0 - f1);
            _emplace_unique(res, Point(
                    p0[0] + t * (p1[0] - p0[0]),
                    p0[1] + t * (p1[1] - p0[1])));
        }
    }
public:
    _FunctorIntersectOrientLineUnitBox_():on_x(0), on_y(0){};

    _FunctorIntersectOrientLineUnitBox_(const Self& other):
        on_x(other.on_x), on_y(other.on_y){};

    ListPoint operator()(const double& a, const double& b, const double& alpha,// Point max=(1,1)
                        const double& tol = 1e-10){
        this->on_x = 0.0;
        this->on_y = 0.0;
        ListPoint res;

        if (IsZero(a) && IsZero(b)) {
            return res;
        }

        if ((a + b - alpha < -tol) || (alpha < -tol)) {
            return res;
        }

        this->on_y = IsZero(b) ? 0.0 : alpha / b;
        this->on_x = IsZero(a) ? 0.0 : alpha / a;

        _append_edge_intersections(
                res, a, b, alpha, Point(0.0, 0.0), Point(0.0, 1.0));
        _append_edge_intersections(
                res, a, b, alpha, Point(0.0, 1.0), Point(1.0, 1.0));
        _append_edge_intersections(
                res, a, b, alpha, Point(0.0, 0.0), Point(1.0, 0.0));
        _append_edge_intersections(
                res, a, b, alpha, Point(1.0, 0.0), Point(1.0, 1.0));

        if (res.size() == 2) {
            auto first = res.begin();
            auto second = first;
            ++second;
            const double d0 = (*first)[0] - (*second)[0];
            const double d1 = (*first)[1] - (*second)[1];
            if (d0 * (-b) + d1 * a < 0.0) {
                std::swap(*first, *second);
            }
        }
        return res;
    } 
};
template<class POINT>
class _FunctorIntersectOrientPlaneUnitBox_{
protected:
    typedef std::list<POINT> ListPoint;
    typedef _FunctorIntersectOrientPlaneUnitBox_<POINT> Self;
    typedef POINT Point;
    double on_y;
    double on_x;
    double on_z;

    static void _emplace_unique(std::vector<Point>& res, const Point& p) {
        for (const auto& op : res) {
            if (IsSame(op, p)) {
                return;
            }
        }
        res.emplace_back(p);
    }
    static double _dot(
            const double& x0, const double& y0, const double& z0,
            const double& x1, const double& y1, const double& z1) {
        return x0 * x1 + y0 * y1 + z0 * z1;
    }
    static void _append_edge_intersections(
            std::vector<Point>& res,
            const double& a, const double& b, const double& c, const double& alpha,
            const Point& p0, const Point& p1) {
        const double f0 = a * p0[0] + b * p0[1] + c * p0[2] - alpha;
        const double f1 = a * p1[0] + b * p1[1] + c * p1[2] - alpha;
        const bool z0 = IsZero(f0);
        const bool z1 = IsZero(f1);

        if (z0 && z1) {
            _emplace_unique(res, p0);
            _emplace_unique(res, p1);
            return;
        }
        if (z0) {
            _emplace_unique(res, p0);
            return;
        }
        if (z1) {
            _emplace_unique(res, p1);
            return;
        }
        if ((f0 < 0.0 && f1 > 0.0) || (f0 > 0.0 && f1 < 0.0)) {
            const double t = f0 / (f0 - f1);
            _emplace_unique(res, Point(
                    p0[0] + t * (p1[0] - p0[0]),
                    p0[1] + t * (p1[1] - p0[1]),
                    p0[2] + t * (p1[2] - p0[2])));
        }
    }
    static ListPoint _ordered_polygon(
            std::vector<Point>& points,
            const double& a, const double& b, const double& c) {
        ListPoint res;
        if (points.empty()) {
            return res;
        }
        if (points.size() <= 2) {
            for (const auto& p : points) {
                res.emplace_back(p);
            }
            return res;
        }

        double cx = 0.0;
        double cy = 0.0;
        double cz = 0.0;
        for (const auto& p : points) {
            cx += p[0];
            cy += p[1];
            cz += p[2];
        }
        const double inv_size = 1.0 / static_cast<double>(points.size());
        cx *= inv_size;
        cy *= inv_size;
        cz *= inv_size;

        double ux = 0.0;
        double uy = 0.0;
        double uz = 0.0;
        if (std::abs(a) > std::abs(b)) {
            ux = -c;
            uz = a;
        } else {
            uy = c;
            uz = -b;
        }
        const double vx = b * uz - c * uy;
        const double vy = c * ux - a * uz;
        const double vz = a * uy - b * ux;

        std::sort(points.begin(), points.end(),
                [&](const Point& p0, const Point& p1) {
            const double r0x = p0[0] - cx;
            const double r0y = p0[1] - cy;
            const double r0z = p0[2] - cz;
            const double r1x = p1[0] - cx;
            const double r1y = p1[1] - cy;
            const double r1z = p1[2] - cz;
            const double a0 = std::atan2(
                    _dot(r0x, r0y, r0z, vx, vy, vz),
                    _dot(r0x, r0y, r0z, ux, uy, uz));
            const double a1 = std::atan2(
                    _dot(r1x, r1y, r1z, vx, vy, vz),
                    _dot(r1x, r1y, r1z, ux, uy, uz));
            return a0 < a1;
        });

        for (const auto& p : points) {
            res.emplace_back(p);
        }
        return res;
    }
public:
    _FunctorIntersectOrientPlaneUnitBox_():on_x(0), on_y(0), on_z(0){};
    _FunctorIntersectOrientPlaneUnitBox_(const Self& other):
        on_x(other.on_x), on_y(other.on_y), on_z(other.on_z){};

    ListPoint operator()(const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
                        const double& tol = 1e-10){
        this->on_x = 0.0;
        this->on_y = 0.0;
        this->on_z = 0.0;

        if (IsZero(a) && IsZero(b) && IsZero(c)) {
            return ListPoint();
        }
        if ((a + b + c - alpha < -tol) || (alpha < -tol)) {
            return ListPoint();
        }

        this->on_x = IsZero(a) ? 0.0 : alpha / a;
        this->on_y = IsZero(b) ? 0.0 : alpha / b;
        this->on_z = IsZero(c) ? 0.0 : alpha / c;

        std::vector<Point> points;
        points.reserve(6);

        const Point p000(0.0, 0.0, 0.0);
        const Point p100(1.0, 0.0, 0.0);
        const Point p010(0.0, 1.0, 0.0);
        const Point p110(1.0, 1.0, 0.0);
        const Point p001(0.0, 0.0, 1.0);
        const Point p101(1.0, 0.0, 1.0);
        const Point p011(0.0, 1.0, 1.0);
        const Point p111(1.0, 1.0, 1.0);

        _append_edge_intersections(points, a, b, c, alpha, p000, p100);
        _append_edge_intersections(points, a, b, c, alpha, p010, p110);
        _append_edge_intersections(points, a, b, c, alpha, p001, p101);
        _append_edge_intersections(points, a, b, c, alpha, p011, p111);
        _append_edge_intersections(points, a, b, c, alpha, p000, p010);
        _append_edge_intersections(points, a, b, c, alpha, p100, p110);
        _append_edge_intersections(points, a, b, c, alpha, p001, p011);
        _append_edge_intersections(points, a, b, c, alpha, p101, p111);
        _append_edge_intersections(points, a, b, c, alpha, p000, p001);
        _append_edge_intersections(points, a, b, c, alpha, p100, p101);
        _append_edge_intersections(points, a, b, c, alpha, p010, p011);
        _append_edge_intersections(points, a, b, c, alpha, p110, p111);

        return _ordered_polygon(points, a, b, c);
    } 
};

template<class POINT>
std::list<POINT> _UnitBoxPolygon2d(){
    std::list<POINT> res;
    res.emplace_back(POINT(0.0, 0.0));
    res.emplace_back(POINT(1.0, 0.0));
    res.emplace_back(POINT(1.0, 1.0));
    res.emplace_back(POINT(0.0, 1.0));
    return res;
}

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
        if (alpha >= a + b - tol){
            return _UnitBoxPolygon2d<POINT>();
        }
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
        if (alpha <= tol){
            return _UnitBoxPolygon2d<POINT>();
        }
        ListPoint res = this->Base::operator()(a, b, alpha, tol);
        if (res.size() <= 1){
            return res;
        }
        // additional points
        if (IsInRange(0.0, this->on_y, 1.0, _oo_, tol)){
            res.emplace_front(Point(0.0, 1.0));
        }
        // point on X, Y = 0
        if (IsInRange(0.0, this->on_x, 1.0, _oo_, tol)){
            res.emplace_back(Point(1.0, 0.0));
            res.emplace_back(Point(1.0, 1.0));
        }else if (this->on_x >= 1.0 - tol){
            res.emplace_back(Point(1.0, 1.0));
        }
        return res;
    }  
};

template<class POINT, class FUNCTOR>
std::list<POINT>
_Orient(const double& a, const double& b, const double& alpha,// Point max=(1,1)
        const double& tol){     
    double na = std::abs(a);
    double nb = std::abs(b);
    double nalpha = alpha;
    const bool negative_a = a < -tol;
    const bool negative_b = b < -tol;
    if (negative_a){
        nalpha = alpha - a;
    }
    if (negative_b){
        nalpha = nalpha - b;
    }
    FUNCTOR fun;
    auto res = fun(na, nb, nalpha, tol);

    for(auto& p : res){
        if (negative_a){
            p[0] = 1.0 - p[0];
        }
        if (negative_b){
            p[1] = 1.0 - p[1];
        }
    }
    
    return res;
}
template<class POINT, class FUNCTOR>
std::list<POINT>
_Orient(const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
        const double& tol){
    double na = std::abs(a);
    double nb = std::abs(b);
    double nc = std::abs(c);
    double nalpha = alpha;
    const bool negative_a = a < -tol;
    const bool negative_b = b < -tol;
    const bool negative_c = c < -tol;
    if (negative_a){
        nalpha = alpha - a;
    }
    if (negative_b){
        nalpha = nalpha - b;
    }
    if (negative_c){
        nalpha = nalpha - c;
    }
    FUNCTOR fun;
    auto res = fun(na, nb, nc, nalpha, tol);

    for(auto& p : res){
        if (negative_a){
            p[0] = 1.0 - p[0];
        }
        if (negative_b){
            p[1] = 1.0 - p[1];
        }
        if (negative_c){
            p[2] = 1.0 - p[2];
        }
    }
    
    return res;
}
template<class POINT, class FUNCTOR>
std::list<POINT>
_TranslateAndScale(const POINT& min, const POINT& max, 
            const double& a, const double& b, const double& alpha,
            const double& tol){ 
    auto arr_scale = max - min;
    for(auto& d : arr_scale){
        if (std::abs(d) <= tol){
            return std::list<POINT>();
        }
    }

    const double scaled_a = a * arr_scale[0];
    const double scaled_b = b * arr_scale[1];
    const double scaled_alpha = alpha - a * min[0] - b * min[1];

    auto res = _Orient<POINT, FUNCTOR>(scaled_a, scaled_b, scaled_alpha, tol);
    
    for (auto& p : res) {
        p[0] = p[0] * arr_scale[0] + min[0];
        p[1] = p[1] * arr_scale[1] + min[1];
    }
    
    return res;
}
template<class POINT, class FUNCTOR>
std::list<POINT>
_TranslateAndScale(const POINT& min, const POINT& max, 
            const double& a, const double& b, const double& c, const double& alpha,
            const double& tol){ 
    auto arr_scale = max - min;
    for(auto& d : arr_scale){
        if (IsZero(d)){
            return std::list<POINT>();
        }
    }

    const double scaled_a = a * arr_scale[0];
    const double scaled_b = b * arr_scale[1];
    const double scaled_c = c * arr_scale[2];
    const double scaled_alpha = alpha - a * min[0] - b * min[1] - c * min[2];

    auto res = _Orient<POINT, FUNCTOR>(
            scaled_a, scaled_b, scaled_c, scaled_alpha, tol);
    
    for (auto& p : res) {
        p[0] = p[0] * arr_scale[0] + min[0];
        p[1] = p[1] * arr_scale[1] + min[1];
        p[2] = p[2] * arr_scale[2] + min[2];
    }
    
    return res;
}

template<class POINT>  
std::list<POINT>
IntersectLineBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorIntersectOrientLineUnitBox_<POINT> Functor;
    return _TranslateAndScale<POINT, Functor>(min, max, a, b, alpha, tol);
}
template<class POINT>
std::list<POINT>
IntersectPlaneBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& c, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorIntersectOrientPlaneUnitBox_<POINT> Functor;
    return _TranslateAndScale<POINT, Functor>(min, max, a, b, c, alpha, tol);
}

template<class POINT>
std::list<POINT>
NegativeLineBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorNegativeOrientLineUnitBox_<POINT> Functor;
    return _TranslateAndScale<POINT, Functor>(min, max, a, b, alpha, tol);
}
template<class NUM>
std::list<Point_<NUM, 2> >
NegativeLineBox(const NUM& dx, const NUM& dy, 
                const Line_<NUM>& line){
    typedef Point_<NUM, 2> Point;     
    typedef _FunctorNegativeOrientLineUnitBox_<Point> Functor;
    return _TranslateAndScale<Point, Functor>(
                Point(0, 0), Point(dx, dy), 
                line.a(), line.b(), line.alpha(), DefaultTolerance<NUM>());
}

template<class POINT>
std::list<POINT>
PositiveLineBox(const POINT& min, const POINT& max, 
                 const double& a, const double& b, const double& alpha,// Point max=(1,1)
                 const double& tol = 1e-14){     
    typedef _FunctorPositiveOrientLineUnitBox_<POINT> Functor;
    return _TranslateAndScale<POINT, Functor>(min, max, a, b, alpha, tol);
}
template<class NUM>
std::list<Point_<NUM, 2> >
PositiveLineBox(const NUM& dx, const NUM& dy, 
                const Line_<NUM>& line){
    typedef Point_<NUM, 2> Point;     
    typedef _FunctorPositiveOrientLineUnitBox_<Point> Functor;
    return _TranslateAndScale<Point, Functor>(
                Point(0, 0), Point(dx, dy), 
                line.a(), line.b(), line.alpha(), DefaultTolerance<NUM>());
}


template<class POINT>
double IntersectionAreaNegative(
        const POINT& min, const POINT& max,
        const double& a, const double& b, const double& alpha,
        const double& tol = 1e-14){
    auto polygon = NegativeLineBox(min, max, a, b, alpha, tol);
    return AreaPointChain(polygon);
}

template<class POINT>
double IntersectionAreaPositive(
        const POINT& min, const POINT& max,
        const double& a, const double& b, const double& alpha,
        const double& tol = 1e-14){
    auto polygon = PositiveLineBox(min, max, a, b, alpha, tol);
    return AreaPointChain(polygon);
}

template<class TYPE>
double IntersectionAreaNegative(
        const Line_<TYPE>& line,
        const Box_<TYPE, 2>& box){
    return IntersectionAreaNegative(
            box.min(), box.max(),
            line.a(), line.b(), line.alpha(),
            DefaultTolerance<TYPE>());
}

template<class TYPE>
double IntersectionAreaPositive(
        const Line_<TYPE>& line,
        const Box_<TYPE, 2>& box){
    return IntersectionAreaPositive(
            box.min(), box.max(),
            line.a(), line.b(), line.alpha(),
            DefaultTolerance<TYPE>());
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               LineTag, BoxTag){
    (void)method;
    return IntersectionAreaNegative(g1, g2);
}

template<class GEO1, class GEO2>
auto IntersectArea(const GEO1& g1, const GEO2& g2,
               const std::string& method,
               BoxTag, LineTag){
    (void)method;
    return IntersectionAreaNegative(g2, g1);
}

// Tools for VOF

}

#endif
