#ifndef _S_GNUPLOT_ACTOR_COUTOUR_POINTS_HPP
#define _S_GNUPLOT_ACTOR_COUTOUR_POINTS_HPP

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

// ==============================================
//   Top Level 
// ==============================================
    template<class ANY>
    auto _ToGnuplotActorContourPoints(const ANY& a, SFieldFaceTag){
        typedef typename ANY::Tag Tag;
        typedef typename ANY::DimTag DimTag;
        return _SToGAContourPoints(a, Tag(), DimTag()); 
    }
    template<class ANY>
    auto _ToGnuplotActorContourPoints(const ANY& a, SVectorFaceTag){
        typedef typename ANY::Tag Tag;
        typedef typename ANY::DimTag DimTag;
        return _SToGAContourPoints(a, Tag(), DimTag()); 
    }    
    template<class ANY>
    GnuplotActor _ToGnuplotActorContourPoints(const ANY& a, SFieldCenterTag){
        typedef typename ANY::Tag Tag;
        typedef typename ANY::DimTag DimTag;
        return _SToGAContourPoints(a, Tag(), DimTag()); 
    }
    template<class ANY>
    GnuplotActor _ToGnuplotActorContourPoints(const ANY& a, SFieldVertexTag){
        typedef typename ANY::Tag Tag;
        typedef typename ANY::DimTag DimTag;
        return _SToGAContourPoints(a, Tag(), DimTag()); 
    }
    template<class ANY>
    GnuplotActor _ToGnuplotActorContourPoints(const ANY& a, 
        const typename ANY::Index& idx, SFieldVertexTag){
        EXPAND_FIELD_TAG(ANY);
        return _SToGAContourPoints(a, idx, FieldTag(), ValueTag(), DimTag()); 
    }
    template<class ANY>
    GnuplotActor _ToGnuplotActorContourPoints(const ANY& a, const BoundaryIndex& bi,
        SFieldVertexTag){
        typedef typename ANY::Tag Tag;
        typedef typename ANY::DimTag DimTag;
        return _SToGAContourPoints(a, bi, Tag(), DimTag()); 
    }


// ==============================================
//   Local Level 
// ==============================================

    template<class ANY>
    auto _SToGAContourPoints(
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
    auto _SToGAContourPoints(
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
    auto _SToGAContourPoints(const ANY& field, 
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
    auto _SToGAContourPoints(const ANY& field, 
            SFieldVertexTag, Dim2Tag){
        std::list<double> lx, ly, lv;
        for(auto& idx : field.order()){
            auto p = field.grid().v(idx);
            lx.push_back(p.value(_X_));
            ly.push_back(p.value(_Y_));
            lv.push_back(field(idx));
        }
        auto aloc = ToGnuplotActor(lx, ly, lv);
        aloc.style("with points ps 2 pointtype 7 lc palette");
        return aloc;
    }

    template<class ANY>
    auto _SToGAContourPoints(const ANY& field, const typename ANY::Index& idx,
            SFieldVertexTag, LinearPolynomialTag, Dim2Tag){
        auto& grid = field.grid();
        std::list<double> lx, ly, lv;
        auto pc    = grid.v(idx);
        auto& exp  = field(idx);
        for(auto iter = exp.begin(); iter != exp.end(); iter++){
            auto& idxg = iter->first;
            auto& coe  = iter->second;
            auto pg = grid.v(idxg);
            lx.push_back(pg.value(_X_));
            ly.push_back(pg.value(_Y_));
            lv.push_back(coe);
        }
        auto aloc = ToGnuplotActor(lx, ly, lv);
        aloc.style("with points ps 2 pointtype 7 lc palette");
        return aloc;
    }
    
    template<class ANY>
    auto _SToGAContourPoints(const ANY& field, const BoundaryIndex& bi, 
            SFieldVertexTag, Dim2Tag){
        std::list<double> lx, ly, lv;
        for(auto& idx : field.order()){
            auto p = field.grid().v(idx);
            lx.push_back(p.value(_X_));
            ly.push_back(p.value(_Y_));
            lv.push_back(field(idx));
            for (auto& idxg : CrossListIndex(idx)){
                if(field.ghost().is_ghost_vertex(idxg)){
                    auto pg = field.grid().v(idxg);
                    auto pv = FindBoundaryValue(field, bi, idx, idxg, 0.0);
                    lx.push_back(pg.value(_X_));
                    ly.push_back(pg.value(_Y_));
                    lv.push_back(pv);
                }
            }
        }
        auto aloc = ToGnuplotActor(lx, ly, lv);
        aloc.style("with points ps 2 pointtype 7 lc palette");
        return aloc;
    }
    template<class ANY>
    auto _SToGAContourPoints(const ANY& ff, SFieldFaceTag, Dim2Tag){
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
    GnuplotActorGroup _SToGAContourPoints(const ANY& vector_face, SVectorFaceTag, Dim2Tag){
        GnuplotActorGroup gag;
        for(auto& spface: vector_face){
            auto& field_face = *spface;
            auto a = _ToGnuplotActorContourPoints(field_face, SFieldFaceTag());
            // a.show_command();
            gag.push_back(a);
        }
        return gag;    
    }
    

}

#endif