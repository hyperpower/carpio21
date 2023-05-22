#ifndef _INTERSECTION_SS_HPP_
#define _INTERSECTION_SS_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>
#include <iterator>

namespace carpio {


inline std::string ToString(IntersectionTypeSS ss){
    switch(ss){
        case _SS_NO_       :  return "NO";       break;
        case _SS_CONNECT_  :  return "CONNECT";  break;
        case _SS_TOUCH_    :  return "TOUCH";    break;
        case _SS_OVERLAP_  :  return "OVERLAP";  break;
        case _SS_SAME_     :  return "SAME";     break;
        case _SS_INTERSECT_:  return "INTERSECT";break;
    }
    return "ERROR IntersectionType";
}

inline IntersectionTypeSS ToIntersectionTypeSS(int a){
    ASSERT(a >=0 && a <=5);
    switch(a){
        case 0 : return _SS_NO_       ; break;
        case 1 : return _SS_CONNECT_  ; break;
        case 2 : return _SS_TOUCH_    ; break;
        case 3 : return _SS_OVERLAP_  ; break;
        case 4 : return _SS_SAME_     ; break;
        case 5 : return _SS_INTERSECT_; break;
    }
    SHOULD_NOT_REACH;
    return _SS_NO_;
}

static const short _SS_ERROR_   = -2;
static const short _SS_FURTHER_ = -1;

static const short _SSTYPE[7][7] = {
    // -1 further predicate
    // -2 error
    {_SS_OVERLAP_, _SS_TOUCH_,   _SS_TOUCH_,   _SS_OVERLAP_, _SS_OVERLAP_, _SS_OVERLAP_, _SS_OVERLAP_ },
    {_SS_TOUCH_,   _SS_NO_,      _SS_FURTHER_, _SS_CONNECT_, _SS_CONNECT_, _SS_NO_,      _SS_NO_      },
    {_SS_TOUCH_,   _SS_FURTHER_, _SS_NO_,      _SS_CONNECT_, _SS_CONNECT_, _SS_NO_,      _SS_NO_      },
    {_SS_OVERLAP_, _SS_CONNECT_, _SS_CONNECT_, _SS_ERROR_,   _SS_SAME_,    _SS_CONNECT_, _SS_OVERLAP_ },
    {_SS_OVERLAP_, _SS_CONNECT_, _SS_CONNECT_, _SS_SAME_,    _SS_ERROR_,   _SS_OVERLAP_, _SS_CONNECT_ },
    {_SS_OVERLAP_, _SS_NO_,      _SS_NO_,      _SS_CONNECT_, _SS_OVERLAP_, _SS_NO_,      _SS_OVERLAP_ },
    {_SS_OVERLAP_, _SS_NO_,      _SS_NO_,      _SS_OVERLAP_, _SS_CONNECT_, _SS_OVERLAP_, _SS_NO_      }
};


template<class VEC>
inline VEC CalSegmentsIntersection(const VEC& s1s, const VEC& s1e,
                                   const VEC& s2s, const VEC& s2e){
        ASSERT(VEC::Dim == 2);
        auto x0 = s1s[0];
        auto x1 = s1e[0];
        auto x2 = s2s[0];
        auto x3 = s2e[0];

        auto y0 = s1s[1];
        auto y1 = s1e[1];
        auto y2 = s2s[1];
        auto y3 = s2e[1];

        typedef typename VEC::value_type Cvt;
        Cvt denom;
        if constexpr (std::is_floating_point<Cvt>::value){  
            denom = (x0 - x1) * (y2- y3) - (y0 - y1) * (x2- x3) + 1e-20;
        }else{
            denom = (x0-x1) * (y2- y3) - (y0 - y1) * (x2- x3);
        }

        Cvt x = ((x0 * y1 - y0 * x1) * (x2 - x3)
                - (x0 - x1) * (x2 * y3 - y2 * x3)) / denom ;
        Cvt y = ((x0 * y1 - y0 * x1) * (y2 - y3)
                - (y0 - y1) * (x2 * y3 - y2 * x3)) / denom;

        return VEC{x, y};
}
template<class VEC> // s1s = {0,0}
inline VEC CalSegmentsIntersection(const VEC& s1e,
                                   const VEC& s2s, 
                                   const VEC& s2e){
        ASSERT(VEC::Dim == 2);
        // double x0 = 0.0;
        auto x1 = s1e[0];
        auto x2 = s2s[0];
        auto x3 = s2e[0];

        // double y0 = 0.0;
        auto y1 = s1e[1];
        auto y2 = s2s[1];
        auto y3 = s2e[1];

        typedef typename VEC::value_type Cvt;
        Cvt denom = (-x1) * (y2- y3) - (- y1) * (x2- x3) + 1e-20;

        Cvt x = (x1 * (x2 * y3 - y2 * x3)) / denom ;
        Cvt y = (y1 * (x2 * y3 - y2 * x3)) / denom;

        return {{x, y}};
}
// deprecate ===============
template<typename TYPE, St DIM>
class IntersectionPairSS_ {
public:
    static const St Dim = DIM;
    typedef Point_<TYPE, DIM> Point;
    typedef Box_<TYPE, DIM> Box;
    typedef Segment_<TYPE, DIM> Segment;
    typedef TYPE Vt;

    typedef const Point& const_ref_Point;
    typedef const Point* const_pPoint;

protected:
    ArrayListT_<const_pPoint> _arrp; //pPoint list
    ArrayListV_<int> _pointpos;      //point position

    std::shared_ptr<Point> _sppoint; //new intersection point
public:
    IntersectionPairSS_(
            const Point& p1,
            const Point& p2,
            const Point& p3,
            const Point& p4) : _arrp(4), _pointpos(4), _sppoint(nullptr){
        _arrp[0] = &p1;
        _arrp[1] = &p2;
        _arrp[2] = &p3;
        _arrp[3] = &p4;
        _pointpos.assign(-1);
        if (_is_box_in_on()) {
            _get_relation();
        }
    }
    IntersectionPairSS_(
            const Segment& seg1,
            const Segment& seg2
            ) : _arrp(4), _pointpos(4), _sppoint(nullptr){
        _arrp[0] = &(seg1.ps());
        _arrp[1] = &(seg1.pe());
        _arrp[2] = &(seg2.ps());
        _arrp[3] = &(seg2.pe());
        _pointpos.assign(-1);
        if (_is_box_in_on()) {
            _get_relation();
        }
    }

    bool _is_box_in_on(){
        Box b1(*(_arrp[0]), *(_arrp[1]));
        Box b2(*(_arrp[2]), *(_arrp[3]));
        return IsInOn(b1, b2);
    }

    void _get_relation(){
        _pointpos[0] = OnWhichSide7(*(_arrp[2]), *(_arrp[3]), *(_arrp[0]));
        _pointpos[1] = OnWhichSide7(*(_arrp[2]), *(_arrp[3]), *(_arrp[1]));
        if(_get_ss_type1() == _SS_FURTHER_){
            _pointpos[2] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[2]));
            _pointpos[3] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[3]));
        }
    }

    short _get_ss_type1(){ // seg1 to seg2
        short t = _SSTYPE[_pointpos[0]][_pointpos[1]];
        ASSERT(t != _SS_ERROR_);
        return t;
    }

    short _get_ss_type2() { // seg2 to seg1
        short t = _SSTYPE[_pointpos[2]][_pointpos[3]];
        ASSERT(t != _SS_ERROR_);
        return t;
    }

    int point_position(St idx){
        ASSERT(idx >= 0 && idx < 4);
        // 0 start of seg1
        // 1 start of seg1
        // 2 start of seg2
        // 3 start of seg2
        if(idx == 2 || idx == 3){
            _pointpos[2] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[2]));
            _pointpos[3] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[3]));
        }
        return _pointpos[idx];

    }

    IntersectionTypeSS cal_intersection_type(){
        if(_pointpos[1] > -1){
            short t1 = _get_ss_type1();
            if(t1 == _SS_FURTHER_) { // type is further
                short t2 = _get_ss_type2();
                if(t2 == _SS_FURTHER_){
                    return _SS_INTERSECT_;
                }else{
                    return ToIntersectionTypeSS(t2);
                }
            }else{
                return ToIntersectionTypeSS(t1);
            }
        }else{
            return _SS_NO_;
        }
    }

    // normal intersection
    Point cal_intersection_point(){
        // The intersection type must be _SS_INTERSECT_
        ASSERT(DIM == 2);

        return CalSegmentsIntersection((*(_arrp[0])), (*(_arrp[1])), (*(_arrp[2])),(*(_arrp[3])));
    }
};
// ^ ^^^ deprecate


template<class GEO1, class GEO2>
class IntersectionResultImplement_<GEO1, GEO2, SegmentTag, SegmentTag>{
public:
    typedef GEO1 Geo1;
    typedef GEO2 Geo2;
    typedef typename GEO1::Point Point;
    typedef IntersectionResultImplement_<GEO1, GEO2, SegmentTag, SegmentTag> Self;
    typedef const GEO1* cpGeo;
    typedef std::list<cpGeo> ListcpGeo;

    // const Geo1* geo1;
    // const Geo2* geo2;

    int type;
    Point point;
    ListcpGeo list;

    IntersectionResultImplement_(): type(-1){}
    
    IntersectionResultImplement_(const Point& p): 
        type(-1), point(p){}
    
    IntersectionResultImplement_(const Geo1& g1, const Geo2& g2, int t):  type(t){
        list.push_back(&g1);
        list.push_back(&g2);
    }

    void geo1(const Geo1& g){
        auto it = list.begin();
        (*it) = &g;
    }
    void geo2(const Geo1& g){
        auto it = list.begin();
        std::advance(it, 1);
        (*it) = &g;
    }

    void add(const Geo1& g){
        list.push_back(&g);
    }
    
    void show() const{
        auto it = list.begin();
        std::cout << "Seg1 : " << *(*it) << std::endl;
        it++;
        std::cout << "Seg2 : " << *(*it) << std::endl;
        if(list.size() > 2){
            std::cout << "Multi Segments (" << list.size() <<")Intersect" << std::endl;
        }else{
            auto tss = ToIntersectionTypeSS(type);
            std::cout << "Type : " << ToString(tss) << std::endl;
        }
        std::cout << "Point: " << point << std::endl;
    }
};

// Replace  IntersectionPairSS_
template<class GEO1, class GEO2>
class IntersectionImplement_<GEO1, GEO2, SegmentTag, SegmentTag>{
public:
    // static const St Dim = DIM;
    typedef typename GEO1::Point Point;
    typedef typename Point::coord_value_type Cvt;
    typedef Box_<Cvt, Point::Dim> Box;
    // typedef Segment_<TYPE, DIM> Segment;
    // typedef TYPE Vt;
    typedef IntersectionResultImplement_<GEO1, GEO2, SegmentTag, SegmentTag> Result;

    typedef const Point& crPoint;
    typedef const Point* cpPoint;
protected:
    std::array<cpPoint, 4> _arrp; //pPoint list
    std::array<int, 4>     _position;      //point position

    // std::shared_ptr<Point> _sppoint; //new intersection point
    Result _res;
public:
    IntersectionImplement_(
            const Point& p1,
            const Point& p2,
            const Point& p3,
            const Point& p4){
        _arrp[0] = &p1;
        _arrp[1] = &p2;
        _arrp[2] = &p3;
        _arrp[3] = &p4;
        _position.fill(-1);
        if (_is_box_in_on()) {
            _get_relation();
        }
    }
    IntersectionImplement_(
            const GEO1& seg1,
            const GEO2& seg2
            ):_res(seg1, seg2, -1){
        _arrp[0] = &(seg1.ps());
        _arrp[1] = &(seg1.pe());
        _arrp[2] = &(seg2.ps());
        _arrp[3] = &(seg2.pe());
        _position.fill(-1);
        if (_is_box_in_on()) {
            _get_relation();
        }
    }
    IntersectionTypeSS cal_intersection_type(){
        if(_position[1] > -1){
            short t1 = _get_ss_type1();
            if(t1 == _SS_FURTHER_) { // type is further
                short t2 = _get_ss_type2();
                if(t2 == _SS_FURTHER_){
                    return _SS_INTERSECT_;
                }else{
                    return ToIntersectionTypeSS(t2);
                }
            }else{
                return ToIntersectionTypeSS(t1);
            }
        }else{
            return _SS_NO_;
        }
    }
    // normal intersection
    Point cal_intersection_point(){
        // The intersection type must be _SS_INTERSECT_
        ASSERT(Point::Dim == 2);
        return CalSegmentsIntersection((*(_arrp[0])), (*(_arrp[1])), (*(_arrp[2])),(*(_arrp[3])));
    }

    Result execute(){
        this->_res.type = this->cal_intersection_type();
        if(this->_res.type == _SS_INTERSECT_){
            this->_res.point = cal_intersection_point();
        }
        return this->_res;
    }

    PointToSegmentPosition point_position(St idx){
        ASSERT(idx >= 0 && idx < 4);
        // 0 start of seg1
        // 1 end   of seg1
        // 2 start of seg2
        // 3 end   of seg2
        if((idx == 0 || idx == 1) && (_position[idx] == -1)){
            _get_relation();
        }
        if((idx == 2 || idx == 3) && (_position[idx] == -1)){
            _position[2] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[2]));
            _position[3] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[3]));
        }
        return ToPointToSegmentPosition(_position[idx]);
    }
protected:
    short _get_ss_type1(){ // seg1 to seg2
        short t = _SSTYPE[_position[0]][_position[1]];
        ASSERT(t != _SS_ERROR_);
        return t;
    }

    short _get_ss_type2() { // seg2 to seg1
        short t = _SSTYPE[_position[2]][_position[3]];
        ASSERT(t != _SS_ERROR_);
        return t;
    }
    bool _is_box_in_on(){
        Box b1(*(_arrp[0]), *(_arrp[1]));
        Box b2(*(_arrp[2]), *(_arrp[3]));
        return IsInOn(b1, b2);
    }
    void _get_relation(){
        _position[0] = OnWhichSide7(*(_arrp[2]), *(_arrp[3]), *(_arrp[0]));
        _position[1] = OnWhichSide7(*(_arrp[2]), *(_arrp[3]), *(_arrp[1]));
        if(_get_ss_type1() == _SS_FURTHER_){
            _position[2] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[2]));
            _position[3] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[3]));
        }
    }
    
};
// Replace  IntersectionPairSS_


}

#endif
