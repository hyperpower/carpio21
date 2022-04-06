/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */

#ifndef GEOMETRY_TEST_TS_HPP_
#define GEOMETRY_TEST_TS_HPP_

// #include "io/python_interpreter.hpp"
// #include "io/plotly.hpp"
// #include "geometry/io/gplotly_actor_ts.hpp"
#include "geometry/io/gplotly_actor.hpp"
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

using namespace carpio;

TEST(TS, ts_initial){
    typedef TriSurface_<double, 3> TS;
    typedef CreateTS_<double, 3> CreateTS;
    CreateTS creator;
    // typedef GPlotlyActor_TS_<double, 3> PA;

    auto sur = creator.triangle(
            TS::Poi(0.0, 0.0, 0.0),
            TS::Poi(0.1, 0.1, 0.1),
            TS::Poi(0.0, 0.5, 0.3));

    Plotly_ plot;
    plot.add(ToPlotlyActor(*sur));
    // plot.write("./fig/out", "html");

}

TEST(TS, ts_initial2){
    typedef TriSurface_<double, 3> TS;
    typedef CreateTS_<double, 3> CreateTS;
    // typedef GPlotlyActor_TS_<double, 3> PA;

    CreateTS creator;
    auto sur = creator.cone(50, 0.5, 0.0, 1.0);

    Plotly_ plot;
    std::cout << "Plotly Version : " << plot.version() << std::endl;
    plot.add(ToPlotlyActor(*sur));
    plot.add(ToPlotlyActor(*sur, "surface"));
    // // out put div
    // plot.set_output_type("div");
    // plot.set_include_plotlyjs(false);
    // auto str = plot.plot();

    // std::ofstream ofs ("test.div", std::ofstream::out);
    // ofs << str;
    // ofs.close();

    // // output html
    // plot.set_output_type("file");
    // plot.set_include_plotlyjs(true);
    // str = plot.plot();

    // std::cout << str << std::endl;
    // plot.write("./fig/out", "html");

}

TEST(TS, ts_initial3){
    typedef TriSurface_<double, 3> TS;
    typedef CreateTS_<double, 3> CreateTS;
    // typedef GPlotlyActor_TS_<double, 3> PA;

    CreateTS creator;
    // auto sur = creator.from_ply_file("./test/input_files/icosahedron_ascii.ply");
    auto sur = creator.from_ply_file("./test/input_files/elephant.ply");

    Plotly_ plot;
    std::cout << "Plotly Version : " << plot.version() << std::endl;
    plot.add(ToPlotlyActor(*sur, "surface"));
    plot.add(ToPlotlyActor(*sur));
    // // out put div
    // plot.set_output_type("div");
    // plot.set_include_plotlyjs(false);
    // auto str = plot.plot();

    // std::ofstream ofs ("test.div", std::ofstream::out);
    // ofs << str;
    // ofs.close();

    // // output html
    // plot.set_output_type("file");
    // plot.set_include_plotlyjs(true);
    // str = plot.plot();

    // std::cout << str << std::endl;
    plot.write("./fig/out", "html");

}
#endif
