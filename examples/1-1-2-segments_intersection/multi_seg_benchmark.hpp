#ifndef _MULTI_SEGMENTS_HPP_
#define _MULTI_SEGMENTS_HPP_

#include "two_segments.hpp" 

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>


// template<class LISTSEG>
// void PlotListSegment(Gnuplot& gnu, const LISTSEG& sl){
//     // gnu.set_label(1, strtype, -4.5, 4);
//     int index = 1;
//     for(auto seg : sl){
//         auto a = ToGnuplotActor(seg);
//         a.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#00A4EF\"");
//         auto nv = seg.normal_unit_vector();
//         double ratio = 0.5;
//         std::ostringstream sst;
//         sst << "front font \", 18\" textcolor rgb \"#00A4EF\" offset first " 
//             << nv.x() * ratio << ", " << nv.y() * ratio; 
//         gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
//         auto a1 = ToGnuplotActorAsVector(seg);
//         a1.title("Segment " + seg.get_name());
//         a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
//         gnu.add(a);
//         gnu.add(a1);
//         index++;
//     }

// }

// template<class LISTSEGRES>
// void PlotListIntersectionResult(Gnuplot& gnu, const LISTSEGRES& l){
//     for(auto& res : l){
//         auto a = ToGnuplotActor(res.point);
//         a.point_type(7);
//         a.point_size(2);
//         a.line_color_red();
//         // std::cout << a.style() << std::endl;
//         gnu.add(a);
//     }
// }

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

auto GenerateParallelSlanted(int num){
    typedef std::list<Segment> ListSegment;
    ListSegment lseg;
    for (int i = 1; i< num + 1 ; i++){
        lseg.push_back(Segment(Point(-i, i), Point(i, i+i)));
    }
    return lseg; 
}

auto GenerateSparse(int size){
    typedef std::list<Segment> ListSegment;
    ListSegment lseg;
    int num = std::floor(std::sqrt(double(size)));
    for (int i = 1; i< num + 1 ; i++){
        for (int j = 1; j< num + 1; j++){
            double x = i * 10; 
            double y = j * 10;
            double r = Random::nextDouble();
            lseg.emplace_back(Segment(Point(x, y), Point(x + 10 * std::cos(r), y + 10 * std::sin(r)) ) );
        }
    }
    return lseg;
}

auto GenerateGrid(int size){
    typedef std::list<Segment> ListSegment;
    ListSegment lseg;
    int num = std::floor(size / 2.0);
    double dy =-0.3;
    double dx = 0.1;
    for (int i = 1; i< num + 1 ; i++){
        lseg.emplace_back(Segment(Point(dx, i + dy), Point(dx + num, dy + i )));
    }
    for (int i = 1; i< num + 1 ; i++){
        lseg.emplace_back(Segment(Point(dx + i, dy), Point(dx + i, dy + num )));
    }
    return lseg;
}

void MultiSegTestCase1N2(const std::list<Segment>& sl){
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


typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef Kernel::Point_2                                         Point_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;
typedef Traits_2::Curve_2                                       Segment_2;

template<class LISTSEG>
auto ToCGAL(const LISTSEG& lseg){
    std::vector<Segment_2> nl;
    for(auto& s : lseg){
        nl.emplace_back(Segment_2(Point_2(s.psx(), s.psy()), Point_2(s.pex(), s.pey())));
    }
    return nl;
}

void benchmark_test(){
    ProfileClean();
    std::vector<int> arr_num     = {30};
    std::list<double> m1_time;
    std::list<double> m2_time;
    std::list<double> m3_time;

    for(auto& num : arr_num){
        ProfileStart("GenSeg_" + ToString(num));
        auto lseg = GenerateRandomSegments(num, 0, 100, 0, 100);
        // auto lseg = GenerateParallelSlanted(num);
        // auto lseg = GenerateSparse(num);
        // auto lseg = GenerateGrid(num);
        auto nlseg = ToCGAL(lseg);
        std::cout << "Segment size = " << lseg.size() << std::endl;
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
        // Method2 Simple sweeep line =============================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_sl_" + ToString(num));
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
        // Method Ben Ott ==========================================
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
        // Method CGAL ==========================================
        start = std::chrono::system_clock::now();
        ProfileStart("Method_CGAL_" + ToString(num));
        tfm::format(std::cout, "Method_CGAL_%d  ", num); 
        
        std::list<Point_2> pts;
        CGAL::compute_intersection_points(nlseg.begin(), nlseg.end(),
                                     std::back_inserter(pts)); 
        
        tfm::format(std::cout, " find %10d\n", res.size()); 
        ProfileEnd();
	    end = std::chrono::system_clock::now();
    	dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        m3_time.push_back(dt);
        out = tfm::format("%25s :  %15.5f %3s\n",
                                 "CGAL", dt, "s");
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