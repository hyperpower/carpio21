#ifndef _INTERSECTION_MUTISEG_HPP_
#define _INTERSECTION_MUTISEG_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "segment_segment.hpp"
#include "utility/profile.hpp"
#include "utility/avl_tree.hpp"
#include "ben_ott_intersection.hpp"
// #include "ben_ott_intersection_2.hpp"


namespace carpio{

template<class CONTAINER>
auto IntersectN2(const CONTAINER& con, SegmentTag){
    typedef typename CONTAINER::value_type Seg;
    typedef typename Seg::coord_value_type Cvt;
    typedef Intersection_<Seg, Seg> Inter;

    typedef IntersectionResult_<Seg, Seg> InterRes;
    typedef std::list<InterRes> ListInterRes;
    ListInterRes lres;
    for(auto iter = con.begin(); iter != con.end(); ++iter){
        auto& seg1 = *iter;
        for(auto iterin = std::next(iter); iterin != con.end(); ++iterin){
            auto& seg2 = *iterin;
            // ProfileStart("Inter2Seg");
            Inter inter(seg1, seg2);
            auto res = inter.execute();
            // ProfileEnd();
            if(res.type != _SS_NO_ && res.type != _SS_INVALID_){
                lres.push_back(res);
            }
        }
    }
    return lres;
}



template<class GEO>
class IntersectionSweepLineSimple_{
public:
    typedef GEO Geo;
    typedef typename GEO::Tag GTag;
    typedef typename GEO::coord_value_type Cvt;
    // typedef GEO2 Geo2;
    typedef typename GEO::Point Point;
protected:
    struct Event{
        static const int _START_ = 0;
        static const int _END_   = 1;
    
        Cvt          value;
        const Geo*   geo;
        int          type;
        
        Event():value(0), geo(nullptr),type(-1){}

        std::string type_to_string(int t) const{
            switch (t){
            case Event::_START_:
                return "Event_START";
                break;
            case Event::_END_:
                return "Event_END";
                break;
            default:
                return "Event_ERROR";
                break;
            }
        }
        
        std::string type_to_string() const{
            return this->type_to_string(this->type);
        }
    };
    class EventGreatEqual {
        public:
        // Comparator function
        bool operator()(const Event& a,
                        const Event& b) const{
            return a.value >= b.value;
        }
    };

    typedef std::priority_queue<Event, std::deque<Event>, EventGreatEqual> EventQueue;
    typedef std::set<const Geo*> GeoSet;
    typedef IntersectionResult_<Geo, Geo> Result;
    typedef std::list<Result> ListResult;

    EventQueue _eq;
    GeoSet     _set;
    ListResult _list_res;

public:

    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionSweepLineSimple_(const CONTAINER& con){
        _build_priority_queue(con);
    }

    auto execute(){
        int i = 0;
        while (!_eq.empty()){
        // for(int i = 0; i < 5; i++){
            // std::cout << "Step = " << i << " =========== " << std::endl;
            Event event = _eq.top();
            _eq.pop();
            // std::cout <<"This Event = "<< *(event.geo) << ' ' << (event.value) <<" T = " << event.type_to_string() << std::endl;
            if (event.type == Event::_START_){
                auto pair = _set.insert(event.geo);
                ASSERT(pair.second == true);
                auto iter = pair.first;
                check_intersect_with_others(this->_set, iter);
            }else if(event.type == Event::_END_){
                auto iter = _set.find(event.geo);
                if (iter != _set.end()){
                    _set.erase(iter);
                }
            }
            i++;
            // std::cout << "Geo Set size = " << _set.size() << std::endl;
        }
        return _list_res;
    }
protected:
    template<class CONTAINER>
    void _build_priority_queue(const CONTAINER& con){
        for(auto& g : con){
            _insert_event(g, GTag());
        }
    }
    void _insert_event(const GEO& geo, SegmentTag){
        Event es;
        es.geo     = &geo;
        es.value   = MinX(geo);
        es.type    = Event::_START_;
        Event ee;
        ee.geo     = &geo;
        ee.value   = MaxX(geo);
        ee.type    = Event::_END_;
        this->_eq.push(es);
        this->_eq.push(ee);
    }

    void check_intersect_with_others(const GeoSet& s, typename GeoSet::iterator cur_iter){
        for(auto iter = s.begin(); iter != s.end(); ++iter){
            if(iter != cur_iter){
                auto res = check_intersect(*(*(iter)), *(*(cur_iter)), GTag());
                if(res.type != _SS_NO_ && res.type != _SS_INVALID_){
                    _list_res.push_back(res);
                }
            }
        }
    }
    auto check_intersect(const Geo& g1, const Geo& g2, SegmentTag){
        // ProfileStart("Inter2Seg_SL");
        typedef Intersection_<Geo, Geo> Inter;
        Inter inter(g1, g2);
        auto res = inter.execute();
        // ProfileEnd();
        return res;
    }
};

template<class CONTAINER, 
        typename std::enable_if<
                   (! IsGeometry<CONTAINER>::value)
                && IsContainer<CONTAINER>::value
                && IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
auto Intersect(const CONTAINER& con, const std::string& method, SegmentTag){
    // typedef typename CONTAINER::value_type Seg;
    // typedef IntersectionReturn_<Seg> InterRet;
    // typedef std::list<IntersectionReturn_<Seg>> ListInterRet;
    std::string m = ToLowerCase(method);
    if( m == "sweep_line_simple"){
        IntersectionSweepLineSimple_<typename CONTAINER::value_type> inter(con);
        return inter.execute();
    }else if (m == "bentley_ottmann"){
        IntersectionBenOtt_<typename CONTAINER::value_type> inter(con);
        return inter.execute();
    }
    else{
        return IntersectN2(con, SegmentTag());
    }
}
}

#endif