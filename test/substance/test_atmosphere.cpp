#include "gtest/gtest.h"
#include "substance/atmosphere.hpp"
#include "io/gnuplot.hpp"
#include <math.h>

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";

const int fig_width  = 600;
const int fig_height = 900;

TEST(atmosphere, mkdir){
    // Do not disable this test
    MakeDir("./test_output/");
}

TEST(atmosphere, test1) {
    Atmosphere atmos;
    std::vector<Vt> x;
    std::vector<Vt> y;
    for(double z = 0; z <= 86000; z+=1000){
        atmos.set_altitude(z);
        y.push_back(atmos.geo_potential_altitude());
        x.push_back(atmos.molecular_scale_temperature());
    }

    Gnuplot gnu;
    GnuplotActorMaker maker;
    gnu.set_grid();
    gnu.set_title("Figure. 3 in US Standard Atmosphere 1976");
    gnu.set_xlabel("Molecular-scale temperature, K");
    gnu.set_xrange(160, 300);
    gnu.set_ylabel("Geoportential Altitude, m");
    gnu.add(maker.array_xy(
                  x, y,
                  "using 1:2 title \"\" ",
                  "with lines lw 1"));
    gnu.set_terminal_png(OUTPUTPATH + "Fig3MolecularScaleTemperature", fig_width, fig_height);
    gnu.plot();
}

TEST(atmosphere, test2) {
    Atmosphere atmos;
    std::vector<Vt> x;
    std::vector<Vt> y;
    for(double z = 0; z <= 1000; z+=1){
        atmos.set_altitude(z * 1000);
        y.push_back(atmos.altitude()/1000);
        x.push_back(atmos.mean_molecular_weight());
    }

    Gnuplot gnu;
    GnuplotActorMaker maker;
    gnu.set_grid();
    gnu.set_title("Figure. 6 in US Standard Atmosphere 1976");
    gnu.set_xlabel("Molecular Weight, kg/mol");
//	gnu.set_xrange(160, 300);
    gnu.set_ylabel("Geometric Altitude, km");
    gnu.add(maker.array_xy(
                  x, y,
                    "using 1:2 title \"\" ",
                  "with lines lw 1"));
    gnu.set_terminal_png(OUTPUTPATH + "Fig6MolecularWeight", fig_width, fig_height);
    gnu.plot();
}

TEST(atmosphere, pressure) {
    Atmosphere atmos;
    std::vector<Vt> x;
    std::vector<Vt> y;
    for(double z = 0; z <= 1000; z+=1){
        atmos.set_altitude(z * 1000);
        y.push_back(atmos.altitude()/1000);
        x.push_back(atmos.pressure());
    }

    Gnuplot gnu;
    GnuplotActorMaker maker;
    gnu.set_grid();
    gnu.set_title("Figure. 7 in US Standard Atmosphere 1976");
    gnu.set_xlabel("Pressure, Pa");
    gnu.set_xlogscale();
    gnu.set_xformat("%.0e");
//	gnu.set_xrange(160, 300);
    gnu.set_ylabel("Geometric Altitude, km");
    gnu.add(maker.array_xy(
                  x, y,
                    "using 1:2 title \"\" ",
                  "with lines lw 1"));
    gnu.set_terminal_png(OUTPUTPATH + "Fig7Pressure", fig_width, fig_height);
    gnu.plot();
}

TEST(atmosphere, Density) {
    Atmosphere atmos;
    std::vector<Vt> x;
    std::vector<Vt> y;
    for(double z = 0; z <= 1000; z+=1){
        atmos.set_altitude(z * 1000);
        y.push_back(atmos.altitude()/1000);
        x.push_back(atmos.density());
    }

    Gnuplot gnu;
    GnuplotActorMaker maker;

    gnu.set_grid();
    gnu.set_title("Figure. 7 in US Standard Atmosphere 1976");
    gnu.set_xlabel("Density, kg/m3");
    gnu.set_xlogscale();
    gnu.set_xformat("%.0e");
//	gnu.set_xrange(160, 300);
    gnu.set_ylabel("Geometric Altitude, km");
    gnu.add(maker.array_xy(
                  x, y,
                    "using 1:2 title \"\" ",
                  "with lines lw 1"));
    gnu.set_terminal_png(OUTPUTPATH + "Fig7Density", fig_width, fig_height);
    gnu.plot();
}

TEST(atmosphere, SpeedOfSound) {
    Atmosphere atmos;
    std::vector<Vt> x;
    std::vector<Vt> y;
    for(double z = 0; z <= 100; z+=1){
        atmos.set_altitude(z * 1000);
        y.push_back(atmos.altitude()/1000);
        x.push_back(atmos.speed_of_sound());
    }

    Gnuplot gnu;
    GnuplotActorMaker maker;

    gnu.set_grid();
    gnu.set_title("Figure. 13 in US Standard Atmosphere 1976");
    gnu.set_xlabel("Speed of Sound, m/s");
//	gnu.set_xlogscale();
    gnu.set_xformat("%.0f");
    gnu.set_yrange(0, 90);
    gnu.set_ylabel("Geometric Altitude, km");
    gnu.add(maker.array_xy(
                  x, y,
                    "using 1:2 title \"\" ",
                  "with lines lw 1"));
    gnu.set_terminal_png(OUTPUTPATH + "Fig13SpeedOfSound", fig_width, fig_height);
    gnu.plot();
}

TEST(atmosphere, DynamicViscosity) {
    Atmosphere atmos;
    std::vector<Vt> x;
    std::vector<Vt> y;
    for(double z = 0; z <= 100; z+=1){
        atmos.set_altitude(z * 1000);
        y.push_back(atmos.altitude()/1000);
        x.push_back(atmos.dynamic_viscosity() * 1e5);
    }

    Gnuplot gnu;
    GnuplotActorMaker maker;

    gnu.set_grid();
    gnu.set_title("Figure. 14 in US Standard Atmosphere 1976");
    gnu.set_xlabel("Dynamic Viscosity, 1e-5 Pa s");
//	gnu.set_xlogscale();
    gnu.set_xformat("%.1f");
    gnu.set_yrange(0, 90);
    gnu.set_ylabel("Geometric Altitude, km");
    gnu.add(maker.array_xy(
                  x, y,
                    "using 1:2 title \"\" ",
                  "with lines lw 1"));
    gnu.set_terminal_png(OUTPUTPATH + "Fig14DynamciViscosity", fig_width, fig_height);
    gnu.plot();
}

TEST(atmosphere, sea_level) {
    Atmosphere atmos(0);
    std::cout << "Sea level" << std::endl;
    std::cout << atmos << std::endl;

}

TEST(atmosphere, la_paz_bolivia) {
    Atmosphere atmos(3650);
    std::cout << "La Paz, Bolivia" << std::endl;
    std::cout << atmos << std::endl;
}

TEST(atmosphere, Everest) {
    Atmosphere atmos(8850);
    std::cout << "Mt. Everest" << std::endl;
    std::cout << atmos << std::endl;
}

TEST(atmosphere, Other) {
    Atmosphere atmos(51);
    std::cout << "Other" << std::endl;
    std::cout << atmos << std::endl;
}

