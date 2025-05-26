#ifndef _MULTI_SEGMENTS_HPP_
#define _MULTI_SEGMENTS_HPP_

#include <functional>
#include <map>

#include "utility/random.hpp"
// #include "utility/clock.hpp"
#include "utility/profile.hpp"
#include "utility/any.hpp"
#include "geometry/geometry.hpp"
#include "io/text_file.hpp"
#include "utility/tinyformat.hpp"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>


using namespace carpio;

typedef Point_<double, 2>   Point;
typedef Segment_<double, 2> Segment;
typedef std::list<Segment> ListSegment;
typedef Intersection_<Segment, Segment>  Inter;
typedef typename Inter::Result Result;

typedef std::map<std::string, Any> MapAny;
typedef std::function<ListSegment(int)> FunGenerator; 
typedef std::function<MapAny(const ListSegment&)> FunCal;

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
        // gnu.set_label(index, seg.get_name(), seg.pc().x(), seg.pc().y(),  sst.str());
        // auto a1 = ToGnuplotActorAsVector(seg);
        // a1.title("Segment " + seg.get_name());
        // a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
        gnu.add(a);
        // gnu.add(a1);
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


auto GenerateRandomSegments(int num){
    ListSegment lseg;
    const double& xmin = 0; const double& xmax = 100;
    const double& ymin = 0; const double& ymax = 100;
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
    ListSegment lseg;
    int num = std::floor(size / 2.0);
    double dy =-0.3;
    double dx = 0.1;
    for (int i = 1; i< num + 1 ; i++){
        lseg.emplace_back(Segment(Point(dx, i + dy), Point(dx + num, i + dy )));
    }
    for (int i = 1; i< num + 1 ; i++){
        lseg.emplace_back(Segment(Point(dx + i, dy), Point(dx + i, dy + num )));
    }
    return lseg;
}

auto GeneratorList(){
    std::map<std::string, FunGenerator> res;
    res["random"] = GenerateRandomSegments;
    res["grid"] = GenerateGrid;
    res["parallel_slant"] = GenerateParallelSlanted;
    std::cout << res.size() << std::endl;
    return res;
}

auto CalN2(const ListSegment& segs){
    MapAny res;
    res["input_num_segs"] = segs.size();
    res["name_method"] = "n2";
    auto start = std::chrono::system_clock::now();
    auto ires = Intersect(segs);
	auto end = std::chrono::system_clock::now();
  	double dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    res["dt"] = Any(dt);
    res["num_intersects"] = Any(ires.size());
    res["result"] = ires;
    return res;
}
auto CalSweepLineSimple(const ListSegment& segs){
    MapAny res;
    res["input_num_segs"] = segs.size();
    res["name_method"] = "sweep_line_simple";
    auto start = std::chrono::system_clock::now();
    auto ires = Intersect(segs, "sweep_line_simple");
	auto end = std::chrono::system_clock::now();
  	double dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    res["dt"] = Any(dt);
    res["num_intersects"] = Any(ires.size());
    res["result"] = ires;
    return res;
}
auto CalSweepLineBenOtt(const ListSegment& segs){
    MapAny res;
    res["input_num_segs"] = segs.size();
    res["name_method"] = "sweep_line_benott";
    auto start = std::chrono::system_clock::now();
    auto ires = Intersect(segs, "bentley_ottmann");
	auto end = std::chrono::system_clock::now();
  	double dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    res["dt"] = dt;
    res["num_intersects"] = ires.size();
    res["result"] = ires;
    return res;
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

auto CalSweepLineCGAL(const ListSegment& segs){
    std::map<std::string, Any> res;
    auto nlseg = ToCGAL(segs);
    res["input_num_segs"] = segs.size();
    res["name_method"] = "sweep_line_CGAL";
    auto start = std::chrono::system_clock::now();
    std::list<Point_2> pts;
    CGAL::compute_intersection_points(nlseg.begin(), nlseg.end(),
                                     std::back_inserter(pts)); 
        
	auto end = std::chrono::system_clock::now();
  	double dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    res["dt"] = Any(dt);
    res["num_intersects"] = Any(pts.size());
    res["result"] = pts;
    return res;
}

auto CalculatorList(){
    std::map<std::string, FunCal> res;
    res["n2"] = CalN2; 
    res["sweep_line_simple"] = CalSweepLineSimple; 
    res["sweep_line_benott"] = CalSweepLineBenOtt; 
    res["sweep_line_CGAL"] = CalSweepLineCGAL;
    return res; 
}

void Benchmark(){
    std::vector<int> arr_num     = {30};

    auto cal_list = CalculatorList();
    auto gen_list = GeneratorList();
    std::cout << "gen list size : " << gen_list.size() << std::endl;
    std::cout << "cal list size : " << cal_list.size() << std::endl;
    
    // 
    // FunGenerator generator = gen_list["grid"];

    typedef std::list<std::map<std::string, Any> > DataFrameAny;
    DataFrameAny df;

    for(int num : arr_num){
        for(const auto& kv : gen_list){
            tfm::format(std::cout, "gen  name : %10s \n", kv.first);
            auto generator = kv.second;
            tfm::format(std::cout, "input num : %10d \n", num);
            auto lseg = generator(num);
            tfm::format(std::cout, "num segs  : %10d \n", lseg.size());
            // PlotListSegment(lseg);
            for(const auto& kvcal : cal_list){
                tfm::format(std::cout, "cal  name : %10s ", kvcal.first);
                auto calculator = kvcal.second;
                auto rescal     = calculator(lseg);
                tfm::format(std::cout, " -> res = %10d ", any_cast<St>(rescal["num_intersects"]));
                tfm::format(std::cout, " -> dt  = %10.2f ms\n", any_cast<double>(rescal["dt"]));
                rescal["input_num"] = Any(num);
                rescal["gen_name"]  = Any(kv.first);
                rescal["cal_name"]  = Any(kvcal.first);
                df.push_back(rescal);
            }
        }
    }
}



#endif
