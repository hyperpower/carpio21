#include "utility/random.hpp"
#include "utility/clock.hpp"
#include "utility/profile.hpp"
#include "two_segments.hpp"
#include "multi_segments.hpp"

using namespace carpio;

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


int main(int argc, char** argv) {
    MakeDir("./fig/");

    two_segments_test();

    // benchmark_test();
}