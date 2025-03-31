#include "gtest/gtest.h"
#include <chrono>
#include "algebra/interpolation/interp_lagrange.hpp"
#include "algebra/array/array_list_v.hpp"
#include "io/gnuplot.hpp"

using namespace carpio;
typedef std::chrono::milliseconds ms;

TEST(interp, case2){
    typedef ArrayListV_<double> Arr;
    Arr x_data = {3, 1, 4, 2}; 
    Arr y_data = {9, 1, 16, 4}; // f(x) = x^2

    int n = 50;
    double start = 0;
    double end   = 6;
    Arr rx, ry;
    double step = (end - start) / n;

    for (int i = 0; i < n; ++i){
        double rxv =start + i * step;
        rx.push_back(rxv);
        double y_interp = InterpLagrange(x_data, y_data, rxv);
        ry.push_back(y_interp);
    }

    // std::cout << "Interpolated value at x = " << rxv << " is " << y_interp << std::endl;

    Gnuplot gnu;
	gnu.set_terminal_png("./fig/interp_lag1");
    gnu.set_xrange(x_data.min()-2, x_data.max()+2);
    gnu.set_yrange(y_data.min()-2, y_data.max()+2);
    auto ap = ToGnuplotActor(x_data, y_data);
    ap.style("with points ps 2 pt 7");
    // ap.show_command();
	gnu.add(ap);
    auto ar = ToGnuplotActor(rx, ry);
    ar.style("with lines lc 3 lw 2");
	gnu.add(ar);

    gnu.plot();
   
}
