#ifndef _BEN_OTT_INTERSECTION_HPP_
#define _BEN_OTT_INTERSECTION_HPP_

#include "geometry/operation/boolean/ben_ott_structure.hpp"
#include "geometry/operation/boolean/segment_segment.hpp"

#ifdef _DEBUG_MODE_
#include "geometry/io/ggnuplot_actor_maker.hpp"
#endif

namespace carpio {
#ifdef _DEBUG_MODE_
    template<class TYPE>
    void PlotSweepLine(Gnuplot& gnu, const Point_<TYPE, 2>& p){
        typedef Point_<TYPE, 2>   Point;
        typedef Segment_<TYPE, 2> Segment;
        auto a = ToGnuplotActor(p);
        a.point_size(2);
        Point pup(p[0], p[1] + 10);
        Point plow(p[0], p[1] - 10);
        Segment seg(plow, pup);
        auto aseg = ToGnuplotActor(seg);

        gnu.add(a);
        gnu.add(aseg);
    }
    template<class LISTSEG>
    void PlotListSegment(Gnuplot& gnu, const LISTSEG& sl){
        // gnu.set_label(1, strtype, -4.5, 4);
        int index = 1;
        for(auto seg : sl){
            auto a = ToGnuplotActor(seg);
            a.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
            auto nv = seg.normal_unit_vector();
            double ratio = 0.5;
            // std::ostringstream sst;
            // sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
                // << nv.x() * ratio << ", " << nv.y() * ratio; 
            // gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
            auto a1 = ToGnuplotActorAsVector(seg);
            // a1.title("Segment " + seg.get_name());
            a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
            gnu.add(a);
            gnu.add(a1);
            index++;
        }
    }
    template<class LISTSEG>
    void PlotListpSegment(Gnuplot& gnu, 
                          const LISTSEG& sl, 
                          const std::string& color_code = "#00A4EF"){
    for(auto seg : sl){
        auto a1 = ToGnuplotActorAsVector(*seg);
        a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"" + color_code +"\"");
        gnu.add(a1);
        }
    }

#endif
template<class TYPE>
class IntersectionBenOtt_{
public:
    typedef IntersectionBenOtt_<TYPE> Self;

    typedef Point_<TYPE, 2>   Point;
    typedef Segment_<TYPE, 2> Segment;
    typedef const Segment* cpSegment;
    
    typedef SweepEvent_<TYPE> Event;
    typedef SweepEventQueue_<Event, Segment> EventQueue;
    typedef typename EventQueue::Listcp ListcpSeg;

    // typedef std::set<const Geo*> GeoSet;
    typedef CompareSeg_<TYPE> CompareSeg;
    typedef std::set<const Segment*, CompareSeg > StatusTree;
    typedef Intersection_<Segment, Segment> InterTwo;
    typedef IntersectionResult_<Segment, Segment> Result;
    typedef std::list<Result> ListResult;

    typedef SegSlope_<TYPE> Slope;
protected:
#ifdef _DEBUG_MODE_
    typedef std::list<Segment> ListSegment;
    ListSegment listseg;
    Segment diagonal;
#endif
    EventQueue queue;
public:
    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsSegment<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionBenOtt_(const CONTAINER& con){
        _build_priority_queue(con);
    #ifdef _DEBUG_MODE_
        _build_list_segment(con);
    #endif
    }

    auto execute(){
        #ifdef _DEBUG_MODE_
        Gnuplot gnu;
        #endif
        ListResult _list_res;
        
        Point p_sweep(0,0);
        CompareSeg comp(&p_sweep);
        StatusTree status(comp);

        int i = 0;
        while (!queue.empty()){
            #ifdef _DEBUG_MODE_
            std::cout << "loop index = " << i << std::endl;

            gnu.set_terminal_png("./fig/" + ToString(i));
            auto dx = MaxX(diagonal) - MinX(diagonal);
            auto dy = MaxY(diagonal) - MinY(diagonal);
            gnu.set_xrange(MinX(diagonal) - dx * 0.1, MaxX(diagonal) + dx * 0.1);
            gnu.set_yrange(MinY(diagonal) - dy * 0.1, MaxY(diagonal) + dy * 0.1);
            PlotListSegment(gnu, this->listseg);
            #endif

            Event event = queue.top();
            auto& point = event.get_point();
            
            #ifdef _DEBUG_MODE_
            PlotSweepLine(gnu, point);
            #endif

            // HANDLE EVENT POINT(point)
            // 1. Let L(p) be the set of segments whose Left endpoint is p;
            //    these segments are stored with the event point p.
            auto& l_set = queue.begin()->second[0];
            auto& c_set = queue.begin()->second[1];
            auto& r_set = queue.begin()->second[2];

            #ifdef _DEBUG_MODE_
            PlotListpSegment(gnu, l_set, "#FBBC04" );
            #endif


            //2. Find all segments stored in T that contain p; 
            //   they are adjacent in T. 
            //   Let R(p) denote the subset of segments found whose right endpoint is p, 
            //   and let C(p) denote the subset of segments found that contain p in their interior.
            // auto pair_rc = get_sets(point, status);

            //3. if L(p) and C(p) and R(p) contains more than one segment
            //   then Report P as an intersection, together with L(p) and C(p) and R(p) 
            #ifdef _DEBUG_MODE_
            // PlotListpSegment(gnu, pair_rc.first, "#34A853");
            #endif
            if(l_set.size() + c_set.size() + r_set.size() > 1){
                Result res(point);
                
                for(auto s : l_set){
                    res.add(*s);
                }
                for(auto s : c_set){
                    res.add(*s);
                }
                for(auto s : r_set){
                    res.add(*s);
                }
                
                _list_res.emplace_back(res);
            }
            //4. Delete the segments in R(p) and C(p) from T
            for(auto s : r_set){
                status.erase(s);
            }
            for(auto s : c_set){
                status.erase(s);
            }
            p_sweep.x(point.x());
            p_sweep.y(point.y());
            //5. insert l_set and c_set in status tree
            for(auto s : l_set){
                status.insert(s);
            }
            for(auto s : c_set){
                status.insert(s);
            }

            if(l_set.size() + c_set.size() == 0){
                cpSegment s_a, s_b;
                _find_neighboors(p_sweep, status, s_a, s_b); 
                _compute_new_events(s_a, s_b, event);
            } else {
                ListcpSeg lunion;
                std::set_union(l_set.begin(), l_set.end(),
                               c_set.begin(), c_set.end(),
                               std::inserter(lunion, lunion.begin()));
                std::cout << "union size = " << lunion.size() << std::endl;
                cpSegment s_min  = _find_min_slope(lunion);
                if(s_min){
                    std::cout << "s_min   = " << *s_min << std::endl;
                }
                cpSegment s_max  = _find_max_slope(lunion);
                if(s_max){
                    std::cout << "s_max   = " << *s_max << std::endl;
                }
                cpSegment s_lower = _find_lower_neighboor(s_min, status);
                if(s_lower){
                    std::cout << "s_lower = " << *s_lower << std::endl;
                }
                cpSegment s_upper = _find_upper_neighboor(s_max, status);
                if(s_upper){
                    std::cout << "s_upper = " << *s_upper << std::endl;
                }
                
                _compute_new_events(s_min, s_lower, event); 
                _compute_new_events(s_max, s_upper, event);
            }

            queue.pop();
            #ifdef _DEBUG_MODE_
            this->_plot_status_tree(gnu, status, p_sweep);
            this->_plot_res_points(gnu, _list_res);
            gnu.plot();
            gnu.clear();
            std::cout << " end loop ========== " << std::endl;
            #endif
            i++;
            // std::cout << "Geo Set size = " << _set.size() << std::endl;
        }
        return _list_res;
    }
    
    std::pair<ListcpSeg, ListcpSeg> get_sets(const Point& p, StatusTree& tree){
        ListcpSeg r, c;
        if(tree.empty()){
            return std::pair<ListcpSeg, ListcpSeg> (r, c);
        }
        

        return std::pair<ListcpSeg, ListcpSeg>(r, c);
    }
#ifndef _DEBUG_MODE_
protected:
#endif
    IntersectionBenOtt_(){}


    template<class CONTAINER>
    void _build_priority_queue(const CONTAINER& con){
        for(auto& seg : con){
            Event e_left(seg.p_less_x());
            Event e_right(seg.p_greater_x());
            queue.push_left(e_left, &(seg));
            queue.push_right(e_right, &(seg));  
        }
    }
    void _find_neighboors(const Point& p, 
                          const StatusTree& tree,
                          cpSegment& above, cpSegment& below) {
        //create a segment of length 0 representing p :
        Segment s(p.x(), p.x(), p.y(), p.y());
        
        //search for upper neighboor
        auto iter = tree.upper_bound(&s);
        
        if(iter == tree.end()){
            above = nullptr;
        }else{
            above = *iter;
        }

        if(iter == tree.begin()){
            below = nullptr;
        }else{ 
            below = *--iter;
        }
    }

    cpSegment _find_lower_neighboor(cpSegment s, const StatusTree& tree) const{
        auto iter = tree.find(s);
        if(iter == tree.end()){
            throw std::invalid_argument(ToString(*s) + " should in tree!");
        }
        if(iter == tree.begin())
            return nullptr;
        else
            return *--iter;
    }
    cpSegment _find_upper_neighboor(cpSegment s, const StatusTree& tree) const{
        auto iter = tree.find(s);
        if(iter == tree.end()){
            throw std::invalid_argument(ToString(*s) + " should in tree!");
        }
        if(++iter == tree.end())
            return nullptr;
        else
            return *iter;
    }

    cpSegment _find_min_slope(const ListcpSeg& v) const{
        auto it = v.begin();
        auto min = *(v.begin());

        while(++it != v.end()) {
            if(Slope(*(*it)) < Slope(*min))
                min = *it;
        }
        return min;
    }
    cpSegment _find_max_slope(const ListcpSeg& v) const{
        auto it = v.begin();
        auto max = *(v.begin());

        while(++it != v.end()) {
            if(Slope(*max) < Slope(**it))
                max = *it;
        }
        return max;
    }
    
    void _compute_new_events(cpSegment s0, cpSegment s1, const Event &current){
        if (s0 && s1){
            InterTwo inter(*s0, *s1);
            auto res = inter.execute();
            if(res.type == _SS_INTERSECT_){
                Event ev_i(res.point);
                if (current < ev_i && !queue.mem(ev_i)){
                    queue.push_intersect(ev_i, s0);
                    queue.push_intersect(ev_i, s1);
                }
            }
        }
    }
#ifdef _DEBUG_MODE_
    template<class CONTAINER>
    void _build_list_segment(const CONTAINER& con){
        if(! con.empty()){
           diagonal = *(con.begin()); 
        }
        for(auto& seg : con){
            diagonal = EnclosureDiagonal(diagonal, seg);
            listseg.push_back(seg);
        }
    }
    void _plot_status_tree(Gnuplot& gnu, const StatusTree& tree, const Point& sweep){
        gnu.unset_label();
        int index = 1;
        for(auto seg : tree){
            auto a1 = ToGnuplotActorAsVector(*seg);
            std::string color_code = "#EA8982" ;
            a1.style("with vector head filled size screen 0.03,5,80 lw 2 lc rgb \"" + color_code +"\"");
            gnu.add(a1);
            auto nv = seg->normal_unit_vector();
            double ratio = 0.5;
            std::ostringstream sst;
            sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
                << nv.x() * ratio << ", " << nv.y() * ratio; 
            gnu.set_label(index, ToString(index), seg->pc().x(), seg->pc().y(), sst.str());
            index++;
        }
    }
    void _plot_res_points(Gnuplot& gnu, const ListResult& list){
        gnu.unset_label();
        int index = 1;
        for(auto res : list){
            auto p = res.point;
            auto a1 = ToGnuplotActor(p);
            a1.line_color("#202125");
            a1.point_type(4);
            a1.point_size(2);
            // a1.style("with vector head filled size screen 0.03,5,80 lw 2 lc rgb \"" + color_code +"\"");
            gnu.add(a1);
            std::ostringstream sst;
            sst << "right front font \", 18\" textcolor rgb \"#202125\""; 
            gnu.set_label(index, ToString(index), p.x(), p.y(), sst.str());
            index++;
        }
    }
    #endif

};


}

#endif