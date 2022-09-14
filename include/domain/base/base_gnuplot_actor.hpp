#ifndef _BASE_GNUPLOT_ACTOR_HPP
#define _BASE_GNUPLOT_ACTOR_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"
#include "io/gnuplot.hpp"

namespace carpio{


template<class ANY>
GnuplotActor _ToGnuplotActorLines(const ANY& a, const DomainTag& tag){};

template<class ANY>
GnuplotActor ToGnuplotActorLines(const ANY& a){
    typedef typename ANY::Tag Tag;
    return _ToGnuplotActorLines(a, Tag()); 
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
}

#endif