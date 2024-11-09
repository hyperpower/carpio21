#ifndef _BEN_OTT_COMP_SEG_HPP_
#define _BEN_OTT_COMP_SEG_HPP_


#include <memory>
#include <set>
#include <iostream>


namespace carpio {
// Compare Seg at sweep point
// For Status Tree

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

} //namespace


#endif