#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "utility/random.hpp"

using namespace carpio;

typedef Point_<double, 2>         Point;
typedef PointChain_<double, 2>    PC;
typedef GGnuplotActor_<double, 2> GA;


int case_random() {
    std::list<Point> l ={ Point(0,   0),
                          Point(1,   0),
                          Point(0.5 ,0.5),
                          Point(1,   1),
                          Point(0,   1)};

    PC pc(l);

    std::list<Point> ltp;
    // generate random Points
    for (int i = 0; i< 100; i++){
        auto x = Random::nextDouble(-0.5, 1.5);
        auto y = Random::nextDouble(-0.5, 1.5);
        std::cout << "Point " << i << " = " << x << ", " << y << std::endl;
        ltp.push_back(Point(x, y));
    }

    Gnuplot gnu;
    gnu.set_terminal_png(
            "./fig/random_points");
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    int count = 1;
    for(auto& p : ltp){
        bool wn = IsInOn(p, pc);
        auto actor = ToGnuplotActor(p); 
        actor.style("with points ps 2 pt 7 lc " + ToString(wn? 3 : 4));
        gnu.add(actor);
        gnu.set_label(count, wn? "  IN ":"  OUT ", p.x(), p.y(), "left" );
        count++;
    }
    auto a_arrows = ToGnuplotActorAsVector(pc);
    a_arrows.style("with vectors lw 2 lc 2");
    gnu.add(a_arrows);
    gnu.plot();
    return 0;
}

int case_on_edge() {
    std::list<Point> l ={ Point(0,   0),
                          Point(1,   0),
                          Point(0.5 ,0.5),
                          Point(0.7, 0.8),
                          Point(1,   1),
                          Point(0,   1)};

    PC pc(l);

    std::list<Point> ltp;
    // generate random Points
    ltp.push_back(Point(0.1, 0.0));
    ltp.push_back(Point(0.4, 0.0));
    ltp.push_back(Point(0.2, 1.0));
    ltp.push_back(Point(0.0, 0.4));

    Gnuplot gnu;
    gnu.set_terminal_png(
            "./fig/edge_points" //const std::string& filename,
            );
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    int count = 1;
    for(auto& p : ltp){
        bool wn = IsInOn(p, pc);
        auto actor = ToGnuplotActor(p); 
        actor.style("with points ps 2 pt 7 lc " + ToString(wn? 3 : 4));
        gnu.add(actor);
        gnu.set_label(count, wn? "  IN ":"  OUT ", p.x(), p.y(), "left" );
        count++;
    }
    auto a_arrows = ToGnuplotActorAsVector(pc);
    a_arrows.style("with vectors lw 2 lc 2");
    gnu.add(a_arrows);
    gnu.plot();
    return 0;
}


int case_on_vertex() {
    std::list<Point> l ={ Point(0,   0),
                          Point(1,   0),
                          Point(0.5 ,0.5),
                          Point(0.7, 0.8),
                          Point(1,   1),
                          Point(0,   1)};

    PC pc(l);

    std::list<Point> ltp;
    // generate random Points
    ltp.push_back(Point(0.0, 0.0));
    ltp.push_back(Point(1.0, 0.0));
    ltp.push_back(Point(0.5, 0.5));
    ltp.push_back(Point(1.0, 1.0));

    Gnuplot gnu;
    gnu.set_terminal_png(
            "./fig/vertex_points"//const std::string& filename,
           );
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    int count = 1;
    for(auto& p : ltp){
        bool wn = IsInOn(p, pc);
        auto actor = ToGnuplotActor(p); 
        actor.style("with points ps 2 pt 7 lc " + ToString(wn? 3 : 4));
        gnu.add(actor);
        gnu.set_label(count, wn? "  IN ":"  OUT ", p.x(), p.y(), "left" );
        count++;
    }
    auto a_arrows = ToGnuplotActorAsVector(pc);
    a_arrows.style("with vectors lw 2 lc 2");
    gnu.add(a_arrows);
    gnu.plot();
    return 0;
}

int main(int argc, char** argv) {
    case_random();
    case_on_edge();
    case_on_vertex();
}