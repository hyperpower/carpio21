#ifndef _MULTI_SEGMENTS_SIMPLE_HPP_
#define _MULTI_SEGMENTS_SIMPLE_HPP_
#include "io/text_file.hpp"
#include "utility/tinyformat.hpp"

#include "two_segments.hpp" 

template<class LISTSEG>
void PlotListSegment(Gnuplot& gnu, const LISTSEG& sl){
    // gnu.set_label(1, strtype, -4.5, 4);
    int index = 1;
    for(auto seg : sl){
        // auto a = ToGnuplotActor(seg);
        // a.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
        auto nv = seg.normal_unit_vector();
        double ratio = 0.5;
        std::ostringstream sst;
        sst << "front font \", 18\" textcolor rgb \"#3A4044\" offset first " 
            << nv.x() * ratio << ", " << nv.y() * ratio; 
        gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
        auto a1 = ToGnuplotActorAsVector(seg);
        if(index == 1){
            a1.title("Segment");
        }
        a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
        // gnu.add(a);
        gnu.add(a1);
        index++;
    }

}

template<class LISTSEGRES>
void PlotListIntersectionResult(Gnuplot& gnu, const LISTSEGRES& l){
    int index = 1;
    for(auto& res : l){
        auto a = ToGnuplotActor(res.point);
        a.point_type(7);
        a.point_size(2);
        a.line_color_red();
        if(index == l.size()){
            a.title("Intersect Point");
        }
        // std::cout << a.style() << std::endl;
        gnu.add(a);
        index++;
    }
}

template<class LISTSEG>
void OutputCSV(const std::string& filename, const LISTSEG& ls){
    TextFile file("fig/" + filename);
    file.add_line(tfm::format("%15s,%15s,%15s,%15s", "x1", "y1", "x2", "y2"));
    for (auto& s : ls){
        auto line = tfm::format("%15.3f,%15.3f,%15.3f,%15.3f", s[0][0], s[0][1], s[1][0], s[1][1]);
        file.add_line(line);
    }
    file.write();
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
template<class SEG_TYPE>
auto GenerateSegmentsCase2(){ // insersection on one point
    typedef SEG_TYPE Seg;
    typedef typename SEG_TYPE::Point Poi;
    typedef std::vector<SEG_TYPE> ListSegment;
    ListSegment lseg;
    //                 x1   x2   y1   y2
    // lseg.push_back(Seg(Poi(-10,  10), Poi(0, 0)));
    // lseg.push_back(Seg(Poi(-20, 5.0), Poi(0, 0)));
    // lseg.push_back(Seg(Poi(-30,-5.0), Poi(0, 0)));
    // lseg.push_back(Seg(Poi(0.0, 0.0), Poi(-15, -10)));
    lseg.push_back(Seg(Poi(-15, -15), Poi(15, 15)));
    lseg.push_back(Seg(Poi(-5, 15), Poi(5, -15)));
    lseg.push_back(Seg(Poi(10, 2),  Poi(20, 10)));
    lseg.push_back(Seg(Poi(10, 2),  Poi(15, 8)));
    lseg.push_back(Seg(Poi(10, 2),  Poi(13, -8)));
    lseg.push_back(Seg(Poi(10, 2),  Poi(20, -5)));
    lseg.push_back(Seg(Poi(8,  5),  Poi(12, -1)));
    // lseg.push_back(Seg(Poi(6, -1),  Poi(16, 5)));
    int count = 1;
    for(auto& seg : lseg){
        seg.set_name(ToString(count));
        count++;
    }
    // SegmentsPlot("case1", lseg);
    return lseg;
}

void MultiSegTestCase1N2(const std::string& filename, const std::list<Segment>& sl){
    // auto sl = GenerateSegmentsCase1<Segment>();
    // auto sl = GenerateRandomSegments(50, 0, 100, 0, 100);
    std::cout << "Segments n = " << sl.size() << std::endl;
    ProfileStart("Inter N2");
    auto res = Intersect(sl, "N2");
    std::cout << "Intersection Points : " << res.size() << std::endl;
    ProfileEnd();
    // std::cout << "Len Res = " << res.size() << std::endl;
    // for(auto& r : res){
        // r.show();
    // }
    ProfileListShow();
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1_N2");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

    PlotListSegment(gnu, sl);
    PlotListIntersectionResult(gnu, res);

    gnu.plot();
}

template<class LISTSEG>
void MultiSegTest_BenOtt(const std::string& filename, const LISTSEG& sl){
    // auto sl = GenerateSegmentsCase1<Segment>();
    // auto sl = GenerateRandomSegments(50, 0, 100, 0, 100);
    std::cout << "Segments n = " << sl.size() << std::endl;
    ProfileStart("Intersect Ben");
    auto res = Intersect(sl, "bentley_ottmann2");
    std::cout << "Intersection Points : " << res.size() << std::endl;
    ProfileEnd();
    // std::cout << "Len Res = " << res.size() << std::endl;
    // for(auto& r : res){
        // r.show();
    // }
    ProfileListShow();
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/" + filename);
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");
    gnu.set_grid();

    PlotListSegment(gnu, sl);
    PlotListIntersectionResult(gnu, res);

    gnu.plot();
}


int main_simple(){
    // test case 1
    auto ls = GenerateSegmentsCase1<Segment>();
    OutputCSV("bo_case1.txt", ls);
    MultiSegTest_BenOtt("bo_case1", ls);
    ls = GenerateSegmentsCase2<Segment>();
    MultiSegTest_BenOtt("bo_case2", ls);
    return 1;
}


#endif
