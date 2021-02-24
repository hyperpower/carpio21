#ifndef _INTERSECTION_TRITRI_HPP_
#define _INTERSECTION_TRITRI_HPP_

#include "geometry/geometry_define.hpp"
#include "intersection_base.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/triangle.hpp"

#include <memory>

namespace carpio {


template<typename TYPE, St DIM>
class IntersectionTriTri_:public IntersectionBase_<Triangle_<TYPE, DIM>, Triangle_<TYPE, DIM> >{
public:
    typedef IntersectionBase_<Triangle_<TYPE, DIM>, Triangle_<TYPE, DIM> > Base;
    typedef typename Base::Object1 Object1;     
    typedef typename Base::Object2 Object2;     
    typedef typename Base::ReturnType ReturnType;
    typedef Triangle_<TYPE, DIM> Triangle;     
    typedef Point_<TYPE, DIM>                        Point;     
    typedef Point_<TYPE, DIM>                        Vec;     
    typedef std::shared_ptr<Point_<TYPE, DIM> >    spPoint;     
protected:
    std::shared_ptr<Object1> _t1;
    std::shared_ptr<Object1> _t2;
public:
    IntersectionTriTri_():Base(){};

    IntersectionTriTri_(const Object1& obj1, const Object2& obj2){
        _set_triangles(obj1, obj2);
        this->_inited = true;
    };

    bool is_intersect() const{
        if(!this->is_init()){
            std::cerr << "!> Triangles are not initialed" << std::endl;
            return false;
        }
    // 1 re-origin points
    //   set _t2[2] as the origin point
        std::array<Vec, 3> vr;
        vr[0] = (*_t1)[0] - (*_t2)[2];
        vr[1] = (*_t1)[1] - (*_t2)[2];
        vr[2] = (*_t1)[2] - (*_t2)[2];

        std::array<Vec, 3> ve;
        ve[0] = (*_t2)[0] - (*_t2)[2];
        ve[1] = (*_t2)[1] - (*_t2)[2];

        
        return false;

    }
protected:
    void _set_triangles(const Object1& obj1, const Object2& obj2){
        _t1 = std::make_shared<Object1>(obj1);
        _t2 = std::make_shared<Object2>(obj2);
    }

    // 1 re-origin points
    //   set _t2[2] as the origin point

};



}

#endif