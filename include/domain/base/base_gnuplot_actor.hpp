#ifndef _BASE_GNUPLOT_ACTOR_HPP
#define _BASE_GNUPLOT_ACTOR_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"
#include "io/gnuplot.hpp"

namespace carpio{


// template<class ANY>
// GnuplotActor _ToGnuplotActorLines(const ANY& a, const DomainTag& tag){};

// template<class ANY>
// GnuplotActor ToGnuplotActorLines(const ANY& a){
//     typedef typename ANY::Tag Tag;
//     return _ToGnuplotActorLines(a, Tag()); 
// }
template<class ANY>
auto ToGnuplotActorPointContour(const ANY& a){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorPointContour(a, Tag()); 
}
template<class ANY>
GnuplotActor ToGnuplotActorWireFrame(const ANY& a){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorWireFrame(a, Tag()); 
}

template<class ANY>
GnuplotActor ToGnuplotActorContour(const ANY& a){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorContour(a, Tag()); 
}
template<class ANY>
GnuplotActor ToGnuplotActorContourWire(const ANY& a){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorContourWire(a, Tag()); 
}
// a      : any objects
// config : configure string
template<class ANY>
GnuplotActor ToGnuplotActorLabel(const ANY& a, const std::string& config = ""){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorLabel(a, config, Tag()); 
}
template<class ANY>
GnuplotActor ToGnuplotActorVectors(const ANY& a, Vt unit_length = -1.0){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorVectors(a, unit_length, Tag()); 
}

}

#endif