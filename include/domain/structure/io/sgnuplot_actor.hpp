#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "io/gnuplot.hpp"

namespace carpio{

// using namespace GnuplotActor;

template<St DIM>
class GAMStructure_:public GnuplotActorMaker {
public:
    static const St Dim = DIM;

    typedef SGrid_<DIM> Grid;
    typedef SGrid_<1>   Grid1;
    typedef SGrid_<2>   Grid2;

public:
    GAMStructure_(){};
    
    spActor wire_frame(
                const Grid2&     grid,
                int color_idx    = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
        for (St j = 0; j < grid.n(_Y_); j++) {
            for (St i = 0; i < grid.n(_X_); i++) {
                typename Grid2::Index index(i, j);
                for (short o = 0; o < grid.num_vertex(); ++o) {
                    auto p = grid.v(order[o], index);
                    actor->data().push_back(
                                ToString(p.value(_X_),
                                         p.value(_Y_),
                                         c,
                                         " "));
                }
                auto p = grid.v(0, index);
                actor->data().push_back(
                            ToString(p.value(_X_), p.value(_Y_), c,
                                    " "));
                actor->data().push_back("");
            }
        }
        return actor;
    }

    spActor wire_frame(
                const Grid1& grid,
                const Vt&    tik  = 0.1,
                int color_idx    = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        Vt xs, xe;
        for (St i = 0; i < grid.n(_X_); i++) {
            typename Grid1::Index index(i);
            auto p = grid.f(_X_, _M_, index);
            actor->data().push_back(
                    ToString(p.value(_X_), -0.0, c, " "));
            std::cout << ToString(p.value(_X_), -0.0, c, " ") << std::endl;
            actor->data().push_back(
                    ToString(p.value(_X_),  tik, c, " "));
            actor->data().push_back("");
            if(i == 0){
                xs = p.value(_X_);
            }
        }
        typename Grid::Index index(grid.n(_X_) - 1);
        auto p = grid.f(_X_, _P_, index);
        actor->data().push_back(
                ToString(p.value(_X_), -0.0, c, " "));
        actor->data().push_back(
                ToString(p.value(_X_),  tik, c, " "));
        actor->data().push_back("");
        xe = p.value(_X_);

        actor->data().push_back(ToString(xs, 0.0, c, " "));
        actor->data().push_back(ToString(xe, 0.0, c, " "));

        return actor;
    }

};


}

#endif