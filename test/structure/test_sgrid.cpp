#include "gtest/gtest.h"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
// #include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";

const int fig_width  = 900;
const int fig_height = 900;


TEST(structure, initial){
	typedef std::shared_ptr<SGrid_<2> > spGrid;
	typedef SGridUniform_<2> GridUniform;
	typedef std::shared_ptr<GridUniform> spGridUniform;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spGridUniform spsg(new GridUniform(pmin, {10, 10}, 0.3, 2));

	Gnuplot gnu;
	GAMStructure_<2> maker;
	gnu.set_xrange(-0.5, 3.5);
	gnu.set_yrange(-0.5, 3.5);
	gnu.set_equal_ratio();
	gnu.add(maker.wire_frame(*spsg));
    gnu.set_terminal_png(OUTPUTPATH + "UniformSturctureGrid", fig_width, fig_height);
	gnu.plot();

	// Plotly ply;
	// ply.add(PlotlyActor::WireFrame(*spsg));
	// ply.plot();

}

TEST(structure, initial1){
	typedef std::shared_ptr<SGrid_<1> > spGrid;
	typedef SGridUniform_<1> GridUniform;
	typedef std::shared_ptr<GridUniform> spGridUniform;

	Point_<Vt, 1> pmin(0, 0, 0);
	Point_<Vt, 1> pmax(1, 1, 1);
	spGridUniform spsg(new GridUniform(pmin, {10, 10}, 0.3, 2));

	Gnuplot gnu;
	GAMStructure_<1> maker;
	gnu.set_xrange(-0.5, 3.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_ratio();
	gnu.add(maker.wire_frame(*spsg));
    gnu.set_terminal_png(OUTPUTPATH + "UniformSturctureGrid1", fig_width, fig_height);
	gnu.plot();

	// Plotly ply;
	// ply.add(PlotlyActor::WireFrame(*spsg));
	// ply.plot();

}

