#include <functional>

#define _DEBUG_MODE_

#include "utility/random.hpp"
#include "utility/profile.hpp"
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"


using namespace carpio;


typedef double NumType;
// typedef Rational_<long> NumType;
typedef Point_<NumType, 2> Point;
typedef Segment_<NumType, 2> Segment;


TEST(ben_ott, mkdir){
    // check current working dir
    auto curdir = GetWorkingPath();
    std::cout << "Current Working dir = " << curdir << std::endl;
    auto binclude = FileAccessCheck(curdir + "/include", 0);
    if (! binclude) {
        GTEST_FATAL_FAILURE_("Current working directroy may be not in Project Folder");
    }
    // Do not disable this test
    MakeDir("./fig/");
}

template<class LISTSEG>
void AddListSegment(Gnuplot& gnu, const LISTSEG& sl){
    // gnu.set_label(1, strtype, -4.5, 4);
    gnu.unset_label();
    int index = 1;
    for(auto seg : sl){
        auto a = ToGnuplotActor(seg);
        a.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
        auto nv = seg.normal_unit_vector();
        double ratio = 0.5;
        // std::ostringstream sst;
        // sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
            // << nv.x() * ratio << ", " << nv.y() * ratio; 
        // gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
        auto a1 = ToGnuplotActorAsVector(seg);
        // a1.title("Segment " + seg.get_name());
        a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
        gnu.add(a);
        gnu.add(a1);
        index++;
    }
}

template<class SEG_TYPE>
auto GenerateSegmentsCase1(){ // mulit right
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(0, 10), Point(15, 26)));
    lseg.push_back(Seg(Point(15, 26), Point(10, 40)));
    lseg.push_back(Seg(Point(15, 26), Point(2,  20))); //
    lseg.push_back(Seg(Point(15, 26), Point(10,  30)));
    lseg.push_back(Seg(Point(6, 18),  Point(20,  35))); //
    
    return lseg;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase3(){ // mulit left
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(0, 10), Point(15, 26)));
    lseg.push_back(Seg(Point(15, 26), Point(10, 40)));
    lseg.push_back(Seg(Point(2, 10),  Point(23,  20))); //
    lseg.push_back(Seg(Point(2, 10),  Point(10,  30)));
    lseg.push_back(Seg(Point(6, 18),  Point(20,  35))); //
    
    return lseg;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase2(){ //normal
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    //                 x1   x2   y1   y2
    lseg.push_back(Seg(3,  11, 15, 26)); 
    lseg.push_back(Seg(0,  30, 10, 40));
    lseg.push_back(Seg(10, 13, 21, 24));
    lseg.push_back(Seg(30, 10, 10, 30));
    lseg.push_back(Seg(25, 50, 20, 30));
    lseg.push_back(Seg(21, 33, 35, 19));
    lseg.push_back(Seg(18, 34, 25, 35));
    lseg.push_back(Seg(28, 38, 15, 18));
    return lseg;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase4(){ // intersect in mid
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(0, 10),   Point(15,  26)));
    lseg.push_back(Seg(Point(15, 30),  Point(10,  40)));
    lseg.push_back(Seg(Point(10, 10),  Point(20,  20))); //
    lseg.push_back(Seg(Point(15, 15),  Point(10,  30)));
    lseg.push_back(Seg(Point(10, 25),  Point(20,  35))); //
    
    return lseg;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase5(){ // intersect in mid
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(4, 10),   Point(6,  30)));
    lseg.push_back(Seg(Point(10, 15),  Point(10,  40)));
    lseg.push_back(Seg(Point(5,  20),  Point(20,  20)));
    lseg.push_back(Seg(Point(10, 10),  Point(20,  20))); //
    lseg.push_back(Seg(Point(15, 15),  Point(10,  30)));
    lseg.push_back(Seg(Point(10, 25),  Point(20,  35))); //
    
    return lseg;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase6(){ // intersect in mid
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(0, 0),  Point(10, 10)));
    lseg.push_back(Seg(Point(0, 5),  Point(10, 5)));
    lseg.push_back(Seg(Point(5, 0),  Point(5,  10)));
    lseg.push_back(Seg(Point(0, 10), Point(10, 0)));
    lseg.push_back(Seg(Point(2, 3),  Point(5, 5)));
    lseg.push_back(Seg(Point(5, 5),  Point(7, 8)));
    
    return lseg;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase7(){ // overlap
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    lseg.push_back(Seg(Point(0, 0),  Point(10, 10)));
    lseg.push_back(Seg(Point(3, 3),  Point(5, 5)));
    lseg.push_back(Seg(Point(8, 8),  Point(12, 12)));

    lseg.push_back(Seg(Point(0, 1),  Point(0, 10)));
    lseg.push_back(Seg(Point(0, 3),  Point(0, 5)));

    lseg.push_back(Seg(Point(1, 0),  Point(10, 0)));
    lseg.push_back(Seg(Point(3, 0),  Point(5,  0)));
    lseg.push_back(Seg(Point(12, 0),  Point(8, 0)));
    
    return lseg;
}
TEST(ben_ott, two_seg_order){
    Segment s1(Point(15, 26), Point(2,  20));  // left big  
    Segment s2(Point(6, 18),  Point(20,  35)); // left small

    typedef IntersectionBenOtt_<Segment> Inter;
    typedef Intersection_<Segment, Segment> InterTwo;
    InterTwo i(s1, s2);
    auto res = i.execute();
    std::cout << res.point << std::endl;

    Point pc = res.point;
    // pc.x(pc.x() + 0.01);
    Inter::CompareSeg comp(&(pc));
    Inter::StatusTree tree(comp);

    tree.insert(&s1);
    tree.insert(&s2);

    auto y1 = comp.y_at_sweep_point(s1, pc.x());
    auto y2 = comp.y_at_sweep_point(s2, pc.x());

    std::cout << "y1 = " << y1 << " y2 = " << y2 << std::endl;
    std::cout << "py = " << pc.y() << std::endl;
    std::cout << "is equal = " << (y1 == y2) << std::endl;
    std::cout << "is equal = " << (std::abs(y1-y2) < comp._e) << std::endl;
    
    int index = 1;
    for(auto ps : tree){
        std::cout << ToString(index) << "  " << *ps << std::endl;
        index++;
    }
}

template<class SEG_TYPE>
auto GenerateRandomSegments(int num,
                            const double& xmin, const double& xmax,
                            const double& ymin, const double& ymax){
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    // Random::seed(std::time(0));
    for (int i = 0; i< num ; i++){
        double x1 = Random::nextDouble(xmin, xmax);
        double y1 = Random::nextDouble(ymin, ymax);
        double x2 = Random::nextDouble(xmin, xmax);
        double y2 = Random::nextDouble(ymin, ymax);
        lseg.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    }
    return lseg;
}

TEST(ben_ott, case1){
    // auto sl = GenerateSegmentsCase7<Segment>();
    auto sl = GenerateRandomSegments<Segment>(6, 0, 500, 0, 100);
    auto resn2 = Intersect(sl, "N2");
    

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

    typedef IntersectionBenOtt_<Segment> Inter;
    Inter inter(sl);
    auto lres = inter.execute();
    std::cout << "Result size    = " << lres.size() << std::endl;
    std::cout << "Result size n2 = " << resn2.size() << std::endl;
    int count = 0;
    for(auto& res : lres){
        std::cout << count << " : "<< res.point << std::endl;
        count++;
    }
    
    // PlotListIntersectionResult(gnu, res);

    // gnu.plot();
}