#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>              Point2;
typedef Segment_<double, 2>            Seg2;
typedef Box_<double, 2>                Box2;
typedef Line_<double>                  Line;
typedef GGnuplotActor_<double, 2>      GA;
typedef GGnuplotActorMaker_<double, 2> GAM;



// Forware Declare
int  line_box_case(double a, double b, double alpha, std::string name);
void BoxLinePositiveCase(int num_case, 
                         double x, double y,
                         double a, double b, double alpha);
void RotateLineCutBox();

int main(){
    int r = line_box_case(1.5, 1.0, 1.3, "line_box_normal");
    line_box_case(1.5, 1.0, 0.0, "line_box_corner");
    line_box_case(0.0, 1.0, 1.0, "line_box_edge");
    RotateLineCutBox();
}

int line_box_case(double a, double b, double alpha, std::string name){
    std::cout << "Initial box " << " >>>>>>> " << std::endl;
    Point2 min1(0, 0);
    Point2 max1(1, 1);
    Box2 box1(min1, max1);
    std::cout << "The box is " << box1 << std::endl;

    Line line(a, b, alpha);
    std::cout << "Line is " << line << std::endl;

    auto lspp = IntersectLineBox(max1, line);
    std::cout << "Result points : " << lspp.size() << std::endl;
    for(auto& sp : lspp){
        std::cout << "   " << *sp << std::endl;
    }
    
    Gnuplot gnu;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + name);
    auto spbox1 = GA::Lines(box1);
    spbox1->style() = "with lines lw 2 lc 8";
    gnu.add(spbox1);
    auto spline = GA::Lines(line, -0.5, 1.5);
    spline->style() = "with lines lw 3 lc 6";
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for(auto& sp : lspp){
        auto sppoint = GA::Points(*sp);
        sppoint->style() = "with points pt 7 ps 2 lc 7";
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", sp->x(), sp->y()),
                     sp->x(), sp->y()+ 0.05, "left font \",16\"");
        gnu.add(sppoint);
        numlabel++;
    }
    gnu.set_label(1,"(1.0, 1.0)", 1.0, 1.05, "center font \",16\"");
    gnu.set_label(2,"(0.0, 0.0)", 0.0, -0.05, "center font \",16\"");

    gnu.set_label(4,tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(), line.alpha()),
                    0.1, 1.3, "left font \",16\"");
    gnu.plot();

    std::cout << "End of Case " << name << " <<<<<<<< "<< std::endl;
    return 0;
}

void RotateLineCutBox(){
    int n = 100;
    Vt dt = 3.1415926 / double(n);
    for(int i = 0; i<n; i++){
        Vt x1 = 0.5, y1 = 0.5;
        Vt r = 1;
        Vt theta = i * dt;
        Vt x2 = x1 + r * std::cos(theta);
        Vt y2 = y1 + r * std::sin(theta);
        Vt a = y1 - y2;
        Vt b = x2 - x1;
        Vt alpha = x2 * y1 - x1 * y2;

        BoxLinePositiveCase(i, 1, 1, a, b, alpha);
    }
}

void BoxLinePositiveCase(int num_case,
        double x, double y,
        double a, double b, double alpha) {
    Point2 min1(0.0, 0.0);
    Point2 max1(y, y);
    Box2 box1(min1, max1);

    Line line(a, b, alpha);

    auto lspp = IntersectLineNegativeBox(box1, line);

    Gnuplot gnu;
    GAM gam;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png(tfm::format("./fig/lb_%02d.png", num_case));
    auto spbox1 = gam.lines(box1);
    spbox1->style() = "with lines lw 2 lc 8";
    gnu.add(spbox1);
    auto spline = gam.lines(line, -0.5, 1.5);
    spline->style() = "with lines lw 3 lc 6";
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for (auto& sp : lspp) {
        auto sppoint = gam.points(*sp);
        sppoint->style() = "with points pt 7 ps 2 lc 7";
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", sp->x(), sp->y()),
                sp->x(), sp->y() + 0.05, "left font \",16\"");
        gnu.add(sppoint);
        numlabel++;
    }
    gnu.set_object2dp(5, lspp,
            "fillstyle transparent solid 0.3 fc rgb \"blue\"");

//    gnu.set_label(1, tfm::format("(%.1f, %.1f)", min1.x(), min1.y()), min1.x(),
//            min1.y() + 0.06, "center font \",16\"");
//    gnu.set_label(2, tfm::format("(%.1f, %.1f)", max1.x(), max1.y()), max1.x(),
//            max1.y() + 0.06, "center font \",16\"");

    gnu.set_label(4,
            tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(),
                    line.alpha()), 0.1, 1.3, "left font \",16\"");
    gnu.plot();
}
