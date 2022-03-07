#include "gtest/gtest.h"
#include "domain/domain.hpp"
#include "domain/structure/structure.hpp"
// #include "domain/structure/io/splotly_actor.hpp"
// #include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./test_output/";

const int fig_width  = 900;
const int fig_height = 900;


TEST(sgrid, initial){
    typedef std::shared_ptr<SGrid_<2> > spGrid;
    typedef SGridUniform_<2> GridUniform;
    typedef std::shared_ptr<GridUniform> spGridUniform;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGridUniform spsg(new GridUniform(pmin, {10, 10}, 0.3, 2));

    auto out = Stringify(*spsg);
    for(auto& s : out){
        std::cout << s << std::endl;
    }
}

TEST(sgrid, nonuniform){
    typedef std::shared_ptr<SGrid_<2> > spGrid;
    typedef SGridUniform_<2> GridUniform;
    typedef SGridNonUniform_<2> GridNonUniform;
    typedef std::shared_ptr<GridUniform> spGridUniform;
    typedef std::shared_ptr<GridNonUniform> spGridNonUniform;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGridNonUniform spsg(
        new GridNonUniform(pmin, pmax, {10, 10}, 2));

    auto out = Stringify(*spsg);
    // for(auto& s : out){
        // std::cout << s << std::endl;
    // }

}