#ifndef _BEN_OTT_SEGMENT_WITH_SLOPE_HPP_
#define _BEN_OTT_SEGMENT_WITH_SLOPE_HPP_

#include <iostream>
#include "algebra/number/number_define.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/ben-ott/segment_slope.hpp"
// #include "geometry/operation/boolean/segment_segment.hpp"


namespace carpio {

template<class P,
        typename std::enable_if<
            std::is_base_of<PointTag, typename P::Tag>::value, 
        bool>::type = true >
inline bool IsLess(const P& p1, const P& p2){  // x < y
    for(auto d = 0; d < P::Dim; d++){
        if(IsLess(p1[d], p2[d])){
            return true;
        }else if(IsLess(p2[d], p1[d])){
            return false;
        }
    }
    return false;
}
template<class P,
        typename std::enable_if<
            std::is_base_of<PointTag, typename P::Tag>::value, 
        bool>::type = true >
inline bool IsLessEqual(const P& p1, const P& p2){  // x < y
    for(auto d = 0; d < P::Dim; d++){
        if(IsLess(p1[d], p2[d])){
            return true;
        }else if(IsLess(p2[d], p1[d])){
            return false;
        }
    }
    return true;
}

struct SegmentWithSlopeTag: public SegmentTag {};

template<class SEG>
class SegWithSlope_: public SEG{
public:
	static const St Dim = SEG::Dim;
    typedef SEG Base;
    typedef SegWithSlope_<SEG> Self;
    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef const Segment* cpSegment;
    typedef typename SEG::coord_value_type coord_value_type;
    typedef SegmentSlope_<SEG> Slope;

    typedef SegmentWithSlopeTag Tag;

protected:
    cpSegment _cpseg;  //point to original object
    Slope     _slope;

public:
    SegWithSlope_(): _cpseg(nullptr){}

    SegWithSlope_(const Segment& seg):
        Base(seg), _cpseg(&seg){
        if(IsLess(Base::pe(), Base::ps())){
            this->change_orientation();
        }
        _slope = Slope(this->ps(), this->pe());
    }
    
    inline auto cpseg() const{
        return _cpseg;
    }

    inline auto seg() const{
        return *_cpseg;
    }
    
    inline const Slope& slope() const {
        return this->_slope;
    }
    inline const typename Slope::Vt& slope_value() const {
        return this->_slope.value;
    }

    inline bool is_vertical() const{
        return _slope.type == Slope::_NAG_INF_ || _slope.type == Slope::_POS_INF_;
    }
};


} //namespace

#endif