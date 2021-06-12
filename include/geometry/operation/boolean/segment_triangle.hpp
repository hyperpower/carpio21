#ifndef _INTERSECTION_SEGTRI_HPP_
#define _INTERSECTION_SEGTRI_HPP_

#include "geometry/geometry_define.hpp"
#include "intersection_base.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/triangle.hpp"

#include <memory>
#include <mutex>          // std::mutex
#include <unordered_map>
#include <bitset>

namespace carpio {
 
template<typename TYPE, St DIM>
class IntersectionSegTri_:public IntersectionBase_<Segment_<TYPE, DIM>, Triangle_<TYPE, DIM> >{
public:
    typedef IntersectionBase_<Segment_<TYPE, DIM>, Triangle_<TYPE, DIM> > Base;
    typedef typename Base::Object1 Object1;     
    typedef typename Base::Object2 Object2;     
    typedef typename Base::ReturnType ReturnType;
    typedef Triangle_<TYPE, DIM> Triangle;     
    typedef Point_<TYPE, DIM>                        Point;     
    typedef Point_<TYPE, DIM>                        Vec;     
    typedef std::shared_ptr<Vec>                   spVec;     
    typedef std::shared_ptr<Point_<TYPE, DIM> >    spPoint;     
    typedef std::shared_ptr<Triangle>              spTri;     
protected:


};


//  point location code -------------------------------------------------------
//           ^e1
//           |
//      4    3   2
//           |
//  --5------0-----1---->e0
//           |
//      6    7   8
//           |

template<typename TYPE>
class IntersectionSegTri_<TYPE, 2>:public IntersectionBase_<Segment_<TYPE, 2>, Triangle_<TYPE, 2> >{
public:
    typedef IntersectionBase_<Segment_<TYPE, 2>, Triangle_<TYPE, 2> > Base;
    typedef typename Base::Object1 Object1;     
    typedef typename Base::Object2 Object2;     
    typedef typename Base::ReturnType ReturnType;
    typedef Segment_<TYPE,2>                       Segment;
    typedef std::shared_ptr<Segment>             spSeg;
    typedef Triangle_<TYPE, 2>                     Triangle;     
    typedef Point_<TYPE, 2>                        Point;     
    typedef Point_<TYPE, 2>                        Vec;     
    typedef std::shared_ptr<Vec>                   spVec;     
    typedef std::shared_ptr<Point_<TYPE, 2> >    spPoint;     
    typedef std::shared_ptr<Triangle>              spTri;
protected:
    spTri _tri;
    spSeg _seg;
    std::array<Vec, 2> _atri;  // array trangle rebased
    std::array<Vec, 2> _aseg;  // segment rebased

    std::array<int, 2> _code;
public:
    // Intitial from shared_ptr
    IntersectionSegTri_(spTri spt, spSeg sps): _tri(spt), _seg(sps){
    }
    // Intitial from object
    IntersectionSegTri_(const Triangle& t, const Segment& s){
        this->_tri = std::make_shared<Triangle>(t);
        this->_seg = std::make_shared<Segment>(s);

        _rebase(*_tri, *_seg);

        _cal_location_code();
    }
    
    IntersectionSegTri_(const Vec& e0, const Vec& e1, const Vec& t0, const Vec& t1){
        _atri[0] = e0;
        _atri[1] = e1;
        _aseg[0] = t0;
        _aseg[1] = t1;
    }
protected:
    void _rebase(const Triangle& t, const Segment& s){
        // rebase to tri0
        _atri[0] = t[1] - t[0];
        _atri[1] = t[2] - t[0];

        _aseg[0] = s[0] - t[0];
        _aseg[1] = s[1] - t[0];

    }
    // vectors must be rebased.
    void _cal_location_code(){
        SingletonIntersectionHelper* phelper = SingletonIntersectionHelper::GetInstance();
        int side0 = WhichSide32D(_aseg[0], _atri[0]);
        int side1 = WhichSide32D(_aseg[0], _atri[1]);
        _code[0] = phelper->point_location_in_coordinate(side0, side1);
        // std::cout << "location code 0 = " << _code[0] << std::endl;
        side0 = WhichSide32D(_aseg[1], _atri[0]);
        side1 = WhichSide32D(_aseg[1], _atri[1]);
        _code[1] = phelper->point_location_in_coordinate(side0, side1);
        // std::cout << "location code 1 = " << _code[1] << std::endl;
    }


};

template<typename TYPE>
class IntersectionSegTri_<TYPE, 3>:public IntersectionBase_<Segment_<TYPE, 3>, Triangle_<TYPE, 2> >{
public:
    IntersectionSegTri_(){
        std::cout << "Intersection 3" << std::endl;
    }
};
}

#endif //