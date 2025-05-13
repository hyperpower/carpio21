#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>              Point2;
typedef Segment_<double, 2>            Seg2;
typedef Box_<double, 2>                Box2;
typedef Line_<double>                  Line;


auto AddAxes(Gnuplot& gnu){
    gnu.set_label(10, "x",  0.45, -0.08, "textcolor rgb \"#00A4EF\"");
    auto actor_x = ToGnuplotActorVector(Seg2(Point2(0.0,0.0), Point2(0.5,0.0)));
    actor_x.style("with vectors lt 1 lw 3 lc rgb \"#00A4EF\"");
    gnu.add(actor_x);

    gnu.set_label(11, "y", -0.08, 0.45, "textcolor rgb \"#F25022\"");
    auto actor_y = ToGnuplotActorVector(Seg2(Point2(0.0,0.0), Point2(0.0,0.5)));
    actor_y.style("with vectors lt 1 lw 3 lc rgb \"#F25022\"");
    gnu.add(actor_y);
}

int LineBox(double a, double b, double alpha, const std::string& name){
    std::cout << "Initial box " << " >>>>>>> " << std::endl;
    Point2 min1(0, 0);
    Point2 max1(1, 1);
    Box2 box1(min1, max1);
    std::cout << "The box is " << box1 << std::endl;

    Line line(a, b, alpha);
    std::cout << "Line is " << line << std::endl;

    auto lspp = IntersectLineBox(min1, max1, line.a(), line.b(), line.alpha());
    std::cout << "Result points : " << lspp.size() << std::endl;
    for(auto& sp : lspp){
        std::cout << "   " << sp << std::endl;
    }
    
    Gnuplot gnu;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/" + name);
    AddAxes(gnu);
    auto spbox1 = ToGnuplotActor(box1);
    spbox1.style("with lines lw 2 lc 8");
    gnu.add(spbox1);
    std::array<double,2> line_range{-0.5, 1.5};
    auto spline = ToGnuplotActor(line, line_range);
    spline.style("with lines lw 3 lc 6");
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for(auto& sp : lspp){
        auto sppoint = ToGnuplotActor(sp);
        sppoint.style("with points pt 7 ps 2 lc 7");
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", sp.x(), sp.y()),
                     sp.x(), sp.y()+ 0.05, "left font \",13\"");
        gnu.add(sppoint);
        numlabel++;
    }
    // gnu.set_label(1,"(1.0, 1.0)", 1.0, 1.05, "textcolor rgb \"#7FBA00\" center font \",13\"");
    // gnu.set_label(2,"(0.0, 0.0)", 0.0, -0.05, "textcolor rgb \"#7FBA00\" center font \",13\"");

    gnu.set_label(4,tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(), line.alpha()),
                    0.1, 1.3, "left font \",13\"");


    gnu.plot();

    std::cout << "End of Case " << name << " <<<<<<<< "<< std::endl;
    return 0;
}

void BoxLinePositiveCase(int num_case,
        double x, double y,
        double a, double b, double alpha) {
    Point2 min1(-0.1, -0.2);
    Point2 max1(x, y);
    Box2 box1(min1, max1);

    Line line(a, b, alpha);

    auto lspp = PositiveLineBox(min1, max1, line.a(), line.b(), line.alpha());

    Gnuplot gnu;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png(tfm::format("./fig/lb_%02d", num_case));
    AddAxes(gnu);
    auto spbox1 = ToGnuplotActor(box1);
    spbox1.style("with lines lw 2 lc 8");
    gnu.add(spbox1);
    std::array<double,2> line_range{-0.5, 1.5};
    auto spline = ToGnuplotActor(line, line_range);
    spline.style("with lines lw 3 lc 6");
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for (auto& sp : lspp) {
        auto sppoint = ToGnuplotActor(sp);
        sppoint.style("with points pt 7 ps 2 lc 7");
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", 
                      sp.x(), sp.y()),
                      sp.x(), sp.y() + 0.05, "left font \",13\"");
        gnu.add(sppoint);
        numlabel++;
    }
    gnu.set_object2d(5, lspp,
            "fillstyle transparent solid 0.3 fc rgb \"blue\"");

//    gnu.set_label(1, tfm::format("(%.1f, %.1f)", min1.x(), min1.y()), min1.x(),
//            min1.y() + 0.06, "center font \",16\"");
//    gnu.set_label(2, tfm::format("(%.1f, %.1f)", max1.x(), max1.y()), max1.x(),
//            max1.y() + 0.06, "center font \",16\"");

    gnu.set_label(4,
            tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(),
                    line.alpha()), -0.1, 1.3, "left font \",16\"");
    std::cout << "Line : " << line << std::endl;
    gnu.plot();
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

        BoxLinePositiveCase(i, 1.1, 0.8, a, b, alpha);
    }
}


void BoxLinePNCase(int num_case,
        double x, double y,
        double a, double b, double alpha) {
    Point2 min1(-0.1, -0.2);
    Point2 max1(x, y);
    Box2 box1(min1, max1);

    Line line(a, b, alpha);

    auto lspp = PositiveLineBox(min1, max1, line.a(), line.b(), line.alpha());
    auto lspn = NegativeLineBox(min1, max1, line.a(), line.b(), line.alpha());

    Gnuplot gnu;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png(tfm::format("./fig/pn_%02d", num_case));
    AddAxes(gnu);
    auto spbox1 = ToGnuplotActor(box1);
    spbox1.style("with lines lw 2 lc 8");
    gnu.add(spbox1);
    std::array<double,2> line_range{-0.5, 1.5};
    auto spline = ToGnuplotActor(line, line_range);
    spline.style("with lines lw 3 lc 6");
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for (auto& sp : lspp) {
        auto sppoint = ToGnuplotActor(sp);
        sppoint.style("with points pt 7 ps 2 lc 7");
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", 
                      sp.x(), sp.y()),
                      sp.x(), sp.y() + 0.05, "left font \",13\"");
        gnu.add(sppoint);
        numlabel++;
    }
    gnu.set_object2d(5, lspp,
            "fillstyle transparent solid 0.3 fc rgb \"blue\"");
    gnu.set_object2d(6, lspn,
            "fillstyle transparent solid 0.3 fc rgb \"green\"");

//    gnu.set_label(1, tfm::format("(%.1f, %.1f)", min1.x(), min1.y()), min1.x(),
//            min1.y() + 0.06, "center font \",16\"");
//    gnu.set_label(2, tfm::format("(%.1f, %.1f)", max1.x(), max1.y()), max1.x(),
//            max1.y() + 0.06, "center font \",16\"");

    gnu.set_label(4,
            tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(),
                    line.alpha()), -0.1, 1.3, "left font \",16\"");
    std::cout << "Line : " << line << std::endl;
    gnu.plot();
}