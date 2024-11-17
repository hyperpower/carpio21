#ifndef _BEN_OTT_SWEEP_EVENT_HPP_
#define _BEN_OTT_SWEEP_EVENT_HPP_

#include "segment_with_slope.hpp"

namespace carpio {

template<class POI>
class SweepEvent_ {
private:
    typedef SweepEvent_<POI> Self;
    typedef POI Point;
    typedef typename Point::value_type value_type;
    typedef std::shared_ptr<Point> spPoint;

    spPoint _sppoint; 

public:
    SweepEvent_() : _sppoint() {};

    SweepEvent_(const Self& e) : _sppoint(e._sppoint) {};

    SweepEvent_(const Point& o) : _sppoint(std::make_shared<Point>(o)) {};

    const Point& get_point() const{
        return *_sppoint;
    }
    bool operator<(const Self& e) const{
        return IsLess(*_sppoint, *(e._sppoint));
    }  
};

} //namespace


#endif