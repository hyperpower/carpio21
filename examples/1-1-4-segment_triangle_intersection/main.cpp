#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>              Point2;
typedef Segment_<double, 2>            Seg2;
typedef Triangle_<double, 2>           Tri2;
typedef Box_<double, 2>                Box2;
typedef Line_<double>                  Line;
typedef GGnuplotActor_<double, 2>      GA;
typedef GGnuplotActorMaker_<double, 2> GAM;
typedef IntersectionSegTri_<double, 2> Inter;


// Forware Declare
std::array<double, 2> Xlim{{-0.5, 1.5}};
std::array<double, 2> Ylim{{-0.5, 1.5}};

void plot_triangle(Gnuplot& gnu, const Tri2& t);
void plot_segment(Gnuplot& gnu, const Seg2& s);
void plot_extend_line(Gnuplot& gnu, 
                      const Point2& p1, const Point2& p2);
int plot_point_location_code(const std::string& fname, const Tri2& t);
int plot_triangle_location(const std::string& fname, const Tri2& t);
int plot_segment_location(const std::string& fname, const Seg2& s);
void plot_intersection(const std::string& fname, 
                       const Tri2& t, 
                       const Seg2& s);
void plot_emphasis_by_triangle_code(Gnuplot& gnu, const Tri2& t, 
                                     int lc, int code);
void plot_emphasis_by_segment_code(Gnuplot& gnu, const Seg2& s, 
                                 int code);

int main(){
    std::string fn = "point_location_code";
    Point2 x(0.0, 0.0);
    Point2 y(1.0, 1.0);
    Seg2 seg(x, y);
    Point2 t0(0,   0);
    Point2 t1(1.,  -0.1);
    Point2 t2(0.2, 1.1);
    Tri2 tri(t0, t1, t2);
    plot_point_location_code(fn, tri);

    fn = "triangle_location";
    plot_triangle_location(fn, tri);
    fn = "segment_location";
    plot_segment_location(fn, seg);

    fn = "t_intersection";
    Point2 x2(0.1, 0.1);
    Point2 y2(0.4, -0.4);
    Seg2 seg2(x2, y2);
    plot_intersection(fn, tri, seg2);
    
}

void plot_intersection(const std::string& fname, const Tri2& t, const Seg2& s){
    Gnuplot gnu;
    gnu.set_xrange(Xlim[0], Xlim[1]+0.5);
    gnu.set_yrange(Ylim[0], Ylim[1]);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + fname);
    plot_extend_line(gnu, t[0], t[1]);
    plot_extend_line(gnu, t[0], t[2]);
    plot_triangle(gnu, t); 
    plot_segment(gnu,  s);

    Inter inter(t, s);
    inter.cal_intersect();

    int l0 = inter.location_code(0);
    int l1 = inter.location_code(1);

    int ct0 = inter.triangle_intersection_code(0);
    int ct1 = inter.triangle_intersection_code(1);
    int cs0 = inter.segment_intersection_code(0);
    int cs1 = inter.segment_intersection_code(1);

    plot_emphasis_by_triangle_code(gnu, t, l0, ct0);
    if(ct0 != ct1){
        plot_emphasis_by_triangle_code(gnu, t, l1, ct1);    
    }

    plot_emphasis_by_segment_code(gnu, s, cs0);
    if(cs0 != cs1){
        plot_emphasis_by_segment_code(gnu, s, cs1);
    }
    gnu.set_label(1, "Location Code0 = " + ToString(l0), 1.1, 1.3, "front font \",10\"");
    gnu.set_label(2, "Location Code2 = " + ToString(l1), 1.1, 1.2, "front font \",10\"");

    gnu.set_label(3, "Code Triangle = " + ToString(ct0) + ", " + ToString(ct1),
                     1.1, 1.1, "front font \",10\"");
    gnu.set_label(4, "Code Segment  = " + ToString(cs0) + ", " + ToString(cs1),
                     1.1, 1.0, "front font \",10\"");
    
    

    gnu.plot();
}
void plot_emphasis_by_segment_code(Gnuplot& gnu, const Seg2& s, 
                                 int code){
    std::cout << "intersection code  = " << code << std::endl;
    GAM gam;
    switch(code){
        case 0:{
            GAM::spActor actor = gam.points(s[0], -1);
            actor->style() = "with points pt 7 ps 3 lc rgb \"green\"";
            gnu.add(actor);
        };break;
        case 1:{
            GAM::spActor actor = gam.points(s[1], -1);
            actor->style() = "with points pt 7 ps 3 lc rgb \"green\"";
            gnu.add(actor);
        };break;
        case 2:{
            Seg2 tseg(s[0], s[1]);
            tseg.scale(tseg.mid(), 0.6, 0.6);
            GAM::spActor actor = gam.points(tseg.begin(), tseg.end());
            actor->style() = "with lines lw 3 lc rgb \"green\"";
            gnu.add(actor);
        };break;
    }
}

void plot_emphasis_by_triangle_code(Gnuplot& gnu, const Tri2& t, 
                                     int lc, int code){
    std::cout << "loction      code  = " << lc << std::endl;
    std::cout << "intersection code  = " << code << std::endl;
    GAM gam;
    switch(code){
        case 0:{
            GAM::spActor actor = gam.points(t[0], -1);
            actor->style() = "with points pt 7 ps 3 lc rgb \"blue\"";
            gnu.add(actor);
        };break;
        case 1:{
            GAM::spActor actor = gam.points(t[1], -1);
            actor->style() = "with points pt 7 ps 3 lc rgb \"blue\"";
            gnu.add(actor);
        };break;
        case 2:{
            GAM::spActor actor = gam.points(t[2], -1);
            actor->style() = "with points pt 7 ps 3 lc rgb \"blue\"";
            gnu.add(actor);
        };break;
        case 3:{ //edge 0-1
            Seg2 tseg(t[0], t[1]);
            tseg.scale(tseg.mid(), 0.6, 0.6);
            GAM::spActor actor = gam.points(tseg.begin(), tseg.end());
            actor->style() = "with lines lw 3 lc rgb \"blue\"";
            gnu.add(actor);
        };break;
        case 4:{ //edge 0-2
            Seg2 tseg(t[0], t[2]);
            tseg.scale(tseg.mid(), 0.6, 0.6);
            GAM::spActor actor = gam.points(tseg.begin(), tseg.end());
            actor->style() = "with lines lw 3 lc rgb \"blue\"";
            gnu.add(actor);
        };break;
        case 5:{ //edge 0-2
            Seg2 tseg(t[1], t[2]);
            tseg.scale(tseg.mid(), 0.6, 0.6);
            GAM::spActor actor = gam.points(tseg.begin(), tseg.end());
            actor->style() = "with lines lw 3 lc rgb \"blue\"";
            gnu.add(actor);
        };break;
        case 6:{ // inside triangle
            Tri2 ts(t);
            ts.scale(0.9, 0.9);
            for(auto& p : ts){
                std::cout << p << std::endl;
            }
            gnu.set_object2d(1, ts, "fillstyle transparent solid 0.2 lw 0 fc rgb \"web-blue\"");
        };break;
        case 7:{ //out side
        };break;
    };
}

void plot_triangle(Gnuplot& gnu, const Tri2& t){
    GAM gam;
    for(int i = 0; i < 3; i++){
        GAM::spActor actor = gam.points(t[i], -1);
        actor->style() = "with points pt 2 ps 2 lc rgb \"web-blue\"";
        gnu.add(actor);
    }
    // plot edge
    std::array<std::array<int, 2>, 3> arridx = {{
        {{0, 1}},{{0,2}},{{1, 2}}
    }}; 
    for(int i = 0; i < 3; i++){
        auto& pair = arridx[i];    
        auto& p0 = t[pair[0]];
        auto& p1 = t[pair[1]];
        auto actor = gam.arrows(p0,p1);
        actor->style() = "with vectors filled lw 2 lc rgb \"web-blue\"";
        gnu.add(actor);
    }
}

void plot_segment(Gnuplot& gnu, const Seg2& s){
    GAM gam;
    // plot edge
    auto actor = gam.arrows(s[0],s[1]);
    actor->style() = "with vectors filled lw 2 lc rgb \"web-green\"";
    gnu.add(actor);
}

void plot_extend_line(Gnuplot& gnu, const Point2& p1, const Point2& p2){
    GAM gam;
    Line l(p1, p2);
    auto a = gam.lines(l, Xlim[0], Xlim[1]);
    a->style() = "with lines lt 3 lw 2 lc rgb \"grey\"";
    gnu.add(a);
}

int plot_point_location_code(const std::string& fname, const Tri2& t){
    Gnuplot gnu;
    gnu.set_xrange(Xlim[0], Xlim[1]);
    gnu.set_yrange(Ylim[0], Ylim[1]);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + fname);
    gnu.set_label(1, "0 ", 0.03, 0.05, "front font \",18\"");
    gnu.set_label(2, " 1 ", 0.5, -0.05, "front font \",18\"");
    gnu.set_label(3, " 2 ", 0.3, 0.3, "front font \",18\"");
    gnu.set_label(4, " 3 ", 0.0, 0.5, "front font \",18\"");
    gnu.set_label(5, " 4 ", -0.2, 0.3, "front font \",18\"");
    gnu.set_label(6, " 5 ", -0.3, 0.0, "front font \",18\"");
    gnu.set_label(7, "6", -0.3, -0.3, "front font \",18\"");
    gnu.set_label(8, "7", -0.1, -0.3, "front font \",18\"");
    gnu.set_label(9, "8", 0.3, -0.2, "front font \",18\"");
    plot_extend_line(gnu, t[0], t[1]);
    plot_extend_line(gnu, t[0], t[2]);
    plot_triangle(gnu, t);    
    gnu.plot();
    return 0;
}
int plot_triangle_location(const std::string& fname, const Tri2& t){
    Gnuplot gnu;
    gnu.set_xrange(Xlim[0], Xlim[1]);
    gnu.set_yrange(Ylim[0], Ylim[1]);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + fname);
    gnu.set_label(1, "0", 0.03, 0.05, "front font \",18\"");
    gnu.set_label(2, "1", 1.0, -0.1, "front font \",18\"");
    gnu.set_label(3, "2", 0.2, 1.1, "front font \",18\"");
    gnu.set_label(4, " 3 ", 0.5, -0.05, "front font \",18\"");
    gnu.set_label(5, " 4 ", 0.0, 0.5, "front font \",18\"");
    gnu.set_label(6, " 5 ", 0.5, 0.5, "front font \",18\"");
    gnu.set_label(7, "6", 0.3, 0.3, "front font \",18\"");
    gnu.set_label(8, "7", -0.3, -0.3, "front font \",18\"");
    plot_extend_line(gnu, t[0], t[1]);
    plot_extend_line(gnu, t[0], t[2]);
    plot_triangle(gnu, t);    
    gnu.plot();
    return 0;
}
int plot_segment_location(const std::string& fname, const Seg2& s){
    Gnuplot gnu;
    gnu.set_xrange(Xlim[0], Xlim[1]);
    gnu.set_yrange(Ylim[0], Ylim[1]);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + fname);
    gnu.set_label(1, "0", s[0][0],s[0][1], "front font \",18\"");
    gnu.set_label(2, "1", s[1][0],s[1][1], "front font \",18\"");
    gnu.set_label(3, "2", 0.5, 0.5, "front font \",18\"");
    gnu.set_label(4, "3", 0.6, 0.0, "front font \",18\"");
    gnu.set_label(5, "3", 0.0, 0.6, "front font \",18\"");
    plot_extend_line(gnu, s[0], s[1]);
    GAM gam;
    auto a = gam.arrows(s[0], s[1]);
    a->style() = "with vectors filled lw 2 lc rgb \"web-blue\"";
    gnu.add(a);
    // auto b = gam.lines_points(s);
    // b->style() = "with points pt 2 ps 2 rgb \"web-blue\"";
    // gnu.add(b);
    gnu.plot();
    return 0;
}
