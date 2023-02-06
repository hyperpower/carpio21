#ifndef _MULTI_SEGMENTS_HPP_
#define _MULTI_SEGMENTS_HPP_

#include "two_segments.hpp" 
#include "ben-ott/bentley_ottmann.hpp"

auto GenerateSegmentsCase1(){
    typedef std::list<Segment2> ListSegment;
    ListSegment lseg;
    lseg.push_back(Segment2(1.0, 2.0, 1.0, 2.0));
    return lseg;
}


#endif
