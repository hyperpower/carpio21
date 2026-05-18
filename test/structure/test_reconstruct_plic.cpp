#include "gtest/gtest.h"

#include "domain/boundary/boundary_index.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/scommon.hpp"
#include "domain/structure/operator/snorm.hpp"
#include "domain/structure/operator/svof_plic.hpp"
#include "geometry/boolean/line_box.hpp"
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/box.hpp"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace carpio;

namespace {

const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 1200;
const int fig_height = 900;

const double domain_min = 0.0;
const double domain_max = 1.0;
const int grid_n = 20;

using Grid = SGridUniform_<2>;
using Ghost = SGhostRegular_<2, Grid>;
using Order = SOrderXYZ_<2, Grid, Ghost, CenterTag>;
using Field = SFieldCenter_<2, double, Grid, Ghost, Order>;
using Point = Point_<double, 2>;
using Line = Line_<double>;
using Box = Box_<double, 2>;

using spGrid = std::shared_ptr<Grid>;
using spGhost = std::shared_ptr<Ghost>;
using spOrder = std::shared_ptr<Order>;

Line MakePresetLine() {
    return Line(1.0, 1.0, 0.78);
}

spGrid MakeGrid() {
    Point pmin(domain_min, domain_min);
    return spGrid(new Grid(
            pmin, {grid_n, grid_n},
            (domain_max - domain_min) / double(grid_n), 2));
}

Field MakeFractionField(spGrid grid, spGhost ghost, spOrder order) {
    Field fraction(grid, ghost, order);
    Line line = MakePresetLine();

    auto assign_fraction = [&line](Field& field, const Grid::Index& idx) {
        auto& grid = field.grid();
        auto pbox_min = grid.v(0, idx);
        auto pbox_max = grid.v(3, idx);
        Box cell_box(Point(pbox_min.x(), pbox_min.y()),
                     Point(pbox_max.x(), pbox_max.y()));
        field(idx) = IntersectionAreaNegative(line, cell_box) / cell_box.area();
    };
    ForEach(fraction, assign_fraction);
    return fraction;
}

template<class FIELD_INTERFACE>
GnuplotActor ToGnuplotActorReconstructedInterface(
        const FIELD_INTERFACE& interfaces){
    GnuplotActor actor;
    actor.command("using 1:2:(0.0) title \"reconstructed\" ");
    actor.style("with lines lc rgb \"#0072B2\" lw 2");

    const auto& grid = interfaces.grid();
    for(auto& idx : interfaces.order()){
        auto line = interfaces(idx);
        if(line == nullptr){
            continue;
        }

        auto pbox_min = grid.v(0, idx);
        auto pbox_max = grid.v(3, idx);
        Point local_min(0.0, 0.0);
        Point local_max(pbox_max.x() - pbox_min.x(),
                        pbox_max.y() - pbox_min.y());
        auto points = IntersectLineBox(
                local_min, local_max,
                line->a(), line->b(), line->alpha());
        if(points.size() < 2){
            continue;
        }

        for(auto& p : points){
            actor.data().push_back(
                    ToString(p.x() + pbox_min.x(),
                             p.y() + pbox_min.y(), " "));
        }
        actor.data().push_back("");
    }
    return actor;
}

} // namespace

TEST(reconstruct_plic, youngs_line_fraction_field) {
    auto grid = MakeGrid();
    auto ghost = std::make_shared<Ghost>(grid);
    auto order = std::make_shared<Order>(grid, ghost);
    auto fraction = MakeFractionField(grid, ghost, order);

    BoundaryIndex bi;
    auto interfaces = ReconstructInterfacePLIC(fraction, bi, "ccs");
    Field reconstructed_fraction(grid, ghost, order);

    int interface_count = 0;
    const double tol = 1e-12;
    for(auto& idx : fraction.order()){
        const auto value = fraction(idx);
        ASSERT_TRUE(std::isfinite(value));
        ASSERT_GE(value, -tol);
        ASSERT_LE(value, 1.0 + tol);

        if(value <= tol || value >= 1.0 - tol){
            ASSERT_EQ(interfaces(idx), nullptr);
            reconstructed_fraction(idx) = value;
            continue;
        }

        interface_count++;
        ASSERT_NE(interfaces(idx), nullptr);

        const auto dx = grid->s_(_X_, idx);
        const auto dy = grid->s_(_Y_, idx);
        const auto area = FractionVolume(
                interfaces(idx)->a(), interfaces(idx)->b(),
                interfaces(idx)->alpha(), dx, dy);
        reconstructed_fraction(idx) = area / (dx * dy);
        ASSERT_NEAR(reconstructed_fraction(idx), value, 1e-10);
    }
    ASSERT_GT(interface_count, 0);

    auto error_fraction = reconstructed_fraction - fraction;
    std::cout << "PLIC reconstructed fraction error\n";
    std::cout << "Norm1   = " << Norm1(error_fraction) << std::endl;
    std::cout << "Norm2   = " << Norm2(error_fraction) << std::endl;
    std::cout << "NormInf = " << NormInf(error_fraction) << std::endl;

    std::filesystem::create_directories(OUTPUTPATH);

    Gnuplot gnu;
    gnu.set_xrange(domain_min, domain_max);
    gnu.set_yrange(domain_min, domain_max);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_red_blue();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);

    auto agrid = ToGnuplotActorWireFrame(*grid);
    agrid.command("using 1:2:(0.0) title \"Domain\" ");
    agrid.line_color_grey().line_width(1);

    auto line = MakePresetLine();
    auto aline = ToGnuplotActor(
            line, std::vector<double>{domain_min - 0.3, domain_max});
    aline.command("using 1:2:(0.0) title \"line\" ");
    aline.line_color_red().line_width(2);

    gnu.add(ToGnuplotActorContour(fraction));
    gnu.add(agrid);
    gnu.add(aline);
    gnu.add(ToGnuplotActorReconstructedInterface(interfaces));
    gnu.set_terminal_png(OUTPUTPATH + "ReconstructPLICYoungsLine",
                         fig_width, fig_height);
    gnu.plot();
}
