#ifndef _BOOLEAN_INTERSECTION_BASE_HPP_
#define _BOOLEAN_INTERSECTION_BASE_HPP_


#include "geometry/geometry_define.hpp"

namespace carpio{


template<class VEC>
VEC Cross(const VEC& a, const VEC& b){
    
}

template<typename TYPE, St DIM>
struct IntersectionReturn_{
    typedef Point_<TYPE, DIM> Point;
    typedef std::list<std::shared_ptr<Point> > ListspPoint;

    int         return_code;
    ListspPoint points;
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
       _inited             = false;
       _called_intersect = false;
       _called_return    = false;
    }
    bool is_init() const{
        return _inited;
    }

    virtual ~IntersectionBase_(){}

    virtual void set_method(const std::string& method){}
    virtual ReturnType cal_intersection() const{}
    virtual ReturnType cal_intersection(const Object1&, const Object2) const{}

    virtual bool is_intersect() const{}
    virtual bool is_intersect(const Object1&, const Object2) const{}


};
}


#endif