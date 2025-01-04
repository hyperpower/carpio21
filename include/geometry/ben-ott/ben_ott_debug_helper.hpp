#ifndef _BEN_OTT_DEBUG_HELPER_HPP_
#define _BEN_OTT_DEBUG_HELPER_HPP_
// bentley ottmann debug helper
#include "geometry/ben-ott/comp_seg.hpp"
#include "geometry/ben-ott/sweep_event.hpp"
#include "geometry/ben-ott/sweep_event_queue.hpp"
#include "geometry/boolean/intersection_two_segments.hpp"
#include "geometry/boolean/boolean.hpp"


#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "utility/tinyformat.hpp"

namespace carpio {

template<class SEG>
class IntersectionBenOtt_;

template<class SEG>
class DebugIntersectionBenOtt_{
public:
    typedef DebugIntersectionBenOtt_<SEG> Self;
    typedef IntersectionBenOtt_<SEG> Target;

    typedef typename SEG::Point Point;
    typedef SEG Segment;
    typedef typename SEG::coord_value_type Vt;
    typedef const Segment* cpSegment;

    typedef SegWithSlope_<SEG> SegProxy;
    typedef const SegProxy* cpSegProxy;
    typedef std::list<SegProxy> ListSegProxy;
    
    typedef SweepEvent_<Point> Event;
    typedef SweepEventQueue_<Event, SegProxy> EventQueue;
    typedef typename EventQueue::Setcp SetcpSeg;

    // typedef std::set<const Geo*> GeoSet;
    typedef CompareSeg_<SegProxy> CompareSeg;
    typedef std::set<cpSegProxy, CompareSeg > StatusTree;
    typedef Intersection_<Segment, Segment> InterTwo;
    typedef IntersectionResult_<Segment, Segment> Result;
    typedef std::list<Result> ListResult;

    typedef SegmentSlope_<Segment> Slope;
    typedef std::list<Segment> ListSegment;
protected:
    ListSegment listseg;
    Segment     diagonal;
    std::string _case_name; // current case name
    int _debug_loop_i; 
    int _loop_i; // current loop i
    
    Gnuplot gnu;

public:
    DebugIntersectionBenOtt_(const std::string& cn){
        _case_name = cn;
        _debug_loop_i = -1;
    }

    void set_debug_case_name(const std::string& dcn){
        _case_name = dcn;
    }
    void set_debug_loop_i(const int& li){
        _debug_loop_i = li;
    }
    void set_case_name(const std::string& cn){
        _case_name = cn;
    }
    void set_loop_i(const int& i){
        _loop_i = i;
    }
    bool is_debug() const{
        return (_case_name == "ALL" || _case_name == _case_name) && 
                (_debug_loop_i == -1 || _loop_i == _debug_loop_i);
    }
    // bool is_debug(const std::string& cn = "ALL", const int& li = -1){
    //     this->_case_name = cn; 
    //     this->_loop_i = li;
    //     return this->is_debug(); 
    // }

    template<class CONTAINER>
    void build_list_segment(const CONTAINER& con){
        if(! con.empty()){
            diagonal = *(con.begin()); 
        }
        for(auto& seg : con){
            diagonal = EnclosureDiagonal(seg, diagonal);
            listseg.push_back(seg);
        }
    }

    void gnu_setup(const std::string& append_name){
        this->gnu_setup(this->gnu, append_name);
    }

    void gnu_setup(Gnuplot& gnu, const std::string& append_name){
        gnu.set_terminal_png("./fig/"+ _case_name +"_"+ ToString(_loop_i)+ "_" + append_name);
        gnu.set_equal_ratio();
        gnu.set_grid();
        auto dx = MaxX(diagonal) - MinX(diagonal);
        auto dy = MaxY(diagonal) - MinY(diagonal);
        gnu.set_xrange(MinX(diagonal) - dx * 0.1, MaxX(diagonal) + dx * 0.1);
        gnu.set_yrange(MinY(diagonal) - dy * 0.1, MaxY(diagonal) + dy * 0.1);
        gnu.set_xlabel("x " + _case_name + " i = " + ToString(_loop_i) + " " + append_name);
    }
    void plot_input_segments(){
        this->gnu_setup(this->gnu,"");
        this->gnu.set_terminal_png("./fig/"+ _case_name + "_input");
        gnu.set_xlabel("x " + _case_name + " input");
        this->add_list_seg(this->listseg);        
        this->gnu.plot();
        this->gnu.clear();
    }

    void plot_result(const ListResult& lres){
        this->gnu_setup(this->gnu,"");
        this->gnu.set_terminal_png("./fig/"+ _case_name + "_result");
        gnu.set_xlabel(_case_name + " result");
        this->add_list_seg(this->listseg);  

        gnu.unset_label();
        int index = 1;
        for(auto r : lres){
            auto p = r.point;
            auto a1 = ToGnuplotActor(p);
            a1.line_color("#202125");
            a1.point_type(7);
            a1.point_size(2);
            gnu.add(a1);
            std::ostringstream sst;
            sst << "right front font \", 18\" textcolor rgb \"#202125\""; 
            std::string l = ToString(index);
            if(r.list.size() > 2){
                l += "s";
            }else{
                l += " ";
            }
            gnu.set_label(index, l, p.x(), p.y(), sst.str());
            index++;
        }
              
        this->gnu.plot();
        this->gnu.clear();

    }

    void plot(){
        this->gnu.plot();
        this->gnu.clear();
    }
    template<class LISTSEG>
    void add_proxy_segments(const LISTSEG& sl){
        this->add_list_seg(sl);
    }
    
    void add_sweep_line(const Point& p){
        auto a = ToGnuplotActor(p);
        a.point_size(2);
        a.point_type(5);
        a.line_color_black();
        auto dy = MaxY(diagonal) - MinY(diagonal);
        Point pup(p[0],  MaxY(diagonal) + dy * 0.1);
        Point plow(p[0], MinY(diagonal) - dy * 0.1);
        Segment seg(plow, pup);
        auto aseg = ToGnuplotActor(seg);
        aseg.dash_type(2);
        aseg.line_color_grey();
        this->gnu.add(aseg);
        this->gnu.add(a);
        auto lowarrow = ToGnuplotActor(plow);
        lowarrow.point_size(2);
        lowarrow.point_type(9);
        lowarrow.line_color_grey();
        this->gnu.add(lowarrow);
        auto uparrow = ToGnuplotActor(pup);
        uparrow.point_size(2);
        uparrow.point_type(11);
        uparrow.line_color_grey();
        this->gnu.add(uparrow);
    }

    template <class LISTSEG>
    void add_list_seg(const LISTSEG& sl, 
                       const std::string &t = "",
                       const std::string &color_code = "#00A4EF"){
        int index = 1;
        for (auto seg : sl){
            auto a = ToGnuplotActor(seg);
            a.style("with points pointtype 7 pointsize 1 lw 3 lc rgb \"#B5B5B5\"");
            auto nv = seg.normal_unit_vector();
            double ratio = 0.5;
            // std::ostringstream sst;
            // sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first "
            // << nv.x() * ratio << ", " << nv.y() * ratio;
            // gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
            auto a1 = ToGnuplotActorAsVector(seg);
            // a1.title("Segment " + seg.get_name());
            a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#B5B5B5\"");
            this->gnu.add(a);
            this->gnu.add(a1);
            index++;
        } 
    }

    template <class LISTSEG>
    void add_list_pseg(const LISTSEG& sl, 
                       const std::string &t = "",
                       const std::string &color_code = "#00A4EF"){
        int i = 0;
        if (sl.size() == 0){
            auto a = ToGnuplotActorAsVector(Segment_<double, 2>(0, 0, 1, 1));
            a.title(t + " n=" + ToString(sl.size()));
            a.style("with vector head filled size screen 0.03,10,155 lw 2 lc rgb \"" + color_code + "\"");
            gnu.add(a);
        }else{
            for (auto seg : sl){
                auto a1 = ToGnuplotActorAsVector(*seg);
                if (i == 0)
                {
                    a1.title(t + " n=" + ToString(sl.size()));
                }
                a1.style("with vector head filled size screen 0.03,10,155 lw 2 lc rgb \"" + color_code + "\"");
                gnu.add(a1);
                i++;
            }
        }
    }

    void add_label_status_tree_size(const StatusTree& status){
        auto maxy = MaxY(diagonal);
        auto miny = MinY(diagonal);
        auto dy = maxy - miny;
        auto maxx = MaxX(diagonal);
        auto minx = MinX(diagonal);
        auto dx = maxx - minx;
        gnu.set_label(3, "Status Tree\\n Size = " + ToString(status.size()), 
                         maxx + dx * 0.11,
                         maxy , 
                        "enhanced");
    }

    void add_label_set_size(const int& size){
        auto maxy = MaxY(diagonal);
        auto miny = MinY(diagonal);
        auto dy = maxy - miny;
        auto maxx = MaxX(diagonal);
        auto minx = MinX(diagonal);
        auto dx = maxx - minx;
        gnu.set_label(1, "Set Size = " + ToString(size), 
                         maxx + dx * 0.11,
                         maxy + dy * 0.09, 
                        "enhanced");
    }

    void add_status_tree(const StatusTree& tree, const Point& sweep){
        // gnu.unset_label();
        int index = 1;
        for(auto seg : tree){
            auto a1 = ToGnuplotActorAsVector(*seg);
            a1.style("with vector head filled size screen 0.03,5,80 lw 2");
            a1.line_color_blue();
            a1.dash_type(2);
            gnu.add(a1);
            auto nv = seg->normal_unit_vector();
            double ratio = 0.2;
            std::ostringstream sst;
            sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
                << nv.x() * ratio << ", " << nv.y() * ratio; 
            gnu.set_label(index * 10, ToString(index), seg->ps().x(), seg->ps().y(), sst.str());
            index++;
        }
    }
};

}

#endif