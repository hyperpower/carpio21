#include "gtest/gtest.h"
#include <math.h>
#include <array>
#include <cmath>
#include "algebra/array/array_list.hpp"
#include "geometry/geometry.hpp"
#include "io/gnuplot.hpp"


using namespace carpio;
typedef Point_<double, 2>         Point;
typedef Segment_<double, 2>       Seg;
typedef GGnuplotActor_<double, 2> GA;


TEST(gnuplot, change_dpi) {
    std::cout << "Test gnuplot" << std::endl;
   // terminald pngcairo add dpi
	float dpi    = 300;       // dpi (variable)
	float width  = 5.98;    // inch (variable)
	float height = 4.485;  // inch (variable)

	// 1 inch = 72 pt
	float pt2in = 0.013888; // mm (fixed)

	float ptscale = std::round(pt2in*dpi);
	// round(x) = x - floor(x) < 0.5 ? floor(x) : ceil(x);
	float wpx = std::round(width * dpi);
	float hpx = std::round(height * dpi);
 	std::stringstream sst;
    sst << "fontscale " << ptscale << " linewidth "
                << ptscale << " pointscale " << ptscale;
    // Plot
    std::string str_res = "test_p1";
	Gnuplot gnu;
	if (FileAccessCheck("./fig")){
		gnu.set_terminal_png(
			"./fig/" + str_res + ".png", //const std::string& filename,
			wpx,
			hpx,
			"Palatino",
			12,
			sst.str()
			);
	}else{
		std::cerr << "!> Output Folder fig Not exist!" << std::endl;
		FAIL();
	}
	// gnu.set_xrange(-0.5, 1.5);
	// gnu.set_yrange(-0.5, 1.5);
	gnu.set_xlabel("x label (n)");
	gnu.set_ylabel("y label (n)");
	gnu.set_grid();

	gnu.set_label(1, "a TEST string", 1.0, 2.0);

    std::vector<double> x{0.0, 1.0, 2.0,3.0};
    std::vector<double> y{0.0, 1.0, 1.5,2.3};

    gnu.plot_2(x, y, "with linespoints pointtype 7 lw 2");
}