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

// Forware Declare
std::array<double, 2> Xlim{{-0.5, 1.5}};
std::array<double, 2> Ylim{{-0.5, 1.5}};

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
}


