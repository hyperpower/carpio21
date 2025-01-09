#include "gtest/gtest.h"
// #include "domain/domain.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/grid/nonuniform.hpp"
#include "domain/base/base_io.hpp"
#include "domain/structure/io/s_stringify.hpp"
// #include "domain/structure/grid/sgrid.hpp"

// #include "domain/structure/io/splotly_actor.hpp"
// #include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 900;
const int fig_height = 900;


TEST(sgrid_io, initial){
    typedef SGridUniform_<2> GridUniform;
    typedef std::shared_ptr<GridUniform> spGridUniform;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGridUniform spsg(new GridUniform(pmin, {10, 10}, 0.3, 2));

    auto out = Stringify(*spsg);
    // for(auto& s : out){
    //     std::cout << s << std::endl;
    // }
    EXPECT_EQ(out.front(), "## TYPE_NAME : SGridUniform");
}

TEST(sgrid_io, nonuniform){
    typedef SGridNonUniform_<2> GridNonUniform;
    typedef std::shared_ptr<GridNonUniform> spGridNonUniform;

    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    spGridNonUniform spsg(
        new GridNonUniform(pmin, pmax, {10, 10}, 2));

    auto out = Stringify(*spsg);
    EXPECT_EQ(out.front(), "## TYPE_NAME : SGridNonUniform");
    // for(auto& s : out){
        // std::cout << s << std::endl;
    // }
}