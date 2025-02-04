#include "gtest/gtest.h"
#include "test_define.hpp"
#include "domain/structure/field/sfield_face.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "domain/structure/operator/sapply_bc.hpp"

#include "domain/structure/operator/scommon.hpp"

using namespace carpio;

void GnuplotAddGhostCenterValue(Gnuplot& gnu, double x, double y){
        std::list<double> lx, ly, lv;
        lx.push_back(x);
        ly.push_back(y);
        lv.push_back(y);
        auto a = ToGnuplotActor(lx,ly,lv);
        a.style("with points ps 2 pointtype 9 lc palette");
        gnu.add(a);
}
void GnuplotAddBCValue(Gnuplot& gnu, double x, double y){
        std::list<double> lx, ly, lv;
        lx.push_back(x);
        ly.push_back(y);
        lv.push_back(y);
        auto a = ToGnuplotActor(lx,ly,lv);
        a.style("with points ps 2 pointtype 7 lc palette");
        gnu.add(a);
        lx.clear(); ly.clear(); lv.clear();
        lx.push_back(x);  ly.push_back(0.0);
        lx.push_back(x);  ly.push_back(y);
        auto al = ToGnuplotActor(lx,ly);
        al.style("with line lt 0 lw 1.5");
        al.line_color_black();
        gnu.add(al);
}


TEST(bc_on_dim1, type1){

    typedef SGridUniform_<1> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<1, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<1, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<1, double, Grid, Ghost, Order> Field;
    typedef typename Field::Index Index;
    typedef SFieldFace_<1, double, Grid, Ghost, Order> FieldFace;

    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));
    spGhost spghost(new Ghost(spgrid));

    Field f(spgrid, spghost);
    FieldFace ff(spgrid, spghost, _X_);

    f.assign([](typename FieldFace::ValueType x,
                 typename FieldFace::ValueType y,
                 typename FieldFace::ValueType z
                ){
        return 1.0 * std::sin(x) + 1.0;
    });

    

    std::cout << "ff(_M_, 2)  = " << ff(_M_, 2) << std::endl;
    int gl = f.grid().ghost_layer();
    std::cout << "ghost layer = " << gl << std::endl;
    EXPECT_EQ(gl, 2);

    for(auto index : f.order()){
        std::cout << index;
        std::cout << "  is boundary  = "<< 
        ToString(
            f.ghost().is_boundary(index, _X_, _P_) ||
            f.ghost().is_boundary(index, _X_, _M_) 
        ) << std::endl;
    }

    BoundaryConditionValue bc(1, 0.8);

    Gnuplot gnu;
    auto fun = [&gnu, &bc](Field& f, const Index& idx){
        auto& grid  = f.grid();
        auto& ghost = f.ghost();
        int gl = f.grid().ghost_layer();
        if(ghost.is_boundary(idx, _X_, _M_)){
            auto pb = grid.f(_X_, _M_, idx); 
            GnuplotAddBCValue(gnu, pb.value(_X_),bc.value());
            Index gidx(idx);
            for(int step = 1; step <= gl; step++){
                Shift(gidx, _X_, _M_, 1);
                auto gv = GetGhostCenterValue(f, bc, idx, gidx, _X_, _M_);
                auto p = grid.c(gidx);
                GnuplotAddGhostCenterValue(gnu, p.value(_X_), gv);
            }
        }
        if(ghost.is_boundary(idx, _X_, _P_)){
            auto pb = grid.f(_X_, _P_, idx); 
            GnuplotAddBCValue(gnu, pb.value(_X_),bc.value());
            Index gidx(idx);
            for(int step = 1; step <= gl; step++){
                Shift(gidx, _X_, _P_, 1);
                auto gv = GetGhostCenterValue(f, bc, idx, gidx, _X_, _P_);
                auto p = grid.c(gidx);
                GnuplotAddGhostCenterValue(gnu, p.value(_X_), gv);
            }
        }
    };

    ForEach(f, fun);
    
    auto gv = GetGhostCenterValue(f, bc, Index(6), Index(7), _X_, _P_);
    std::cout << "Index(6) = " << f(Index(6)) << std::endl;
    std::cout << "Index(7) = " << gv << std::endl;
    // auto ag1 = ToGnuplotActorLabel(f.grid().c(Index(7)).value(_X_), gv-0.2, ToString(gv));
    // gnu.add(ag1);

    gv = GetGhostCenterValue(f, bc, Index(5), Index(7), _X_, _P_);
    std::cout << "Index(5) = " << f(Index(5)) << std::endl;
    std::cout << "Index(7) = " << gv << std::endl;

    gnu.set_xrange(-1.2, 4.7);
    gnu.set_yrange(-2.2, 3.0);
    gnu.set_xlabel("X");
    gnu.set_ylabel("Value");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    gnu.set_palette_blue_red();
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    agrid.line_color_black();
    gnu.add(agrid);

    auto aghost = ToGnuplotActorWireFrame(*spghost);
    aghost.title("Ghost");
    aghost.line_color_blue();
    aghost.line_width(2);
    gnu.add(aghost);


    // auto aidx = ToGnuplotActorLabel(ff, "index");
    // gnu.add(aidx);

    gnu.set_terminal_png(FIG_PATH + "SturctureFieldFace1d_BC1", 
                        fig_width, fig_height);
    
    auto aloc = ToGnuplotActorContourPoints(f);
    aloc.title("value on x ");
    gnu.add(aloc);
    
    gnu.plot();
}

TEST(bc_on_dim1, type2){

    typedef SGridUniform_<1> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<1, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<1, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<1, double, Grid, Ghost, Order> Field;
    typedef typename Field::Index Index;
    typedef SFieldFace_<1, double, Grid, Ghost, Order> FieldFace;

    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));
    spGhost spghost(new Ghost(spgrid));

    Field f(spgrid, spghost);
    FieldFace ff(spgrid, spghost, _X_);

    f.assign([](typename FieldFace::ValueType x,
                 typename FieldFace::ValueType y,
                 typename FieldFace::ValueType z
                ){
        return 1.0 * std::sin(x) + 1.0;
    });

    

    std::cout << "ff(_M_, 2)  = " << ff(_M_, 2) << std::endl;
    int gl = f.grid().ghost_layer();
    std::cout << "ghost layer = " << gl << std::endl;
    EXPECT_EQ(gl, 2);

    for(auto index : f.order()){
        std::cout << index;
        std::cout << "  is boundary  = "<< 
        ToString(
            f.ghost().is_boundary(index, _X_, _P_) ||
            f.ghost().is_boundary(index, _X_, _M_) 
        ) << std::endl;
    }

    BoundaryConditionValue bc(2, 0.0);

    Gnuplot gnu;
    auto fun = [&gnu, &bc](Field& f, const Index& idx){
        auto& grid  = f.grid();
        auto& ghost = f.ghost();
        int gl = f.grid().ghost_layer();
        if(ghost.is_boundary(idx, _X_, _M_)){
            auto pb = grid.f(_X_, _M_, idx); 
            // GnuplotAddBCValue(gnu, pb.value(_X_),bc.value());
            Index gidx(idx);
            for(int step = 1; step <= gl; step++){
                Shift(gidx, _X_, _M_, 1);
                auto gv = GetGhostCenterValue(f, bc, idx, gidx, _X_, _M_);
                auto p = grid.c(gidx);
                GnuplotAddGhostCenterValue(gnu, p.value(_X_), gv);
            }
        }
        if(ghost.is_boundary(idx, _X_, _P_)){
            // auto pb = grid.f(_X_, _P_, idx); 
            // GnuplotAddBCValue(gnu, pb.value(_X_),bc.value());
            Index gidx(idx);
            for(int step = 1; step <= gl; step++){
                Shift(gidx, _X_, _P_, 1);
                auto gv = GetGhostCenterValue(f, bc, idx, gidx, _X_, _P_);
                auto p = grid.c(gidx);
                GnuplotAddGhostCenterValue(gnu, p.value(_X_), gv);
            }
        }
    };

    ForEach(f, fun);
    
    auto gv = GetGhostCenterValue(f, bc, Index(6), Index(7), _X_, _P_);
    std::cout << "Index(6) = " << f(Index(6)) << std::endl;
    std::cout << "Index(7) = " << gv << std::endl;
    // auto ag1 = ToGnuplotActorLabel(f.grid().c(Index(7)).value(_X_), gv-0.2, ToString(gv));
    // gnu.add(ag1);

    gv = GetGhostCenterValue(f, bc, Index(5), Index(7), _X_, _P_);
    std::cout << "Index(5) = " << f(Index(5)) << std::endl;
    std::cout << "Index(7) = " << gv << std::endl;

    gnu.set_xrange(-1.2, 4.7);
    gnu.set_yrange(-0.5, 3.0);
    gnu.set_xlabel("X");
    gnu.set_ylabel("Value");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    gnu.set_palette_blue_red();
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    agrid.line_color_black();
    gnu.add(agrid);

    auto aghost = ToGnuplotActorWireFrame(*spghost);
    aghost.title("Ghost");
    aghost.line_color_blue();
    aghost.line_width(2);
    gnu.add(aghost);


    // auto aidx = ToGnuplotActorLabel(ff, "index");
    // gnu.add(aidx);

    gnu.set_terminal_png(FIG_PATH + "SturctureFieldFace1d_BC2", 
                        fig_width, fig_height);
    
    auto aloc = ToGnuplotActorContourPoints(f);
    aloc.title("value on x ");
    gnu.add(aloc);
    
    gnu.plot();
}

TEST(bc_on_dim1, type3){

    typedef SGridUniform_<1> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<1, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<1, Grid, Ghost> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<1, double, Grid, Ghost, Order> Field;
    typedef typename Field::Index Index;
    typedef SFieldFace_<1, double, Grid, Ghost, Order> FieldFace;

    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    spGrid spgrid(new Grid(pmin, {7, 7}, 0.5, 2));
    spGhost spghost(new Ghost(spgrid));

    Field f(spgrid, spghost);
    FieldFace ff(spgrid, spghost, _X_);

    f.assign([](typename FieldFace::ValueType x,
                 typename FieldFace::ValueType y,
                 typename FieldFace::ValueType z
                ){
        return 1.0 * std::sin(x) + 1.0;
    });

    std::cout << "ff(_M_, 2)  = " << ff(_M_, 2) << std::endl;
    int gl = f.grid().ghost_layer();
    std::cout << "ghost layer = " << gl << std::endl;
    EXPECT_EQ(gl, 2);

    for(auto index : f.order()){
        std::cout << index;
        std::cout << "  is boundary  = "<< 
        ToString(
            f.ghost().is_boundary(index, _X_, _P_) ||
            f.ghost().is_boundary(index, _X_, _M_) 
        ) << std::endl;
    }

    BoundaryConditionValue bc(3, 0.0);

    Gnuplot gnu;
    auto fun = [&gnu, &bc](Field& f, const Index& idx){
        auto& grid  = f.grid();
        auto& ghost = f.ghost();
        int gl = f.grid().ghost_layer();
        if(ghost.is_boundary(idx, _X_, _M_)){
            auto pb = grid.f(_X_, _M_, idx); 
            // GnuplotAddBCValue(gnu, pb.value(_X_),bc.value());
            Index gidx(idx);
            for(int step = 1; step <= gl; step++){
                Shift(gidx, _X_, _M_, 1);
                auto gv = GetGhostCenterValue(f, bc, idx, gidx, _X_, _M_);
                auto p = grid.c(gidx);
                GnuplotAddGhostCenterValue(gnu, p.value(_X_), gv);
            }
        }
        if(ghost.is_boundary(idx, _X_, _P_)){
            // auto pb = grid.f(_X_, _P_, idx); 
            // GnuplotAddBCValue(gnu, pb.value(_X_),bc.value());
            Index gidx(idx);
            for(int step = 1; step <= gl; step++){
                Shift(gidx, _X_, _P_, 1);
                auto gv = GetGhostCenterValue(f, bc, idx, gidx, _X_, _P_);
                auto p = grid.c(gidx);
                GnuplotAddGhostCenterValue(gnu, p.value(_X_), gv);
            }
        }
    };

    ForEach(f, fun);
    
    auto gv = GetGhostCenterValue(f, bc, Index(6), Index(7), _X_, _P_);
    std::cout << "Index(6) = " << f(Index(6)) << std::endl;
    std::cout << "Index(7) = " << gv << std::endl;
    // auto ag1 = ToGnuplotActorLabel(f.grid().c(Index(7)).value(_X_), gv-0.2, ToString(gv));
    // gnu.add(ag1);

    gv = GetGhostCenterValue(f, bc, Index(5), Index(7), _X_, _P_);
    std::cout << "Index(5) = " << f(Index(5)) << std::endl;
    std::cout << "Index(7) = " << gv << std::endl;

    gnu.set_xrange(-1.2, 4.7);
    gnu.set_yrange(-0.5, 3.0);
    gnu.set_xlabel("X");
    gnu.set_ylabel("Value");
    gnu.set_equal_aspect_ratio();
    gnu.set_key_spacing(1.5);
    gnu.set_palette_blue_red();
    
    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.title("Grid");
    agrid.line_width(2);
    agrid.line_color_black();
    gnu.add(agrid);

    auto aghost = ToGnuplotActorWireFrame(*spghost);
    aghost.title("Ghost");
    aghost.line_color_blue();
    aghost.line_width(2);
    gnu.add(aghost);

    gnu.set_terminal_png(FIG_PATH + "SturctureFieldFace1d_BC3", 
                        fig_width, fig_height);
    
    auto aloc = ToGnuplotActorContourPoints(f);
    aloc.title("value on x ");
    gnu.add(aloc);
    
    gnu.plot();
}