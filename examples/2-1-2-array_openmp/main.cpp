#include <iostream>
#include <memory> 
#include <string>
#include <thread>
#include <sstream>
#include <chrono>
#include "utility/clock.hpp"
#ifdef OPENMP
#include <omp.h>
#endif 
#include "algebra/algebra.hpp"
#include "io/gnuplot.hpp"
#include "utility/tinyformat.hpp"
#include <memory>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
//defined for 32 and 64-bit environments
#include <windows.h>                // for Sleep(), _mktemp()
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
//all UNIX-like OSs (Linux, *BSD, MacOSX, Solaris, ...)
#include <unistd.h>            // for access(), mkstemp()
#else
#error unsupported or unknown operating system
#endif


using namespace carpio;

struct ThreadInfo{
    int t_num;
    tick_t t_start;
    tick_t t_end;

    void show() const{
        std::cout << "Thread Num      = " << t_num << std::endl;
        std::cout << "Wall Time dt    = " << Clock::TimespanToMillisecondsD(t_start, t_end) << " mm"<< std::endl;
    }
};


int ThreadPrint(int nt){
    omp_set_num_threads(nt);
    std::cout << "Set " << nt << " Thread" << std::endl;
    #pragma omp parallel
    {
      std::stringstream ss;
      ss << std::this_thread::get_id();
      int tid = omp_get_thread_num();
      std::cout << "std::this_thread::get_id() = " << ss.str() << std::endl;
      std::cout << "omp_get_thread_num()       = " << tid << std::endl;
    }
    return 0;
}

// input singal task time in milliseconds
// ouput wall time of parallel runing
double TimeTest(double tt, int nt){
    auto begin = std::chrono::high_resolution_clock::now();
    omp_set_num_threads(nt);
// #ifdef OPENMP
    // std::cout << "num thread = " << nt << std::endl;
#pragma omp parallel for
    for(int i=0; i<1000; ++i) {
        // usleep(St(tt * 1000));
        // std::this_thread::sleep_for(std::chrono::microseconds(std::size_t(tt * 1000)));
        // std::cout << "thread" << omp_get_thread_num() << std::endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
        Sleep(tt);
#else defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        usleep(St(tt * 1000));
#endif
    }
// #endif
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    return elapsed.count();
}

GnuplotActor TimeTestRun(int nt){
    
    // std::vector<double> vt = {1,2,3,4,5,6,7,8,9,
                            //   10,20,30,40,50,60,70,80,90,
                            //   100,200,300,400,500,600,700,800,900,
                            //   1000};
    std::vector<double> vt = {1,2,3,4,5,6,7,8,9
                              };
    std::vector<double> wt(vt.size());
    std::vector<double> rt(vt.size()); //ratio = vt / wt * 100%

    tfm::format(std::cout, "%10s %10s %10s\n", "Sigle", "Wall time", "Ratio");
    for(int i = 0; i < vt.size(); i++){
        wt[i] = TimeTest(vt[i], nt);
        rt[i] = wt[i]/ (vt[i] * 1000);
        tfm::format(std::cout, "%10d %10d %10d\n", vt[i], wt[i], rt[i]);
    }

    return ToGnuplotActor(vt, rt);
}

void TimeTestPlot(){
    Gnuplot gnu;
    gnu.set_terminal_png(
            "./fig/timetest", //const std::string& filename,
            800,
            600,
            "Courier",
            14);
    gnu.set_grid();
    gnu.set_xlogscale();
    gnu.set("key right top");
    gnu.set_yrange(80, 300.0);
    gnu.set_xlabel("Time of single task (ms)");
    gnu.set_ylabel("Time ratio (Parallel Wall Time / Single Task Time (%)");
    std::vector<int> vnt = {1, 2, 4, 8, 16};
    for(auto& nt: vnt){
        std::cout << "Run by " << nt << " Threads" << std::endl;
        auto a = TimeTestRun(nt);
        a.command() = "using 1:2 title \"Num of Threads = " + ToString(nt) + "\" ";
        a.style()   = "with lines lw 2";
        gnu.add(a);
    }
    gnu.plot();
}

double ArrayOp(int nt, double na){
    ArrayListV_<double> a(na), b(na);
    a.assign(2);
    b.assign(1);
    tick_t start = Clock::Tick();
    omp_set_num_threads(nt);
    b = a + b - a * b + 5.0 * a;
    tick_t end = Clock::Tick();
    std::cout << "Num of threads = " << nt;
    std::cout << " Num of Array = "  << na;
    std::cout << " Time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;
    return Clock::TimespanToMillisecondsD(start, end);
}

void ArrayOpPlot(){
    Gnuplot gnu;
    gnu.set_terminal_png(
            "./fig/arrayop", //const std::string& filename,
            800,
            600,
            "Courier",
            14);
    gnu.set_grid();
    gnu.set("key right top");
    // gnu.set_yrange(80, 300.0);
    gnu.set_xlabel("Number of threads (-)");
    gnu.set_ylabel("Wall Time (ms)");
    gnu.set_ylogscale();
    ArrayListV_<double> vna = {1e4, 1e6, 5e6};
    ArrayListV_<double> vnt = {1, 2, 4, 6, 8, 10, 12, 14, 15, 16};
    for(auto& na: vna){
        std::cout << "Array Size = " << na << std::endl;
        ArrayListV_<double> vdt;
        for(auto& nt: vnt){
            auto dt = ArrayOp(nt, na);
            vdt.push_back(dt);
        }
        auto a = ToGnuplotActor(vnt, vdt);
        a.command() = "using 1:2 title \"Array Size = " + ToString(na) + "\" ";
        a.style()   = "with lines lw 2";
        gnu.add(a);
    }
    gnu.plot();
}

int main(int argc, char** argv) {
    ThreadPrint(3);
    TimeTestPlot();
    ArrayOpPlot();
}