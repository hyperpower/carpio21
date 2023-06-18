#ifndef _BEN_OTT_STRUCTURE_HPP_
#define _BEN_OTT_STRUCTURE_HPP_
// bentley ottmann 
#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "algebra/number/number_define.hpp"
#include "geometry/objects/objects.hpp"

namespace carpio{

template<class SEG>
struct SegSlope_ {
    typedef SegSlope_<SEG> Self;
    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef typename SEG::coord_value_type Vt;

    static const int _RATIONAL_ = 1;
    static const int _INFINITY_ = 0;
    static const int _INVALID_  = -1;

    int  type; 
    Vt   value;

    SegSlope_():type(_INVALID_){} 

    SegSlope_(const Segment& seg){
        make(seg);
    }

    void make(const Vt& x0,const Vt& y0,
              const Vt& x1,const Vt& y1){
        if(x0 == x1)
            type = _INFINITY_;
        else {
            type = _RATIONAL_;
            value = (y0 - y1) / (x0 - x1) ;
        }
    }

    void make(const Segment& seg){
        auto& left  = seg.p_less_x();
        auto& right = seg.p_greater_x();
        auto xa = left.x();
        auto ya = left.y();
        auto xb = right.x();
        auto yb = right.y();
        make(xa, ya, xb, yb);
    }

    bool operator<(const Self& s) const{
        if (type == _INFINITY_)
            return false;
        else
            return (s.type == _INFINITY_ || value < s.value);
    }
    bool operator==(const Self &rhs) const{
        if(!(this->operator<(rhs)) && !rhs.operator<(*this)){
            return true;
        }else{
            return false;
        }
    }
};

// template<class T ...>
// class SegProxy;

struct SegmentProxyTag: public SegmentTag {};

template<class SEG>
class SegProxy_{
public:
    typedef SegProxy_<SEG> Self;
    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef const Segment* cpSegment;
    typedef typename SEG::coord_value_type coord_value_type;
    typedef SegSlope_<SEG> Slope;

    typedef SegmentProxyTag Tag;

protected:
    cpSegment _cpseg;
    SegSlope_<Segment> _slope;

public:
    SegProxy_(): _cpseg(nullptr){}

    SegProxy_(const Segment& seg):_cpseg(&seg),_slope(seg){
    }
    
    inline auto seg() const{
        return *_cpseg;
    }

    inline auto p(int idx) const{
        return _cpseg->p(idx);
    }

    inline auto& p_less_x() const{
		return _cpseg->p_less_x();
	}
	inline auto& p_less_y() const{
		return _cpseg->p_less_y();
	}
	inline auto& p_greater_x() const{
		return _cpseg->p_greater_x();
	}
	inline auto& p_greater_y() const{
		return _cpseg->p_greater_y();
	}
    inline auto x() const {
        return _cpseg->x();
    }
    inline auto y() const {
        return _cpseg->y();
    }
    inline const Slope& slope() const {
        return this->_slope;
    }    

    inline bool is_vertical() const{
        return _slope.type == Slope::_INFINITY_;
    }

};


template<class SEG>
class SweepEvent_ {
private:
    typedef typename SEG::Point Point;
    typedef typename Point::value_type value_type;
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
        return CompareLess(*_sppoint, *e._sppoint);
    }

    static inline bool CompareLess(const Point& p1, const Point& p2){
        auto dx = p2[0] - p1[0];
        return (dx > 1e-14)
            || ( std::abs(dx) < 1e-14 && p1[1] < p2[1]); 
    }
};


template <class EVENT, class VALUE> 
class SweepEventQueue_ : public std::map<EVENT, 
            std::array<std::set<const VALUE*>, 3> > {
public:
    typedef const VALUE* constp_Value;
    typedef std::map<EVENT, std::list<constp_Value> > Base;
    typedef std::set<constp_Value> Setcp;
    typedef std::array<Setcp, 3>   ArrSetcp;


    void add(const EVENT& event) {
        Setcp set_empty; //empty vector
        this->insert(std::pair<EVENT, ArrSetcp> (event, {set_empty, set_empty, set_empty}));
    }
    void add_event(const EVENT& event, const int& idx, typename Setcp::value_type assoc) {
        auto iter = this->find(event);
        if(iter != this->end()){
            auto res = iter->second[idx].insert(assoc);
        }else{
            Setcp v, set_empty; 
            v.insert(assoc);
            switch (idx){
            case 0:
                this->insert(std::pair<EVENT, ArrSetcp> (event, {v, set_empty, set_empty}));
                break;
            case 1:
                this->insert(std::pair<EVENT, ArrSetcp> (event, {set_empty, v, set_empty}));
                break;
            case 2:
                this->insert(std::pair<EVENT, ArrSetcp> (event, {set_empty, set_empty, v}));
                break;
            }
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
template<class SEG>
struct CompareSeg_ {
    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef const Segment* cpSegment;
    typedef std::set<cpSegment> Setcp;
    typedef typename SEG::coord_value_type Vt;
    typedef SegSlope_<SEG>   Slope;

    Point* _ppoint;
    Setcp* _pset;

    double _e;

    CompareSeg_(): _ppoint(nullptr), _pset(nullptr), _e(1e-14) {};

    CompareSeg_(Point* c) : _ppoint(c), _pset(nullptr), _e(1e-14){ }; 

    CompareSeg_(Point* c, Setcp* s) : _ppoint(c), _pset(s), _e(1e-14){ }; 

    bool operator() (cpSegment a, cpSegment b) const{
        if(_pset != nullptr && !(_pset->empty())){
            auto itera = _pset->find(a);
            if(itera != _pset->end()){
                auto iterb = _pset->find(b);
                if(iterb != _pset->end()){
                    return this->less_slope(*a, *b);
                }
            }
        }
        // ProfileStart("Less");
        bool res =  this->less(*a, *b, *(this->_ppoint)); 
        // ProfileEnd();
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
            
            return (yb - ya)/(xb - xa) * (p.x() - xa) + ya;
        }
    }
    bool less(const Segment& a, const Segment& b, const Point& p_sweep) const{
        // ProfileStart("Y_at_sweep");
        auto ay = y_at_sweep_point(a, p_sweep);
        auto by = y_at_sweep_point(b, p_sweep);
        // ProfileEnd();
        if(std::abs(ay - by) > _e){
            return ay < by;
        }else{
            auto sa = _get_slope(a, typename Segment::Tag());
            auto sb = _get_slope(b, typename Segment::Tag());
            if(sa == sb){
                return &a < &b;
            }else{
                return sa < sb;
            }
        }
    }

    bool less_slope(const Segment& a, const Segment& b) const{
        auto sa = _get_slope(a, typename Segment::Tag());
        auto sb = _get_slope(b, typename Segment::Tag());
        if(sa == sb){
            return &a < &b;
        }else{
            return sa < sb;
        }
    }

    inline auto _get_slope(const Segment& a, SegmentProxyTag) const{
        return a.slope();
    }

    inline auto _get_slope(const Segment& a, SegmentTag) const{
        return Slope(a);
    }
};


}


#endif