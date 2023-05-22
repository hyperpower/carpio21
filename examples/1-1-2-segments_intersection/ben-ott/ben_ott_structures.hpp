#ifndef _BEN_OPOINTPOINT_SPOINTRUCPOINTURES_HPP_
#define _BEN_OPOINTPOINT_SPOINTRUCPOINTURES_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "geometry/operation/boolean/segment_segment.hpp"


namespace carpio{

template<class TYPE>
class SweepEvent_ {
private:
    typedef Point_<TYPE, 2> Point;
    typedef std::shared_ptr<Point> spObj;
    spObj _sppoint; 
public:
    SweepEvent_() : _sppoint() {};

    SweepEvent_(const SweepEvent_& e) : _sppoint(e._sppoint) {};

    SweepEvent_(const Point& o) : _sppoint(std::make_shared<Point>(o)) {};

    const Point& get_point() const{
        return *_sppoint;
    }
    bool operator<(const SweepEvent_& e) const{
        return (
            (_sppoint->x() < e._sppoint->x())
            || (   (_sppoint->x() == e._sppoint->x())
                && (_sppoint->y()  < e._sppoint->y()))
            );
    }
};

template <class EVENT, class VALUE> 
class SweepEventQueue_ : public std::map<EVENT, std::list<const VALUE*> > {
public:
    typedef const VALUE* constp_Value;
    typedef std::map<EVENT, std::list<constp_Value> > Base;
    typedef std::list<constp_Value> List;

    void push(const EVENT& event) {
        List list; //empty vector
        this->insert(std::pair<EVENT, List> (event, list));
    }

    void push(const EVENT& event, typename List::value_type assoc) {
        List v; 
        v.push_back(assoc);
        std::pair<typename Base::iterator, bool> ret =
            this->insert(std::pair<EVENT, List > (event, v));
        if(false == ret.second){
            ret.first->second.push_back(assoc);
        }
        return;
    }

    EVENT top() {
        return this->begin()->first;
    }

    void pop() {
        this->erase(this->begin());
    }

    bool mem(const EVENT& value) {
        return (this->find(value) != this->end());
    }
};

template<class TYPE>
struct SegSlope_ {
    typedef Point_<TYPE, 2> Point;
    typedef Segment_<TYPE, 2> Segment;

    static const int _RATIONAL_ = 1;
    static const int _INFINITY_ = 0;

    int  type; 
    TYPE value; 

    SegSlope_(const Segment& seg){
        make(seg);
    }

    void make(const TYPE& x0,const TYPE& y0,
              const TYPE& x1,const TYPE& y1){
        if(x0 == x1)
            type = _INFINITY_;
        else {
            type = _RATIONAL_;
            value = (y0 - y1) / (x0 - x1) ;
        }
    }

    void make(const Segment& seg){
        auto &left = seg.p_less_x();
        auto &right = seg.p_greater_x();
        auto xa = left.x();
        auto ya = left.y();
        auto xb = right.x();
        auto yb = right.y();
        make(xa, ya, xb, yb);
    }

    bool operator<(const SegSlope_<TYPE>& s) const{
        if (type == _INFINITY_)
            return false;
        else
            return (s.type == _INFINITY_ || value < s.value);
    }
};
// StatusTree
template<class TYPE>
struct CompareSeg_ {
    typedef Point_<TYPE, 2> Point;
    typedef Segment_<TYPE, 2> Segment;
    typedef SegSlope_<TYPE>   Slope;

    Point* _ppoint;

    CompareSeg_(Point* c) : _ppoint(c) { }; 

    bool operator() (const Segment* a, const Segment* b) const{
        return this->operator()((*a), (*b));
    }
    bool operator() (const Segment& a, const Segment& b) const{
        // return a->less(*b,*_ppoint); 
        auto ay = y_at_sweep_point(a);
        auto by = y_at_sweep_point(b);
        auto sa = Slope(a);
        auto sb = Slope(b);

        return (ay < by || (ay == by && (sa < sb)));
    }

    auto y_at_sweep_point(const Segment& seg) const{
        if(seg.is_vertical()) { // find max y if vertical
            auto py = _ppoint->y();
            auto ly = seg.p_less_x().y();
            auto ry = seg.p_greater_x().y();
            if(py < ly) // ?
                return ly;
            else if(py > ry)
                return ry;
            else
                return py;
        }else {
            auto& left  = seg.p_less_x();
            auto& right = seg.p_greater_x();
            auto xa = left.x();
            auto ya = left.y();
            auto xb = right.x();
            auto yb = right.y();
            
            return (yb - ya)/(xb - xa)*(_ppoint->x() - xa) + ya;
        }
    }
};


template<class TYPE>
class IntersectionBenOtt_{
public:
    typedef IntersectionBenOtt_<TYPE> Self;

    typedef Point_<TYPE, 2>   Point;
    typedef Segment_<TYPE, 2> Segment;
    
    typedef SweepEvent_<TYPE> Event;
    typedef SweepEventQueue_<Event, Segment> EventQueue;
    typedef typename EventQueue::List List;

    // typedef std::set<const Geo*> GeoSet;
    typedef CompareSeg_<TYPE> CompareSeg;
    typedef std::set<const Segment*, CompareSeg > StatusTree;
    typedef IntersectionResult_<Segment, Segment> Result;
    typedef std::list<Result> ListResult;
protected:
    EventQueue queue;
public:
    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsSegment<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionBenOtt_(const CONTAINER& con){
        _build_priority_queue(con);
    }

    auto execute(){
        ListResult _list_res;
        
        Point p_sweep(0,0);
        CompareSeg comp(&p_sweep);
        StatusTree status(comp);

        int i = 0;
        while (!queue.empty()){
        // for(int i = 0; i < 5; i++){
            // std::cout << "Step = " << i << " =========== " << std::endl;
            std::cout << " ------------- " << std::endl;
            // current event : p event on top of set
            std::cout << "queue size = " << queue.size()<< std::endl;
            Event event = queue.top();
            auto& point = event.get_point();
            std::cout << "current point = "<< point << std::endl;
            p_sweep.x(point.x());
            p_sweep.y(point.y());

            // HANDLE EVENT POINT(point)
            // 1. Let L(p) be the set of segments whose Left endpoint is p;
            //    these segments are stored with the event point p.
            auto& l_set = queue.begin()->second;    
            queue.pop();

            //2. Find all segments stored in T that contain p; 
            //   they are adjacent in T. 
            //   Let R(p) denote the subset of segments found whose right endpoint is p, 
            //   and let C(p) denote the subset of segments found that contain p in their interior.
            auto pair_rc = get_sets(p_sweep, status);

            //3. if L(p) and C(p) and R(p) contains more than one segment
            //   then Report P as an intersection, together with L(p) and C(p) and R(p) 
            if(l_set.size() + pair_rc.first.size() + pair_rc.second.size() > 1){
                Result res(point);
                
                for(auto s : l_set){
                    res.add(*s);
                }
                for(auto s : pair_rc.first){
                    res.add(*s);
                }
                for(auto s : pair_rc.second){
                    res.add(*s);
                }
                
                _list_res.emplace_back(res);
            }

            //4. Delete the segments in R(p) and C(p) from T




            i++;
            // std::cout << "Geo Set size = " << _set.size() << std::endl;
        }
        return _list_res;
    }

    std::pair<List, List> get_sets(const Point& p, StatusTree& tree){
        List r, c;
        if(tree.empty()){
            return std::pair<List, List> (r, c);
        }
        auto x = p.x();
        auto y = p.y();
        Segment s(x, x, y, y);

        auto it  = tree.upper_bound(&s);
        auto rit = std::reverse_iterator(it);

        while(rit != tree.rend()) { // p is on segment
            auto ps = (*rit)->p_less_x();
            auto pe = (*rit)->p_greater_x();
            auto flag = OnWhichSide7(ps, pe, p);
            if(flag == _PS_ON_END_){
                r.push_back(*rit);
            }else if(flag == _PS_IN_){
                c.push_back(*rit);
            }
            rit++;
        }

        return std::pair<List, List>(r, c);
    }
protected:
    template<class CONTAINER>
    void _build_priority_queue(const CONTAINER& con){
        for(auto& seg : con){
            Event e_left(seg.p_less_x());
            Event e_right(seg.p_greater_x());
            queue.push(e_left, &(seg));
            queue.push(e_right);  
        }
    }

};
}

#endif