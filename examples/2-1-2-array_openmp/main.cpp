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
double TimeTest(double single_dt, int n_threads, int n_loop){
    auto begin = std::chrono::system_clock::now();
    omp_set_num_threads(n_threads);
// #ifdef OPENMP
    // std::cout << "num thread = " << nt << std::endl;
#pragma omp parallel for
    for(int i=0; i<n_loop; ++i) {
        // usleep(St(single_dt * 1000));
        // std::this_thread::sleep_for(std::chrono::microseconds(std::size_t(single_dt * 1000)));
        // std::cout << "thread" << omp_get_thread_num() << std::endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
        Sleep(single_dt);
#else defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        usleep(St(single_dt * 1000));
#endif
    
    }
// #endif
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    
    return elapsed.count() / 1000.0;
}

auto TimeTestRun(double single_dt){
    std::vector<double> nts = {1, 2, 4, 8, 16};
    // std::vector<double> nls = {10,50,100,500,1000};
    int n_loop = 1000;
    std::vector<double> rts; //ratio = vt / wt * 100%
    tfm::format(std::cout, "%10s %10s %10s %10s\n", "Sigle", "n Loop", "Wall time", "Ratio");
    
    for(auto nt: nts){
        double wt = TimeTest(single_dt, nt, n_loop);
        double rt = wt / (single_dt * n_loop);
        tfm::format(std::cout, "%10d %10d %10d %10d\n", single_dt, n_loop, wt, rt);
        rts.push_back(rt);
    }

    return ToGnuplotActor(nts, rts);
}

auto TimeTestNumThread(const std::vector<double>& t_times, int n_thread){
    // std::vector<double> t_times = {1, 2, 4, 8, 16};
    int n_loop = 1000;
    std::vector<double> rts; //ratio = vt / wt * 100%
    std::vector<double> stt;
    tfm::format(std::cout, "%10s %10s %10s %10s %10s\n","Single", "n Thread", "n Loop", "Wall time", "Ratio");

    for(auto tt : t_times){
        double st = TimeTest(tt, 1, n_loop);
        tfm::format(std::cout, "%10d %10d %10d %10d %10d\n", tt, 1, n_loop, st, 1.0);
        stt.push_back(st);
        // rts.push_back(1);
        if(n_thread > 1){
            double wt = TimeTest(tt, n_thread, n_loop);
            double rt = st / wt / n_thread;
            tfm::format(std::cout, "%10d %10d %10d %10d %10d\n", tt, n_thread, n_loop, wt, rt);
            rts.push_back(rt);
        }
    }

    return ToGnuplotActor(stt, rts);

}

auto TheoryRatio(int max_threads){
    std::list<double> nts;
    std::list<double> rts;
    for (int i = 1 ; i <= max_threads; i++){
        nts.push_back(i);
        rts.push_back(1.0/double(i)); 
    }

    return ToGnuplotActor(nts, rts);
}

void TimeTestPlot(){
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/timetest");
    gnu.set_grid();
    // gnu.set_xlogscale();
    gnu.set("key right bottom");
    gnu.set_yrange(0.0, 1.2);
    gnu.set_xlabel("Single Thread Wall Time (ms)");
    gnu.set_xlogscale();
    gnu.set_ylabel("Ratio");
    std::vector<int> vnt = {2, 4, 6, 8};
    std::vector<double> sts = {0.001, 0.005, 0.01, 0.05, 0.1, 0.5, 1, 5};
    for(auto& nt: vnt){
        // std::cout << "Single Task Time = " << sdt << "ms" << std::endl;
        auto a = TimeTestNumThread(sts, nt);
        a.command("using 1:2 title \"Num of Threads = " + ToString(nt) + "\" ");
        a.style("with linespoints pt 2 lw 2");
        gnu.add(a);
    }
    // auto at = TheoryRatio(16);
    // at.command("using 1:2 title \"Theory Time Ratio\" ");
    // at.style("with lines lw 2");
    // gnu.add(at);
    
    gnu.plot();
}

double ArrayOp(int nt, double na){
    ArrayListV_<double> a(na), b(na);
    a.assign(2);
    b.assign(1);
    omp_set_num_threads(nt);
    auto start = std::chrono::system_clock::now();
    b = a + b - a * b + 5.0 * a;
    auto end  = std::chrono::system_clock::now();
    std::cout << "Num of threads = " << nt;
    std::cout << " Size = " << na;
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << " Time = " << elapsed.count() / 1000.0 << "ms" << std::endl;
    return elapsed.count();
}

void ArrayOpPlot(){
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/arrayop");
    gnu.set_grid();
    gnu.set("key left top");
    gnu.set_yrange(0.0, 1.0);
    gnu.set_xlabel("Single Thread Wall Time (ms)");
    gnu.set_ylabel("Ratio");
    gnu.set_xlogscale();
    // gnu.set_ylogscale();
    ArrayListV_<double> vna = {1e4, 1e5, 1e6, 5e6, 1e7, 5e7, 1e8, 5e8};
    ArrayListV_<double> vnt = {2, 4, 6, 8}; // n > 2
    ArrayListV_<double> vst(vna.size());
    for(int i = 0; i< vna.size(); i++){
        vst[i] = ArrayOp(1, vna[i]);
    }
    for(auto& nt: vnt){
        ArrayListV_<double> vr;
        ArrayListV_<double> vdt;
        for(int i = 0; i< vna.size(); i++){
            auto st = vst[i];
            auto dt = ArrayOp(nt, vna[i]);
            double r = st / dt / nt;
            vr.push_back(r);
            vdt.push_back(st);
        }
        auto a = ToGnuplotActor(vdt, vr);
        a.command() = "using 1:2 title \"Num of Thread = " + ToString(nt) + "\" ";
        a.style()   = "with linespoints ps 2 lw 2";
        gnu.add(a);
    }
    gnu.plot();
}

int main(int argc, char** argv) {
    // ThreadPrint(3);
    TimeTestPlot();
    ArrayOpPlot();
}