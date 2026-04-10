#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
#include "geometry/io/gplotly_actor.hpp"

#include "geometry/tri-tri/tri_tri_intersection.hpp"


using namespace carpio;

const std::string OUTPUTPATH = "./fig/";
const St dim = 3;

typedef Point_<float, dim> Point;
typedef Point_<float, 3> Point3;
typedef Point_<float, 2> Point2;
typedef GGnuplotActor_<float, 2> GA;
typedef Triangle_<float, 3> Tri;


TEST(tritri, initial){
    SCOPED_TRACE("tritri.initial");

    Point3 x(0,   0,    0);
    Point3 y(0.5, 0.5, -1.0);
    Point3 z(0.5, 0.5, 1.0);

    Point3 x2(0.0, 0.0, 0.0);
    Point3 y2(1.0, 0.0, 0.0);
    Point3 z2(0.0, 1.0, 0.0);


    Tri t1(x, y, z);
    Tri t2(x2,y2,z2);

    auto res = IsIntersect(t1[0], t1[1], t1[2],
                           t2[0], t2[1], t2[2]);
    EXPECT_EQ(res, 1)
        << "t1 and t2 should intersect.\n"
        << "t1 = " << t1 << "\n"
        << "t2 = " << t2 << "\n"
        << "intersection result = " << res;
}

TEST(tritri, inter3){
    SCOPED_TRACE("tritri.inter3");

    // Define vertices of two triangles
    Point V0 = {0.0, 0.0, 0.0}; // Triangle 1 vertex 0
    Point V1 = {1.0, 0.0, 0.0}; // Triangle 1 vertex 1
    Point V2 = {0.0, 1.0, 0.0}; // Triangle 1 vertex 2

    Point U0 = {0.5, 0.5, -0.5}; // Triangle 2 vertex 0
    Point U1 = {0.5, 0.5, 0.5};  // Triangle 2 vertex 1
    Point U2 = {0.5, -0.5, 0.0}; // Triangle 2 vertex 2

    Tri t1(V0, V1, V2);
    Tri t2(U0, U1, U2);

    // Test if the triangles intersect
    int result = IsIntersect(V0, V1, V2, U0, U1, U2);
    EXPECT_EQ(result, 1)
        << "Expected the triangles to intersect.\n"
        << "t1 = " << t1 << "\n"
        << "t2 = " << t2;

    // Test if the triangles intersect and get the intersection line
    bool coplanar = false; // Variable to check if triangles are coplanar
    Point isectpt1, isectpt2; // Intersection line endpoints
    result = Intersect(V0, V1, V2, U0, U1, U2, coplanar, isectpt1, isectpt2);
    ASSERT_EQ(result, 1)
        << "Expected Intersect to report an intersection.\n"
        << "t1 = " << t1 << "\n"
        << "t2 = " << t2;
    EXPECT_FALSE(coplanar)
        << "Expected a non-coplanar intersection.\n"
        << "t1 = " << t1 << "\n"
        << "t2 = " << t2 << "\n"
        << "isectpt1 = " << isectpt1 << "\n"
        << "isectpt2 = " << isectpt2;
    EXPECT_NE(isectpt1, isectpt2)
        << "Expected the intersection line segment to have two distinct endpoints.\n"
        << "isectpt1 = " << isectpt1 << "\n"
        << "isectpt2 = " << isectpt2;
}


TEST(tritri, triangle){
    SCOPED_TRACE("tritri.triangle");

    Point3 x(0,   0,    0);
    Point3 y(0.5, 0.5, -1.0);
    Point3 z(0.5, -0.5, 1.0);
    
    Tri t1(x,y,z);
    
    Point3 x2(0.1, 0.1,    0);
    Point3 y2(0.5, 0.5,  0.0);
    Point3 z2(0.5, -0.5, 0.0);
    
    Tri t2(x2,y2,z2);
    // Plot two triangles intersection
    bool coplanar = false; // Variable to check if triangles are coplanar
    Point isectpt1, isectpt2; // Intersection line endpoints
    auto result = Intersect(x, y, z, x2, y2, z2, coplanar, isectpt1, isectpt2);
    ASSERT_EQ(result, 1)
        << "Expected the triangles to intersect.\n"
        << "t1 = " << t1 << "\n"
        << "t2 = " << t2;
    EXPECT_FALSE(coplanar)
        << "Expected a non-coplanar intersection.\n"
        << "t1 = " << t1 << "\n"
        << "t2 = " << t2;
    EXPECT_NE(isectpt1, isectpt2)
        << "Expected the intersection line segment to have two distinct endpoints.\n"
        << "isectpt1 = " << isectpt1 << "\n"
        << "isectpt2 = " << isectpt2;

    Plotly plot;
    plot.add(ToPlotlyActor(t1, "t1", "wireframe"));
    plot.add(ToPlotlyActor(t2, "t2", "surface"));
    // plot.add(ToPlotlyActor(isectpt1, isectpt2, "wireframe"));
    plot.add(MakePlotlyArrow("intersection", isectpt1, isectpt2));
    plot.write("./fig/atri", "html");

}
