#ifndef _BASE_PLOTY_ACTOR_HPP
#define _BASE_PLOTY_ACTOR_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"
#include "io/plotly.hpp"

namespace carpio{


template<class ANY>
PlotlyActor _ToPlotlyActorWireFrame(const ANY& a, const DomainTag& tag){};

template<class ANY>
PlotlyActor ToPlotlyActorWireFrame(const ANY& a){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _ToPlotlyActorWireFrame(a, t); 
}
template<class ANY,
        typename std::enable_if<
            std::is_base_of<DomainTag, typename ANY::Tag>::value,
        bool>::type = true >
PlotlyActor ToPlotlyActorPoints(const ANY& a){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _ToPlotlyActorPoints(a, t); 
}
template<class ANY,
        typename std::enable_if<
            std::is_base_of<DomainTag, typename ANY::Tag>::value,
        bool>::type = true >
PlotlyActor ToPlotlyActorVolume(const ANY& a){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _ToPlotlyActorVolume(a, t); 
}

}

#endif