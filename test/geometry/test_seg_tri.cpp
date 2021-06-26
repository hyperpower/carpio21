#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";
const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActorMaker_<double, 2> GAM;
typedef Triangle_<double, 2> Tri2;
typedef Segment_<double, 2> Seg2;
typedef IntersectionSegTri_<double, 2> Inter;

void plot_triangle_by_code(Gnuplot& gnu, const Tri2& t, int code0, int code1){
    // input location code
    GAM gam;
    for(int i = 0; i < 3; i++){
        GAM::spActor actor = gam.points(t[i], -1);
        if((code0 == i) || (code1 == i)){
            actor->style() = "with points pt 5 ps 1 lc variable";
        }else{
            actor->style() = "with points pt 2 ps 1 lc variable";
        }
        gnu.add(actor);
    }
    // plot edge
    std::array<std::array<int, 2>, 3> arridx = {{
        {{0, 1}},{{0,2}},{{1, 2}}
    }}; 
    for(int i = 0; i < 3; i++){
        auto& pair = arridx[i];    
        auto& p0 = t[pair[0]];
        auto& p1 = t[pair[1]];
        auto actor = gam.arrows(p0,p1);
        if((code0 == i + 3) || (code1 == i + 3)){
            std::cout << "T edge = " << i + 3 << "intersect" << std::endl;
            actor->style() = "with vectors lw 3 lc variable";
        }else{
            actor->style() = "with vectors lc variable";
        }
        gnu.add(actor);
    }
}

void plot_segment_by_code(Gnuplot& gnu, const Seg2& seg, int code0, int code1){
    GAM gam;
    for(int i = 0; i < 2; i++){
        GAM::spActor actor = gam.points(seg[i], 1);
        if((code0 == i) || (code1 == i)){
            actor->style() = "with points pt 6 ps 2 lc variable";
        }else{
            actor->style() = "with points pt 2 ps 1 lc variable";
        }
        gnu.add(actor);
    }
    // plot edge
    for(int i = 0; i < 3; i++){
        // auto& pair = arridx[i];    
        auto& p0 = seg[0];
        auto& p1 = seg[1];
        auto actor = gam.arrows(p0,p1, 1);
        if((code0 == 2) || (code1 == 2)){
            std::cout << "S edge = " << i << "intersect" << std::endl;
            actor->style() = "with vectors lw 3 lc variable";
        }else{
            actor->style() = "with vectors lc variable";
        }
        gnu.add(actor);
    }
}

void one_test(const Tri2& t,const Seg2& s, 
    int loc0, int loc1, 
    short act0, short act1, short acs0, short acs1, 
    bool plot = false){
    Inter inter(t, s);
    inter.is_intersect();

    int l0 = inter.location_code(0);
    int l1 = inter.location_code(1);
    ASSERT_EQ(l0, loc0);
    ASSERT_EQ(l1, loc1);

    int ct0 = inter.triangle_intersection_code(0);
    int ct1 = inter.triangle_intersection_code(1);
    int cs0 = inter.segment_intersection_code(0);
    int cs1 = inter.segment_intersection_code(1);

    if(plot){
        Gnuplot gnu;
        gnu.set_xrange(-0.5, 1.5);
        gnu.set_yrange(-0.5, 1.5);
        plot_triangle_by_code(gnu, t, ct0, ct1);
        plot_segment_by_code(gnu, s, cs0, cs1);
        gnu.set_equal_aspect_ratio();
        gnu.plot();
    }
    ASSERT_EQ(ct0, act0);	
    ASSERT_EQ(ct1, act1);	
    ASSERT_EQ(cs0, acs0);	
    ASSERT_EQ(cs1, acs1);	
}
void one_test_reverse_seg(const Tri2& t,const Seg2& s, 
    int loc0, int loc1, 
    short act0, short act1, short acs0, short acs1, 
    bool plot = false){
    Seg2 sr(s[1],s[0]);
    Inter inter(t, sr);
    inter.is_intersect();

    int l0 = inter.location_code(0);
    int l1 = inter.location_code(1);
    ASSERT_EQ(l0, loc1);
    ASSERT_EQ(l1, loc0);

    int ct0 = inter.triangle_intersection_code(0);
    int ct1 = inter.triangle_intersection_code(1);
    int cs0 = inter.segment_intersection_code(0);
    int cs1 = inter.segment_intersection_code(1);

    if(plot){
        Gnuplot gnu;
        gnu.set_xrange(-0.5, 1.5);
        gnu.set_yrange(-0.5, 1.5);
        plot_triangle_by_code(gnu, t, ct0, ct1);
        plot_segment_by_code(gnu, sr, cs0, cs1);
        gnu.set_equal_aspect_ratio();
        gnu.plot();
    }

    ASSERT_EQ(ct0, act1);	
    ASSERT_EQ(ct1, act0);
    	
    short racs0 = (acs1 == 1)? 0 : acs1;
    short racs1 = (acs0 == 0)? 1 : acs0;
    ASSERT_EQ(cs0, racs0);	
    ASSERT_EQ(cs1, racs1);	
}

TEST(segtri, just_one){
    Point2 x(0.5, 0.0);
    Point2 y(0.5, 0.5);
    Seg2 seg(x, y);
    Point2 t0(0,   0);
    Point2 t1(1.,  0.0);
    Point2 t2(0.0, 1.0);
    Tri2 tri(t0, t1, t2);
    one_test(tri, seg, 1, 2, 3, 5, 0, 1, false);
}

std::list<std::vector<double> > data{{
    // 1-2
    {{0.5, 0.0, 0.5, 0.5, 1, 2, 3, 5, 0, 1}},
    {{0.5, 0.5, 0.5, 0.0, 2, 1, 5, 3, 0, 1}},
    {{0.5, 0.0, 0.3, 0.5, 1, 2, 3, 6, 0, 1}},
    {{0.5, 0.0, 0.7, 0.5, 1, 2, 3, 5, 0, 2}},
    {{1.2, 0.0, 0.5, 0.5, 1, 2, 7, 5, 3, 1}},
    {{1.2, 0.0, 0.3, 0.5, 1, 2, 5, 6, 2, 1}},
    {{1.2, 0.0, 1.3, 0.5, 1, 2, 7, 7, 3, 3}},
    // 3-2
    {{0.0, 0.5, 0.5, 0.5, 3, 2, 4, 5, 0, 1}},
    {{0.0, 0.5, 0.7, 0.5, 3, 2, 4, 5, 0, 2}},
    {{0.0, 0.5, 0.3, 0.3, 3, 2, 4, 6, 0, 1}},
    {{0.0, 1.3, 0.3, 0.3, 3, 2, 5, 6, 2, 1}},
    {{0.0, 1.3, 0.5, 0.5, 3, 2, 7, 5, 3, 1}},
    {{0.0, 1.3, 0.7, 0.7, 3, 2, 7, 7, 3, 3}},
    // 0-1 0-3
    {{0.0, 0.0, 0.3, 0.0, 0, 1, 0, 3, 0, 1}},
    {{0.0, 0.0, 1.0, 0.0, 0, 1, 0, 1, 0, 1}},
    {{0.0, 0.0, 1.3, 0.0, 0, 1, 0, 1, 0, 2}},
    {{0.0, 0.0, 0.0, 0.3, 0, 3, 0, 4, 0, 1}},
    {{0.0, 0.0, 0.0, 1.0, 0, 3, 0, 2, 0, 1}},
    {{0.0, 0.0, 0.0, 1.1, 0, 3, 0, 2, 0, 2}},
    // 0-2
    {{0.0, 0.0, 0.3, 0.3, 0, 2, 0, 6, 0, 1}},
    {{0.0, 0.0, 0.5, 0.5, 0, 2, 0, 5, 0, 1}},
    {{0.0, 0.0, 0.7, 0.7, 0, 2, 0, 5, 0, 2}},
    // 11 33
    {{0.1, 0.0, 0.6, 0.0, 1, 1, 3, 3, 0, 1}},
    {{0.1, 0.0, 1.0, 0.0, 1, 1, 3, 1, 0, 1}},
    {{0.1, 0.0, 1.2, 0.0, 1, 1, 3, 1, 0, 2}},
    {{0.0, 0.1, 0.0, 0.6, 3, 3, 4, 4, 0, 1}},
    {{0.0, 0.1, 0.0, 1.0, 3, 3, 4, 2, 0, 1}},
    {{0.0, 0.1, 0.0, 1.1, 3, 3, 4, 2, 0, 2}},
    // 0-45678
    {{0.0, 0.0, -0.3, 0.3, 0, 4, 0, 7, 0, 3}},
    {{0.0, 0.0, -0.3, 0.0, 0, 5, 0, 7, 0, 3}},
    {{0.0, 0.0, -0.3, -0.3, 0,6, 0, 7, 0, 3}},
    {{0.0, 0.0,  0.0, -0.3, 0,7, 0, 7, 0, 3}},
    {{0.0, 0.0,  0.3, -0.3, 0,8, 0, 7, 0, 3}},
    // 22
    {{0.1, 0.1,  0.3, 0.3, 2, 2, 6, 6, 0, 1}},
    {{0.1, 0.1,  0.5, 0.5, 2, 2, 6, 5, 0, 1}},
    {{0.1, 0.1,  0.7, 0.7, 2, 2, 6, 5, 0, 2}},
    // 13 
    {{0.1, 0.0, 0.0, 0.3, 1, 3, 3, 4, 0, 1}},
    {{0.1, 0.0, 0.0, 1.0, 1, 3, 3, 2, 0, 1}},
    {{0.1, 0.0, 0.0, 1.3, 1, 3, 3, 5, 0, 2}},
    {{1.0, 0.0, 0.0, 0.3, 1, 3, 1, 4, 0, 1}},
    {{1.0, 0.0, 0.0, 1.0, 1, 3, 1, 2, 0, 1}},
    {{1.0, 0.0, 0.0, 1.3, 1, 3, 1, 7, 0, 3}},
    // 14
    {{0.5, 0.0, -0.3, 0.5, 1, 4, 3, 4, 0, 2}},
    {{0.5, 0.0, -0.5, 2.0, 1, 4, 3, 2, 0, 2}},
    {{0.5, 0.0, -0.1, 1.3, 1, 4, 3, 5, 0, 2}},
    {{1.0, 0.0, -0.3, 0.5, 1, 4, 1, 4, 0, 2}},
    {{1.3, 0.0, -0.3, 0.5, 1, 4, 5, 4, 2, 2}},
    {{1.0, 0.0, -0.3, 1.5, 1, 4, 1, 7, 0, 3}},
    {{1.0, 0.0, -0.5, 1.5, 1, 4, 1, 2, 0, 2}},
    {{0.5, 0.1, -0.3, 0.5, 2, 4, 6, 4, 0, 2}}, // 2-4
    {{0.25,0.5,-0.25, 1.5, 2, 4, 6, 2, 0, 2}},
    {{0.5, 0.1, -0.1, 1.3, 2, 4, 6, 5, 0, 2}},
    {{0.5, 0.5, -0.1, 1.3, 2, 4, 5, 7, 0, 3}},
    {{0.6, 0.6, -0.1, 1.3, 2, 4, 7, 7, 3, 3}},
    {{0.0, 0.5, 0.5, -0.3, 3, 8, 4, 3, 0, 2}}, // 3-8
    {{0.0, 1.0, 0.5, -0.3, 3, 8, 2, 3, 0, 2}}, 
    {{0.0, 1.3, 0.5, -0.3, 3, 8, 5, 3, 2, 2}}, 
    {{0.0, 0.5, 1.3, -0.1, 3, 8, 4, 5, 0, 2}}, 
    {{0.1, 0.5, 0.5, -0.3, 2, 8, 6, 3, 0, 2}}, // 2-8
    {{0.5, 0.5, 1.5, -0.5, 2, 8, 5, 1, 0, 2}}, 
    {{0.6, 0.6, 0.8, -0.3, 2, 8, 5, 3, 2, 2}}, 
    // 3-4
    {{0.0, 0.5, -0.2,  0.3, 3, 4, 4, 7, 0, 3}}, 
    {{0.0, 1.0, -0.2,  0.3, 3, 4, 2, 7, 0, 3}}, 
    {{0.0, 1.1, -0.2,  0.3, 3, 4, 7, 7, 3, 3}}, 
    {{0.0, 1.1, -0.2,  0.0, 3, 5, 7, 7, 3, 3}}, // 3-5 
    {{0.0, 1.0, -0.2,  0.0, 3, 5, 2, 7, 0, 3}}, 
    {{0.0, 0.5, -0.2,  0.0, 3, 5, 4, 7, 0, 3}}, 
    {{0.0, 1.1, -0.2, -0.2, 3, 6, 7, 7, 3, 3}}, // 3-6 
    {{0.0, 1.0, -0.2, -0.3, 3, 6, 2, 7, 0, 3}}, 
    {{0.0, 0.5, -0.2, -0.4, 3, 6, 4, 7, 0, 3}}, 
    {{0.5, 0.0,  0.2, -0.3, 1, 8, 3, 7, 0, 3}}, // 1-8 
    {{1.0, 0.0,  0.2, -0.3, 1, 8, 1, 7, 0, 3}}, 
    {{1.1, 0.0,  0.2, -0.3, 1, 8, 7, 7, 3, 3}}, 
    {{0.5, 0.0,  0.0, -0.3, 1, 7, 3, 7, 0, 3}}, // 1-7 
    {{1.0, 0.0,  0.0, -0.3, 1, 7, 1, 7, 0, 3}}, 
    {{1.1, 0.0,  0.0, -0.3, 1, 7, 7, 7, 3, 3}}, 
    {{0.5, 0.0, -0.2, -0.3, 1, 6, 3, 7, 0, 3}}, // 1-6 
    {{1.0, 0.0, -0.2, -0.3, 1, 6, 1, 7, 0, 3}}, 
    {{1.1, 0.0, -0.2, -0.3, 1, 6, 7, 7, 3, 3}}, 
    // 2-5
    {{0.3, 0.3, -0.2, 0.0, 2, 5, 6, 4, 0, 2}},  
    {{0.3, 2.0, -0.3, 0.0, 2, 5, 2, 2, 2, 2}},  
    {{0.3, 2.1, -0.3, 0.0, 2, 5, 7, 7, 3, 3}},  
    {{0.3, 1.2, -0.3, 0.0, 2, 5, 5, 4, 2, 2}},  
    {{0.5, 0.5, -0.3, 0.0, 2, 5, 5, 4, 0, 2}},  
    {{0.5, 0.6, -0.3, 0.0, 2, 5, 5, 4, 2, 2}},  
    {{0.3, 0.3,  0.0,-0.2, 2, 7, 6, 3, 0, 2}}, // 2-7 
    {{2.0, 0.3,  0.0,-0.3, 2, 7, 1, 1, 2, 2}},  
    {{1.5, 0.1,  0.0,-0.3, 2, 7, 7, 7, 3, 3}},  
    // 1-5
    {{0.3, 0.0,  -0.2, 0.0, 1, 5, 3, 0, 0, 2}},  
    {{1.0, 0.0,  -0.2, 0.0, 1, 5, 1, 0, 0, 2}},  
    {{1.1, 0.0,  -0.2, 0.0, 1, 5, 1, 0, 2, 2}},  
    {{0.0, 0.3,  0.0, -0.2, 3, 7, 4, 0, 0, 2}}, // 3-7 
    {{0.0, 1.0,  0.0, -0.2, 3, 7, 2, 0, 0, 2}},   
    {{0.0, 1.1,  0.0, -0.2, 3, 7, 2, 0, 2, 2}},   

}};

TEST(segtri, many){
    int n = data.size();
    int count =0;
    for(auto& r : data){
        std::cout << "[INDDX "<< count << "   ] ============" << count << std::endl;
        Point2 x(r[0], r[1]);
        Point2 y(r[2], r[3]);
        Seg2 seg(x, y);
        Point2 t0(0,   0);
        Point2 t1(1.,  0.0);
        Point2 t2(0.0, 1.0);
        Tri2 tri(t0, t1, t2);
        bool is_plot = (count == n-1)? true : false;
        std::cout << "[NORMAL SEG] ============" << std::endl;
        one_test(tri, seg, r[4], r[5], r[6], r[7], r[8], r[9], is_plot);
        std::cout << "[REVERSESEG] ============" << std::endl;
        one_test_reverse_seg(tri, seg, r[4], r[5], r[6], r[7], r[8], r[9],  is_plot);
        count++;
    }
}
