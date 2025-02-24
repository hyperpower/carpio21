#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include <string>

#include "domain/base/base_gnuplot_actor.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/sghost.hpp"
#include "domain/structure/order/sorder.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/field/sfield_vertex.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "io/gnuplot.hpp"
#include "s_stringify.hpp"

namespace carpio{

// ToGnuplotActorXXX(ANY)
// _ToGunplotActorXXX(ANY, XXXTag)
// _ToGnuplotActorXXXDim(ANY, XXXTag, DimTag)

// Objects to be plotted:
//  1. Grid
//  2. Ghost
//  3. Order
//  4. Field Center
//  5. Field Face
//  6. Vector Center
//  7. Vector Face

// Type can be plotted 
//  Points, Lines, LinesPoints, WireFrame, Contour, ContourWire, Label，

//         Type  : Dim  : Config
// Points: Grid  : 1 2  : center, facecenter, vertex
//       : Ghost : 1 2  : center, facecenter, vertex
//       : FC    : 1 2  : vari_color
//       : FF    : 1 2  : vari_color
//       : VC    : 1 2  : vari_color

// LinesPoints
// Lines : Grid  x
//       : Ghost x
//       : FC    : 1    : vari_color
//       : FF    : 1    : vari_color
//       : VC    : 1    : vari_color

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrameDim(const ANY& grid, SGridTag, Dim1Tag){
    GnuplotActor actor; 
    actor.command( "using 1:2 title \"\" ");
    actor.style( "with line lc -1");
    Vt tik = 0.1;

    Vt xs=0.0, xe=0.0;
    for (St i = 0; i < grid.n(_X_); i++){
        typename ANY::Index index(i);
        auto p = grid.f(_X_, _M_, index);
        actor.data().push_back(
            ToString(p.value(_X_), -0.0,  " "));
        actor.data().push_back(
            ToString(p.value(_X_), tik, " "));
        actor.data().push_back("");
        if (i == 0){
            xs = p.value(_X_);
        }
    }
    typename ANY::Index index(grid.n(_X_) - 1);
    auto p = grid.f(_X_, _P_, index);
    actor.data().push_back(
        ToString(p.value(_X_), -0.0, " "));
    actor.data().push_back(
        ToString(p.value(_X_), tik, " "));
    actor.data().push_back("");
    xe = p.value(_X_);

    actor.data().push_back(ToString(xs, 0.0, " "));
    actor.data().push_back(ToString(xe, 0.0, " "));

    // boundary
    actor.data().push_back(ToString(xs, 0.0, " "));
    actor.data().push_back(ToString(xs, 2.0 * tik, " "));
    actor.data().push_back("");

    actor.data().push_back(ToString(xe, 0.0, " "));
    actor.data().push_back(ToString(xe, 2.0 * tik, " "));
    actor.data().push_back("");

    return actor;
}
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrameDim(const ANY& ghost, SGhostTag, Dim1Tag){
    GnuplotActor actor; 
    actor.command( "using 1:2 title \"\" ");
    actor.style( "with line");
    Vt tik = 0.1;

    auto& grid = ghost.grid();
    auto gl    = ghost.ghost_layer();

    Vt xs=0.0, xe=0.0;
    for (int i = -gl; i <= 0; i++){
        typename ANY::Index index(i);
        auto p = grid.f(_X_, _M_, index);
        actor.data().push_back(
            ToString(p.value(_X_), -0.0,  " "));
        actor.data().push_back(
            ToString(p.value(_X_), tik, " "));
        actor.data().push_back("");
        if(i == -gl){
            xs = p.value(_X_);
        }
        if(i == 0){
            xe = p.value(_X_);
        }
    }
    actor.data().push_back(
        ToString(xs, 0.0,  " "));
    actor.data().push_back(
        ToString(xe, 0.0, " "));
    actor.data().push_back("");
    
    for (int i = grid.n(_X_) - 1; i < grid.n(_X_) + gl; i++){
        typename ANY::Index index(i);
        auto p = grid.f(_X_, _P_, index);
        actor.data().push_back(
            ToString(p.value(_X_), -0.0,  " "));
        actor.data().push_back(
            ToString(p.value(_X_), tik, " "));
        actor.data().push_back("");
        if(i == grid.n(_X_) - 1){
            xs = p.value(_X_);
        }
        if(i == grid.n(_X_) + gl -1 ){
            xe = p.value(_X_);
        }
    }
    actor.data().push_back(
        ToString(xs, 0.0,  " "));
    actor.data().push_back(
        ToString(xe, 0.0, " "));
    actor.data().push_back("");
    return actor;
}
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrameDim(const ANY& grid, SGridTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2 title \"\" ");
    actor.style("with line lc 0");

    for (St j = 0; j < grid.n(_Y_); j++) {
        for (St i = 0; i < grid.n(_X_); i++) {
            typename ANY::Index index(i, j);
                
            actor.data().splice(actor.data().end(), 
                        _StringifyCell(grid, index, SGridTag(), Dim2Tag()));

            actor.data().push_back("");
        }
    }
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SGridTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _ToGnuplotActorWireFrameDim(a, t, dt); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SGhostTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    Tag t;
    DimTag dt;
    return _ToGnuplotActorWireFrameDim(a, t, dt); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorWireFrame(const ANY& a, SFieldCenterTag){
    // Todo: not ok for masked grid, need upgrade.
    auto& grid = a.grid();
    return _ToGnuplotActorWireFrame(grid, SGridTag()); 
}

template<class ANY>
GnuplotActor _ToGnuplotActorContourDim(const ANY& f, SFieldCenterTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3:4:5:6:7 title \"\" ");
    actor.style("with boxxy fs solid fc palette ");
    for(auto& index : f.order()){
        auto pc   = f.grid().c(index);
        auto pmin = f.grid().v(0, index);
        auto pmax = f.grid().v(3, index);
        actor.data().push_back(
            ToString(pc(_X_),   pc(_Y_), 
                     pmin(_X_), pmax(_X_),
                     pmin(_Y_), pmax(_Y_),
                     f(index),  " "));
    }
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorContour(const ANY& a, SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourDim(a, Tag(), DimTag()); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourWireDim(const ANY& f, 
    SFieldCenterTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with line lc palette");
    for (St i = 0; i < f.grid().n(_X_); i++){
        for (St j = 0; j < f.grid().n(_Y_); j++){
            typename ANY::Grid::Index index(i, j);
            auto pc = f.grid().c(index);
            if (f.ghost().is_normal(index) == true){
                actor.data().push_back(
                    ToString(pc(_X_), pc(_Y_),
                             f(index), f(index), " "));
            }
        }
        actor.data().push_back("");
    }
    return actor;
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourWireDim(
        const ANY& f, SFieldCenterTag, Dim1Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with lines lc palette");
    for (St i = 0; i < f.grid().n(_X_); i++){
        typename ANY::Grid::Index index(i);
        auto pc = f.grid().c(index);
        if (f.ghost().is_normal(index) == true){
            actor.data().push_back(
                ToString(pc(_X_), f(index), f(index), " "));
        }
    }
    return actor;
}


template<class ANY>
GnuplotActor _ToGnuplotActorContourWire(const ANY& a, SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourWireDim(a, Tag(), DimTag()); 
}

template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(const ANY& order, 
                                const std::string& config, 
                                SOrderTag){
    typedef typename ANY::LocationTag LocTag;
    return _ToGnuplotActorLabel(order, config, SOrderTag(), LocTag());
}
template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(const ANY& order, 
                                const std::string& config, 
                                SOrderTag, CenterTag){
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");
    
    auto &grid  = order.grid();
    typedef typename ANY::Index Index;
    auto fun = [&grid, &actor, &order, &config](const Index &idx)
    {
        auto cp = grid.c(idx);
        auto o  = order.get_order(idx);
        std::string text = "";
        if (config == "index"){
            text =  ToString(idx);
        }else if(config == "order"){
            text =  ToString(o);
        }else{
            text =  ToString(idx);
        }
        actor.data().push_back(
                    ToString(cp(_X_), cp(_Y_)," ") + " \"" + text + "\"");
    };
    grid.for_each(fun);
    return actor;
}
template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(const ANY& order, 
                                const std::string& config, 
                                SOrderTag, VertexTag){
   GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt 2");
    
    auto &grid  = order.grid();
    auto &ghost = order.ghost();
    typedef typename ANY::Index Index;
    auto fun = [&grid, &ghost, &actor, &order, &config](const Index &idx)
    {
        if(ghost.is_normal_vertex(idx)){
            auto vp = grid.v(idx);
            auto s  = grid.s_(idx, _X_);
            auto o  = order.get_order(idx);
            std::string text = "";
            if (config == "index"){
                text =  ToString(idx);
            }else if(config == "order"){
                text =  ToString(o);
            }else{
                text =  ToString(idx);
            }
            actor.data().push_back(
                        ToString(vp(_X_) + s * 0.1, vp(_Y_) + s * 0.1," ") + " \"" + text + "\"");
        }
        
    };
    grid.for_each(fun);
    return actor; 
}
template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(
    const ANY& ff, const std::string& config, 
    SFieldFaceTag ft){
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorLabel(ff, config, ft, DimTag());
}
template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(
    const ANY& ff, const std::string& config, 
    SFieldCenterTag tag){
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorLabel(ff, config, tag, DimTag());
}
template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(
    const ANY& field, const std::string& config, 
    SFieldCenterTag, Dim1Tag)
{
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt -1");
    auto& order = field.order();
    auto& grid  = field.grid();
    typedef typename ANY::Index Index;
    for(auto& cidx : field.order()){
        auto cp   = field.grid().c(cidx);
        std::string text = "";
        if (config == "index"){
            text =  ToString(cidx);
        }else if(config == "order"){
            auto o = order.get_order(cidx);
            text =  ToString(o);
        }else{
            text =  ToString(cidx);
        }
        actor.data().push_back(
                    ToString(cp(_X_), -0.1," ") + " \"" + text + "\"");
    }
    return actor;
}

template<class ANY>
GnuplotActor  _ToGnuplotActorLabel(
    const ANY& ff, const std::string& config, 
    SFieldFaceTag, Dim1Tag)
{
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with labels center textcolor lt -1");
    auto& order = ff.order();
    auto& grid  = ff.grid();
    typedef typename ANY::Index Index;
    for(auto& fidx : ff.order()){
        auto cidx = ff.grid().face_index_to_cell_index(fidx, ff.face_axe());
        auto fp   = ff.grid().f(ff.face_axe(), _M_, cidx);
        std::string text = "";
        if (config == "index"){
            text =  ToString(fidx);
        }else if(config == "order"){
            auto o = order.get_order_face_index(fidx, ff.face_axe());
            text =  ToString(o);
        }else{
            text =  ToString(fidx);
        }
        actor.data().push_back(
                    ToString(fp(_X_), -0.1," ") + " \"" + text + "\"");
        if(grid.is_last(cidx, ff.face_axe())){// not a good code
            fp = ff.grid().f(ff.face_axe(), _P_, cidx);
            text = "";
            if (config == "index"){
                text = ToString(fidx);
            }
            else if (config == "order"){
                auto o = order.get_order_face_index(fidx, ff.face_axe());
                text = ToString(o);
            }else{
                text = ToString(fidx);
            }
            actor.data().push_back(
                ToString(fp(_X_), -0.1, " ") + " \"" + text + "\"");
        }
    }
    return actor;
}

template<class ANY>
auto _ToGnuplotActorContourPoints(
    const ANY& ff,
    SFieldFaceTag, Dim1Tag)
{    
    std::list<double> lx, ly, lv;
    for(auto& fidx : ff.order()){
        auto cidx = ff.grid().face_index_to_cell_index(fidx, ff.face_axe());
        auto p = ff.grid().f(ff.face_axe(), _M_, cidx);
        lx.push_back(p.value(_X_));
        ly.push_back(ff(_M_, cidx));
        lv.push_back(ff(_M_, cidx));

        if(ff.grid().is_last(cidx, ff.face_axe())){
            p = ff.grid().f(ff.face_axe(), _P_, cidx);
            lx.push_back(p.value(_X_));
            ly.push_back(ff(_P_, cidx));
            lv.push_back(ff(_P_, cidx));
        }
    }
    auto aloc = ToGnuplotActor(lx, ly, lv);
    aloc.style("with points ps 2 pointtype 13 lc palette");
    return aloc;
}

template<class ANY>
auto _ToGnuplotActorContourPoints(
    const ANY& fcenter,
    SFieldCenterTag, Dim1Tag){
    
    std::list<double> lx, ly, lv;
    for(auto& cidx : fcenter.order()){
        auto p = fcenter.grid().c(cidx);
        lx.push_back(p.value(_X_));
        ly.push_back(fcenter(cidx));
        lv.push_back(fcenter(cidx));
    }
    auto aloc = ToGnuplotActor(lx, ly, lv);
    aloc.style("with points ps 2 pointtype 7 lc palette");
    return aloc;
}
template<class ANY>
auto _ToGnuplotActorContourPoints(const ANY& field, 
        SFieldCenterTag, Dim2Tag){
    std::list<double> lx, ly, lv;
    for(auto& cidx : field.order()){
        auto p = field.grid().c(cidx);
        lx.push_back(p.value(_X_));
        ly.push_back(p.value(_Y_));
        lv.push_back(field(cidx));
    }
    auto aloc = ToGnuplotActor(lx, ly, lv);
    aloc.style("with points ps 2 pointtype 7 lc palette");
    return aloc;
}

template<class ANY>
auto _ToGnuplotActorContourPoints(const ANY& field, 
        SFieldVertexTag, Dim2Tag){
    std::list<double> lx, ly, lv;
    for(auto& cidx : field.order()){
        auto p = field.grid().v(cidx);
        lx.push_back(p.value(_X_));
        ly.push_back(p.value(_Y_));
        lv.push_back(field(cidx));
    }
    auto aloc = ToGnuplotActor(lx, ly, lv);
    aloc.style("with points ps 2 pointtype 7 lc palette");
    return aloc;
}

template<class ANY>
auto _ToGnuplotActorContourPoints(const ANY& ff, SFieldFaceTag, Dim2Tag){
    std::list<double> lx, ly, lv;
    auto a = ff.face_axe();
    for(auto& fidx : ff.order()){
        auto cidx = ff.grid().face_index_to_cell_index(fidx, a);
        auto p = ff.grid().f(a, _M_, cidx);
        lx.push_back(p.value(_X_));
        ly.push_back(p.value(_Y_));
        lv.push_back(ff(_M_, cidx));

        if(ff.grid().is_last(cidx, a)){
            p = ff.grid().f(a, _P_, cidx);
            lx.push_back(p.value(_X_));
            ly.push_back(p.value(_Y_));
            lv.push_back(ff(_P_, cidx));
        }
    }
    auto aloc = ToGnuplotActor(lx, ly, lv);
    aloc.style("with points ps 2 pointtype 13 lc palette");
    return aloc;
}
template<class ANY>
GnuplotActorGroup _ToGnuplotActorContourPoints(const ANY& vector_face, SVectorFaceTag, Dim2Tag){
    GnuplotActorGroup gag;
    for(auto& spface: vector_face){
        auto& field_face = *spface;
        auto a = _ToGnuplotActorContourPoints(field_face, SFieldFaceTag());
        // a.show_command();
        gag.push_back(a);
    }
    return gag;    
}
template<class ANY>
auto _ToGnuplotActorContourPoints(const ANY& a, SFieldFaceTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourPoints(a, Tag(), DimTag()); 
}

template<class ANY>
auto _ToGnuplotActorContourPoints(const ANY& a, SVectorFaceTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourPoints(a, Tag(), DimTag()); 
}    
template<class ANY>
GnuplotActor _ToGnuplotActorContourPoints(const ANY& a, SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourPoints(a, Tag(), DimTag()); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorContourPoints(const ANY& a, SFieldVertexTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorContourPoints(a, Tag(), DimTag()); 
}
template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& vector_center, Vt unit_length, 
        SVectorCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorVectors(vector_center, unit_length, Tag(), DimTag());  
}
template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& ff, Vt unit_length, 
        SFieldFaceTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorVectors(ff, unit_length, Tag(), DimTag());  
}
template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& fc, Vt unit_length, 
        SFieldCenterTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorVectors(fc, unit_length, Tag(), DimTag());  
}
template<class ANY>
auto _ToGnuplotActorVectors(const ANY& vector_face, Vt unit_length, 
        SVectorFaceTag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _ToGnuplotActorVectors(vector_face, unit_length, Tag(), DimTag());  
}
template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& vc, Vt unit_length, 
        SVectorCenterTag, Dim1Tag){
    return _ToGnuplotActorVectors(vc[_X_], unit_length, SFieldCenterTag(), Dim1Tag());
}
template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& fc, Vt unit_length, 
        SFieldCenterTag, Dim1Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3:4:5 title \"\" ");
    actor.style("with vectors lw 2 lc palette head filled");
    if (unit_length <= 0){
        unit_length = fc.max() * 2.0;
    }
    auto gmin_size = fc.grid().min_size();

    for (auto &index : fc.order()){
        auto x = fc.grid().c_(_X_, index);
        Vt   y = 0.0;
        auto v = fc(index);
        auto dx = v / unit_length * gmin_size;
        Vt   dy = 0.0;
        
        actor.data().push_back(ToString(x, y, dx, dy, v, " "));
    }
    return actor;    
}
template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& vc, Vt unit_length, 
        SVectorCenterTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3:4:5 title \"\" ");
    actor.style("with vectors lw 2 lc palette head filled");
    if (unit_length <= 0)
    {
        auto xmax = vc[_X_].max() * 2.0;
        auto ymax = vc[_Y_].max() * 2.0;
        unit_length = std::max(xmax, ymax);
    }
    auto gmin_size = vc.grid().min_size();

    for (auto &index : vc.order())
    {
        auto x = vc.grid().c_(_X_, index);
        auto y = vc.grid().c_(_Y_, index);
        auto vx = vc[_X_](index);
        auto vy = vc[_Y_](index);
        auto dx = vx / unit_length * gmin_size;
        auto dy = vy / unit_length * gmin_size;

        auto v = std::sqrt(vx * vx + vy * vy);
        
        actor.data().push_back(ToString(x, y, dx, dy, v, " "));
    }
    return actor;
}

template<class ANY>
GnuplotActor _ToGnuplotActorVectors(const ANY& ff, Vt unit_length, 
        SFieldFaceTag, Dim2Tag){
    GnuplotActor actor;
    actor.command("using 1:2:3:4:5 title \"\" ");
    actor.style("with vectors lw 2 lc palette head filled");
    if (unit_length <= 0){
        unit_length = ff.max() * 2.0;;
    }
    auto gmin_size = ff.grid().min_size();
    auto a = ff.face_axe();

    for (auto &fidx : ff.order())
    {
        auto cidx = ff.grid().face_index_to_cell_index(fidx, a);
        auto fm = ff.grid().f(a, _M_, cidx);
        auto v  = ff(_M_, cidx);
        auto dl = v / unit_length * gmin_size;

        auto x = (a == _X_) ? fm(a) - (dl * 0.5) : fm.x();
        auto y = (a == _X_) ? fm.y() : (fm(a) - (dl * 0.5));
        auto dx = (a == _X_) ? dl : 0.0;
        auto dy = (a == _X_) ? 0.0 : dl;

        actor.data().push_back(ToString(x, y, dx, dy, v, " "));

        if(ff.grid().is_last(cidx, a)){
            fm = ff.grid().f(a, _P_, cidx);
            v  = ff(_P_, cidx);
            dl = v / unit_length * gmin_size;
            x  = (a == _X_) ? fm(a) - (dl * 0.5) : fm.x();
            y  = (a == _X_) ? fm.y() : (fm(a) - (dl * 0.5));
            dx = (a == _X_) ? dl : 0.0;
            dy = (a == _X_) ? 0.0 : dl;

            actor.data().push_back(ToString(x, y, dx, dy, v, " "));
        }
    }
    return actor;
}
template<class ANY>
auto _ToGnuplotActorVectors(const ANY& vf, Vt unit_length, 
        SVectorFaceTag, Dim2Tag){
   GnuplotActorGroup gag;
    for(auto& spface: vf){
        auto& field_face = *spface;
        auto a = _ToGnuplotActorVectors(field_face, SFieldFaceTag());
        gag.push_back(a);
    }
    return gag;     
}

template<class ANY>
GnuplotActor _ToGnuplotActorSection(const ANY& f, Axes a, Vt v, 
    SFieldCenterTag){
    EXPAND_FIELD_TAG(ANY);
    return _SFieldCenterToGASection(f, a, v, 
        ValueTag(), GridTag(), GhostTag(), OrderTag(), DimTag());
}

template<class ANY>
GnuplotActor _SFieldCenterToGASection(const ANY& f, Axes a, Vt v, 
    ArithmeticTag, SGridTag, SGhostTag, SOrderTag, Dim2Tag){
    // 2D Field Center
    GnuplotActor actor;
    actor.command("using 1:2:3 title \"\" ");
    actor.style("with points lw 2 lc palette");
    auto& grid = f.grid();
    for (auto &index : f.order()){
        Vt fpm = grid.f_(a, _M_, index);
        Vt fpp = grid.f_(a, _P_, index);
        if (IsInRange(fpm, v, fpp, _co_)) {
            Vt cor = grid.c_((a == _X_) ? _Y_ : _X_, index);
            Vt val = f(index);
            actor.data().push_back(ToString(cor, val, val," "));
        }
    }
    return actor;
}

}

#endif