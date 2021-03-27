#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include <thread>

using namespace carpio;

const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef GGnuplotActor_<double, 2> GA;
typedef Triangle_<double, 3> Tri;



TEST(tritri, initial){
    Point3 x(0, 0, 0);
    Point3 y(0, 1, 0);
    Point3 z(0, 0, 1);

    Point3 x2(0.0, 0.0, 0.0);
    Point3 y2(0,   1.2, 0.0);
    Point3 z2(1.1,   0, 0.0);

    Tri t1(x,y,z);
    Tri t2(x2,y2,z2);

    IntersectionTriTri_<Vt, 3> inter(t1, t2);
    IntersectionTriTri_<Vt, 3> inter2(t1, t2);
    std::cout << "Is Init     : " << inter.is_init() <<std::endl;

    inter.is_intersect();

    t1.show();
    
}