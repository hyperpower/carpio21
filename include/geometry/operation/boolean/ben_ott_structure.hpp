#ifndef _BEN_OTT_STRUCTURE_HPP_
#define _BEN_OTT_STRUCTURE_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "geometry/objects/objects.hpp"

namespace carpio{

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
class SweepEventQueue_ : public std::map<EVENT, 
            std::array<std::list<const VALUE*>, 3> > {
public:
    typedef const VALUE* constp_Value;
    typedef std::map<EVENT, std::list<constp_Value> > Base;
    typedef std::list<constp_Value> Listcp;
    typedef std::array<Listcp, 3>   ArrListcp;

    void push(const EVENT& event) {
        Listcp list; //empty vector
        this->insert(std::pair<EVENT, ArrListcp> (event, {list, list, list}));
    }

    void push_left(const EVENT& event, typename Listcp::value_type assoc) {
        auto iter = this->find(event);
        if(iter != this->end()){
            auto liter = std::find (iter->second[0].begin(), iter->second[0].end(), assoc);
            if(liter == iter->second[0].end()){
                iter->second[0].push_back(assoc);
            }
        }else{
            Listcp v; 
            v.push_back(assoc);
            Listcp emptylist;
            this->insert(std::pair<EVENT, ArrListcp> (event, {v, emptylist, emptylist}));
        }
    }
    void push_right(const EVENT& event, typename Listcp::value_type assoc) {
        auto iter = this->find(event);
        if(iter != this->end()){
            auto liter = std::find (iter->second[2].begin(), iter->second[2].end(), assoc);
            if(liter == iter->second[2].end()){
                iter->second[2].push_back(assoc);
            }
        }else{
            Listcp v; 
            v.push_back(assoc);
            Listcp emptylist;
            this->insert(std::pair<EVENT, ArrListcp> (event, {emptylist, emptylist, v}));
        }
    }
    void push_intersect(const EVENT& event, 
                        typename Listcp::value_type assoc) {
        auto iter = this->find(event);
        if(iter != this->end()){
            auto liter = std::find (iter->second[1].begin(), iter->second[1].end(), assoc);
            if(liter == iter->second[1].end()){
                iter->second[1].push_back(assoc);
            }
        }else{
            Listcp v; 
            v.push_back(assoc);
            Listcp emptylist;
            this->insert(std::pair<EVENT, ArrListcp> (event, {emptylist, v, emptylist}));
        }
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

// StatusTree
template<class TYPE>
struct CompareSeg_ {
    typedef Point_<TYPE, 2> Point;
    typedef Segment_<TYPE, 2> Segment;
    typedef SegSlope_<TYPE>   Slope;

    Point* _ppoint;

    double _e;

    CompareSeg_(): _ppoint(nullptr), _e(1e-14) {};

    CompareSeg_(Point* c) : _ppoint(c), _e(1e-14){ }; 

    bool operator() (const Segment* a, const Segment* b) const{
        return this->operator()((*a), (*b));
    }
    bool operator() (const Segment& a, const Segment& b) const{
        bool res =  this->less(a, b, *(this->_ppoint)); 
        return res;
    }

    auto y_at_sweep_point(const Segment& seg) const{
        return this->y_at_sweep_point(seg, *(this->_ppoint)); 
    }
    auto y_at_sweep_point(const Segment& seg, const Point& p) const{
        if(seg.is_vertical()) { // find max y if vertical
            auto py = p.y();
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
            
            return (yb - ya)/(xb - xa)*(p.x() - xa) + ya;
        }
    }
    bool less(const Segment& a, const Segment& b, const Point& p_sweep) const{
        auto ay = y_at_sweep_point(a, p_sweep);
        auto by = y_at_sweep_point(b, p_sweep);
        if(by - ay > _e){
            return true;
        }else{
            return ( std::abs(ay - by) < _e && (Slope(a) < Slope(b)));
        }
    }
};


}


#endif