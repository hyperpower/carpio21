#include "multi_seg_benchmark.hpp"

using namespace carpio;

void benchmark_test(){
    ProfileClean();
    std::vector<int> arr_num     = {30};
    std::list<double> m1_time;
    std::list<double> m2_time;
    std::list<double> m3_time;

    for(auto& num : arr_num){
        ProfileStart("GenSeg_" + ToString(num));
        auto lseg = GenerateRandomSegments(num);
        // auto lseg = GenerateParallelSlanted(num);
        // auto lseg = GenerateSparse(num);
        // auto lseg = GenerateGrid(num);
        auto nlseg = ToCGAL(lseg);
        std::cout << "Segment size = " << lseg.size() << std::endl;
        ProfileEnd();
        // Method1 ==========================================
        auto start = std::chrono::system_clock::now();
        ProfileStart("Method_N2_" + ToString(num));
        tfm::format(std::cout, "Method_N2_%d  ", num); 
        auto res = Intersect(lseg);
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    auto end = std::chrono::system_clock::now();
    	double dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m1_time.push_back(dt);
        std::string out = tfm::format("%25s :  %15.5f %3s\n",
                                 "Method N2", dt, "s");
    	std::cout << out;
        // Method2 Simple sweeep line =============================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_sl_" + ToString(num));
        tfm::format(std::cout, "Method_SweepLine_%d  ", num); 
        res = Intersect(lseg, "sweep_line_simple");
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    end = std::chrono::system_clock::now();
    	dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m2_time.push_back(dt);
        out = tfm::format("%25s :  %15.5f %3s\n",
                                 "Simple Sweep", dt, "s");
    	std::cout << out;
        // Method Ben Ott ==========================================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_Ben_" + ToString(num));
        tfm::format(std::cout, "Method_Ben_%d  ", num); 
        res = Intersect(lseg, "bentley_ottmann");
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    end = std::chrono::system_clock::now();
    	dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m3_time.push_back(dt);
        out = tfm::format("%25s :  %15.5f %3s\n",
                                 "bentley ottmann", dt, "s");
    	std::cout << out;
        // Method CGAL ==========================================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_CGAL_" + ToString(num));
        tfm::format(std::cout, "Method_CGAL_%d  ", num); 
        
        std::list<Point_2> pts;
        CGAL::compute_intersection_points(nlseg.begin(), nlseg.end(),
                                     std::back_inserter(pts)); 
        
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    end = std::chrono::system_clock::now();
    	dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m3_time.push_back(dt);
        out = tfm::format("%25s :  %15.5f %3s\n",
                                 "CGAL", dt, "s");
    	std::cout << out;
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

    // benchmark_test();
    Benchmark();
} 