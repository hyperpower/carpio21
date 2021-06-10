#ifndef _BOOLEAN_INTERSECTION_BASE_HPP_
#define _BOOLEAN_INTERSECTION_BASE_HPP_

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


template<typename TYPE, St DIM>
struct IntersectionReturn_{
    typedef Point_<TYPE, DIM> Point;
    typedef std::list<int>  ListSize;
    typedef std::list<std::shared_ptr<Point> > ListspPoint;

    int         return_code;  //
    // std::string return_sting; // intersection name
    ListSize    indexs;       // size of each return objects
    ListspPoint points;

    // return code 1 intersect
    //             0 No intersect
    //             2 touch

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