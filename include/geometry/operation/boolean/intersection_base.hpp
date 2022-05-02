#ifndef _BOOLEAN_INTERSECTION_BASE_HPP_
#define _BOOLEAN_INTERSECTION_BASE_HPP_

#include <list>
#include <memory>

#include "geometry/geometry_define.hpp"

namespace carpio{


// template<class VEC>
// inline VEC Cross(const VEC& a, const VEC& b){
//     return VEC(a[1]*b[2]-b[1]*a[2], 
//                b[0]*a[2]-a[0]*b[2],
//                a[0]*b[1]-b[0]*a[1]);
// }
template<class VEC>
inline Vt Dot(const VEC& a, const VEC& b){
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]; 
}

template<class VEC, class NUM>
inline VEC Times(VEC& vec, const NUM& v){
    return VEC(vec[0] * v, 
               vec[1] * v, 
               vec[2] * v); 
}
template<class VEC>
inline Vt SquareSum(VEC& vec){
    return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]; 
}


// template<class VEC>
// inline short WhichSide32D(const VEC& v1, const VEC& v2, const VEC& v3){
//     /// d1    = v1 - v3
//     /// d2    = v2 - v3
//     /// cross = d1x * d2y - d1y * d2x
//     double res = ((v1[0] - v3[0]) * (v2[1] - v3[1])
//                 - (v2[0] - v3[0]) * (v1[1] - v3[1]));
//     if (res < 0){
//         return 2;
//     }else if (res > 0){
//         return 1;
//     }else{
//         return 0;
//     }
// }

template<class GEO1, class GEO2 = GEO1, class POINT = typename GEO1::Point>
class IntersectionReturn_{
public:
    typedef POINT Point;
    typedef const Point* cpPoint;
    typedef std::shared_ptr<Point> spPoint;
    typedef GEO1  Geo1;
    typedef const GEO1*  cpGeo1;
    typedef GEO2  Geo2;
    typedef const GEO2*  cpGeo2;

    typedef std::list<Point> ListPoint;

    typedef IntersectionReturn_<GEO1, GEO2, POINT> Self;

    cpGeo1      geo1;
    cpGeo2      geo2; 
    ListPoint   points;

    std::string  return_msg;  //
    // return code 1 intersect
    //             0 No intersect
    //             2 point touch
    //             3 line touch
    //             4 ploy touch

    IntersectionReturn_():
        geo1(nullptr),
        geo2(nullptr),
        return_msg(0){
    }

    IntersectionReturn_(const GEO1& g1, const GEO2& g2, const std::string& c):
        geo1(&g1), geo2(&g2),
        return_msg(c){
    }

    IntersectionReturn_(const Self& o): 
        geo1(o.geo1), geo2(o.geo2), return_msg(o.return_msg), points(o.points){
    }

    Self& operator=(const Self& o){
        if(this = &o){
            return *this;
        }
        geo1 = o.geo1;
        geo2 = o.geo2; 
        return_msg = o.return_msg;
        points = o.points;
        return *this;
    }

    void push_back(const Point& p){
        points.push_back(p);
    }


};

template<class GEO1, class GEO2 = GEO1, class POINT = typename GEO1::Point>
std::ostream& operator<<(std::ostream& stream, const IntersectionReturn_<GEO1, GEO2, POINT>& ret) {
    stream << ret.return_msg << " ";
    int count = 0;
    for(auto& p : ret.points){
        stream << count <<"_"<< p << " ";
        count ++;
    }
    // stream << std::endl;
    return stream;
}

template<class OBJ1, class OBJ2>
class IntersectionBase_{
public:
    typedef OBJ1 Object1;
    typedef OBJ2 Object2;
    typedef typename Object1::ValueType ValueType ;
    typedef IntersectionReturn_<OBJ1, OBJ2> ReturnType;
protected:
    bool _inited;
    bool _called_intersect;
    bool _called_return;
public:
    IntersectionBase_(){
       _inited           = false;
       _called_intersect = false;
       _called_return    = false;
    }
    bool is_init() const{
        return _inited;
    }

    virtual ReturnType no_intersections() const { 
        return ReturnType();
    }

    virtual ~IntersectionBase_(){}

    virtual void set_method(const std::string& method){}
    // virtual ReturnType cal_intersection() const{}
    // virtual ReturnType cal_intersection(const Object1&, const Object2) const{}

    // virtual void intersection_code() const{}
    // virtual void intersection_code(const Object1&, const Object2) const{}


};




}


#endif