#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/scommon.hpp"
#include "geometry/boolean/line_box.hpp"
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/box.hpp"

#include "line_recontruct.h"

#include <filesystem>
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
using Point = Point_<Vt, 2>;
using Line = Line_<Vt>;
using Box = Box_<Vt, 2>;

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

void PlotProblemDescription() {
    std::filesystem::create_directories(OUTPUTPATH);

    auto grid = MakeGrid();
    Line line = MakePresetLine();

    Gnuplot gnu;
    gnu.set_xrange(domain_min - 0.18, domain_max + 0.18);
    gnu.set_yrange(domain_min - 0.18, domain_max + 0.18);
    gnu.set_equal_aspect_ratio();

    auto agrid = ToGnuplotActorWireFrame(*grid);
    agrid.command("using 1:2 title \"Domain\" ");
    agrid.line_color_grey().line_width(1);

    auto aline = ToGnuplotActor(line, std::vector<double>{domain_min - 0.3, domain_max});
    aline.command("using 1:2 title \"line\" ");
    aline.line_color_red().line_width(2);

    GnuplotActorDistanceIndicator width_indicator(domain_min, domain_min,
                                                  domain_max, domain_min);
    width_indicator.set_space(0.03);
    width_indicator.set_height(-0.05);
    width_indicator.set_normal_line_length(0.10);
    width_indicator.build_actors();

    GnuplotActorDistanceIndicator height_indicator(domain_max, domain_min,
                                                   domain_max, domain_max);
    height_indicator.set_space(0.03);
    height_indicator.set_height(-0.05);
    height_indicator.set_normal_line_length(0.10);
    height_indicator.build_actors();

    auto width_label_position = width_indicator.text_center(0.03);
    gnu.set_label(1, "width = 1.0",
                  width_label_position.x(), width_label_position.y(),
                  "center noenhance");

    auto height_label_position = height_indicator.text_center(0.03);
    gnu.set_label(2, "height = 1.0",
                  height_label_position.x(), height_label_position.y(),
                  "center rotate by 90 noenhance");
     gnu.set_label(3,"line: a X + b Y = alpha",
                  0.42, 0.42,
                  "center rotate by -45 noenhance");

    gnu.add(agrid);
    gnu.add(aline);
    gnu.add(width_indicator);
    gnu.add(height_indicator);
    gnu.set_terminal_png(OUTPUTPATH + "problem_description",
                         fig_width, fig_height);
    gnu.plot();
}

void PlotFractionPreset() {
    std::filesystem::create_directories(OUTPUTPATH);

    auto grid = MakeGrid();
    auto ghost = std::make_shared<Ghost>(grid);
    auto order = std::make_shared<Order>(grid, ghost);

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

    Gnuplot gnu;
    gnu.set_xrange(domain_min, domain_max);
    gnu.set_yrange(domain_min, domain_max);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_blues();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);

    auto agrid = ToGnuplotActorWireFrame(*grid);
    agrid.command("using 1:2:(0.0) title \"Domain\" ");
    agrid.line_color_grey().line_width(1);

    auto aline = ToGnuplotActor(line, std::vector<double>{domain_min - 0.3, domain_max});
    aline.command("using 1:2:(0.0) title \"line\" ");
    aline.line_color_red().line_width(2);

    gnu.add(ToGnuplotActorContour(fraction));
    gnu.add(agrid);
    gnu.add(aline);
    gnu.set_terminal_png(OUTPUTPATH + "fraction_preset",
                         fig_width, fig_height);
    gnu.plot();
}

}

int main() {
    PlotProblemDescription();
    PlotFractionPreset();
    line_recontruct::RunAll();
    return 0;
}
