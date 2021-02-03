#ifndef GEOMETRY_TEST_AABBTREE_HPP_
#define GEOMETRY_TEST_AABBTREE_HPP_

#include "geometry/geometry.hpp"
#include "utility/random.hpp"
#include "gtest/gtest.h"


namespace carpio {

typedef Segment_<double, 2> Seg;
typedef AABBox_<double, 2, Seg> AABBox;
typedef AABBoxCompareLess_<double, 2, Seg> CompLess;
typedef BBTree_<AABBox> BBTree;

typedef GGnuplotActor_<double, 2> GA;

template<typename TYPE, St DIM>
class TempGnuplotActor_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;
	typedef Segment_<TYPE, DIM> Segment;
	typedef Segment& ref_Segment;
	typedef const Segment& const_ref_Segment;

	typedef PointChain_<TYPE, DIM> PointChain;

	typedef std::shared_ptr<Gnuplot_actor> spActor;
	typedef std::list<spActor> list_spActor;

	typedef AABBox_<TYPE, DIM, Segment> AABBox;
	typedef AABBoxCompareLess_<TYPE, DIM, Segment> CompLess;
	typedef BBTree_<AABBox> BBTree;

	static spActor LinesPoints(
			const std::set<AABBox, CompLess>& con,
			int color_idx = -1) {
		ASSERT(Dim == 2);
		int color = color_idx > 0 ? color_idx : 0;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with linespoints lc variable";

		auto iter = con.begin();
		for (; iter != con.end(); ++iter) {
			actor->data().push_back(
					ToString(iter->object().psx(), iter->object().psy(), color, " "));
			actor->data().push_back(
					ToString(iter->object().pex(), iter->object().pey(), color, " "));
			actor->data().push_back("");
		}
		return actor;
	}

	static spActor WireFrame(
			const std::set<AABBox, CompLess>& con,
			int color_idx = -1) {
		ASSERT(Dim == 2);
		int color = color_idx > 0 ? color_idx : 0;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with lines lc variable";

		auto iter = con.begin();
		for (; iter != con.end(); ++iter) {
			auto minx = iter->min().x();
			auto miny = iter->min().y();
			auto maxx = iter->max().x();
			auto maxy = iter->max().y();
			actor->data().push_back(
					ToString(minx, miny, color, " "));
			actor->data().push_back(
					ToString(maxx, miny, color, " "));
			actor->data().push_back(
					ToString(maxx, maxy, color, " "));
			actor->data().push_back(
					ToString(minx, maxy, color, " "));
			actor->data().push_back(
					ToString(minx, miny, color, " "));
			actor->data().push_back("");
		}
		return actor;
	}

	static spActor WireFrame(
			BBTree& tree,
			int color_idx = -1) {
		ASSERT(Dim == 2);
		int color = color_idx > 0 ? color_idx : 0;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with lines lc variable";

		typename BBTree::Fun fun = [&actor](typename BBTree::pNode pn){
			int color = pn->height();
			auto minx = pn->box().min().x();
			auto miny = pn->box().min().y();
			auto maxx = pn->box().max().x();
			auto maxy = pn->box().max().y();

			actor->data().push_back(
					ToString(minx, miny, color, " "));
			actor->data().push_back(
					ToString(maxx, miny, color, " "));
			actor->data().push_back(
					ToString(maxx, maxy, color, " "));
			actor->data().push_back(
					ToString(minx, maxy, color, " "));
			actor->data().push_back(
					ToString(minx, miny, color, " "));
		};

		tree.PreOrder(fun);
		return actor;
	}


};

TEST(box, aabbtree){
	std::cout << "test aabbox" <<std::endl;
	Seg seg(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	AABBox box(&seg);
	ASSERT_EQ(box.min(_X_), 0.0);
	ASSERT_EQ(box.max(_X_), 1.0);
}

TEST(builder, aabbtree){
	std::cout << "Generate random segments" <<std::endl;
	typedef std::shared_ptr<Seg> spSeg;
	std::set<AABBox, CompLess> set;
	int n = 10;
//	Random::randomSeed();
	for(int i = 0; i < n; i++){
		Seg* ps = new Seg(
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0));
		AABBox box(ps);
		box.object().show();
		set.insert(box);
	}
	// show set info
	std::cout << "set size = " << set.size() << std::endl;

	// build tree
	BBTree tree(set);

	typedef TempGnuplotActor_<double, 2> TGA;
	Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);
	auto a1 = TGA::LinesPoints(set, 1);
	auto a2 = TGA::WireFrame(tree);
	gnu.add(a1);
	gnu.add(a2);
	gnu.plot();

}
TEST(builder, aabbtree_plotly){
	std::cout << "Generate random segments" <<std::endl;
	typedef GPlotlyActor_<double, 2> PA;
	Plotly py;
	std::list<Seg*> lseg;
	int n = 10;
	for (int i = 0; i < n; i++) {
		Seg* ps = new Seg(
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0),
				Random::nextDouble(0.0, 1.0));
		lseg.push_back(ps);
	}
	auto a = PA::LinesPoints(lseg, typename std::list<Seg*>::value_type());
	py.add(a);
//	py.plot();
}

}

#endif
