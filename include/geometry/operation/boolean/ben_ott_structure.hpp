#ifndef _BEN_OTT_STRUCTURE_HPP_
#define _BEN_OTT_STRUCTURE_HPP_
// bentley ottmann 
#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "algebra/number/number_define.hpp"
#include "geometry/objects/objects.hpp"
#ifdef _DEBUG_MODE_
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "utility/tinyformat.hpp"
#endif
namespace carpio{


template<class P>
inline bool CompareLess(const P& p1, const P& p2, double tol = 1e-14){
        auto dx = p2[0] - p1[0];
        return (dx > tol)
            || ( std::abs(dx) <= tol && p1[1] < p2[1]); 
}
template<class P>
inline bool CompareLessEqual(const P& p1, const P& p2, double tol = 1e-14){
        auto dx = p2[0] - p1[0];
        return (dx > tol)
            || ( std::abs(dx) <= tol && p1[1] <= p2[1]); 
}
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

    // SegSlope_(const Segment& seg){
    //     make(seg);
    // }
    SegSlope_(const Point& ps, const Point& pe){
        make(ps[0], ps[1], pe[0], pe[1]);
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

    // void make(const Segment& seg){
    //     auto& left  = seg.p_less_x();
    //     auto& right = seg.p_greater_x();
    //     auto xa = left.x();
    //     auto ya = left.y();
    //     auto xb = right.x();
    //     auto yb = right.y();
    //     make(xa, ya, xb, yb);
    // }

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


struct SegmentWithSlopeTag: public SegmentTag {};

template<class SEG>
class SegWithSlope_:public SEG{
public:
	static const St Dim = SEG::Dim;
    typedef SEG Base;
    typedef SegWithSlope_<SEG> Self;
    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef const Segment* cpSegment;
    typedef typename SEG::coord_value_type coord_value_type;
    typedef SegSlope_<SEG> Slope;

    typedef SegmentWithSlopeTag Tag;

protected:
    cpSegment _cpseg;
    Slope     _slope;

public:
    SegWithSlope_(): _cpseg(nullptr){}

    SegWithSlope_(const Segment& seg):
        Base(seg), _cpseg(&seg){
        if(CompareLess(Base::pe(), Base::ps())){
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
        return _slope.type == Slope::_INFINITY_;
    }
};


template<class SEG>
class SweepEvent_ {
private:
    typedef typename SEG::Point Point;
    typedef typename Point::value_type value_type;
    typedef std::shared_ptr<Point> spPoint;
    spPoint _sppoint; 
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
        Setcp set_empty;  //empty vector
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

    #ifdef _DEBUG_MODE_
    void output(std::ofstream& ofs) const{

    }
    #endif
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
            auto ly = seg.psy();
            auto ry = seg.pey();
            if(py < ly) // ?
                return ly;
            else if(py > ry)
                return ry;
            else
                return py;
        }else {
            auto& left  = seg.ps();
            auto& right = seg.pe();
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
            auto sa = a.slope();
            auto sb = b.slope();
            if(sa == sb){
                return &a < &b;
            }else{
                return sa < sb;
            }
        }
    }

    bool less_slope(const Segment& a, const Segment& b) const{
        auto sa = a.slope();
        auto sb = b.slope();
        if(sa == sb){
            return &a < &b;
        }else{
            return sa < sb;
        }
    }

   
};


}


#endif