#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

#include "geometry/tri-tri/tri_tri_intersection.hpp"


using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";
const St dim = 3;

typedef Point_<float, dim> Point;
typedef Point_<float, 3> Point3;
typedef Point_<float, 2> Point2;
typedef GGnuplotActor_<float, 2> GA;
typedef Triangle_<float, 3> Tri;


TEST(tritri, initial){
    Point3 x(0,   0,    0);
    Point3 y(0.5, 0.5, -1.0);
    Point3 z(0.5, 0.5, 1.0);

    Point3 x2(0.0, 0.0, 0.0);
    Point3 y2(1.0, 0.0, 0.0);
    Point3 z2(0.0, 1.0, 0.0);

    Tri t1(x, y, z);
    Tri t2(x2,y2,z2);

    t1.show();
    t2.show();

    auto res = tri_tri_intersect(t1[0].data(), t1[1].data(), t1[2].data(),
                        t2[0].data(), t2[1].data(), t2[2].data());
    std::cout << "Intersection Result : " << res << std::endl;
    // IntersectionTriTri_<Vt, 3> inter(t1, t2);
    // std::cout << "Is Init     : " << inter.is_init() <<std::endl;

    // inter.is_intersect();
    
}

TEST(tritri, inter3){
    // Define vertices of two triangles
    Point V0 = {0.0, 0.0, 0.0}; // Triangle 1 vertex 0
    Point V1 = {1.0, 0.0, 0.0}; // Triangle 1 vertex 1
    Point V2 = {0.0, 1.0, 0.0}; // Triangle 1 vertex 2

    Point U0 = {0.5, 0.5, -0.5}; // Triangle 2 vertex 0
    Point U1 = {0.5, 0.5, 0.5};  // Triangle 2 vertex 1
    Point U2 = {0.5, -0.5, 0.0}; // Triangle 2 vertex 2

    // Test if the triangles intersect
    int result = carpio::tri_tri_intersect(V0.data(), V1.data(), V2.data(),
                                           U0.data(), U1.data(), U2.data());
    if (result == 1) {
        std::cout << "Triangles intersect!" << std::endl;
    } else {
        std::cout << "Triangles do not intersect." << std::endl;
    }

    // Test if the triangles intersect and get the intersection line
    int coplanar = 0; // Variable to check if triangles are coplanar
    float isectpt1[3], isectpt2[3]; // Intersection line endpoints
    result = carpio::tri_tri_intersect_with_isectline(V0.data(), V1.data(), V2.data(), 
                                                      U0.data(), U1.data(), U2.data(), &coplanar, isectpt1, isectpt2);
    if (result == 1) {
        if (coplanar) {
            std::cout << "Triangles are coplanar." << std::endl;
        } else {
            std::cout << "Triangles intersect!" << std::endl;
            std::cout << "Intersection line endpoints: (" 
                      << isectpt1[0] << ", " << isectpt1[1] << ", " << isectpt1[2] << ") and ("
                      << isectpt2[0] << ", " << isectpt2[1] << ", " << isectpt2[2] << ")" << std::endl;
        }
    } else {
        std::cout << "Triangles do not intersect." << std::endl;
    }
}


TEST(tritri, triangle){
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