#ifndef _BEN_OTT_DEBUG_HELPER_HPP_
#define _BEN_OTT_DEBUG_HELPER_HPP_
// bentley ottmann debug helper
#include "geometry/ben-ott/comp_seg.hpp"
#include "geometry/ben-ott/sweep_event.hpp"
#include "geometry/ben-ott/sweep_event_queue.hpp"
#include "geometry/operation/boolean/segment_segment.hpp"

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
    int _loop_i;
    ListSegment listseg;
    Segment     diagonal;
    std::string _case_name;
    std::string _debug_case_name;
    Gnuplot gnu;
    std::ofstream ofs;

public:
    DebugIntersectionBenOtt_(){

    }

};

}

#endif