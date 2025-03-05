#ifndef _EXAMPLE_FD_H
#define _EXAMPLE_FD_H

#include <iostream>
#include <fstream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "equation/equation.hpp"
#include "domain/structure/io/splotly_actor.hpp"

#include "convergence_analysis.hpp"

using namespace carpio;

namespace fd{

const std::size_t dim = 3;
typedef SGridUniform_<dim> Grid;
typedef std::shared_ptr<Grid> spGrid;

typedef SGhostRegular_<dim, Grid> Ghost;
typedef std::shared_ptr<Ghost> spGhost;

typedef SOrderXYZ_<dim, Grid, Ghost, VertexTag> Order;
typedef std::shared_ptr<Order> spOrder;

typedef Point_<double,dim> Point;

typedef SFieldVertex_<dim, double, Grid, Ghost, Order> Field;


void LaplaceSolver(const std::string& scheme, int n, 
                   std::list<double>& l1, 
                   std::list<double>& l2, 
                   std::list<double>& li,
                   std::list<std::list<double> >& lr){
    std::cout << "[  Laplace ] Solver"<<std::endl;
    std::cout << "[   INFO   ] Dim = " << dim << std::endl;
    std::cout << "[   INFO   ] n   = " << n << std::endl;
    Point p(0.,0.,0.);

    spGrid  spgrid(new Grid(p, n, 1.0, 2));
    spGhost spghost(new Ghost(spgrid));
    spOrder sporder(new Order(spgrid, spghost));

    typedef StructureDomain_<dim, Grid, Ghost, Order> Domain;
    typedef Laplace_<Domain> Laplace;

    // Define the equation
    Laplace equ(spgrid, spghost, sporder);

    // Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
	spBC spbc_zero(new BoundaryConditionValue(BC::_BC1_, 0.0));
    spBC spbc_x0(new BoundaryConditionFunXYZ(BC::_BC1_, [](
        typename Domain::ValueType x,
        typename Domain::ValueType y,
        typename Domain::ValueType z) {
            return std::sin(_PI_*y)*std::sin(_PI_*z);
        }));
    spBC spbc_x1(new BoundaryConditionFunXYZ(BC::_BC1_, [](
        typename Domain::ValueType x,
        typename Domain::ValueType y,
        typename Domain::ValueType z) {
            return 2 * std::sin(_PI_*y)*std::sin(_PI_*z);
        }));
    spbi->insert(0, spbc_x0);
	spbi->insert(1, spbc_x1);
	spbi->insert(2, spbc_zero);
	spbi->insert(3, spbc_zero);
	spbi->insert(4, spbc_zero);
	spbi->insert(5, spbc_zero);
	equ.set_boundary_index("phi", spbi);

    // // Set solver
	equ.set_solver("SOR", 20000, 1e-12);
    equ.set_space_scheme(scheme);

    
    // Add events
	typedef Event_<Domain> Event;
	typedef std::shared_ptr<Event>  spEvent;
	spEvent spetime(new EventOutputTime_<Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

    equ.run();
    
    //residual 
    auto spsolver = equ.get_solver();
    lr.push_back(spsolver->get_residual_array());

    // // error
    auto exact = equ.field("phi").new_compatible_zero();
    exact.assign([](typename Field::ValueType x,
                    typename Field::ValueType y,
                    typename Field::ValueType z,
                    double t){
        return (std::sin( _PI_ * y) * std::sin( _PI_ * z) / std::sinh( _PI_ * std::sqrt(2.0)))
            * (2 * std::sinh( _PI_ * x * std::sqrt(2.0) )
            +  std::sinh( _PI_ * (1 - x) * std::sqrt(2.0) ));
    });
    auto error = exact - equ.field("phi");

    l1.push_back(Norm1(error));
    l2.push_back(Norm2(error));
    li.push_back(NormInf(error));
}


void AScheme(const std::string& scheme){
    std::vector<int> vn = {4, 8, 16, 32};
    // std::vector<int> vn = {8 };
    std::list<double> l1,l2,li;
    std::list<std::list<double> > lr;
    for(auto& n : vn){
        LaplaceSolver(scheme, n, l1, l2, li, lr);
    }
    // // output to a file
    OutputError(FIG_PATH +"/"+ scheme + "_error_table.txt",vn, l1, l2, li);

    // // plot residual
    PlotResidual(FIG_PATH +"/"+ scheme + "_residual", vn, lr);
    if(scheme == "HOC4"){
        PlotError(FIG_PATH +"/"+ scheme + "_error", 4, vn, l1, l2, li);
    }else{
        PlotError(FIG_PATH +"/"+ scheme + "_error", 2, vn, l1, l2, li);
    }
}

}


#endif