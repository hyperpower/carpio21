#ifndef _BEN_OTT_INTERSECTION_HPP_
#define _BEN_OTT_INTERSECTION_HPP_

#include "geometry/operation/boolean/ben_ott_structure.hpp"


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
    typedef std::list<Segment> ListSegment;
    
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
    #ifdef _DEBUG_MODE_
    ListSegment listseg;
    Segment diagonal;
    #endif
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

    auto execute(
        ){
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
            gnu.set_terminal_png("./fig/" + ToString(i));
            auto dx = MaxX(diagonal) - MinX(diagonal);
            auto dy = MaxY(diagonal) - MinY(diagonal);
            gnu.set_xrange(MinX(diagonal) - dx * 0.1, MaxX(diagonal) + dx * 0.1);
            gnu.set_yrange(MinY(diagonal) - dy * 0.1, MaxY(diagonal) + dy * 0.1);
            PlotListSegment(gnu, this->listseg);
            #endif

            Event event = queue.top();
            auto& point = event.get_point();
            p_sweep.x(point.x());
            p_sweep.y(point.y());
            
            #ifdef _DEBUG_MODE_
            PlotSweepLine(gnu, p_sweep);
            #endif

            // HANDLE EVENT POINT(point)
            // 1. Let L(p) be the set of segments whose Left endpoint is p;
            //    these segments are stored with the event point p.
            auto l_set = queue.begin()->second;

            #ifdef _DEBUG_MODE_
            PlotListpSegment(gnu, l_set, "#FBBC04" );
            #endif

            queue.pop();

            //2. Find all segments stored in T that contain p; 
            //   they are adjacent in T. 
            //   Let R(p) denote the subset of segments found whose right endpoint is p, 
            //   and let C(p) denote the subset of segments found that contain p in their interior.
            auto pair_rc = get_sets(p_sweep, status);

            //3. if L(p) and C(p) and R(p) contains more than one segment
            //   then Report P as an intersection, together with L(p) and C(p) and R(p) 
            #ifdef _DEBUG_MODE_
            std::cout << "tree  size = " << status.size() << std::endl;
            std::cout << "r_set size = " << pair_rc.first.size() << std::endl;
            std::cout << "c_set size = " << pair_rc.second.size() << std::endl;
            PlotListpSegment(gnu, pair_rc.first, "#34A853");
            #endif
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
            for(auto s : pair_rc.first){
                status.erase(s);
            }
            for(auto s : pair_rc.second){
                status.erase(s);
            }
            //5. insert l_set and c_set in status tree
            std::cout << "l_set size = " << l_set.size() << std::endl;
            for(auto s : l_set){
                status.insert(s);
            }
            for(auto s : pair_rc.second){
                status.insert(s);
            }

            #ifdef _DEBUG_MODE_
            gnu.plot();
            gnu.clear();
            #endif
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
    #endif

};


}

#endif