#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/scommon.hpp"
#include "geometry/boolean/intersection_circle_box.hpp"
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/basic/box.hpp"

#include "circle_reconstruct.h"

#include <cmath>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>

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
using Circle = Circle_<Vt>;
using Box = Box_<Vt, 2>;

using spGrid = std::shared_ptr<Grid>;
using spGhost = std::shared_ptr<Ghost>;
using spOrder = std::shared_ptr<Order>;

Circle MakePresetCircle() {
    return Circle(0.5, 0.5, 0.25);
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
    Circle circle = MakePresetCircle();

    Gnuplot gnu;
    gnu.set_xrange(domain_min - 0.18, domain_max + 0.18);
    gnu.set_yrange(domain_min - 0.18, domain_max + 0.18);
    gnu.set_equal_aspect_ratio();

    auto agrid = ToGnuplotActorWireFrame(*grid);
    agrid.command("using 1:2 title \"Domain\" ");
    agrid.line_color_grey().line_width(1);

    auto acircle = ToGnuplotActor(circle);
    acircle.command("using 1:2 title \"circle\" ");
    acircle.line_color_red().line_width(2);

    auto acenter = ToGnuplotActor(circle.center());
    acenter.command("using 1:2 title \"center\" ");
    acenter.style("with points pt 7 ps 2 lc rgb \"black\"");

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
    gnu.set_label(3, "center",
                  0.5, 0.45,
                  "center noenhance");
    gnu.set_label(4, "(0.5, 0.5)",
                  0.5, 0.4,
                  "center noenhance");

    const double radius_angle = 40.0 * _PI_ / 180.0;
    const double arrow_x0 = circle.xc();
    const double arrow_y0 = circle.yc();
    const double arrow_x1 = circle.xc() + circle.r() * std::cos(radius_angle);
    const double arrow_y1 = circle.yc() + circle.r() * std::sin(radius_angle);
    std::ostringstream radius_arrow;
    radius_arrow << "set arrow 1 from first " << arrow_x0 << ", " << arrow_y0
                 << " to first " << arrow_x1 << ", " << arrow_y1
                 << " front filled size screen 0.02,15,60 lw 2 lc rgb \"black\"";
    gnu.cmd(radius_arrow.str());

    const double label_x = 0.5 * (arrow_x0 + arrow_x1)
                         - 0.035 * std::sin(radius_angle);
    const double label_y = 0.5 * (arrow_y0 + arrow_y1)
                         + 0.035 * std::cos(radius_angle);
    gnu.set_label(5, "r=0.5", label_x, label_y, "center rotate by 40 noenhance");

    gnu.add(agrid);
    gnu.add(acircle);
    gnu.add(acenter);
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
    Circle circle = MakePresetCircle();

    auto assign_fraction = [&circle](Field& field, const Grid::Index& idx) {
        auto& grid = field.grid();
        auto pbox_min = grid.v(0, idx);
        auto pbox_max = grid.v(3, idx);
        Box cell_box(Point(pbox_min.x(), pbox_min.y()),
                     Point(pbox_max.x(), pbox_max.y()));
        field(idx) = IntersectAreaCircleBox(circle, cell_box) / cell_box.area();
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

    auto acircle = ToGnuplotActor(circle);
    acircle.command("using 1:2:(0.0) title \"circle\" ");
    acircle.line_color_red().line_width(2);

    gnu.add(ToGnuplotActorContour(fraction));
    gnu.add(agrid);
    gnu.add(acircle);
    gnu.set_terminal_png(OUTPUTPATH + "fraction_preset",
                         fig_width, fig_height);
    gnu.plot();
}

}

int main() {
    PlotProblemDescription();
    PlotFractionPreset();
    circle_reconstruct::RunAll();
    return 0;
}
