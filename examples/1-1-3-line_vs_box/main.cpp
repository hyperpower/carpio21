#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"
#include "line_box.h"
#include "plane_box.h"

using namespace carpio;
// typedef Point_<double, 3>              Point3;
// typedef Box_<double, 3>                Box3;
typedef Plane_<double>                 Plane;
// typedef PointChain_<double, 3>         PointChain3;

void Case2d(){
    int r = LineBox(1.5, 1.0, 1.3, "line_box_normal");
    LineBox(1.5, 1.0, 0.0, "line_box_corner");
    LineBox(0.0, 1.0, 1.0, "line_box_edge");
    RotateLineCutBox();
    BoxLinePNCase(1, 1.3, 0.9,  1.5, 1.0, 1.1);
    BoxLinePNCase(2, 1.3, 0.9, -1.5, -1.0, -1.5);
}

void Case3d(){
    // 3d
    Point3 pmin(0.1,0.2,0.3);
    Point3 pmax(1.1,1.3,1.2);
    Box3 box3(pmin, pmax);
    Plane plane(-1.0, -1.1, -1.2, -2.5);
    PlaneBoxIntersection("plane_box_normal",box3, plane);

    // special case
    pmin = Point3(0.0,0.0,0.0);
    pmax = Point3(1.1,1.3,1.2);
    box3 = Box3(pmin, pmax);
    plane = Plane(-1.0, -1.1, -1.5, 0);
    PlaneBoxIntersection("plane_box_corner",box3, plane);
    
    pmin = Point3(0.0,0.0,0.0);
    pmax = Point3(1.1,1.3,1.2);
    box3 = Box3(pmin, pmax);
    plane = Plane(1.0, 0, 0.0, 1.1);
    PlaneBoxIntersection("plane_box_edge",box3, plane);
}

int main(){
    Case2d();
    Case3d();
}

