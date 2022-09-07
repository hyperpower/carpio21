#ifndef _BASE_PLOTY_ACTOR_HPP
#define _BASE_PLOTY_ACTOR_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"
#include "io/plotly.hpp"

namespace carpio{


template<class ANY>
PlotlyActor _ToPlotlyActorLines(const ANY& a, const DomainTag& tag){};

template<class ANY>
PlotlyActor ToPlotlyActorLines(const ANY& a){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _ToPlotlyActorLines(a, t); 
}

}

#endif