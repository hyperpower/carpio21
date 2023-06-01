#include <functional>

#define _DEBUG_MODE_

#include "geometry/geometry.hpp"
#include "utility/random.hpp"
#include "utility/profile.hpp"
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
TEST(ben_ott, two_seg_order){
    Segment s1(Point(15, 26), Point(2,  20));  // left big  
    Segment s2(Point(6, 18),  Point(20,  35)); // left small

    typedef IntersectionBenOtt_<NumType> Inter;
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

TEST(ben_ott, case1){
    auto sl = GenerateSegmentsCase5<Segment>();

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

    typedef IntersectionBenOtt_<NumType> Inter;
    Inter inter(sl);
    auto lres = inter.execute();
    std::cout << "Result size = " << lres.size() << std::endl;
    int count = 0;
    for(auto& res : lres){
        std::cout << count << " : "<< res.point << std::endl;
        count++;
    }
    // PlotListIntersectionResult(gnu, res);

    gnu.plot();
}