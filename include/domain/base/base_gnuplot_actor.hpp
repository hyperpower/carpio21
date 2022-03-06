#ifndef _BASE_GNUPLOT_ACTOR_HPP
#define _BASE_GNUPLOT_ACTOR_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"
#include "io/gnuplot.hpp"

namespace carpio{


template<class ANY>
spGnuplotActor _ToGnuplotActorLines(const ANY& a, const DomainTag& tag){};

template<class ANY>
spGnuplotActor ToGnuplotActorLines(const ANY& a){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _ToGnuplotActorLines(a, t); 
}



}

#endif