#ifndef _MULTI_SEGMENTS_HPP_
#define _MULTI_SEGMENTS_HPP_

#include "two_segments.hpp" 

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
        // std::cout << a.style() << std::endl;
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

auto GenerateRandomSegments(int num,
                            const double& xmin, const double& xmax,
                            const double& ymin, const double& ymax){
    typedef std::list<Segment> ListSegment;
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

void MultiSegTestCase1N2(const std::list<Segment>& sl){
    // auto sl = GenerateSegmentsCase1<Segment>();
    // auto sl = GenerateRandomSegments(50, 0, 100, 0, 100);
    std::cout << "Segments n = " << sl.size() << std::endl;
    ProfileStart("Intersect N2");
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
void MultiSegTestCase1BenOtt(const std::list<Segment>& sl){
    // auto sl = GenerateSegmentsCase1<Segment>();
    // auto sl = GenerateRandomSegments(50, 0, 100, 0, 100);
    std::cout << "Segments n = " << sl.size() << std::endl;
    ProfileStart("Intersect Ben");
    auto res = Intersect(sl, "bentley_ottmann");
    std::cout << "Intersection Points : " << res.size() << std::endl;
    ProfileEnd();
    // std::cout << "Len Res = " << res.size() << std::endl;
    // for(auto& r : res){
        // r.show();
    // }
    ProfileListShow();
    
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1_ben_ott");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

    PlotListSegment(gnu, sl);
    PlotListIntersectionResult(gnu, res);

    gnu.plot();
}



void benchmark_test(){
    ProfileClean();
    std::vector<int> arr_num     = {100, 500, 1000, 2000};
    std::list<double> m1_time;
    std::list<double> m2_time;
    std::list<double> m3_time;

    for(auto& num : arr_num){
        ProfileStart("GenerateSegments_" + ToString(num));
        auto lseg = GenerateRandomSegments(num, 0, 100, 0, 100);
        ProfileEnd();
        // Method1 ==========================================
        auto start = std::chrono::system_clock::now();
        ProfileStart("Method_N2_" + ToString(num));
        tfm::format(std::cout, "Method_N2_%d  ", num); 
        auto res = Intersect(lseg);
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    auto end = std::chrono::system_clock::now();
    	double dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m1_time.push_back(dt);
        std::string out = tfm::format("%25s :  %15.5f %3s\n",
                                 "Method N2", dt, "s");
    	std::cout << out;
        // Method2 ==========================================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_SweepLine_" + ToString(num));
        tfm::format(std::cout, "Method_SweepLine_%d  ", num); 
        res = Intersect(lseg, "sweep_line_simple");
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    end = std::chrono::system_clock::now();
    	dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m2_time.push_back(dt);
        out = tfm::format("%25s :  %15.5f %3s\n",
                                 "Simple Sweep", dt, "s");
    	std::cout << out;
        // Method2 ==========================================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_Ben_" + ToString(num));
        tfm::format(std::cout, "Method_Ben_%d  ", num); 
        res = Intersect(lseg, "bentley_ottmann");
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    end = std::chrono::system_clock::now();
    	dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m3_time.push_back(dt);
        out = tfm::format("%25s :  %15.5f %3s\n",
                                 "bentley ottmann", dt, "s");
    	std::cout << out;
    }
    ProfileListShow();
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/benchmark");
    gnu.set_xlabel("number of segments");
    gnu.set_ylabel("time (s)");
    // gnu.set_label(1, strtype, -4.5, 4);

    auto a1 = ToGnuplotActor(arr_num, m1_time);
    a1.command("using 1:2 title \"Method n2\" ");
    a1.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");

    auto a2 = ToGnuplotActor(arr_num, m2_time);
    a2.command("using 1:2 title \"Method Sweep Line Simple\" ");
    a2.style("with linespoints pointtype 7 pointsize 3 lw 3 lc rgb \"#F25022\"");

    gnu.add(a1);
    gnu.add(a2);
    gnu.plot();
}


#endif
