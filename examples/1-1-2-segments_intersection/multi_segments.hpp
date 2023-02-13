#ifndef _MULTI_SEGMENTS_HPP_
#define _MULTI_SEGMENTS_HPP_

#include "two_segments.hpp" 
#include "ben-ott/bentley_ottmann.hpp"
#include "ben-ott/ben_ott_structures.hpp"

template<class LISTSEG>
int SegmentsPlot(const std::string& fn, const LISTSEG& sl){
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/"+fn);
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");
    // gnu.set_label(1, strtype, -4.5, 4);

    auto a1 = ToGnuplotActor(sl);
    // a1.command("using 1:2 title "" ");
    a1.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");

    gnu.add(a1);
    gnu.plot();
    return 0;
}
template<class SEG_TYPE>
auto GenerateSegmentsCase1(){
    typedef SEG_TYPE Seg;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    //                      x1   x2   y1   y2
    lseg.push_back(Seg(0, 3, 1, 4));
    lseg.push_back(Seg(1, 3, 3, 1));
    lseg.push_back(Seg(2, 5, 2, 3));
    // lseg.push_back(Seg(2.1, 3.3, 3.5, 2.0));
    // lseg.push_back(Seg(1.8, 3.3, 2.5, 3.5));
    // lseg.push_back(Seg(2.8, 3.8, 1.5, 1.8));
    SegmentsPlot("case1", lseg);
    return lseg;
}
void MultiSegTestCase1(){
    auto sl = GenerateSegmentsCase1<ref::Segment>();
    // SegmentsPlot("case1", sl);
    auto res = ref::bentley_ottmann(sl);
    std::cout << "Size Res" << res.size() << std::endl;
    for (auto m : res){
        auto& e = m.first;
        auto& vs = m.second;
        std::cout << "x ="<<e.get_point().get_abscissa() << " y = " << e.get_point().get_ordinate() << std::endl;
    }
}

int TEST_DS(){
    typedef IntersectionBenOtt_<double> Inter;
    auto sl = GenerateSegmentsCase1<Segment_<double, 2> >();

    Inter inter(sl);

    inter.execute();

    return 1;
}


#endif
