#include "geometry/geometry.hpp"
// #include "geometry/operation/polygon_partition.hpp"
#include "utility/tinyformat.hpp"
#include "geometry/io/gplotly_actor.hpp"
#include "gtest/gtest.h"

using namespace carpio;

typedef Point_<double, 3>   Point3;
typedef Point_<double, 2>   Point2;
typedef Segment_<double, 2> Seg2;
typedef Box_<double, 2>     Box2;
typedef Box_<double, 3>     Box3;
typedef Line_<double>       Line;
// typedef PolygonPartition_<Point2> PP;

typedef PointChain_<double, 2> PointChain;
//typedef GGnuplotActor_<double, 2> GA;
typedef GGnuplotActorMaker_<double, 2> GAM;

TEST(bezier, mkdir){
    // Do not disable this test
    MakeDir("./fig/");
}

TEST(partition, test1){
    auto pc = ReadFile<PointChain>("./test/input_files/examplepolygon");

    std::cout << "PointChain size = " << pc.size() << std::endl;
    std::cout << "PointChain size = " << pc.get(0) << std::endl;

    // PP partition;
    // partition.new_partition_vertex(pc);

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/point_chain_partition");
	// gnu.set_xrange(100, 1000);
	// gnu.set_yrange(100, 1000);
	gnu.add(ToGnuplotActor(pc));
	gnu.plot();
}

TEST(partition, trydata){
    typedef typename std::vector<double> Vec;
    std::cout << HasData<Vec>::value << std::endl;
}


