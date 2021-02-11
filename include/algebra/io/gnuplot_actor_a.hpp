#ifndef _ALGEBRA_GNUPLOT_ACTOR_HPP
#define _ALGEBRA_GNUPLOT_ACTOR_HPP

#include "algebra/array/array_list.hpp"
#include "io/gnuplot.hpp"

namespace carpio{

namespace GnuplotActor{

template<class T>
spActor TwoArrayList(
               const ArrayListT_<T>& arrx,
               const ArrayListT_<T>& arry,
               int   color_idx = -1,
               const std::string& style  = "with points lc variable"){
    spActor actor    = spActor(new Gnuplot_actor());
    actor->command() = "using 1:2:3 title \"\" ";
    actor->style()   = style;
    ASSERT(arrx.size() == arry.size());
    for(St i = 0; i < arrx.size(); ++i){
        if (color_idx >= 0) {
            actor->data().push_back(ToString(arrx[i], arry[i], color_idx, " "));
        } else {
            actor->data().push_back(ToString(arrx[i], arry[i], 0, " "));
        }
    }
    actor->data().push_back("");
    return actor;
}

template<class T>
spActor Points(const ArrayListT_<T>& arrx,
               const ArrayListT_<T>& arry,
               int   color_idx = -1){
    TwoArrayList(arrx, arry, color_idx, "with points lc variable");
}

template<class T>
spActor LinesPoints(
               const ArrayListT_<T>& arrx,
               const ArrayListT_<T>& arry,
               int   color_idx = -1){
    TwoArrayList(arrx, arry, color_idx, "with linespoints lc variable");
}

template<class T>
spActor OneArrayList(
               const ArrayListT_<T>& arrx,
               int   color_idx = -1,
               const std::string& style = "with points lc variable"){
    spActor actor    = spActor(new Gnuplot_actor());
    actor->command() = "using 1:2:3 title \"\" ";
    actor->style()   = style;
    for(St i = 0; i < arrx.size(); ++i){
        if (color_idx >= 0) {
            actor->data().push_back(ToString(i, arrx[i], color_idx, " "));
        } else {
            actor->data().push_back(ToString(i, arrx[i], 0, " "));
        }
    }
    actor->data().push_back("");
    return actor;
}

template<class T>
spActor Points(const ArrayListT_<T>& arrx,
               int   color_idx = -1){
    return OneArrayList(arrx, color_idx, "with points lc variable");
}

template<class T>
spActor LinesPoints(const ArrayListT_<T>& arrx,
                    int   color_idx = -1){
    return OneArrayList(arrx, color_idx, "with linespoints lc variable");
}

template<class T>
spActor Lines(const ArrayListT_<T>& arrx,
                    int   color_idx = -1){
    return OneArrayList(arrx, color_idx, "with lines lc variable");
}


}

}

#endif
