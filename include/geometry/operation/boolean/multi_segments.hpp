#ifndef _INTERSECTION_MUTISEG_HPP_
#define _INTERSECTION_MUTISEG_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "segment_segment.hpp"
#include "utility/profile.hpp"
#include "utility/avl_tree.hpp"


namespace carpio{

template<class CONTAINER>
auto IntersectN2(const CONTAINER& con, SegmentTag){
    typedef typename CONTAINER::value_type Seg;
    typedef typename Seg::coord_value_type Cvt;
    typedef Intersection_<Seg, Seg> Inter;
    // typedef IntersectionPairSS_<Cvt, Seg::Dim> Inter;

    typedef IntersectionResult_<Seg, Seg> InterRes;
    typedef std::list<InterRes> ListInterRes;
    ListInterRes res;
    for(auto iter = con.begin(); iter != con.end(); ++iter){
        auto& seg1 = *iter;
        for(auto iterin = std::next(iter); iterin != con.end(); ++iterin){
            ProfileStart("IntersectCall");
            auto& seg2 = *iterin;
            Inter inter(seg1, seg2);
            auto t = inter.cal_intersection_type();
            if(t != _SS_NO_){
                InterRes ret(seg1, seg2, t);
                if(t == _SS_INTERSECT_){
                    ret.point = inter.cal_intersection_point();
                }
                res.push_back(ret);
            }
            ProfileEnd();
        }
    }
    return res;
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
                if(res.type != _SS_NO_){
                    _list_res.push_back(res);
                }
            }
        }
    }
    auto check_intersect(const Geo& g1, const Geo& g2, SegmentTag){
        ProfileStart("SIntersectCall");
        typedef Intersection_<Geo, Geo> Inter;
        Inter inter(g1, g2);
        auto t = inter.cal_intersection_type();
        Result ret(g1, g2, t);
        if(t == _SS_INTERSECT_){
            auto p = inter.cal_intersection_point();
            ret.point = p;
        }
        ProfileEnd();
        return ret;
    }
};

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
        const Geo*   geo1;
        const Geo*   geo2;
        int          type;

        Event():p(nullptr), geo1(nullptr), geo2(nullptr),type(-1){
        }

        std::string type_to_string(int t) const{
            switch (t){
            case _START_:
                return "Event_START";
                break;
            case _END_:
                return "Event_END";
                break;
            case _CROSS_:
                return "Event_CROSS";
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
            double avalue = a.p->x() + 1e-5 * a.p->y();            
            double bvalue = b.p->x() + 1e-5 * b.p->y();            
            return avalue >= bvalue;
        }
    };
    typedef typename GEO::Tag GTag;
    typedef std::priority_queue<Event,std::vector<Event>, EventGreatEqual> EventQueue;

    struct GeoComponent{
        typedef SegmentTag Tag;
        const Geo* geo1;
        double     key_value;

        GeoComponent(const Geo& g, SegmentTag){
            geo1 = &g;
            key_value = g[0].y() + 1e-5 * g[0].x();            
        }
        GeoComponent(const Geo* g, SegmentTag){
            geo1 = g;
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
        es.geo1 = &geo;
        es.p   = &(geo[0]);
        es.type = _START_;
        Event ee;
        ee.geo1 = &geo;
        ee.p   = &(geo[1]);
        ee.type = _END_;
        EventGreatEqual comp;
        if (comp(es, ee)){
            es.type = _END_;
            ee.type = _START_;
        }
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

    typedef IntersectionResult_<Geo, Geo> Result;
    typedef std::list<Result> ListResult;

    ListResult _list_res;


    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsGeometry<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionLineSweep_(const CONTAINER& con){
        _build_priority_queue(con);
    }

    void execute(){
        for(int i = 0; i < 5; i++){
            std::cout << "Step = " << i << " =========== " << std::endl;
            Event event = _eventq.top();
            _eventq.pop();
            std::cout <<"This Event = "<< *(event.geo1) << ' ' << *(event.p) <<" T = " << event.type_to_string() << std::endl;
            if (event.type == _START_){
                auto pair = _geo_set.insert(GeoComponent(event.geo1, GTag()));
                ASSERT(pair.second == true);
                auto iter = pair.first;
                check_intersect_with_next(iter);
                check_intersect_with_prev(iter);
            }else if(event.type == _END_){
                GeoComponent gc(event.geo1, GTag());
                auto iter = _geo_set.find(gc);
                if (iter != _geo_set.end()){
                    check_intersect_neighbors(iter);
                    _geo_set.erase(iter);
                }
            }else if(event.type == _CROSS_){
                GeoComponent g1(event.geo1, GTag());
                GeoComponent g2(event.geo2, GTag());
                auto iter1 = _geo_set.find(g1);
                auto iter2 = _geo_set.find(g2);
                if (iter1 != _geo_set.end() && iter2 != _geo_set.end()){
                    _geo_set.erase(iter1);
                    _geo_set.erase(iter2);
                }
                
            }
            std::cout << "Geo Set size = " << _geo_set.size() << std::endl;
        }
    }
    void check_intersect_neighbors(typename GeoSet::iterator iter){
        std::cout << "check neighbors" << std::endl;
        auto next = std::next(iter);
        if(next != _geo_set.end() && iter != _geo_set.begin()){
            auto prev = std::prev(iter);
            auto res = check_intersect(*(prev->geo1), *(next->geo1), GTag());
            if(res.type == _SS_INTERSECT_){
                _list_res.push_back(res);
                Event e;
                e.geo1  = prev->geo1;
                e.geo2  = next->geo1;
                e.p    = &(_list_res.back().point);
                e.type = _CROSS_;
                _eventq.push(e);
            }
            res.show();
        }
    }
    void check_intersect_with_next(typename GeoSet::iterator iter){
        std::cout << "check next" << std::endl;
        auto next = std::next(iter);
        if(next != _geo_set.end()){
            auto res = check_intersect(*(iter->geo1), *(next->geo1), GTag());
            if(res.type == _SS_INTERSECT_){
                _list_res.push_back(res);
                Event e;
                e.geo1  = iter->geo1;
                e.geo2  = next->geo1;
                e.p    = &(_list_res.back().point);
                e.type = _CROSS_;
                _eventq.push(e);
            }
            res.show();
        }
    }
    void check_intersect_with_prev(typename GeoSet::iterator iter){
        std::cout << "check prev" << std::endl;
        if(iter != _geo_set.begin()){
            auto prev = std::prev(iter);
            auto res = check_intersect(*(iter->geo1), *(prev->geo1), GTag());
            if(res.type == _SS_INTERSECT_){
                _list_res.push_back(res);
                Event e;
                e.geo1  = iter->geo1;
                e.geo2  = prev->geo1;
                e.p    = &(_list_res.back().point);
                e.type = _CROSS_;
                _eventq.push(e);
            }
            res.show();
        }
    }
        
            
    auto check_intersect(const Geo& g1, const Geo& g2, SegmentTag){
        typedef Intersection_<Geo, Geo> Inter;
        Inter inter(g1, g2);
        auto t = inter.cal_intersection_type();
        Result ret(g1, g2, t);
        if(t == _SS_INTERSECT_){
            auto p = inter.cal_intersection_point();
            ret.point = p;
        }
        return ret;
    }

    void print_event_queue() { // NB: pass by value so the print uses a copy
        while(!_eventq.empty()) {
            auto top = _eventq.top();
            std::cout << *(top.geo1) << ' ' << *(top.p) << std::endl;
            _eventq.pop();
        }
        std::cout << '\n';
    }

};

template<class GEO>
struct GeoComponent_{
        typedef GEO Geo;
        const Geo* geo1;
        double  sweep_x;
        double  value_y;

        GeoComponent_():geo1(nullptr), sweep_x(0), value_y(0){};

        GeoComponent_(const Geo* geo, double x, double y): 
             geo1(geo), sweep_x(x), value_y(y){};
    };

template<class VALUE>
class GeoStatus_: public AvlTree_<TreeNode_<VALUE,2> >{
public:
    typedef AvlTree_<TreeNode_<VALUE, 2> > Base;
    typedef GeoStatus_<VALUE> Self;

    typedef VALUE value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef typename Base::Node Node;

    typedef Node*        pNode;
    typedef const Node* cpNode;

public:
    GeoStatus_(): Base(){}

    template<class COMPFUN>
    pNode insert(reference value, COMPFUN cfun){
        pNode res = nullptr;
        if(this->empty()){
            this->root(new Node(value));
            return this->root();
        }else{
            res = _insert(this->_end->left_child, value, cfun);
        }
        return res;
    }


    template<class COMPFUN>
    pNode _insert(pNode& cur, reference value, COMPFUN cfun){
        pNode res = nullptr;
        if (cur == nullptr) {
            cur = new Node(value);
            return cur;
        } else if (cfun(value, cur->value)) {  // value < cur-value 
            res = _insert(cur->left_child, value, cfun);
            cur->left_child->father = cur;
            if (!(this->_is_balanced(cur))){
                if (cfun(value, cur->left_child->value)){
                    this->_right_rotate(cur);
                }else{
                    this->_LR_rotate(cur);
                }
            }
        } else if (cfun(cur->value, value)){   // cur->value < value
            res = _insert(cur->right_child, value, cfun);
            cur->right_child->father = cur;
            if (!(this->_is_balanced(cur))){
                if (cfun(cur->right_child->value, value)){
                    this->_left_rotate(cur);
                }else{
                    this->_RL_rotate(cur);
                }
            }
        } else {
            return nullptr;
        }
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
    typedef typename CONTAINER::value_type Seg;
    typedef IntersectionReturn_<Seg> InterRet;
    typedef std::list<IntersectionReturn_<Seg>> ListInterRet;
    std::string m = ToLowerCase(method);
    if( m == "sweep_line_simple"){
        IntersectionSweepLineSimple_<typename CONTAINER::value_type> inter(con);
        return inter.execute();
    }else{
        return IntersectN2(con, SegmentTag());
    }
}
}

#endif