#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";
const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActor_<double, 2> GA;
typedef Triangle_<double, 3> Tri;



TEST(tritri, initial){
    Point3 x(0,   0,    0);
    Point3 y(0.5, 0.5, -1.0);
    Point3 z(0.5, 0.5, 1.0);

    Point3 x2(0.0, 0.0, 0.0);
    Point3 y2(1.0, 0.0, 0.0);
    Point3 z2(0.0, 1.0, 0.0);

    Tri t1(x,y,z);
    Tri t2(x2,y2,z2);

    t1.show();
    t2.show();
    IntersectionTriTri_<Vt, 3> inter(t1, t2);
    std::cout << "Is Init     : " << inter.is_init() <<std::endl;

    inter.is_intersect();


    
}


TEST(output_triangle, triangle){
    Point3 x(0,   0,    0);
    Point3 y(0.5, 0.5, -1.0);
    Point3 z(0.5, 0.5, 1.0);
    
    Tri t1(x,y,z);
    std::list<Tri> ts;
    ts.push_back(t1);

    t1.show();

    GFile_<double, 3> gf;
    gf.WriteTriangles(OUTPUTPATH + "tri.txt", ts);
}