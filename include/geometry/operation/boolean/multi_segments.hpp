#ifndef _INTERSECTION_MUTISEG_HPP_
#define _INTERSECTION_MUTISEG_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "segment_segment.hpp"

namespace carpio{


template<class GEO, class POINT = typename GEO::Point>
class IntersectionLineSweep_{
public:
    typedef GEO Geo;
    // typedef GEO2 Geo2;
    typedef POINT Point;
    static const int _START_ = 0;
    static const int _END_   = 1;
    static const int _CROSS_ = 2;
protected:
    struct Event{
        const Point* p;
        const Geo*   geo;
        int          type;
    };

    class EventGreatEqual {
        public:
        // Comparator function
        bool operator()(const Event& a,
                        const Event& b) const{
            if (*(a.p->begin()) > *(b.p->begin())){
                return true;
            }else if( *(a.p->begin()) == *(b.p->begin()) ){
                return a.type > b.type;
            }else{
                return false;
            }
        }
    };
    typedef typename GEO::Tag GTag;
    typedef std::priority_queue<Event,std::vector<Event>, EventGreatEqual> EventQueue;

    struct GeoComponent{
        const Geo* geo;
        double     key_value;

        GeoComponent(const Geo& g, SegmentTag){
            geo = &g;
            key_value = g[0].y() + 1e-5 * g[0].x();            
        }
        GeoComponent(const Geo* g, SegmentTag){
            geo = g;
            key_value = (*g)[0].y() + 1e-5 * (*g)[0].x();            
        }
    };
    class GeoComponentGreatEqual {
        public:
        // Comparator function
        bool operator()(const GeoComponent& a,
                        const GeoComponent& b) const{
            return a.key_value > b.key_value;
        }
    };

    typedef std::set<GeoComponent, GeoComponentGreatEqual> GeoSet;

    void _insert_event(const GEO& geo, SegmentTag){
        Event es;
        es.geo = &geo;
        es.p   = &(geo[0]);
        es.type = _START_;
        Event ee;
        ee.geo = &geo;
        ee.p   = &(geo[1]);
        es.type = _END_;
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
    GeoSet _geo_set;

    typedef IntersectionReturn_<Geo> InterRet;
    typedef std::list<IntersectionReturn_<Geo>> ListInterRet;

    ListInterRet _list_res;


    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionLineSweep_(const CONTAINER& con){
        _build_priority_queue(con);
    }

    void execute(){
        for(int i = 0; i < 2; i++){
            std::cout << "Step = " << i << " =========== " << std::endl;
            Event event = _eventq.top();
            _eventq.pop();
            std::cout <<"This Event = "<< *(event.geo) << ' ' << *(event.p) << std::endl;
            if (event.type == _START_){
                auto pair = _geo_set.insert(GeoComponent(event.geo, GTag()));
                ASSERT(pair.second == true);
                auto iter = pair.first;

            }
        }
    }

    void check_intersect_with_next(typename GeoSet::iterator iter){
        auto next = std::next(iter);
        if(next != _geo_set.end()){
            auto res = check_intersect(*(iter->geo), *(next->geo), GTag());

        }
    }

    bool check_intersect(const Geo& g1, const Geo& g2, SegmentTag){
        typedef typename Geo::coord_value_type Cvt;
        typedef IntersectionPairSS_<Cvt, Geo::Dim> Inter;
        Inter inter(g1, g2);
        auto t = inter.cal_intersection_type();
        InterRet ret(g1, g2, ToString(t));
        if(t == _SS_INTERSECT_){
            auto p = inter.cal_intersection_point();
            ret.push_back(p);
            _list_res.push_back(ret);
            // add event
            Event e;
            e.geo = &g1;
            e.p   = _list_res.back().
        }
        
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