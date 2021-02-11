/*
 *  ggnuplot_actor_maker.hpp
 *
 *  Created on: Feb 5, 2020
 *
 *  Author: zhou
 */

#ifndef _GEOMETRY_GNUPLOT_ACTOR_MAKER_HPP_
#define _GEOMETRY_GNUPLOT_ACTOR_MAKER_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
#include "geometry/objects/objects.hpp"
#include "io/gnuplot.hpp"
#include <memory>
#include <cmath>

namespace carpio {

template<typename TYPE, St DIM>
class GGnuplotActorMaker_ : public GnuplotActorMaker{
public:
    static const St Dim = DIM;
    typedef GnuplotActorMaker Base;
    typedef TYPE Vt;
    typedef Point_<TYPE, DIM> Point;
    typedef Box_<TYPE, DIM>   Box;
    typedef Point_<TYPE, DIM>& ref_Point;
    typedef const Point_<TYPE, DIM>& const_ref_Point;
    typedef Segment_<TYPE, DIM> Segment;
    typedef Segment& ref_Segment;
    typedef const Segment& const_ref_Segment;

    typedef Line_<TYPE> Line;

    typedef PointChain_<TYPE, DIM> PointChain;

    typedef std::shared_ptr<Gnuplot_actor> spActor;
    typedef std::list<spActor> list_spActor;

public:
    GGnuplotActorMaker_(){};

    spActor points(
            const Point& p,
            int color_idx = -1) {
        ASSERT(Dim == 2);
        int color = color_idx > 0? color_idx : 0;
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with points lc variable";

        actor->data().push_back(ToString(p.x(), p.y(), color, " "));

        actor->data().push_back("");
        return actor;
    }
    template<class ITERPOINTS>
    spActor points(
            ITERPOINTS begin,
            ITERPOINTS end,
            int color_idx      =-1) {
        ASSERT(Dim == 2);
        int color = color_idx > 0? color_idx : 0;
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with points lc variable";

        auto iterps = begin;
        for (; iterps != end; iterps++) {
            actor->data().push_back(
                    ToString(iterps->x(), iterps->y(), color, " "));
        }
        actor->data().push_back("");
        return actor;
    }

    template<class ITERPOINTS>
    spActor lines_points(
                ITERPOINTS begin,
                ITERPOINTS end,
                int color_idx      = -1,
                bool close         = false) {
        ASSERT(Dim == 2);
        int color = color_idx > 0? color_idx : 0;
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with lines_points lc variable";

        auto iterps = begin;
        auto iterpe = std::next(begin);
        for (; iterpe != end; iterps++, iterpe++) {
            actor->data().push_back(
                    ToString(iterps->x(), iterps->y(), color, " "));
            actor->data().push_back(
                    ToString(iterpe->x(), iterpe->y(), color, " "));
            actor->data().push_back("");
        }

        if (close) {
            auto iterf = begin;
            auto iterb = std::prev(end);
            actor->data().push_back(
                    ToString(iterb->x(), iterb->y(), color, " "));
            actor->data().push_back(
                    ToString(iterf->x(), iterf->y(), color, " "));
            actor->data().push_back("");
        }

        return actor;
    }

    template<class ContainerSegments>
     spActor lines_points(
            ContainerSegments con,
            int color_idx      = -1,
            bool close         = false,
            Segment dummy      = typename ContainerSegments::value_type()) {
            ASSERT(Dim == 2);
            int color = color_idx > 0? color_idx : 0;
            spActor actor = spActor(new Gnuplot_actor());
            actor->command() = "using 1:2:3 title \"\" ";
            actor->style()   = "with linespoints lc variable";

            auto iter = con.begin();
            for (; iter != con.end(); ++iter) {
                actor->data().push_back(
                        ToString(iter->psx(), iter->psy(), color, " "));
                actor->data().push_back(
                        ToString(iter->pex(), iter->pey(), color, " "));
                actor->data().push_back("");
            }

            if (close) {
                auto iter = con.begin();
                actor->data().push_back(
                        ToString(iter->psx(), iter->psy(), color, " "));
                actor->data().push_back(
                        ToString(iter->pex(), iter->pey(), color, " "));
                actor->data().push_back("");
            }

            return actor;
        }


    template<class ITERPOINTS>
    spActor lines(ITERPOINTS begin,
                         ITERPOINTS end,
                         int color_idx = -1,
                         bool close = false) {
        ASSERT(Dim == 2);
        int color = color_idx > 0? color_idx : 0;
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with lines lc variable";

        auto iterps = begin;
        auto iterpe = std::next(begin);
        for (; iterpe != end; iterps++, iterpe++) {
            actor->data().push_back(
                    ToString(iterps->x(), iterps->y(), color, " "));
            actor->data().push_back(
                    ToString(iterpe->x(), iterpe->y(), color, " "));
            actor->data().push_back("");
        }

        if (close) {
            auto iterf = begin;
            auto iterb = std::prev(end);
            actor->data().push_back(
                    ToString(iterb->x(), iterb->y(), color, " "));
            actor->data().push_back(
                    ToString(iterf->x(), iterf->y(), color, " "));
            actor->data().push_back("");
        }

        return actor;
    }

     spActor lines(const Box& box, int color_idx = -1){
        ASSERT(Dim == 2);
        std::vector<Point> ln = {
            box.get_point(_M_, _M_, _M_),
            box.get_point(_P_, _M_, _M_),
            box.get_point(_P_, _P_, _M_),
            box.get_point(_M_, _P_, _M_),
        };
        return this->lines(ln.begin(), ln.end(), color_idx, true);
    }

    spActor lines(const Line& l,
                         Vt xmin = 0.0,
                         Vt xmax = 1.0, int color_idx = -1){
        ASSERT(Dim == 2);
        int color = color_idx > 0? color_idx : 0;
        int n     = 10;   // number of segment
        Vt dx     = (xmax - xmin) / n;
        std::list<Point> listp;
        for(int i = 0; i < n + 1; i++){
            Vt x = xmin + i * dx;
            Vt y = l.cal_y(x);
            listp.push_back(Point(x, y));
        }
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with lines lc variable";
        for(auto& p : listp){
            actor->data().push_back(ToString(p.x(), p.y(), color, " "));
        }
        actor->data().push_back("");
        return actor;
    }

    spActor lines_points(
            const Segment& seg,
            int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        int color = color_idx > 0? color_idx : 0;
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";
        if (seg.empty()) {
            actor->data().push_back("");
            return actor;
        }
        actor->data().push_back(ToString(seg.psx(), seg.psy(), color, " "));
        actor->data().push_back(ToString(seg.pex(), seg.pey(), color, " "));

        actor->data().push_back("");
        return actor;
    }

    spActor lines_points(
            const PointChain& pc,
            int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        int color = color_idx > 0? color_idx : 0;
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style() = "with linespoints lc variable";
        if (pc.empty()) {
            actor->data().push_back("");
            return actor;
        }

        for (auto& p : pc) {
            actor->data().push_back(
                    ToString(p.x(), p.y(), color, " "));
        }

        actor->data().push_back(ToString(pc.front().x(), pc.front().y(), color, " "));
        return actor;
    }
    spActor arrows(const Segment& seg, int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        int color = color_idx > 0 ? color_idx : 0;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        actor->data().push_back(
                ToString(seg.psx(), seg.psy(),
                         seg.pex() - seg.psx(),
                         seg.pey() - seg.psy(), color, " "));

        return actor;
    }

    spActor arrows(
            const PointChain& pc,
            int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        int color = color_idx > 0? color_idx : 0;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        if (pc.empty()) {
            actor->data().push_back("");
            return actor;
        }

        auto iterps = pc.begin();
        auto iterpe = pc.begin();
        std::advance(iterpe, 1);
        for (;iterpe !=pc.end();iterps++, iterpe++) {
            actor->data().push_back(
                    ToString(iterps->x(), iterps->y(),
                             iterpe->x()-iterps->x() ,
                             iterpe->y()-iterps->y(), color, " "));
        }
        if (pc.closed()) {
            actor->data().push_back(
                    ToString(pc.back().x(), pc.back().y(),
                             pc.front().x() - pc.back().x(),
                             pc.front().y() - pc.back().y(), color,
                             " "));
        }
        return actor;
    }
};

}

#endif /* _ACTOR_GNUPLOT_HPP_ */
