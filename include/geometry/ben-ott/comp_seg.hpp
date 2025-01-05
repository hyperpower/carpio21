#ifndef _BEN_OTT_COMP_SEG_HPP_
#define _BEN_OTT_COMP_SEG_HPP_

#include <memory>
#include <set>
#include <iostream>
#include "algebra/number/number_define.hpp"
#include "segment_slope.hpp"

#ifdef _DEBUG_MODE_
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "utility/tinyformat.hpp"
DEFINE_HAS_MEMBER(get_name);
#endif
// #define _DEBUG_BEN_OTT_COMP_SEG_ 

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
    typedef SegmentSlope_<SEG>   Slope;

    Point* _ppoint;
    Setcp* _pset;


    CompareSeg_(): _ppoint(nullptr), _pset(nullptr) { };

    CompareSeg_(Point* c) : _ppoint(c), _pset(nullptr){ }; 

    CompareSeg_(Point* c, Setcp* s) : _ppoint(c), _pset(s){ }; 

    bool operator() (cpSegment a, cpSegment b) const{
    #if defined(_DEBUG_MODE_) && false
        std::cout << "CompareSeg ===== " << std::endl;
        std::cout << "&a =" << a << std::endl;
        std::cout << "&b =" << b << std::endl;
        std::cout << "a =" << *a << std::endl;
        std::cout << "b =" << *b << std::endl;
    #endif
        if(_pset != nullptr && !(_pset->empty())){
            auto itera = _pset->find(a);
            if(itera != _pset->end()){     // a in set
                auto iterb = _pset->find(b);
                if(iterb != _pset->end()){ // b in set
                    return this->less_slope(*a, *b);
                }
            }
        }
        bool res =  this->less(*a, *b, *(this->_ppoint)); 
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
            if(py < ly)
                return ly;
            else if(py > ry)
                return ry;
            else
                return py;
        }else {  // normal case
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
    #if defined(_DEBUG_MODE_) && false
        // Gnuplot gnu;
        // if (_debug_flag){
            std::cout << "ay = " << ay << std::endl;
            std::cout << "by = " << by << std::endl;
            // plot_less(gnu, a, b, p_sweep, ay, by);
        // }
    #endif
        if(IsLess(ay, by)){
            // ay < by
    #if defined(_DEBUG_MODE_) && false
        // if (_debug_flag)
            // gnu.plot();
    #endif
            return true;
        }else if(IsLess(by, ay)){
            // ay > by
    #if defined(_DEBUG_MODE_) && false
        // if (_debug_flag)
            // gnu.plot();
    #endif
            return false;
        }else{ // ay == by
    #if defined(_DEBUG_MODE_) && false
        // if (_debug_flag){
            std::cout << "ay == by --> compare slope" << std::endl;
            // gnu.set_label_with_box(3, "by == ay", 0.0, 1.0);
            // gnu.plot();
        // }
    #endif
            return less_slope(a, b);
        }
    }

    bool less_slope(const Segment& a, const Segment& b) const{
        auto sa = a.slope();
        auto sb = b.slope();
    #if defined(_DEBUG_MODE_) && false
        std::cout << " slop a = " << sa << std::endl;
        std::cout << " slop b = " << sb << std::endl;
    #endif
        if(sa < sb){
    #if defined(_DEBUG_MODE_) && false
        std::cout << "sa < sb" << std::endl;
    #endif
            return true;
        }else if(sb < sa){
    #if defined(_DEBUG_MODE_) && false
        std::cout << "sa > sb" << std::endl;
    #endif
            return false;
        }else{
    #if defined(_DEBUG_MODE_) && false
        std::cout << " slop a == b --> compare adress" << std::endl;
        std::cout << " &sa < &sb = " << ToString(&a < &b) << std::endl;
    #endif
            return &a < &b;
        }

    }

#ifdef _DEBUG_MODE_
    bool _debug_flag;
    std::string fig_name;

    typedef typename Segment::Base Seg;

    CompareSeg_(Point* c, Setcp* s, const std::string& name, bool flag = false) :
         _ppoint(c), _pset(s), fig_name(name), _debug_flag(flag){
        };
    
    void plot_less(Gnuplot& gnu,
                   const Segment& a, 
                   const Segment& b, 
                   const Point& p_sweep, double ay, double by) const{
        gnu.set_terminal_png("./fig/" + fig_name);
        auto a_box = a.box();
        auto b_box = b.box();
        auto u_box = Union(a_box, b_box);
        std::vector<double> sa = {1.3, 1.3};
        Scale(u_box, sa, u_box.center());
        gnu.set_xrange(u_box.xmin(), u_box.xmax());
        gnu.set_yrange(u_box.ymin(), u_box.ymax());
        // gnu.set_label(1, strtype, 1.0, 4);
        gnu.add(ToGnuplotActor(a).line_color_red().line_width(3).title("seg a"));
        gnu.add(ToGnuplotActor(b).line_color_blue().line_width(3).title("seg b"));
        gnu.add(ToGnuplotActor(p_sweep)
                 .title("sweep point")
                 .point_type(7)
                 .point_size(3)
                 .line_color_black());
        gnu.add(ToGnuplotActor(Seg(Point(p_sweep.x(), u_box.ymax()),
                                   Point(p_sweep.x(), u_box.ymin())))
                 .dash_type(2)
                 .line_color_black()
                 .line_width(2));
        gnu.set_label_with_box(1, "ay = " + ToString(ay), 0.2 , ay);
        gnu.add(ToGnuplotActor(Seg(Point(u_box.xmin(), ay),
                                   Point(p_sweep.x(),  ay)))
                 .dash_type(3)
                 .line_color_black()
                 .line_width(2));
        gnu.set_label_with_box(2, "by = " + ToString(by), 0.2 , by);
        gnu.add(ToGnuplotActor(Seg(Point(u_box.xmin(), by),
                                   Point(p_sweep.x(),  by)))
                 .dash_type(3)
                 .line_color_black()
                 .line_width(2));
        }
#endif

};

} //namespace


#endif