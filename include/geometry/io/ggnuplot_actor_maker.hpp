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
#include "geometry/shape/distance_indicator.hpp"
#include <array>
#include "geometry/objects/objects.hpp"
#include "io/gnuplot.hpp"
#include <memory>
#include <cmath>

namespace carpio {


template<typename TYPE, St DIM>
class GGnuplotActorMaker_ { //deprecate ----
public:
    static const St Dim = DIM;
    // typedef GnuplotActorMaker Base;
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

    typedef std::shared_ptr<GnuplotActor> spActor;
    typedef std::list<spActor> list_spActor;

public:
    GGnuplotActorMaker_(){};

    spActor points(
            const Point& p,
            int color_idx = -1) {
        ASSERT(Dim == 2);
        int color = color_idx > 0? color_idx : 0;
        spActor actor = spActor(new GnuplotActor());
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
        spActor actor = spActor(new GnuplotActor());
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
        spActor actor = spActor(new GnuplotActor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";

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
            spActor actor = spActor(new GnuplotActor());
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
        spActor actor = spActor(new GnuplotActor());
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
        if(std::abs(l.slope())<1){
            for(int i = 0; i < n + 1; i++){
                Vt x = xmin + i * dx;
                Vt y = l.cal_y(x);
                listp.push_back(Point(x, y));
            }
        }else{
            for(int i = 0; i < n + 1; i++){
                Vt y = xmin + i * dx;
                Vt x = l.cal_x(y);
                listp.push_back(Point(x, y));
            }
        }
        spActor actor = spActor(new GnuplotActor());
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
        spActor actor = spActor(new GnuplotActor());
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
        spActor actor = spActor(new GnuplotActor());
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
        return arrows(seg[0], seg[1], color_idx);
    }

    spActor arrows(const Point& p1, const Point& p2, int color_idx = -1) {
        spActor actor = spActor(new GnuplotActor());
        int color = color_idx > 0 ? color_idx : 0;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        actor->data().push_back(
                ToString(p1[0],  p1[1],
                         p2[0] - p1[0],
                         p2[1] - p1[1], color, " "));
        return actor;
    }
    spActor arrows(
            const PointChain& pc,
            int color_idx = -1) {
        spActor actor = spActor(new GnuplotActor());
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


// Function version
// Geometry objects entry


// template<class ANY, class VALUE,
//         typename std::enable_if<
//             IsGeometry<ANY>::value,
//         bool>::type = true>
// auto ToGnuplotActor(const ANY& geo, const VALUE& value){
//     typedef std::shared_ptr<GnuplotActor> spActor;
//     spActor actor = spActor(new GnuplotActor());
//     MakeGnuplotActor(*actor, geo, value, ANY::Tag());    
//     return actor;
// }

template<typename ANY>
void MakeGnuplotActor(GnuplotActor& actor, const ANY& point, PointTag){
    actor.command() = "using 1:2 title \"\" ";
    actor.set_using(ANY::Dim);
    actor.style()   = "with points lc 1"; // default color is 1

    actor.data().push_back(ToString(point, " ") );

    actor.data().push_back("");
}

template<typename ANY, typename CONTAINER, 
          typename std::enable_if<IsNumber<typename CONTAINER::value_type>::value, bool>::type = true>
void MakeGnuplotActor(GnuplotActor& actor, const ANY& line, const CONTAINER& con, LineTag){
    assert(con.size() == 2);
    typedef Point_<double, 2> Point;
    int n = 10; // number of segment
    double xmax = *(con.begin());
    double xmin = *(std::next(con.begin())); 
    double dx = (xmax - xmin) / n;
    
    std::list<Point> listp;
    if (std::abs(line.slope()) < 1){
        for (int i = 0; i < n + 1; i++){
            Vt x = xmin + i * dx;
            Vt y = line.cal_y(x);
            listp.push_back(Point(x, y));
        }
    }
    else{
        for (int i = 0; i < n + 1; i++){
            Vt y = xmin + i * dx;
            Vt x = line.cal_x(y);
            listp.push_back(Point(x, y));
        }
    } 
    actor.command() = "using 1:2 title \"\" ";
    actor.style() = "with lines";
    for (auto &p : listp)
    {
        actor.data().push_back(ToString(p, " "));
    }
    actor.data().push_back("");
}
template<typename ANY>
void MakeGnuplotActor(GnuplotActor& actor, const ANY& seg, SegmentTag){
    actor.command() = "using 1:2 title \"\" ";
    actor.set_using(ANY::Dim);
    actor.style()   = "with lines lc 1"; // default color is 1
    if (seg.empty()) {
        actor.data().push_back("");
        return;
    }
    actor.data().push_back(ToString(seg[0], " "));
    actor.data().push_back(ToString(seg[1], " "));

    actor.data().push_back("");
}
template<typename ANY>
void MakeGnuplotActorAsVector(GnuplotActor& actor, const ANY& seg, SegmentTag){
    actor.command("using 1:2:3:4 title \"\" ");
    actor.set_using(ANY::Dim * 2);
    actor.style("with vectors head filled lc 1"); // default color is 1
    if (seg.empty()) {
        actor.data().push_back("");
        return;
    }
    actor.data().push_back(
        ToString(seg.p(0), " ") 
        + " " 
        + ToString(seg.tangent_vector(), " "));

    actor.data().push_back("");
}
template<typename ANY>
void MakeGnuplotActorAsVector(GnuplotActor& actor, const ANY& pc, PointChainTag){
    actor.command() = "using 1:2:3:4 title \"\" ";
    actor.set_using(ANY::Dim * 2);
    actor.style()   = "with vectors lc 1"; // default color is 1
    if (pc.empty()){
        actor.data().push_back("");
        return;
    }

    auto iterps = pc.begin();
    auto iterpe = pc.begin();
    std::advance(iterpe, 1);
    for (; iterpe != pc.end(); iterps++, iterpe++) {
        actor.data().push_back(
            ToString(iterps->x(), iterps->y(),
                     iterpe->x() - iterps->x(),
                     iterpe->y() - iterps->y(), " "));
    }
    if (pc.closed()){
        actor.data().push_back(
            ToString(pc.back().x(), pc.back().y(),
                     pc.front().x() - pc.back().x(),
                     pc.front().y() - pc.back().y(), 
                     " "));
    }
    actor.data().push_back("");
}
template<typename ANY>
void MakeGnuplotActor(GnuplotActor& actor, const ANY& box, BoxTag){
    actor.command() = "using 1:2 title \"\" ";
    actor.set_using(ANY::Dim);
    actor.style()   = "with lines lc 1"; // default color is 1

    if constexpr (ANY::Dim >= 2){
        actor.data().push_back(ToString(box.get_point(_M_, _M_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_P_, _M_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_P_, _P_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_M_, _P_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_M_, _M_, _M_), " "));
    } 
    if constexpr (ANY::Dim == 3){
        actor.data().push_back("");
        actor.data().push_back(ToString(box.get_point(_M_, _M_, _P_), " "));
        actor.data().push_back(ToString(box.get_point(_P_, _M_, _P_), " "));
        actor.data().push_back(ToString(box.get_point(_P_, _P_, _P_), " "));
        actor.data().push_back(ToString(box.get_point(_M_, _P_, _P_), " "));
        actor.data().push_back(ToString(box.get_point(_M_, _M_, _P_), " "));
        actor.data().push_back("");
        actor.data().push_back(ToString(box.get_point(_M_, _M_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_M_, _M_, _P_), " "));
        actor.data().push_back("");
        actor.data().push_back(ToString(box.get_point(_P_, _M_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_P_, _M_, _P_), " "));
        actor.data().push_back("");
        actor.data().push_back(ToString(box.get_point(_P_, _P_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_P_, _P_, _P_), " "));
        actor.data().push_back("");
        actor.data().push_back(ToString(box.get_point(_M_, _P_, _M_), " "));
        actor.data().push_back(ToString(box.get_point(_M_, _P_, _P_), " "));
    }
    actor.data().push_back("");
}
template<typename ANY>
void MakeGnuplotActor(GnuplotActor& actor, const ANY& pc, PointChainTag){
    actor.command() = "using 1:2 title \"\" ";
    actor.set_using(ANY::Dim);
    actor.style()   = "with lines lc 1"; // default color is 1
    
    if (pc.empty()) {
		actor.data().push_back("");
		return;
	}

	for (auto& p : pc) {
    	actor.data().push_back(ToString(p, " "));
	}

    if(pc.closed()){
	    actor.data().push_back(ToString(pc.front(), " "));
    }
    actor.data().push_back("");
}
// Geomentry objects in container entry
template<class ANY, 
        typename std::enable_if<
                   (! IsGeometry<ANY>::value)
                && IsContainer<ANY>::value 
                && IsGeometry<typename ANY::value_type>::value
        , bool>::type = true>
auto ToGnuplotActor(const ANY& any) {
    GnuplotActor actor;
    for (auto& geo : any){
        MakeGnuplotActor(actor, geo, typename ANY::value_type::Tag());
    }    
    return actor;
}
template<class ANY, 
        typename std::enable_if<
                   (! IsGeometry<ANY>::value)
                && IsContainer<ANY>::value 
                && std::is_pointer<typename ANY::value_type>::value
                && IsGeometry<typename std::remove_pointer<typename ANY::value_type>::type>::value
        , bool>::type = true>
auto ToGnuplotActor(const ANY& any, unsigned int jump = 1) {
    typedef typename ANY::value_type pType;
    typedef typename std::remove_pointer<pType>::type Type;
    GnuplotActor actor;
    int count = 0;
    for (auto& geo : any){
        MakeGnuplotActor(actor, *geo, Type::Tag());
        if(jump > 0){
            if((count % jump == 0)){
                ++count;
                continue;
            }
        }
        actor.data().pop_back();
        ++count;
    }    
    return actor;
}
template<typename ANY,
        typename std::enable_if<
            IsGeometry<ANY>::value,
        bool>::type = true>
auto ToGnuplotActor(const ANY& geo){
    GnuplotActor actor;
    MakeGnuplotActor(actor, geo, typename ANY::Tag());    
    return actor;
}
template<typename ANY, typename CONTAINER,
        typename std::enable_if<
            IsGeometry<ANY>::value
         && IsContainer<CONTAINER>::value, 
        bool>::type = true>
auto ToGnuplotActor(const ANY& geo, const CONTAINER& con){
    GnuplotActor actor;
    MakeGnuplotActor(actor, geo, con, typename ANY::Tag());    
    return actor;
}
template<typename ANY,
        typename std::enable_if<
            IsGeometry<ANY>::value,
        bool>::type = true>
auto ToGnuplotActorAsVector(const ANY& geo){
    GnuplotActor actor;
    MakeGnuplotActorAsVector(actor, geo, typename ANY::Tag());    
    return actor;
}



class GnuplotActorDistanceIndicator : public GnuplotActorGroup, public DistanceIndicator{
public:
    typedef std::shared_ptr<GnuplotActor> spActor;
    typedef std::list<GnuplotActor> list_Actor;
    typedef GnuplotActorGroup ActorGroup;
    typedef DistanceIndicator Shape;
    typedef typename Shape::Point Point;
    typedef GnuplotActorDistanceIndicator Self;
public:
    GnuplotActorDistanceIndicator():ActorGroup(), Shape(){    
    };
    GnuplotActorDistanceIndicator(
        const double& x1, const double& y1,
        const double& x2, const double& y2):
        ActorGroup(), Shape(x1, y1, x2, y2){    
        double dis = Distance(this->_p1, this->_p2);
        // this->build_actors();
    }

    void build_actors(){
        // vector  x y xdelta ydelta
        GnuplotActor arrow;
        Point  midp  = this->arrow_center();
        Point  left  = this->arrow_left();
        Point  right = this->arrow_right();
        arrow.command("using 1:2:3:4 title \"\"");
        arrow.style("with vectors filled head lc black");
        arrow.data().push_back(ToString(midp[0], midp[1], left[0] - midp[0], left[1] - midp[1] , " "));
        arrow.data().push_back(ToString(midp[0], midp[1], right[0] - midp[0], right[1] - midp[1], " "));
        arrow.data().push_back(" ");
        this->_actors.push_back(arrow);
        // normal line left
        GnuplotActor normall;
        Point l0 = this->normal_line_left(0);
        Point l1 = this->normal_line_left(1);
        normall.command("using 1:2 title \"\"");
        normall.style("with lines lc black");
        normall.data().push_back(ToString(l0[0], l0[1] , " "));
        normall.data().push_back(ToString(l1[0], l1[1] , " "));
        normall.data().push_back(" ");
        this->_actors.push_back(normall);
        // 
        GnuplotActor normalr;
        Point r0 = this->normal_line_right(0);
        Point r1 = this->normal_line_right(1);
        normalr.command("using 1:2 title \"\"");
        normalr.style("with lines lc black");
        normalr.data().push_back(ToString(r0[0], r0[1] , " "));
        normalr.data().push_back(ToString(r1[0], r1[1] , " "));
        normalr.data().push_back(" ");
        this->_actors.push_back(normalr);
        // 
        if(this->is_offset()){
            GnuplotActor ao;
            Point o = this->offset();
            ao.command("using 1:2 title \"\"");
            ao.style("with lines lc black");
            ao.data().push_back(ToString(midp[0], midp[1] , " "));
            ao.data().push_back(ToString(o[0], o[1] , " "));
            ao.data().push_back("");
            this->_actors.push_back(ao);
        }
    }
};


}

#endif /* _ACTOR_GNUPLOT_HPP_ */
