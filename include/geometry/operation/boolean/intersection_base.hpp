#ifndef _BOOLEAN_INTERSECTION_BASE_HPP_
#define _BOOLEAN_INTERSECTION_BASE_HPP_

#include <mutex>          // std::mutex

#include "geometry/geometry_define.hpp"

namespace carpio{


template<class VEC>
inline VEC Cross(const VEC& a, const VEC& b){
    return VEC(a[1]*b[2]-b[1]*a[2], 
               b[0]*a[2]-a[0]*b[2],
               a[0]*b[1]-b[0]*a[1]);
}
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

template<class VEC>
inline short WhichSide32D(const VEC& p, const VEC& vec){
    // vec must be 0 base
    /// d1    = v1 - v3
    /// d2    = v2 - v3
    /// cross = d1x * d2y - d1y * d2x
    double res =  (p[0] * vec[1]) - (vec[0] * p[1]);
    if (res < 0){
        return 2;
    }else if (res > 0){
        return 1;
    }else{
        return 0;
    }
}

template<class VEC>
inline short WhichSide32D(const VEC& v1, const VEC& v2, const VEC& v3){
    /// d1    = v1 - v3
    /// d2    = v2 - v3
    /// cross = d1x * d2y - d1y * d2x
    double res = ((v1[0] - v3[0]) * (v2[1] - v3[1])
                - (v2[0] - v3[0]) * (v1[1] - v3[1]));
    if (res < 0){
        return 2;
    }else if (res > 0){
        return 1;
    }else{
        return 0;
    }
}

template<typename TYPE, St DIM>
struct IntersectionReturn_{
    typedef Point_<TYPE, DIM> Point;
    typedef std::list<int>  ListSize;
    typedef std::list<std::shared_ptr<Point> > ListspPoint;

    double      return_code;  //
    // std::string return_sting; // intersection name
    ListSize    indexs;       // size of each return objects
    ListspPoint points;

    // return code 1 intersect
    //             0 No intersect
    //             2 point touch
    //             3 line touch
    //             4 ploy touch

    IntersectionReturn_():
        return_code(0){
    }

    IntersectionReturn_(const int& c):
        return_code(c){
    }
};


template<class OBJ1, class OBJ2>
class IntersectionBase_{
public:
    typedef OBJ1 Object1;
    typedef OBJ2 Object2;
    typedef typename Object1::ValueType ValueType ;
    typedef IntersectionReturn_<ValueType, Object1::Dim> ReturnType;
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

    // virtual bool is_intersect() const{}
    // virtual bool is_intersect(const Object1&, const Object2) const{}


};




}


#endif