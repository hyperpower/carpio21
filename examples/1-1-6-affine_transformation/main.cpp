#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>              Poi;
typedef Segment_<double, 2>            Seg;
typedef Box_<double, 2>                Box;
typedef PointChain_<double, 2>         PointChain;
typedef Line_<double>                  Line;
typedef std::vector<double>            Vec;

// Forware Declare
void TestTranslate();
void TestScale();
void TestScaleAbout();
void TestRotate();
void TestRotateAbout();
void TestReflectAboutAxes(Axes);
void TestReflectAboutPoint();
void TestShearIn(Axes, double);
void TestShearAbout(Axes, double, Poi);

// main 
int main(){
    std::cout << "Affine Transformation " << std::endl;
    TestTranslate();
    TestScale();
    TestScaleAbout();
    TestRotate();
    TestRotateAbout();
    TestReflectAboutAxes(_X_);
    TestReflectAboutAxes(_Y_);
    TestReflectAboutPoint();
    TestShearIn(_X_, 0.2);
    TestShearIn(_Y_, 0.2);

    Poi about = {0.4, 0.5};
    TestShearAbout(_X_, 0.2, about);
    TestShearAbout(_Y_, 0.2, about);
}

auto MakeBox(){
    typedef PointChain_<double, 2> PointChain;
    typedef Point_<double, 2> Point;
    PointChain pc;
    pc.set_close();
    pc.push_back(Point(0.0,0.0));
    pc.push_back(Point(1.0,0.0));
    pc.push_back(Point(1.0,1.0));
    pc.push_back(Point(0.0,1.0));
    return pc;
}

auto MakeFShape(){
    typedef PointChain_<double, 2> PointChain;
    typedef Point_<double, 2> Point;
    PointChain pc;
    pc.set_close();
    pc.push_back(Point(0.3,0.1));
    pc.push_back(Point(0.4,0.1));
    pc.push_back(Point(0.4,0.45));
    pc.push_back(Point(0.7,0.45));
    pc.push_back(Point(0.7,0.55));
    pc.push_back(Point(0.4,0.55));
    pc.push_back(Point(0.4,0.8));
    pc.push_back(Point(0.8,0.8));
    pc.push_back(Point(0.8,0.9));
    pc.push_back(Point(0.3,0.9));
    return pc;
}

auto AddActorsOriginal(Gnuplot& gnu, const PointChain& box, const PointChain& pc){
    auto actor_pori = ToGnuplotActor(box.get(0));
    actor_pori->style("with points pt 7 ps 2 lc rgb \"#7FBA00\"");
    gnu.add(actor_pori);
    auto actor_px = ToGnuplotActor(box.get(1));
    actor_px->style("with points pt 7 ps 2 lc rgb \"#00A4EF\"");
    gnu.add(actor_px);
    auto actor_py = ToGnuplotActor(box.get(3));
    actor_py->style("with points pt 7 ps 2 lc rgb \"#F25022\"");
    gnu.add(actor_py);
    auto actor_box = ToGnuplotActor(box);
    actor_box->style("with lines lt 1 dashtype 2 lw 2 lc \"black\"");
    gnu.add(actor_box);
    auto actor_pc  = ToGnuplotActor(pc);
    actor_pc->style("with lines lt 1 dashtype 2 lw 2 lc rgb \"#FFB900\"");
    gnu.add(actor_pc);
}
auto AddActorsTransformed(Gnuplot& gnu, const PointChain& box, const PointChain& pc){
    auto actor_pori = ToGnuplotActor(box.get(0));
    actor_pori->style("with points pt 7 ps 2 lc rgb \"#7FBA00\"");
    gnu.add(actor_pori);
    auto actor_px = ToGnuplotActor(box.get(1));
    actor_px->style("with points pt 7 ps 2 lc rgb \"#00A4EF\"");
    gnu.add(actor_px);
    auto actor_py = ToGnuplotActor(box.get(3));
    actor_py->style("with points pt 7 ps 2 lc rgb \"#F25022\"");
    gnu.add(actor_py);
    auto actor_box = ToGnuplotActor(box);
    actor_box->style("with lines lt 1 lw 2 lc \"black\"");
    gnu.add(actor_box);
    auto actor_pc  = ToGnuplotActor(pc);
    actor_pc->style("with lines lt 1 lw 2 lc rgb \"#FFB900\"");
    gnu.add(actor_pc);
}

auto AddAxes(Gnuplot& gnu){
    gnu.set_label(1, "x",  0.45, -0.08, "textcolor rgb \"#00A4EF\"");
    auto actor_x = ToGnuplotActorAsVector(Seg(Poi(0.0,0.0), Poi(0.5,0.0)));
    actor_x->style("with vectors lt 1 lw 3 lc rgb \"#00A4EF\"");
    gnu.add(actor_x);

    gnu.set_label(2, "y", -0.08, 0.45, "textcolor rgb \"#F25022\"");
    auto actor_y = ToGnuplotActorAsVector(Seg(Poi(0.0,0.0), Poi(0.0,0.5)));
    actor_y->style("with vectors lt 1 lw 3 lc rgb \"#F25022\"");
    gnu.add(actor_y);
}

void TestTranslate(){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_trans");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    Poi trans_vec = {0.2, 0.3}; 
    auto actor_tv = ToGnuplotActorAsVector(Seg(Poi(0.0,0.0), Poi(trans_vec)));
    actor_tv->style("with vectors lt 1 lw 3 lc rgb \"#7FBA00\"");
    gnu.add(actor_tv);
    Poi mid = Mid(trans_vec, {0.0,0.0});
    gnu.set_label(3, "Translate Vetor",  mid[0]+0.05, mid[1], "textcolor rgb \"#7FBA00\"");

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Translate(box, trans_vec);
    Translate(pc,  trans_vec);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestScale(){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_scale");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    Poi vec = {1.2, 1.3}; 
    auto actor_tv = ToGnuplotActorAsVector(Seg(Poi(0.0,0.0), Poi(vec)));
    actor_tv->style("with vectors lt 1 lw 3 lc rgb \"#7FBA00\"");
    gnu.add(actor_tv);
    Poi mid = Mid(vec, {0.0,0.0});
    gnu.set_label(3, "Scale Vetor",  mid[0]+0.05, mid[1], "textcolor rgb \"#7FBA00\"");

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Scale(box, vec);
    Scale(pc,  vec);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestScaleAbout(){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_scale_about");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    Poi about = {0.4, 0.5};
    auto actor_about = ToGnuplotActor(about);
    actor_about->style("with points pt 7 ps 2 lc \"black\"");
    gnu.add(actor_about);

    Poi vec   = {1.2, 1.3}; 
    gnu.set_label(3, "Scale Center",  about[0], about[1]-0.1, "center textcolor rgb \"black\"");

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Scale(box, vec, about);
    Scale(pc,  vec, about);
    AddActorsTransformed(gnu, box, pc);
    
    gnu.plot();
}

void TestRotate(){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_rotate");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    double theta = 0.2;

    gnu.set_label(3, "Rotate Center",  0.0, -0.1, "center textcolor \"black\"");

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Rotate(box, theta);
    Rotate(pc,  theta);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestRotateAbout(){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_rotate_about");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    double theta = 0.2;

    Poi about = {0.7, 0.8};
    auto actor_about = ToGnuplotActor(about);
    actor_about->style("with points pt 7 ps 2 lc \"black\"");
    gnu.add(actor_about);

    gnu.set_label(3, "Rotate Center",   about[0], about[1]-0.1, "center textcolor \"black\"");

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Rotate(box, theta, about);
    Rotate(pc,  theta, about);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestReflectAboutAxes(Axes axe){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_reflect_about_" + ToString(axe));
    if (axe == _X_){
        gnu.set_xrange(-0.5, 1.5);
        gnu.set_yrange(-1.5, 1.5);
    }else if (axe == _Y_){
        gnu.set_xrange(-1.5, 1.5);
        gnu.set_yrange(-0.5, 1.5);
    }
    gnu.set_equal_aspect_ratio();

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Reflect(box, axe);
    Reflect(pc,  axe);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestReflectAboutPoint(){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();
    // about origin 
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_reflect_about_o");
    gnu.set_xrange(-1.5, 1.5);
    gnu.set_yrange(-1.5, 1.5);
    gnu.set_equal_aspect_ratio();

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Reflect(box);
    Reflect(pc);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();

    // about a point
    gnu.set_terminal_png("./fig/affine_reflect_about_point");
    gnu.set_equal_aspect_ratio();
    gnu.set_xrange(-.5, 2.0);
    gnu.set_yrange(-.5, 2.0);

    Poi about = {0.7, 0.8};
    auto actor_about = ToGnuplotActor(about);
    actor_about->style("with points pt 7 ps 2 lc \"black\"");
    gnu.add(actor_about);
    gnu.set_label(3, "Reflect Center",   about[0], about[1]-0.1, "center textcolor \"black\"");

    box = MakeBox();
    pc  = MakeFShape();

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Reflect(box, about);
    Reflect(pc,  about);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestShearIn(Axes axe, double theta){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();
    // about origin 
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_shear_" + ToStringAsAxes(axe));
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Shear(box, theta, axe);
    Shear(pc,  theta, axe);
    AddActorsTransformed(gnu, box, pc);

    gnu.plot();
}

void TestShearAbout(Axes axe, double theta, Poi about){
    PointChain box = MakeBox();
    PointChain pc  = MakeFShape();
    // about origin 
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/affine_shear_about_in_" + ToStringAsAxes(axe));
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();

    AddAxes(gnu);
    AddActorsOriginal(gnu, box, pc);
    Shear(box, theta, about, axe);
    Shear(pc,  theta, about, axe);
    AddActorsTransformed(gnu, box, pc);

    auto actor_about = ToGnuplotActor(about);
    actor_about->style("with points pt 7 ps 2 lc \"black\"");
    gnu.add(actor_about);
    gnu.set_label(3, "Shear Center",   about[0], about[1]-0.1, "center textcolor \"black\"");

    gnu.plot();
}