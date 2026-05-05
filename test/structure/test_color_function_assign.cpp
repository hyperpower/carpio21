#include "gtest/gtest.h"

#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/scommon.hpp"
#include "geometry/boolean/intersection_box_box.hpp"
#include "geometry/boolean/intersection_circle_box.hpp"
#include "geometry/io/ggnuplot_actor_maker.hpp"
#include "geometry/objects/analytic/circle.hpp"
#include "geometry/objects/basic/box.hpp"

#include <cmath>
#include <filesystem>

using namespace carpio;

const std::string OUTPUTPATH = "./fig/";

const int fig_width  = 1200;
const int fig_height = 900;

TEST(color_function_assign, circle_area_field) {
    const std::size_t dim = 2;
    typedef SGridUniform_<dim> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;
    typedef typename Grid::Index Index;
    typedef Point_<double, dim> Point;
    typedef Box_<double, dim> Box;
    typedef Circle_<double> Circle;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 20;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x() / double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid, spghost));

    Field area(spgrid, spghost, sporder);
    Circle circle(0.5, 0.5, 0.25);

    auto assign_area = [&circle](Field& field, const Index& idx) {
        auto& grid = field.grid();
        auto pbox_min = grid.v(0, idx);
        auto pbox_max = grid.v(3, idx);
        Box box(Point(pbox_min.x(), pbox_min.y()),
                Point(pbox_max.x(), pbox_max.y()));
        auto intersect_area = IntersectAreaCircleBox(circle, box);
        field(idx) = intersect_area / box.area();
    };
    ForEach(area, assign_area);

    double sum_area = 0.0;
    for (auto& idx : area.order()) {
        const auto value = area(idx);
        ASSERT_GE(value, 0.0);
        ASSERT_LE(value, 1.0);
        sum_area += value;
    }

    // ASSERT_NEAR(sum_area, circle.area(), 1e-10);
    // ASSERT_NEAR(area(Index(0, 0)), 0.0, 1e-10);
    // ASSERT_GT(area(Index(n / 2 - 1, n / 2 - 1)), 0.0);

    std::filesystem::create_directories(OUTPUTPATH);

    Gnuplot gnu;
    gnu.set_xrange(0.0, 1.0);
    gnu.set_yrange(0.0, 1.0);
    // gnu.set_zrange(0.0, spgrid->volume(Index(0, 0)));
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_red_blue();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);

    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.command("using 1:2:(0.0) title \"grid\" ");
    agrid.line_color_grey().line_width(1);

    auto acircle = ToGnuplotActor(circle);
    acircle.command("using 1:2:(0.0) title \"circle\" ");
    acircle.line_color_red().line_width(2);

    gnu.add(ToGnuplotActorContour(area));
    gnu.add(agrid);
    gnu.add(acircle);
    gnu.set_terminal_png(OUTPUTPATH + "ColorFunctionAssignCircleArea",
                         fig_width, fig_height);
    gnu.plot();
}

TEST(color_function_assign, slotted_disk) {
    const std::size_t dim = 2;
    typedef SGridUniform_<dim> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;
    typedef typename Grid::Index Index;
    typedef Point_<double, dim> Point;
    typedef Box_<double, dim> Box;
    typedef Circle_<double> Circle;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(4, 4, 1);
    int n = 200;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x() / double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid, spghost));

    Field area(spgrid, spghost, sporder);
    Circle circle(2.0, 2.75, 0.5);
    Box slot_box(Point(1.94, 2.2), Point(2.06, 2.85));

    auto assign_area = [&circle, &slot_box](Field& field, const Index& idx) {
        auto& grid = field.grid();
        auto pbox_min = grid.v(0, idx);
        auto pbox_max = grid.v(3, idx);
        Box cell_box(Point(pbox_min.x(), pbox_min.y()),
                     Point(pbox_max.x(), pbox_max.y()));
        auto circle_area = IntersectAreaCircleBox(circle, cell_box);
        auto slot_area = IntersectAreaBoxBox(slot_box, cell_box);
        field(idx) = std::max(0.0, (circle_area - slot_area)) / cell_box.area();
    };
    ForEach(area, assign_area);

    double sum_area = 0.0;
    for (auto& idx : area.order()) {
        const auto value = area(idx);
        ASSERT_TRUE(std::isfinite(value));
        sum_area += value * spgrid->volume(idx);
    }
    // ASSERT_NEAR(sum_area, circle.area() - slot_box.area(), 1e-8);

    std::filesystem::create_directories(OUTPUTPATH);

    Gnuplot gnu;
    gnu.set_xrange(1.2, 2.8);
    gnu.set_yrange(2.0, 3.6);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_red_blue();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);

    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.command("using 1:2:(0.0) title \"grid\" ");
    agrid.line_color_grey().line_width(1);

    auto acircle = ToGnuplotActor(circle);
    acircle.command("using 1:2:(0.0) title \"circle\" ");
    acircle.line_color_red().line_width(2);

    auto abox = ToGnuplotActor(slot_box);
    abox.command("using 1:2:(0.0) title \"slot box\" ");
    abox.line_color_black().line_width(2);

    gnu.add(ToGnuplotActorContour(area));
    gnu.add(agrid);
    gnu.add(acircle);
    gnu.add(abox);
    gnu.set_terminal_png(OUTPUTPATH + "ColorFunctionAssignSlottedDisk",
                         fig_width, fig_height);
    gnu.plot();
}

TEST(color_function_assign, box_area_field) {
    const std::size_t dim = 2;
    typedef SGridUniform_<dim> Grid;
    typedef std::shared_ptr<Grid> spGrid;

    typedef SGhostRegular_<dim, Grid> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    typedef SOrderXYZ_<dim, Grid, Ghost, CenterTag> Order;
    typedef std::shared_ptr<Order> spOrder;

    typedef SFieldCenter_<dim, double, Grid, Ghost, Order> Field;
    typedef typename Grid::Index Index;
    typedef Point_<double, dim> Point;
    typedef Box_<double, dim> Box;

    Point_<Vt, dim> pmin(0, 0, 0);
    Point_<Vt, dim> pmax(1, 1, 1);
    int n = 20;
    spGrid spgrid(new Grid(pmin, {n, n}, pmax.x() / double(n), 2));

    spGhost spghost(new Ghost(spgrid));

    spOrder sporder(new Order(spgrid, spghost));

    Field area(spgrid, spghost, sporder);
    Box target_box(Point(0.2, 0.2), Point(0.73, 0.73));

    auto assign_area = [&target_box](Field& field, const Index& idx) {
        auto& grid = field.grid();
        auto pbox_min = grid.v(0, idx);
        auto pbox_max = grid.v(3, idx);
        Box cell_box(Point(pbox_min.x(), pbox_min.y()),
                     Point(pbox_max.x(), pbox_max.y()));
        auto intersect_area = IntersectAreaBoxBox(target_box, cell_box);
        field(idx) = intersect_area / cell_box.area();
    };
    ForEach(area, assign_area);

    double sum_area = 0.0;
    for (auto& idx : area.order()) {
        const auto value = area(idx);
        ASSERT_GE(value, 0.0);
        ASSERT_LE(value, 1.0);
        sum_area += value * spgrid->volume(idx);
    }
    ASSERT_NEAR(sum_area, target_box.area(), 1e-12);
    ASSERT_NEAR(area(Index(0, 0)), 0.0, 1e-12);
    ASSERT_NEAR(area(Index(n / 2, n / 2)), 1.0, 1e-12);

    std::filesystem::create_directories(OUTPUTPATH);

    Gnuplot gnu;
    gnu.set_xrange(0.0, 1.0);
    gnu.set_yrange(0.0, 1.0);
    gnu.set_equal_aspect_ratio();
    gnu.set_palette_red_blue();
    gnu.set_ticslevel(0.1);
    gnu.set_view(55, 25, 1.2, 1.0);

    auto agrid = ToGnuplotActorWireFrame(*spgrid);
    agrid.command("using 1:2:(0.0) title \"grid\" ");
    agrid.line_color_grey().line_width(1);

    auto abox = ToGnuplotActor(target_box);
    abox.command("using 1:2:(0.0) title \"box\" ");
    abox.line_color_red().line_width(2);

    gnu.add(ToGnuplotActorContour(area));
    gnu.add(agrid);
    gnu.add(abox);
    gnu.set_terminal_png(OUTPUTPATH + "ColorFunctionAssignBoxArea",
                         fig_width, fig_height);
    gnu.plot();
}
