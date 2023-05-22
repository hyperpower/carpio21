#ifndef _MULTI_SEGMENTS_HPP_
#define _MULTI_SEGMENTS_HPP_

#include "two_segments.hpp" 
#include "ben-ott/bentley_ottmann.hpp"
#include "ben-ott/ben_ott_structures.hpp"

template<class LISTSEG>
void PlotListSegment(Gnuplot& gnu, const LISTSEG& sl){
    // gnu.set_label(1, strtype, -4.5, 4);
    int index = 1;
    for(auto seg : sl){
        auto a = ToGnuplotActor(seg);
        a.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
        auto nv = seg.normal_unit_vector();
        double ratio = 0.5;
        std::ostringstream sst;
        sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
            << nv.x() * ratio << ", " << nv.y() * ratio; 
        gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
        auto a1 = ToGnuplotActorAsVector(seg);
        a1.title("Segment " + seg.get_name());
        a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
        gnu.add(a);
        gnu.add(a1);
        index++;
    }

}

template<class LISTSEGRES>
void PlotListIntersectionResult(Gnuplot& gnu, const LISTSEGRES& l){
    for(auto& res : l){
        auto a = ToGnuplotActor(res.point);
        a.point_type(7);
        a.point_size(2);
        a.line_color_red();
        std::cout << a.style() << std::endl;
        gnu.add(a);
    }
}
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
    lseg.push_back(Seg(21, 33, 35, 20));
    lseg.push_back(Seg(18, 33, 25, 35));
    lseg.push_back(Seg(28, 38, 15, 18));
    int count = 1;
    for(auto& seg : lseg){
        seg.set_name(ToString(count));
        count++;
    }
    // SegmentsPlot("case1", lseg);
    return lseg;
}

// void MultiSegTestCase1(){
//     auto sl = GenerateSegmentsCase1<ref::Segment>();
//     SegmentsPlot("case1", sl);
//     auto res = ref::bentley_ottmann(sl);
//     std::cout << "Size Res" << res.size() << std::endl;
//     for (auto m : res){
//         auto& e = m.first;
//         auto& vs = m.second;
//         std::cout << "x ="<<e.get_point().get_abscissa() << " y = " << e.get_point().get_ordinate() << std::endl;
//     }
// }
void MultiSegTestCase1N2(){
    auto sl = GenerateSegmentsCase1<Segment>();
    auto res = Intersect(sl, "N2");
    std::cout << "Len Res = " << res.size() << std::endl;
    for(auto& r : res){
        r.show();
    }
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1_N2");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

    PlotListSegment(gnu, sl);
    PlotListIntersectionResult(gnu, res);

    gnu.plot();
}
// int TEST_DS(){
//     typedef IntersectionBenOtt_<double> Inter;
//     auto sl = GenerateSegmentsCase1<Segment_<double, 2> >();

//     Inter inter(sl);

//     inter.execute();

//     return 1;
// }


#endif
