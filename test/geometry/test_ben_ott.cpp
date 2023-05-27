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

// template<class LISTSEG>
// void PlotListSegment(Gnuplot& gnu, const LISTSEG& sl){
//     // gnu.set_label(1, strtype, -4.5, 4);
//     int index = 1;
//     for(auto seg : sl){
//         auto a = ToGnuplotActor(seg);
//         a.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
//         auto nv = seg.normal_unit_vector();
//         double ratio = 0.5;
//         // std::ostringstream sst;
//         // sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
//             // << nv.x() * ratio << ", " << nv.y() * ratio; 
//         // gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
//         auto a1 = ToGnuplotActorAsVector(seg);
//         // a1.title("Segment " + seg.get_name());
//         a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
//         gnu.add(a);
//         gnu.add(a1);
//         index++;
//     }
// }

template<class SEG_TYPE>
auto GenerateSegmentsCase1(){
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    //                 x1   x2   y1   y2
    lseg.push_back(Seg(3,  11, 15, 26));
    lseg.push_back(Seg(0,  30, 10, 40));
    lseg.push_back(Seg(30, 10, 10, 30));
    lseg.push_back(Seg(25, 50, 20, 30));
    lseg.push_back(Seg(21, 33, 35, 19));
    lseg.push_back(Seg(18, 34, 25, 35));
    lseg.push_back(Seg(28, 38, 15, 18));
    // int count = 1;
    // for(auto& seg : lseg){
    //     seg.set_name(ToString(count));
    //     count++;
    // }
    // SegmentsPlot("case1", lseg);
    return lseg;
}

TEST(ben_ott, case1){
    auto sl = GenerateSegmentsCase1<Segment>();
    // check current working dir
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

    typedef IntersectionBenOtt_<NumType> Inter;
    Inter inter(sl);
    inter.execute();
    // PlotListIntersectionResult(gnu, res);

    gnu.plot();
}