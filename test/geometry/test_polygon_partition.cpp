#include "geometry/geometry.hpp"
#include "geometry/operation/polygon_partition.hpp"
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
typedef PolygonPartition_<Point2> PP;

typedef PointChain_<double, 2> PointChain;
typedef PointChain_<double, 3> PointChain3;
//typedef GGnuplotActor_<double, 2> GA;
typedef GGnuplotActorMaker_<double, 2> GAM;

TEST(bezier, mkdir){
    // Do not disable this test
    MakeDir("./fig/");
}

TEST(partition, orient_test1){
    Point2 p1(0.0,0.0);
    Point2 p2(1.0,0.0);
    Point2 p3(0.0,1.0);
    Point2 p(-0.1,0.1);

    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p3 = " << p3 << std::endl;

    PP pp;
    std::cout << "is convex = " << pp.is_convex(p1, p2, p3) << std::endl;
    ASSERT_EQ(pp.is_convex(p1, p2, p3), true);
    std::cout << "is convex = " << pp.is_convex(p2, p1, p3) << std::endl;
    ASSERT_EQ(pp.is_convex(p2, p1, p3), false);
    std::cout << "in cone   = " << p  << "  " << pp.in_cone(p1, p2, p3, p) << std::endl;
    p = {0.1, 0.1};
    std::cout << "in cone   = " << p  << "  " << pp.in_cone(p1, p2, p3, p) << std::endl;

    std::array<double, 3> ap{1.0,1.2,1.3};
    Normalize(ap);
}


TEST(partition, test1){
    auto pc = ReadFile<PointChain>("./test/input_files/examplepolygon.txt");
    pc.set_close();

    std::cout << "PointChain size = " << pc.size() << std::endl;

    std::cout << "Poly size = " << pc.size() << std::endl;
    PP partition;
    typename PP::ListTri res;
    partition.ear_clipping(pc, res);
    
    std::cout << "Res size = " << res.size() << std::endl;

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/point_chain_partition");
	// gnu.set_xrange(100, 1000);
	// gnu.set_yrange(100, 1000);
	// gnu.add(ToGnuplotActor(pc));
    int c = 1;
    for(auto& t : res){
        std::cout << "Tri count = " << c <<std::endl;
	    auto tactor = ToGnuplotActor(t, 0);
        // tactor.style("with lines lc "+ ToString(c));
        gnu.add(tactor);
        c++;
    }
	gnu.plot();
}

TEST(partition, trydata){
    typedef typename std::vector<double> Vec;
    // std::cout << HasData<Vec>::value << std::endl;
    Point3 p1(0.0, 0.0, 0.0);
    Point3 p2(1.0, 0.0, 0.0);
    Point3 p3(0.0, 1.0, 0.0);
    Point3 p(-0.1, 0.1, 0.0);

    PointChain3 pc(p1, p2, p3);
    auto vn = NormalFirstThree(pc);
    std::cout << "Normal Vec : "<< vn << std::endl;

    auto ppc = ProjectAlong(pc, _Z_);
    ppc.show();
}


