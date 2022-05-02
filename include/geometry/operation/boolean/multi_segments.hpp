#ifndef _INTERSECTION_MUTISEG_HPP_
#define _INTERSECTION_MUTISEG_HPP_

#include <memory>
#include <queue>
#include <iostream>

#include "segment_segment.hpp"

namespace carpio{


template<class GEO, class POINT = typename GEO::Point>
class IntersectionLineSweep_{
public:
    typedef GEO Geo;
    // typedef GEO2 Geo2;
    typedef POINT Point;

protected:
    struct Event{
        const Point* p;
        const Geo*   geo;
    };

    class EventGreatEqual {
        public:
        // Comparator function
        bool operator()(const Event& a,
                        const Event& b){
            return *(a.p->begin()) >= *(b.p->begin());
        }
    };
    typedef typename GEO::Tag GTag;
    typedef std::priority_queue<Event,std::vector<Event>, EventGreatEqual> EventQueue;


    void _insert_event(const GEO& geo, SegmentTag){
        Event es;
        es.geo = &geo;
        es.p   = &(geo[0]);
        Event ee;
        ee.geo = &geo;
        ee.p   = &(geo[1]);
        _eventq.push(es);
        _eventq.push(ee);
    }

    template<class CONTAINER>
    void _build_priority_queue(const CONTAINER& con){
        for(auto& g : con){
            _insert_event(g, GTag());
        }
    }

public:

    EventQueue _eventq;

    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionLineSweep_(const CONTAINER& con){
        _build_priority_queue(con);
    }

    void print_event_queue() { // NB: pass by value so the print uses a copy
        while(!_eventq.empty()) {
            auto top = _eventq.top();
            std::cout << *(top.geo) << ' ' << *(top.p) << std::endl;
            _eventq.pop();
        }
        std::cout << '\n';
    }

};



}

#endif