#ifndef _S_PLOTLY_ACTOR_HPP
#define _S_PLOTLY_ACTOR_HPP

#include "domain/base/base_plotly_actor.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "s_stringify.hpp"

namespace carpio{

// ToGunplotActor
//  - ToGnuplotActorDim
//   - ToGnuplotActor


template<class ANY>
auto _ToPlotlyActorLinesDim(const ANY& grid, SGridTag, Dim3Tag){
    // std::string trace_type = (type == "auto") ? "Scatter3d" : "Scatter";
    PlotlyActor actor("Scatter3d");
    actor.update("mode", "lines");
    std::list<typename ANY::Point> list;

    std::vector<int> order = {0,1,1,3,3,2,2,0,4,5,5,7,7,6,6,4,0,4,1,5,2,6,3,7};

    for (St k = 0; k < grid.n(_Z_); k++) {
        for (St j = 0; j < grid.n(_Y_); j++) {
            for (St i = 0; i < grid.n(_X_); i++) {            
                for(auto o : order){
                    typename ANY::Index index(i,j,k);
                    list.emplace_back(grid.v(o, index));
                }
            }
        }
    }
    actor.data_xyz(list, 2);
    return actor;
}

template<class ANY>
auto _ToPlotlyActorLines(const ANY& a, SGridTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dtag;
    return _ToPlotlyActorLinesDim(a, t, dtag); 
}


template<class ANY>
auto _ToPlotlyActorPointsDim(const ANY& grid, SGridTag, Dim3Tag){
    // std::string trace_type = (type == "auto") ? "Scatter3d" : "Scatter";
    PlotlyActor actor("Scatter3d");
    actor.update("mode", "markers");
    std::list<typename ANY::Point> list;

    std::vector<int> order = {0,1,1,3,3,2,2,0,4,5,5,7,7,6,6,4,0,4,1,5,2,6,3,7};

    for (St k = 0; k < grid.n(_Z_); k++) {
        for (St j = 0; j < grid.n(_Y_); j++) {
            for (St i = 0; i < grid.n(_X_); i++) {            
                typename ANY::Index index(i,j,k);
                list.emplace_back(grid.c(index));
            }
        }
    }
    actor.data_xyz(list, 1);
    return actor;
}
template<class ANY>
auto _ToPlotlyActorPoints(const ANY& a, SGridTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dtag;
    return _ToPlotlyActorPointsDim(a, t, dtag); 
}

}

#endif