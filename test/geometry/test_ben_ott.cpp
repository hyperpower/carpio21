#include <functional>

#define _DEBUG_MODE_

#include "utility/random.hpp"
#include "utility/profile.hpp"
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>


using namespace carpio;


typedef double NumType;
// typedef Rational_<long> NumType;
typedef Point_<NumType, 2> Point;
typedef Segment_<NumType, 2> Segment;
typedef IntersectionBenOtt_<Segment>  Inter;

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
        auto a1 = ToGnuplotActorVector(seg);
        // a1.title("Segment " + seg.get_name());
        a1.style("with vector head filled size screen 0.03,15,135 lw 3 lc rgb \"#00A4EF\"");
        gnu.add(a);
        gnu.add(a1);
        index++;
    }
}

template<class RES>
void AddIntersectionPoints(Gnuplot& gnu, const RES& r){
    for(auto a : r ){
        auto actor = ToGnuplotActor(a.point);
        actor.style("with points pointtype 7 pointsize 3 lw 3 lc rgb \"#E01C76\"");
        gnu.add(actor);
    }
}
template<class INPUT, class RES>
void PlotIntersection(const std::string& fn, const INPUT& list, const RES& res){
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/" + fn);
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");
    gnu.set_equal_aspect_ratio();
    AddListSegment(gnu, list);
    AddIntersectionPoints(gnu, res);
    gnu.plot();
}





TEST(ben_ott, two_seg_order){
    Segment s1(Point(15, 26), Point(2,  20));  // left big  
    Segment s2(Point(6, 18),  Point(20,  35)); // left small

    std::cout << "Two segments " << std::endl;
    std::cout << "1 - " << s1 << std::endl; 
    std::cout << "2 - " << s2 << std::endl; 

    typedef Intersection_<Segment, Segment> InterTwo;
    InterTwo i(s1, s2);
    auto res = i.execute();
    std::cout << "intersect at " << res.point << std::endl;

    Point pc = res.point;
    // pc.x(pc.x() + 0.01);
    Inter::CompareSeg comp(&(pc));
    typedef Inter::SegProxy SegProxy;
    Inter::StatusTree tree(comp);

    SegProxy sp1(s1);
    SegProxy sp2(s2);

    tree.insert(&sp1);
    tree.insert(&sp2);

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

TEST(ben_ott,random){
    // auto sl = GenerateSegmentsCase7<Segment>();
    auto sl = GenerateRandomSegments<Segment>(6, 0, 500, 0, 100);
    auto resn2 = Intersect(sl, "N2");
    

    Gnuplot gnu;
    gnu.set_terminal_png("./fig/case1");
    gnu.set_xlabel("x");
    gnu.set_ylabel("y");

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


// TEST(ben_ott, update){
//     typedef SegWithSlope_<Segment> SegS;
//     auto sl = GenerateSegmentsCase6<Segment>();
//     // auto sl = GenerateRandomSegments<Segment>(10, 0, 500, 0, 500);
//     // typedef std::list<SegProxy_<Segment> > ListSegProxy;
//     // ListSegProxy listseg;
//     // for(auto& seg : sl){
//         // listseg.emplace_back(seg);
//     // }

//     // ProfileStart("1 Ben Ott");
//     // Inter inter(sl);
//     // auto lres = inter.execute();
//     // std::cout << "1 Result size    = " << lres.size() << std::endl;
//     // ProfileEnd();
//     ProfileStart("1 Ben Ott");
//     Inter inter(sl);
//     auto lres = inter.execute();
//     std::cout << "1 Result size    = " << lres.size() << std::endl;
//     ProfileEnd();

//     ProfileListShow();
//     ProfileClean();
// }

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

// TEST(cgal_example, test){
//   // Construct the input segments.
//   Segment_2 segments[] = {Segment_2 (Point_2 (1, 5), Point_2 (8, 5)),
//                           Segment_2 (Point_2 (1, 1), Point_2 (8, 8)),
//                           Segment_2 (Point_2 (3, 1), Point_2 (3, 8)),
//                           Segment_2 (Point_2 (8, 5), Point_2 (8, 8))};
//   // Compute all intersection points.
//   std::list<Point_2> pts;
//   CGAL::compute_intersection_points(segments, segments + 4,
//                                     std::back_inserter(pts));
//   // Print the result.
//   std::cout << "Found " << pts.size() << " intersection points: " << std::endl;
//   std::copy(pts.begin(), pts.end(),
//             std::ostream_iterator<Point_2>(std::cout, "\n"));
//   // Compute the non-intersecting sub-segments induced by the input segments.
//   std::list<Segment_2> sub_segs;
//   CGAL::compute_subcurves(segments, segments + 4, std::back_inserter(sub_segs));
//   std::cout << "Found " << sub_segs.size()
//             << " interior-disjoint sub-segments." << std::endl;
//   assert(CGAL::do_curves_intersect (segments, segments + 4));
  
// }
TEST(cgal_random, DISABLED_test){
    auto sl = GenerateRandomSegments<Segment>(100, 0, 600, 0, 500);
    auto nsl = ToCGAL(sl);

    std::cout << "Input size     = " << nsl.size() << std::endl;
    ProfileStart("CGAL");
    std::list<Point_2> pts;
    CGAL::compute_intersection_points(nsl.begin(), nsl.end(),
                                     std::back_inserter(pts));
    std::cout << "CGAL   size    = " << pts.size() << std::endl;
    // std::copy(pts.begin(), pts.end(),
            // std::ostream_iterator<Point_2>(std::cout, "\n"));
    ProfileEnd();

    ProfileStart("BenOtt");
    Inter inter(sl);
    auto lres = inter.execute();
    std::cout << "Ben-Ott size   = " << lres.size() << std::endl;
    ProfileEnd();

    ProfileStart("N2");    
    auto resn2 = Intersect(sl, "N2");
    std::cout << "Ben-Ott size   = " << resn2.size() << std::endl;
    ProfileEnd();

    ProfileListShow();
}