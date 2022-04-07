#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"
#include "affine_transform_in_2d.h"
#include "affine_transform_in_3d.h"

using namespace carpio;

// main 
int main(){
    std::cout << "Affine Transformation 2D" << std::endl;
    TestTranslate();
    TestScale();
    TestScaleAbout();
    TestRotate();
    TestRotateAbout();
    TestReflectAboutAxes(_X_);
    TestReflectAboutAxes(_Y_);
    TestReflectAboutPoint();

    TestReflectAboutLine({1.0,1.0,1.5});

    TestShearIn(_X_, 0.2);
    TestShearIn(_Y_, 0.2);

    Point2 about = {0.4, 0.5};
    TestShearAbout(_X_, 0.2, about);
    TestShearAbout(_Y_, 0.2, about);


    std::cout << "Affine Transformation 3D" << std::endl;
    TestScaleIn3d();
}

