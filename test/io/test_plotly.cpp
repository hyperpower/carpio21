#include "gtest/gtest.h"
#include <math.h>
#include "algebra/array/array_list.hpp"
#include "geometry/geometry.hpp"
#include "io/python_interpreter.hpp"
#include "io/plotly.hpp"
using namespace carpio;

typedef Point_<double, 3> Point3;


TEST(test_p1, test1) {
    std::cout << "Test python lib works" << std::endl;
    PlotlyActor actor("Scatter3d");
    Plotly plotly;
    std::cout << "Plotly version : " << plotly.version() << std::endl;
}

TEST(test_plotly, test2) {
    std::cout << "aa" << std::endl;
    PlotlyActor actor("Scatter3d");
    Plotly plotly;
    std::cout << "Plotly version : " << plotly.version() << std::endl;
    actor.name("first");
    std::cout << "actor name     : " << actor.name() << std::endl;
    actor.opacity(0.5);
    std::cout << "opacity        : " << actor.opacity() << std::endl;
    std::cout << "trace          : " << actor.trace()   << std::endl;

    Point3 x(3, 1, 1);
    Point3 y(1, 4, 1);
    Point3 z(1, 1, 1);
    std::list<Point3> lp;
    lp.push_back(x);
    lp.push_back(y);
    lp.push_back(z);
    actor.data_xyz(lp);

    plotly.add(actor);
    // plotly.title("a title name ");
    plotly.margin(40, 40, 50, 40);
    plotly.layout("width", 800.0);
    plotly.layout("height", 600.0);
    plotly.layout("scene_aspectmode", "cube");
    plotly.layout("scene_xaxis_range", 0, 3);
    plotly.layout("scene_yaxis_range", 0, 3);
    plotly.layout("scene_zaxis_range", 0, 3);
    plotly.layout_false("scene_xaxis_showbackground");
    plotly.layout("scene_xaxis_gridcolor",      "black");
    plotly.layout_false("scene_yaxis_showbackground");
    plotly.layout("scene_yaxis_gridcolor",      "black");
    plotly.layout_false("scene_zaxis_showbackground");
    plotly.layout("scene_zaxis_gridcolor",      "black");


    // plotly.show();
    plotly.write("./fig/out", "png");
    
}
