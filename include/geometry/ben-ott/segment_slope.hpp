#ifndef _BEN_OTT_SEGMENT_SLOPE_HPP_
#define _BEN_OTT_SEGMENT_SLOPE_HPP_

#include "algebra/number/number_define.hpp"
// #include "geometry/operation/boolean/segment_segment.hpp"


namespace carpio {

template<class SEG>
struct SegmentSlope_ {
    typedef SegmentSlope_<SEG> Self;
    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef typename SEG::coord_value_type Vt;

    static const int _RATIONAL_ = 2;
    static const int _NAG_INF_  = 0;
    static const int _POS_INF_  = 1;
    static const int _INVALID_  = -1;

    int  type; 
    Vt   value;

    SegmentSlope_():type(_INVALID_){} 

    SegmentSlope_(const SEG& seg){
        make(seg.psx(), seg.psy(), seg.pex(), seg.pey());
    }

    SegmentSlope_(const Point& ps, const Point& pe){
        make(ps[0], ps[1], pe[0], pe[1]);
    }

    void make(const Vt& x0,const Vt& y0,
              const Vt& x1,const Vt& y1){
        if(IsEqual(x0, x1)){
            if(IsGreater(y0, y1)){ //y0 > y1
                type = _NAG_INF_;
                value = (y0+y1) * 0.5;
            }else{
                type = _POS_INF_;
                value = (y0+y1) * 0.5;
            }
        }else {
            type = _RATIONAL_;
            value = (y0 - y1) / (x0 - x1) ;
        }
    }

    bool operator<(const Self& s) const{
        if(s.type == _INVALID_){
            throw std::runtime_error("Segment Slope INVALID");
            return false;
        }
        switch (type)
        {
        case _INVALID_: {
            throw std::runtime_error("Segment Slope INVALID");
            return false;
        }
        case _NAG_INF_: {
            if(s.type == _NAG_INF_){
                return value < s.value;
            } else {
                return true;
            }
        }
        case _POS_INF_: {
            if(s.type == _POS_INF_){
                return value < s.value;
            } else {
                return false;
            }
        }
        case _RATIONAL_: {
            if(s.type == _NAG_INF_){
                return false;
            }else if(s.type == _POS_INF_){
                return true;
            }else{
                return value < s.value;
            }
        }
        }
    }
    bool operator==(const Self &rhs) const{
        if(!(this->operator<(rhs)) && !rhs.operator<(*this)){
            return true;
        }else{
            return false;
        }
    }
};


} //namespace

#endif