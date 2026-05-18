#ifndef _EXAMPLE_CIRCLE_RECONSTRUCT_H_
#define _EXAMPLE_CIRCLE_RECONSTRUCT_H_

#include "domain/boundary/boundary_index.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "domain/structure/operator/scommon.hpp"
#include "domain/structure/operator/snorm.hpp"
#include "domain/structure/operator/svof_plic.hpp"
#include "domain/structure/order/xyz.hpp"
#include "geometry/boolean/intersection_circle_box.hpp"
#include "geometry/boolean/line_box.hpp"
#include "geometry/fraction/error_area.hpp"
#include "geometry/fraction/piecewise_linear.hpp"
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/basic/box.hpp"

#include "convergence_analysis.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

using namespace carpio;

namespace circle_reconstruct {

const std::size_t dim = 2;
const std::string OUTPUTPATH = "./fig/";
const int fig_width = 1200;
const int fig_height = 900;
const double domain_min = 0.0;
const double domain_max = 1.0;
const double circle_xc = 0.5;
const double circle_yc = 0.5;
const double circle_r = 0.25;

typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double, dim> Point;
typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;
typedef Circle_<double> Circle;
typedef Box_<double, dim> Box;

inline Circle MakePresetCircle() {
    return Circle(circle_xc, circle_yc, circle_r);
}

inline spGrid MakeGrid(int n) {
    Point pmin(domain_min, domain_min);
    return spGrid(new Grid(
            pmin, {n, n}, (domain_max - domain_min) / double(n), 2));
}

inline Field MakeFractionField(spGrid grid, spGhost ghost, spOrder order) {
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
    return fraction;
}

template<class FIELD_INTERFACE>
inline GnuplotActor ToGnuplotActorReconstructedInterface(
        const FIELD_INTERFACE& interfaces) {
    GnuplotActor actor;
    actor.command("using 1:2:(0.0) title \"reconstructed\" ");
    actor.style("with lines lc rgb \"#FFBA08\" lw 3");

    const auto& grid = interfaces.grid();
    for(auto& idx : interfaces.order()) {
        auto line = interfaces(idx);
        if(line == nullptr) {
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
        if(points.size() < 2) {
            continue;
        }

        for(auto& p : points) {
            actor.data().push_back(
                    ToString(p.x() + pbox_min.x(),
                             p.y() + pbox_min.y(), " "));
        }
        actor.data().push_back("");
    }
    return actor;
}

template<class FIELD_INTERFACE>
inline void PlotReconstructedInterface(
        const std::string& method, int n,
        const Field& fraction,
        const FIELD_INTERFACE& interfaces) {
    std::filesystem::create_directories(OUTPUTPATH);

    Gnuplot gnu;
    gnu.set_xrange(domain_min, domain_max);
    gnu.set_yrange(domain_min, domain_max);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_blues();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);

    auto agrid = ToGnuplotActorWireFrame(fraction.grid());
    agrid.command("using 1:2:(0.0) title \"Domain\" ");
    agrid.line_color_grey().line_width(1);

    auto circle = MakePresetCircle();
    auto acircle = ToGnuplotActor(circle);
    acircle.command("using 1:2:(0.0) title \"circle\" ");
    acircle.line_color_red().line_width(3);

    gnu.add(ToGnuplotActorContour(fraction));
    gnu.add(agrid);
    gnu.add(acircle);
    gnu.add(ToGnuplotActorReconstructedInterface(interfaces));
    gnu.set_terminal_png(
            OUTPUTPATH + "reconstruct_" + method + "_n" + ToString(n),
            fig_width, fig_height);
    gnu.plot();
}

template<class FIELD_INTERFACE>
inline void PlotErrorAreaContour(
        const std::string& method, int n,
        const Field& error_area,
        const FIELD_INTERFACE& interfaces) {
    std::filesystem::create_directories(OUTPUTPATH);

    double max_error_area = 0.0;
    for(auto& idx : error_area.order()) {
        max_error_area = std::max(max_error_area, error_area(idx));
    }

    Gnuplot gnu;
    gnu.set_xrange(domain_min, domain_max);
    gnu.set_yrange(domain_min, domain_max);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_red_blue();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);
    if(max_error_area > 0.0) {
        gnu.set_cbrange(0.0, 10e-3);
    }

    auto agrid = ToGnuplotActorWireFrame(error_area.grid());
    agrid.command("using 1:2:(0.0) title \"Domain\" ");
    agrid.line_color_grey().line_width(1);

    auto circle = MakePresetCircle();
    auto acircle = ToGnuplotActor(circle);
    acircle.command("using 1:2:(0.0) title \"circle\" ");
    acircle.line_color_red().line_width(3);

    gnu.add(ToGnuplotActorContour(error_area));
    gnu.add(agrid);
    gnu.add(acircle);
    gnu.add(ToGnuplotActorReconstructedInterface(interfaces));
    gnu.set_terminal_png(
            OUTPUTPATH + "error_area_" + method + "_n" + ToString(n),
            fig_width, fig_height);
    gnu.plot();
}

inline void ReconstructSolver(const std::string& method, int n,
                              std::list<double>& l1,
                              std::list<double>& l2,
                              std::list<double>& li) {
    std::cout << "[  PLIC  ] Reconstruct circle" << std::endl;
    std::cout << "[  INFO  ] Method = " << method << std::endl;
    std::cout << "[  INFO  ] n      = " << n << std::endl;

    auto grid = MakeGrid(n);
    auto ghost = std::make_shared<Ghost>(grid);
    auto order = std::make_shared<Order>(grid, ghost);
    auto fraction = MakeFractionField(grid, ghost, order);

    BoundaryIndex bi;
    auto interfaces = ReconstructInterfacePLIC(fraction, bi, method);
    PlotReconstructedInterface(method, n, fraction, interfaces);

    double domain_area = 0.0;
    double l1_integral = 0.0;
    double l2_integral = 0.0;
    double linf_average = 0.0;

    Field error_area(grid, ghost, order);
    error_area.assign(0.0);

    const double tol = 1e-16;
    auto circle = MakePresetCircle();
    for(auto& idx : fraction.order()) {
        const auto dx = grid->s_(_X_, idx);
        const auto dy = grid->s_(_Y_, idx);
        const auto cell_area = dx * dy;
        domain_area += cell_area;

        const auto value = fraction(idx);

        if(value <= tol || value >= 1.0 - tol) {
            continue;
        }

        ASSERT(interfaces(idx) != nullptr);
        if(interfaces(idx) == nullptr) {
            continue;
        }

        auto pbox_min = grid->v(0, idx);
        Circle local_circle(
                circle.xc() - pbox_min.x(),
                circle.yc() - pbox_min.y(),
                circle.r());
        const auto cell_error_area = ErrorArea(
                dx, dy, *(interfaces(idx)), local_circle);
        const auto cell_error_average = cell_error_area / cell_area;
        error_area(idx) = cell_error_area;

        l1_integral += cell_error_area;
        l2_integral += cell_error_average * cell_error_average * cell_area;
        linf_average = std::max(linf_average, cell_error_area);
    }
    PlotErrorAreaContour(method, n, error_area, interfaces);

    auto n1 = l1_integral / domain_area;
    auto n2 = std::sqrt(l2_integral / domain_area);
    auto ni = linf_average;

    std::cout << "Norm1   = " << n1 << std::endl;
    std::cout << "Norm2   = " << n2 << std::endl;
    std::cout << "NormInf = " << ni << std::endl;

    l1.push_back(n1);
    l2.push_back(n2);
    li.push_back(ni);
}

inline void RunMethod(const std::string& method) {
    std::filesystem::create_directories(OUTPUTPATH);

    std::vector<int> vn = {8, 16, 32, 64, 128};
    std::list<double> l1, l2, li;
    for(auto& n : vn) {
        ReconstructSolver(method, n, l1, l2, li);
    }

    OutputError(OUTPUTPATH + method + "_error_table.txt", vn, l1, l2, li);
    PlotError(OUTPUTPATH + method + "_error", 1, vn, l1, l2, li);
}

inline void RunAll() {
    RunMethod("ccs");
    RunMethod("youngs");
}

}

#endif
