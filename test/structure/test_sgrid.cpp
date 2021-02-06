#include "gtest/gtest.h"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio; 


TEST(structure, initial){
	typedef std::shared_ptr<SGrid_<2> > spSGrid;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {15, 10},
									   0.3, 2 ));

//	Gnuplot ply;
//	ply.set_xrange(-0.5, 3.5);
//	ply.set_yrange(-0.5, 3.5);
//	ply.set_equal_ratio();
//	ply.add(GnuplotActor::WireFrame(*spsg));
//	ply.plot();

	// Plotly ply;
	// ply.add(PlotlyActor::WireFrame(*spsg));
	// ply.plot();

}

TEST(structure, initial2){
	typedef std::shared_ptr<SGrid_<2> > spSGrid;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          20,
									  3, 2 ));
//	Gnuplot ply;
//	ply.set_xrange(-0.5, 3.5);
//	ply.set_yrange(-0.5, 3.5);
//	ply.set_equal_ratio();
//	ply.add(GnuplotActor::WireFrame(*spsg));
//	ply.plot();
}