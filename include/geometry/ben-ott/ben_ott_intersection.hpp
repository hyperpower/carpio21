#ifndef _BEN_OTT_INTERSECTION_HPP_
#define _BEN_OTT_INTERSECTION_HPP_
// bentley ottmann 
#include "geometry/ben-ott/comp_seg.hpp"
#include "geometry/ben-ott/sweep_event.hpp"
#include "geometry/ben-ott/sweep_event_queue.hpp"
#include "geometry/boolean/boolean_base.hpp"
#include "geometry/boolean/boolean.hpp"

#ifdef _DEBUG_MODE_
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "utility/tinyformat.hpp"
#include "ben_ott_debug_helper.hpp"
#endif

namespace carpio {
#ifdef _DEBUG_MODE_
    
    
    
    template<class SEG>
    void PlotpSegment(Gnuplot& gnu, 
                      const SEG* seg, 
                      const std::string& color_code = "#00A4EF"){
        auto a1 = ToGnuplotActorAsVector(*seg);
        a1.style("with vector head filled size screen 0.03,15,145 lw 3 lc rgb \"" + color_code +"\"");
        gnu.add(a1);
    }
    template<class POINT>
    void PlotNewPoint(Gnuplot& gnu,
                      const POINT& p){
        auto a = ToGnuplotActor(p);
        a.style("with points pointtype 12 pointsize 3 lw 2 lc rgb \"#151516\"");
        gnu.add(a);
    }
#endif

template<class SEG>
class IntersectionBenOtt_{
public:
    typedef IntersectionBenOtt_<SEG> Self;

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
protected:
#ifdef _DEBUG_MODE_
    typedef DebugIntersectionBenOtt_<SEG> Debug;
    typedef std::list<Segment> ListSegment;
    int _loop_i;
    ListSegment listseg;
    Segment     diagonal;
    std::string _case_name;
    std::string _debug_case_name;
    Gnuplot gnu;
    std::ofstream ofs;

    // friend class DebugIntersectionBenOtt_<SEG>;
    Debug* pde;

#endif
    EventQueue queue;
    ListSegProxy listsegproxy;    
public:
    template<class CONTAINER, 
            typename std::enable_if<
                IsContainer<CONTAINER>::value
            &&  IsSegment<typename CONTAINER::value_type>::value 
        , bool>::type = true>
    IntersectionBenOtt_(
        const CONTAINER& con
    #ifdef _DEBUG_MODE_
        ,Debug* p = nullptr
    #endif
    ){
    #ifdef _DEBUG_MODE_
        pde = p;
        pde->build_list_segment(con);
    #endif
        _build_list_segproxy(con);
    #ifdef _DEBUG_MODE_
        _output_seg_with_slope(this->listsegproxy);
    #endif
        _build_priority_queue(this->listsegproxy);
    }

    ~IntersectionBenOtt_(){
    #ifdef _DEBUG_MODE_
        ofs.close();
    #endif
    }

    auto execute(){
    #ifdef _DEBUG_MODE_
        std::cout << "Ben-Ott execute()" << std::endl;
        _loop_i = 0;
    #endif
        ListResult _list_res;
        
        Point p_sweep(0,0);
        SetcpSeg ulc;
        CompareSeg comp(&p_sweep, &ulc);
        StatusTree status(comp);

        while (!queue.empty() 
            #ifdef _DEBUG_MODE_
                // && _loop_i< 6
            #endif
            ){
        #ifdef _DEBUG_MODE_
            std::cout << "loop index = " << _loop_i << std::endl;
            this->pde->set_loop_i(_loop_i);
        #endif
            Event event = queue.top();
            auto& point = event.get_point();
            
        #ifdef _DEBUG_MODE_
            if(this->pde->is_debug()){
                pde->gnu_setup("");
                pde->add_list_seg(listsegproxy);
                pde->add_sweep_line(point);
            }
        #endif

            // HANDLE EVENT POINT(point)
            // 1. Let L(p) be the set of segments whose Left endpoint is p;
            //    these segments are stored with the event point p.
            auto& l_set = queue.begin()->second[0];
            auto& c_set = queue.begin()->second[1];
            auto& r_set = queue.begin()->second[2];

            #ifdef _DEBUG_MODE_
            if(this->pde->is_debug()){
                pde->add_list_pseg(l_set, "left set",   "#FBBC04");
                pde->add_list_pseg(c_set, "center set", "#F35426");
                pde->add_list_pseg(r_set, "right set",  "#81BC06");
            }
            #endif


            //2. Find all segments stored in T that contain p; 
            //   they are adjacent in T. 
            //   Let R(p) denote the subset of segments found whose right endpoint is p, 
            //   and let C(p) denote the subset of segments found that contain p in their interior.
            // auto pair_rc = get_sets(point, status);

            //3. if L(p) and C(p) and R(p) contains more than one segment
            //   then Report P as an intersection, together with L(p) and C(p) and R(p) 

            // ProfileStart("BO_SetSize");
            auto set_size = l_set.size() + c_set.size() + r_set.size();
            #ifdef _DEBUG_MODE_
            if(this->pde->is_debug()){
                std::cout <<"left   Set size  = " << l_set.size() << std::endl;
                std::cout <<"center Set size  = " << c_set.size() << std::endl;
                std::cout <<"right  Set size  = " << r_set.size() << std::endl;
                std::cout <<"  3    Set size  = " << set_size << std::endl;
                std::cout <<"Before Tree size = " << status.size() << std::endl;
                for(auto& k : status){
                    std::cout << k << std::endl;
                }
                // std::cout <<"old ulc = " << ulc.size() << std::endl;
                // for(auto& v : ulc){
                    // std::cout << v << std::endl;
                // }
                this->pde->add_label_set_size(set_size);
            }
            #endif

            if(set_size > 1){
                _new_result(event, _list_res);
            }
            //4. Delete the segents in R(p) and C(p) from T
            for(auto s : r_set){
                std::cout << "delete right seg = " << *s << std::endl;
                auto r = _erase_seg_in_status(status, s);
                if(r == 0)
                    throw std::invalid_argument(ToString(*s) + " should in status tree!");
            }
            for(auto s : c_set){
                auto r = _erase_seg_in_status(status, s);
                if(r == 0)
                    throw std::invalid_argument(ToString(*s) + " should in status tree!");
            }
            #ifdef _DEBUG_MODE_
            if(this->pde->is_debug()){
                std::cout <<"Delete r and c set in tree" << std::endl;
                std::cout <<"After Tree size = " << status.size() << std::endl;
                std::cout <<"Before inster Tree size = " << status.size() << std::endl;
            }
            #endif

            p_sweep.x(point.x());
            p_sweep.y(point.y());

            //5. insert l_set and c_set in status tree
            ulc.clear();  
            std::set_union(l_set.begin(), l_set.end(),
                            c_set.begin(), c_set.end(),
                            std::inserter(ulc, ulc.begin()));
            // auto it_hint = status.begin();
            for(auto s : ulc){
                std::cout << "inster = " << s << std::endl; 
                status.insert(s);
            }
            
            #ifdef _DEBUG_MODE_
            if(this->pde->is_debug()){
                std::cout <<"ulc size = " << ulc.size() << std::endl;
                std::cout <<"After inster Tree size = " << status.size() << std::endl;
                this->pde->add_label_status_tree_size(status);
            }
            #endif
            

            if(ulc.size() == 0){
                cpSegProxy s_a, s_b;
                _find_neighboors(p_sweep, status, s_a, s_b); 
                _compute_new_events(s_a, s_b, event);
            } else {
                auto s_min  = _find_min_slope(ulc);
                auto s_max  = _find_max_slope(ulc);
            #ifdef _DEBUG_MODE_
                if(this->pde->is_debug()){
                if(s_max){
                    std::cout << "s_max = " << s_max->seg() << std::endl;
                }
                if(s_min){
                    std::cout << "s_min = " << s_min->seg() << std::endl;
                }
                }
            #endif
                auto s_lower = _find_lower_neighboor(s_min, status);
            #ifdef _DEBUG_MODE_
                if(this->pde->is_debug()){
                if(s_lower){
                    std::cout << "s_lower = " << s_lower->seg() << std::endl;
                }
                }
            #endif
                auto s_upper = _find_upper_neighboor(s_max, status);

            #ifdef _DEBUG_MODE_
                if(this->pde->is_debug()){
                if(s_upper){
                    std::cout << "s_upper = " << s_upper->seg() << std::endl;
                }
                }
            #endif


                #ifdef _DEBUG_MODE_
                if(this->pde->is_debug()){
                    std::cout << "call min vs lower -> ";
                }
                #endif
                _compute_new_events(s_min, s_lower, event); 
                #ifdef _DEBUG_MODE_
                if(this->pde->is_debug()){
                    std::cout << "call max vs upper -> ";
                }
                #endif
                _compute_new_events(s_max, s_upper, event);
            }
            // ProfileEnd();
            // update res
            auto n_set_size = l_set.size() + c_set.size() + r_set.size();
            if(n_set_size > 1 && n_set_size > set_size ){
                if(set_size > 1){
                    _list_res.pop_back();
                }
                _new_result(event, _list_res);
            }
            
            queue.pop();
            #ifdef _DEBUG_MODE_
                if(this->pde->is_debug()){
                    std::cout << "Tree Size = " << status.size() << std::endl; 
                    pde->add_status_tree(status);
                    // pde->save_plot_cmd();
                    pde->plot();
                }
                std::cout << " ======= end loop ========== " << std::endl;
                std::cout << std::endl;
                _loop_i++;
            #endif
        }
        return _list_res;
    }
    
    std::pair<SetcpSeg, SetcpSeg> get_sets(const Point& p, StatusTree& tree){
        SetcpSeg r, c;
        if(tree.empty()){
            return std::pair<SetcpSeg, SetcpSeg> (r, c);
        }
        

        return std::pair<SetcpSeg, SetcpSeg>(r, c);
    }
#ifndef _DEBUG_MODE_
protected:
#endif
    IntersectionBenOtt_(){}

    void _new_result(const Event& current, ListResult& list){
        Result res(current.get_point());

        auto& l_set = queue.begin()->second[0];
        auto& c_set = queue.begin()->second[1];
        auto& r_set = queue.begin()->second[2];

        for(auto s : l_set){
            res.add(s->cpseg());
        }
        for(auto s : c_set){
            res.add(s->cpseg());
        }
        for(auto s : r_set){
            res.add(s->cpseg());
        }
                
        list.emplace_back(res);
    }

    template<class CONTAINER>
    void _build_priority_queue(const CONTAINER& con){
        for(auto& seg : con){
            Event e_left(seg.ps());
            Event e_right(seg.pe());
            queue.add_event(e_left,  0, &(seg)); //idx = 0 point start (left)
            queue.add_event(e_right, 2, &(seg)); //idx = 2 point end (right)
        }
    }

    template<class CONTAINER>
    void _build_list_segproxy(const CONTAINER& con){
        for(auto& seg : con){
            this->listsegproxy.emplace_back(SegProxy(seg));
        }
    }

    void _find_neighboors(const Point& p, 
                          const StatusTree& tree,
                          cpSegProxy& above, cpSegProxy& below) {
        //create a segment of length 0 representing p :
        Segment s(p.x(), p.x(), p.y(), p.y());
        SegProxy sp(s);
        
        //search for upper neighboor
        auto iter = tree.upper_bound(&sp);
        
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

    bool _erase_seg_in_status(StatusTree& tree, const cpSegProxy& cps) const{
        auto removed = tree.erase(cps);
        if (removed == 0){
            for(auto iter = tree.begin(); iter!= tree.end(); iter++){
                if(*iter == cps){
                    tree.erase(iter);
                    removed = 1;
                    break;
                }
            }
        }
        return removed;
    }

    cpSegProxy _find_lower_neighboor(cpSegProxy cps, const StatusTree& tree) const{
        auto iter = tree.find(cps);
        if(iter == tree.end()){
            throw std::invalid_argument(ToString(cps->seg()) + " should in status tree!");
        }
        if(iter == tree.begin()){
            return nullptr;
        }else{
            std::advance(iter, -1); 
            return *iter;
        }
    }
    cpSegProxy _find_upper_neighboor(cpSegProxy s, const StatusTree& tree) const{
        auto iter = tree.find(s);
        if(iter == tree.end()){
            throw std::invalid_argument(ToString(s->seg()) + " should in status tree!");
        }
        if(++iter == tree.end())
            return nullptr;
        else
            return *iter;
    }

    cpSegProxy _find_min_slope(const SetcpSeg& v) const{
        auto it = v.begin();
        auto min = *(it);
      
        if(v.size() == 1){
            return min;
        }

        while(++it != v.end()) {
            if((*it)->slope() < min->slope())
                min = *it;
        }
        return min;
    }
    cpSegProxy _find_max_slope(const SetcpSeg& v) const{
        auto it = v.begin();
        auto max = *(it);

        if(v.size() == 1){
            return max;
        }

        while(++it != v.end()) {
            if(max->slope() < (*it)->slope())
                max = *it;
        }
        return max;
    }
    
    void _compute_new_events(cpSegProxy s0, cpSegProxy s1, const Event &current){
        if (s0 != nullptr && s1 != nullptr){
            InterTwo inter(s0->seg(), s1->seg());
            auto res = inter.execute();
            #ifdef _DEBUG_MODE_
            if(pde->is_debug()){
                std::cout << "new intersect " << ToString(res.type) << std::endl;
            }
            #endif
            if (IsLessEqual(current.get_point(), res.point) ){
                if(res.type == _SS_INTERSECT_){
                    Event ev_i(res.point);
                    auto iter = queue.find(ev_i);
                    if(iter == queue.end()){
                        typename EventQueue::Setcp v, set_empty; 
                        v.insert(s0);
                        v.insert(s1);
                        queue.insert(std::pair<Event, typename EventQueue::ArrSetcp> (ev_i, {set_empty, v, set_empty}));
                    }
                }else if(res.type == _SS_TOUCH_ || res.type == _SS_OVERLAP_){
                    #ifdef _DEBUG_MODE_
                    if(pde->is_debug()){
                        std::cout << " touch or overlap" << std::endl;
                    }
                    #endif
                    for(short i = 0 ; i < 2; i++){
                        auto pos = inter.point_position(i);
                        if(pos == _PS_IN_){
                            queue.add_event(Event(s0->cpseg()->p(i)), 1, s1);
                            #ifdef _DEBUG_MODE_
                            if(pde->is_debug()){
                                PlotNewPoint(gnu, s0->cpseg()->p(i));
                            }
                            #endif
                        }
                    }
                    for(short i = 2; i < 4; i++){
                        auto pos = inter.point_position(i);
                        if(pos == _PS_IN_){
                            queue.add_event(Event(s1->cpseg()->p(i-2)), 1, s0);
                            // break;
                            #ifdef _DEBUG_MODE_
                            if(pde->is_debug()){
                                std::cout << s1->p(i-2) << std::endl;
                                // PlotNewPoint(gnu, s1->cpseg()->p(i-2));
                            }
                            #endif
                        }
                    }
                }
            }
            #ifdef _DEBUG_MODE_
            else if(pde->is_debug()){
                std::cout << " oo " << std::endl;
            }
            #endif
        }
        #ifdef _DEBUG_MODE_
        else if(pde->is_debug()){
            std::cout << " xx " << std::endl;
        }
        #endif
    }
#ifdef _DEBUG_MODE_
    
    template<class CONTAINER> 
    void _output_list_segment(const CONTAINER& con){
        std::ofstream ofstream;
        ofstream.open("./data/debug_output_" + _case_name + ".txt");
        tfm::format(ofstream, "## Input Segments\n");
        tfm::format(ofstream, "%5s, %5s, %15s, %15s, %15s, %15s\n", 
            "Order", "Name", "start X", "start Y", "end X", "end Y");
        int count = 0;
        for(auto& seg : con){
            _output_a_seg(ofstream, seg, count);
            count++;
        }
        ofs.close();
    }
    template<class SEGMENT, 
             typename std::enable_if<
                Has_get_name<SEGMENT, std::string(void)>::value, 
                bool>::type = true
             >
    void _output_a_seg(std::ofstream& ofs, const SEGMENT& seg, int count){
        tfm::format(ofs, "%5d, %5s, %15.4f, %15.4f, %15.4f, %15.4f\n",
                count, seg.get_name(), seg.psx(), seg.psy(), seg.pex(), seg.pey());
    }

    template<class SEGMENT, 
             typename std::enable_if<
                ! Has_get_name<SEGMENT, std::string(void)>::value, 
                bool>::type = true
             >
    void _output_a_seg(std::ofstream& ofs, const SEGMENT& seg, int count){
        tfm::format(ofs, "%5d, %15.4f, %15.4f, %15.4f, %15.4f\n",
                count, seg.psx(), seg.psy(), seg.pex(), seg.pey());
    }

    template<class CONTAINER>
    void _output_seg_with_slope(const CONTAINER& con){
       std::ofstream ofstream;
        ofstream.open("./data/debug_output_" + _case_name + "_slope.txt");
        tfm::format(ofstream, "## Segments with slope\n");
        tfm::format(ofstream, "%5s, %5s, %15s\n", 
            "Order", "Name", "Slope");
        int count = 0;
        for(auto& seg : con){
            if constexpr (Has_get_name<decltype(seg), std::string(void)>::value){
            tfm::format(ofstream, "%5d, %10s, %15.8f\n",
                count, seg.get_name(), seg.slope_value());
            }else{
            tfm::format(ofstream, "%5d, %15.8f\n",
                count,  seg.slope_value());
            }
            count++;
        }
        ofs.close(); 
    }
#endif

};


}

#endif