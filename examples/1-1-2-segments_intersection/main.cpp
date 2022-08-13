#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "utility/random.hpp"
#include "utility/clock.hpp"
#include "utility/profile.hpp"

using namespace carpio;

typedef Point_<double, 2>   Point2;
typedef Segment_<double, 2> Segment2;
typedef Intersection_<Segment2, Segment2>  Inter;
typedef GGnuplotActor_<double, 2>      GA;


auto GenerateRandomSegments(int num,
                            const double& xmin, const double& xmax,
                            const double& ymin, const double& ymax){
    typedef std::list<Segment2> ListSegment;
    ListSegment lseg;
    // Random::seed(std::time(0));
    for (int i = 0; i< num ; i++){
        double x1 = Random::nextDouble(xmin, xmax);
        double y1 = Random::nextDouble(ymin, ymax);
        double x2 = Random::nextDouble(xmin, xmax);
        double y2 = Random::nextDouble(ymin, ymax);
        lseg.push_back(Segment2(x1, x2, y1, y2));
    }
    return lseg;
}

void benchmark_test(){
    std::vector<int> arr_num     = {10, 100, 1000, 2000, 5000};
    std::list<double> m1_time;
    std::list<double> m2_time;

    for(auto& num : arr_num){
        ProfileStart("GenerateSegments_" + ToString(num));
        auto lseg = GenerateRandomSegments(num, 0, 100, 0, 1);
        ProfileEnd();
        // Method1 ==========================================
        tick_t start = Clock::Tick();
        ProfileStart("Method_N2");
        auto res = Intersect(lseg);
        ProfileEnd();
	    tick_t end = Clock::Tick();
    	double dt =  Clock::TicksToMillisecondsD(end - start);
        m1_time.push_back(dt/1000);
    	std::cout << "M1 Time  = " << dt << "ms" << std::endl;
    	// std::cout << "Res size = " << res.size() << std::endl;
        // Method1 ==========================================
        start = Clock::Tick();
        ProfileStart("Method_SweepLine");
        res = Intersect(lseg, "sweep_line_simple");
        ProfileEnd();
	    end = Clock::Tick();
    	dt =  Clock::TicksToMillisecondsD(end - start);
        m2_time.push_back(dt/1000);
    	std::cout << "M2 Time = " << dt << "ms" << std::endl;
    	// std::cout << "Res size = " << res.size() << std::endl;
    }
    ProfileListShow();
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/benchmark");
    gnu.set_xlabel("number of segments");
    gnu.set_ylabel("time (s)");
    // gnu.set_label(1, strtype, -4.5, 4);

    auto a1 = ToGnuplotActor(arr_num, m1_time);
    a1.command("using 1:2 title \"Method n2\" ");
    a1.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");

    auto a2 = ToGnuplotActor(arr_num, m2_time);
    a2.command("using 1:2 title \"Method Sweep Line Simple\" ");
    a2.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#F25022\"");

    gnu.add(a1);
    gnu.add(a2);
    gnu.plot();
}


int a_case(const Point2& p1,
           const Point2& p2,
           const Point2& p3,
           const Point2& p4){
    Segment2 seg1(p1, p2);
    Segment2 seg2(p3, p4);

    Inter inter(seg1, seg2);
    auto strtype = ToString(inter.cal_intersection_type());
    std::cout << "Intersection Type : "<< strtype << std::endl;
    Point2 np = inter.cal_intersection_point();
    std::cout << "new point         : "<< np << std::endl;

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/" + strtype);
    gnu.set_xrange(-5, 5);
    gnu.set_yrange(-5, 5);
    gnu.set_label(1, strtype, -4.5, 4);

    auto a1 = ToGnuplotActor(seg1);
    a1.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");

    auto a2 = ToGnuplotActor(seg2);
    a2.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#F25022\"");

    auto a3 = ToGnuplotActor(np);
    a3.style("with linespoints pointtype 1 pointsize 3 lw 3 lc black");

    gnu.add(a1);
    gnu.add(a2);
    gnu.add(a3);
    gnu.plot();
    return 0;
}


int main(int argc, char** argv) {
    Point2 p1(  -1,    0);
    Point2 p2(   3,  0.5);
    Point2 p3( 0.8,  2.0);
    Point2 p4(-0.3, -1.0);
    // intersect
    a_case(p1, p2, p3, p4);
    // NO
    a_case(Point2( -1,   0), Point2(3,    0.5),
           Point2(0.8, 2.0), Point2(-0.3, 1.0));
    // CONNECT
    a_case(Point2( -1,   0), Point2(3,    0.5),
           Point2( -1,   0), Point2(-0.3, 1.0));
    // TOUCH
    a_case(Point2( -1,   0), Point2(3,    0.0),
           Point2(  0,   0), Point2(-0.5, 1.0));
    // OVERLAP
    a_case(Point2(  0, -1), Point2( 4,   3),
           Point2( -1, -2), Point2( 2,   1));
    // SAME
    a_case(Point2( -1, -1), Point2( 2,   2),
           Point2( -1, -1), Point2( 2,   2));

    benchmark_test();
}