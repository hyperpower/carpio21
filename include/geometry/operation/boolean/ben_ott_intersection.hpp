#ifndef _BEN_OTT_INTERSECTION_HPP_
#define _BEN_OTT_INTERSECTION_HPP_
// bentley ottmann 
#include "geometry/operation/boolean/ben_ott_structure.hpp"
#include "geometry/operation/boolean/segment_segment.hpp"

#ifdef _DEBUG_MODE_
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "utility/tinyformat.hpp"
DEFINE_HAS_MEMBER(get_name);
#endif

namespace carpio {
#ifdef _DEBUG_MODE_
    
    template<class LISTSEG>
    void _PlotListSegment(Gnuplot& gnu, const LISTSEG& sl){
        // gnu.set_label(1, strtype, -4.5, 4);
        int index = 1;
        for(auto seg : sl){
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
            gnu.add(a);
            gnu.add(a1);
            index++;
        }
    }
    template<class LISTSEG>
    void _PlotListpSegment(Gnuplot& gnu, 
                          const LISTSEG& sl, 
                          const std::string& t = "",
                          const std::string& color_code = "#00A4EF"){
    int i = 0;
    if(sl.size() == 0 ){
        auto a = ToGnuplotActorAsVector(Segment_<double, 2>(0,0,1,1));
        a.title(t + " n=" + ToString(sl.size()));
        a.style("with vector head filled size screen 0.03,10,155 lw 2 lc rgb \"" + color_code +"\"");
        gnu.add(a);
    }else{
        for(auto seg : sl){
            auto a1 = ToGnuplotActorAsVector(*seg);
            if(i == 0){
                a1.title(t + " n=" + ToString(sl.size()));
            }
            a1.style("with vector head filled size screen 0.03,10,155 lw 2 lc rgb \"" + color_code +"\"");
            gnu.add(a1);
            i++;
            }
        }
    }
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
    
    typedef SweepEvent_<SegProxy> Event;
    typedef SweepEventQueue_<Event, SegProxy> EventQueue;
    typedef typename EventQueue::Setcp SetcpSeg;

    // typedef std::set<const Geo*> GeoSet;
    typedef CompareSeg_<SegProxy> CompareSeg;
    typedef std::set<cpSegProxy, CompareSeg > StatusTree;
    typedef Intersection_<Segment, Segment> InterTwo;
    typedef IntersectionResult_<Segment, Segment> Result;
    typedef std::list<Result> ListResult;

    typedef SegSlope_<Segment> Slope;
protected:
#ifdef _DEBUG_MODE_
    typedef std::list<Segment> ListSegment;
    int _loop_i;
    ListSegment listseg;
    Segment     diagonal;
    std::string _case_name;
    std::string _debug_case_name;
    Gnuplot gnu;
    std::ofstream ofs;
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
        const CONTAINER& con,
        const std::string& case_name = ""){
    #ifdef _DEBUG_MODE_
        _build_list_segment(con);
        _case_name = case_name;
        _debug_case_name = "case9";
        _loop_i = 0;
        _output_list_segment(con);
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
            gnu.clear();
            std::cout << "loop index = " << _loop_i << std::endl;
            if(_debug_condition()){
                _plot_setup(gnu);
            }
        #endif

            Event event = queue.top();
            auto& point = event.get_point();
            
        #ifdef _DEBUG_MODE_
            if(_debug_condition()){
                _plot_sweep_line(gnu, point, diagonal);
            }
        #endif

            // HANDLE EVENT POINT(point)
            // 1. Let L(p) be the set of segments whose Left endpoint is p;
            //    these segments are stored with the event point p.
            auto& l_set = queue.begin()->second[0];
            auto& c_set = queue.begin()->second[1];
            auto& r_set = queue.begin()->second[2];

            #ifdef _DEBUG_MODE_
            if(_debug_condition()){
                _PlotListpSegment(gnu, l_set, "l set", "#FBBC04" );
                _PlotListpSegment(gnu, c_set, "c set", "#F35426" );
                _PlotListpSegment(gnu, r_set, "r set", "#81BC06" );
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
            // std::cout << "three set size = " << set_size << std::endl;
            // ProfileEnd();
            if(set_size > 1){
                // ProfileStart("BO_ReportRes");
                _new_result(event, _list_res);
                // ProfileEnd();
            }
            //4. Delete the segents in R(p) and C(p) from T
            // ProfileStart("BO_DeleteRC");
            for(auto s : r_set){
                auto r = _erase_seg_in_status(status, s);
                if(r == 0)
                    throw std::invalid_argument(ToString(*s) + " should in status tree!");
            }
            for(auto s : c_set){
                auto r = _erase_seg_in_status(status, s);
                if(r == 0)
                    throw std::invalid_argument(ToString(*s) + " should in status tree!");
                // status.erase(s);
            }
            // ProfileEnd();
            p_sweep.x(point.x());
            p_sweep.y(point.y());
            //5. insert l_set and c_set in status tree
            ulc.clear();
            std::set_union(l_set.begin(), l_set.end(),
                            c_set.begin(), c_set.end(),
                            std::inserter(ulc, ulc.begin()));
            auto it_hint = status.begin();
            for(auto s : ulc){
                it_hint = status.insert(it_hint, s);
            }
            // for(auto s : ulc){
                // status.insert(s);
            // }

            // std::cout << "status size = "<< status.size() << std::endl;
            // ProfileStart("BO_FindNew");
            if(ulc.size() == 0){
                cpSegProxy s_a, s_b;
                _find_neighboors(p_sweep, status, s_a, s_b); 
                // ProfileStart("FindN_0");
                _compute_new_events(s_a, s_b, event);
                // ProfileEnd();
            } else {
                auto s_min  = _find_min_slope(ulc);
                auto s_max  = _find_max_slope(ulc);
            #ifdef _DEBUG_MODE_
                if(_debug_condition()){
                if(s_max){
                    std::cout << "s_max = " << s_max->seg() << std::endl;
                    PlotpSegment(gnu, s_max->cpseg(), "#A0C347" );
                }
                if(s_min){
                    std::cout << "s_min = " << s_min->seg() << std::endl;
                    PlotpSegment(gnu, s_min->cpseg(), "#A0C347" );
                }
                }
            #endif
                auto s_lower = _find_lower_neighboor(s_min, status);
            #ifdef _DEBUG_MODE_
                if(_debug_condition()){
                if(s_lower){
                    std::cout << "s_lower = " << s_lower->seg() << std::endl;
                }
                }
            #endif
                auto s_upper = _find_upper_neighboor(s_max, status);

            #ifdef _DEBUG_MODE_
                if(_debug_condition()){
                if(s_upper){
                    std::cout << "s_upper = " << s_upper->seg() << std::endl;
                    PlotpSegment(gnu, s_upper->cpseg(), "#F04137" );
                }
                }
            #endif


                #ifdef _DEBUG_MODE_
                if(_debug_condition()){
                    std::cout << "call min vs lower -> ";
                }
                #endif
                _compute_new_events(s_min, s_lower, event); 
                #ifdef _DEBUG_MODE_
                if(_debug_condition()){
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
                if(_debug_condition()){
                    this->_plot_status_tree(gnu, status, p_sweep);
                    this->_plot_res_points(gnu, _list_res);
                    gnu.plot();
                    gnu.clear();
                }
                std::cout << " end loop ========== " << std::endl;
                _loop_i++;
            #endif
            // std::cout << "Geo Set size = " << _set.size() << std::endl;
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
            throw std::invalid_argument(ToString(cps->seg()) + " should in tree!");
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
            throw std::invalid_argument(ToString(s->seg()) + " should in tree!");
        }
        if(++iter == tree.end())
            return nullptr;
        else
            return *iter;
    }

    cpSegProxy _find_min_slope(const SetcpSeg& v) const{
        auto it = v.begin();
        auto min = *(v.begin());

        while(++it != v.end()) {
            if((*it)->slope() < min->slope())
                min = *it;
        }
        return min;
    }
    cpSegProxy _find_max_slope(const SetcpSeg& v) const{
        auto it = v.begin();
        auto max = *(v.begin());

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
                if(_debug_condition()){
                    std::cout << ToString(res.type) << std::endl;
                }
            #endif
            if (CompareLessEqual(current.get_point(), res.point) ){
                if(res.type == _SS_INTERSECT_){
                    Event ev_i(res.point);
                    auto iter = queue.find(ev_i);
                    if(iter == queue.end()){
                        typename EventQueue::Setcp v, set_empty; 
                        v.insert(s0);
                        v.insert(s1);
                        queue.insert(std::pair<Event, typename EventQueue::ArrSetcp> (ev_i, {set_empty, v, set_empty}));
                    }
                #ifdef _DEBUG_MODE_
                if(_debug_condition()){
                    PlotNewPoint(gnu, res.point);
                }
                #endif
                }else if(res.type == _SS_TOUCH_ || res.type == _SS_OVERLAP_){
                    #ifdef _DEBUG_MODE_
                    if(_debug_condition()){
                        std::cout << " touch or overlap" << std::endl;
                    }
                    #endif
                    for(short i = 0 ; i < 2; i++){
                        auto pos = inter.point_position(i);
                        if(pos == _PS_IN_){
                            queue.add_event(Event(s0->cpseg()->p(i)), 1, s1);
                            #ifdef _DEBUG_MODE_
                            if(_debug_condition()){
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
                            if(_debug_condition()){
                                std::cout << s1->p(i-2) << std::endl;
                                PlotNewPoint(gnu, s1->cpseg()->p(i-2));
                            }
                            #endif
                        }
                    }
                }
            }
            #ifdef _DEBUG_MODE_
            else if(_debug_condition()){
                std::cout << " oo " << std::endl;
            }
            #endif
        }
        #ifdef _DEBUG_MODE_
        else if(_debug_condition()){
            std::cout << " xx " << std::endl;
        }
        #endif
    }
#ifdef _DEBUG_MODE_
    template<class CONTAINER>
    void _build_list_segment(const CONTAINER& con){
        if(! con.empty()){
            diagonal = *(con.begin()); 
        }
        for(auto& seg : con){
            diagonal = EnclosureDiagonal(seg, diagonal);
            listseg.push_back(seg);
        }
    }
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

    template<class CONTAINER>
    void _output_event_queue(const CONTAINER& con){
    
    }

    void _plot_setup(Gnuplot& gnu){
        gnu.set_terminal_png("./fig/"+ _case_name +"_"+ ToString(_loop_i));
        gnu.set_equal_ratio();
        gnu.set_grid();
        auto dx = MaxX(diagonal) - MinX(diagonal);
        auto dy = MaxY(diagonal) - MinY(diagonal);
        gnu.set_xrange(MinX(diagonal) - dx * 0.1, MaxX(diagonal) + dx * 0.1);
        gnu.set_yrange(MinY(diagonal) - dy * 0.1, MaxY(diagonal) + dy * 0.1);
        gnu.set_xlabel("x " + _case_name + " i = " + ToString(_loop_i));
        // gnu.set_key("right top");
        // gnu.set_key("outside");
        _PlotListSegment(gnu, this->listseg);
    }

    void _plot_sweep_line(Gnuplot& gnu, const Point& p,
                                      const Segment& dia){
        auto a = ToGnuplotActor(p);
        a.point_size(1);
        a.point_type(5);
        auto dy = MaxY(dia) - MinY(dia);
        Point pup(p[0],  MaxY(dia) + dy * 0.2);
        Point plow(p[0], MinY(dia) - dy * 0.2);
        Segment seg(plow, pup);
        auto aseg = ToGnuplotActor(seg);

        gnu.add(a);
        gnu.add(aseg);
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

    void _set_debug_case_name(const std::string& name){
        this->_debug_case_name = name;
    }

    inline int _loop_index_any(int index){
        return index;
    }
    bool _debug_condition(){
        // return _case_name == _debug_case_name && _loop_i == 5;
        return _case_name == _debug_case_name && _loop_i == _loop_index_any(_loop_i);
    }
#endif

};


}

#endif