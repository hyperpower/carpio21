#ifndef _EQUATION_BASE_HPP_
#define _EQUATION_BASE_HPP_

#include "type_define.hpp"
#include "utility/any.hpp"
#include "time_control.hpp"
#include "stop_control.hpp"
#include "domain/boundary/boundary_index.hpp"
#include <memory>
#include <map>

namespace carpio {

template<class EQU> class Event_;

template<St DIM, class D>
class EquationBase_{
public:
    typedef D Domain;
    typedef typename Domain::ValueType   Vt;
    typedef typename Domain::Index       Index;
    typedef typename Domain::Grid         Grid;
    typedef typename Domain::spGrid      spGrid;
    typedef typename Domain::Ghost       Ghost;
    typedef typename Domain::spGhost     spGhost;
    typedef typename Domain::Order       Order;
    typedef typename Domain::spOrder     spOrder;
    typedef typename Domain::FieldCenter FieldCenter;
    typedef typename Domain::FieldCenterExp FieldCenterExp;

    typedef EquationBase_<DIM, D> Self;

    typedef Event_<Self>                   Event;
    typedef std::shared_ptr<Event>       spEvent;
    typedef std::shared_ptr<FieldCenter>    spFieldCenter;
    typedef std::shared_ptr<FieldCenterExp> spFieldCenterExp;
    typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

    typedef std::map<std::string, Any>             Configures;
    typedef std::map<std::string, spFieldCenter>   Fields;
    typedef std::map<std::string, spBoundaryIndex> BIs;
    typedef std::map<std::string, spEvent>         Events;

    typedef TimeControl_<DIM> TimeControl;
    typedef std::shared_ptr<TimeControl> spTimeControl;

    typedef StopControl_<Self> StopControl;
    typedef std::shared_ptr<StopControl> spStopControl;

    typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;

    typedef Solver_<Vt>             Solver;
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt> Solver_Jacobi;
    typedef SOR_<Vt>    Solver_SOR;
    typedef CG_<Vt>     Solver_CG;


protected:
    spGrid  _spgrid; 
    spGhost _spghost; 
    spOrder _sporder;

    Configures _configs;
    Fields     _fields;
    BIs        _bis;
    Events     _events; 

    spTimeControl _time;
    spStopControl _stop;
public:
    EquationBase_(){}; 

    EquationBase_(spGrid  spgrid, 
                  spGhost spghost,
                  spOrder sporder):
        _spgrid(spgrid), 
        _spghost(spghost),
        _sporder(sporder), 
        _time(nullptr){
    }

    virtual ~EquationBase_(){};

    virtual int run_one_step(St step) = 0;

    virtual int initialize() = 0; 

    virtual int finalize() =0; 

    virtual int solve() = 0;

    void run() {
        // the equation don't have time
        if (this->_time == nullptr) {
            initialize();
            // start events
            run_events(0, 0.0, Event::START);

            solve();

            run_events(1, 0.0, Event::END);
            finalize();
        } else {
            // events before calculation
            initialize();
            run_events(this->_time->current_step(), //
                    this->_time->current_step(),    //
                    Event::START);
            // loop
            while (!this->_time->is_end() && (!_stop->is_stop())) {
                //
                // events before each step
                run_events(this->_time->current_step(),  //
                        this->_time->current_time(),     //
                        Event::BEFORE);

                // run one step =================
                run_one_step(this->_time->current_step());
                // ==============================

                // events after each step
                run_events(this->_time->current_step(),  //
                        this->_time->current_time(),     //
                        Event::AFTER);
                //
                this->_time->advance();
            }
            // if(this->has_event("_STOP_")){
            //     this->_events["_STOP_"]->show();
            // }
            // events after calculation
            run_events(this->_time->current_step(),    //
                    this->_time->current_time(),       //
                    Event::END);
            finalize();
        }
    }

    void run_events(St step, Vt t, int fob) {
        for (auto& event : this->_events) {
            if (event.second->do_execute(step, t, fob)) {
                event.second->execute(step, t, fob, this);
            }
        }
    }

    // virtual void run_events(St step, Vt t, int fob) = 0; 
    

    virtual Ghost& ghost() {
        return *(this->_spghost);
    }

    virtual Grid& grid() {
        return *(this->_spgrid);
    }

    virtual const Ghost& ghost() const{
        return *(this->_spghost);
    }

    virtual const Grid& grid() const{
        return *(this->_spgrid);
    }

    virtual const TimeControl& time_control() const{
        return *(this->_time);
    }

    bool has_field(const std::string& key) const {
        auto it = this->_fields.find(key);
        if (it != this->_fields.end()) {
            return true;
        }
        return false;
    }

    bool has_flag(const std::string& key) const {
        auto it = this->_configs.find(key);
        if (it != this->_configs.end()) {
            return true;
        }
        return false;
    }

    void set_solver(const std::string&    name,
                    const int& max_iter = 1000,
                    const Vt&  tol      = 1e-4,
                    const Any& any      = 1.0
                    ) {
        // name should be
        ASSERT(name == "Jacobi" || name == "CG" || name == "SOR");
        this->_configs["set_solver"]           = name;
        this->_configs["set_solver_max_iter"]  = max_iter;
        this->_configs["set_solver_tolerence"] = tol;
        if (name == "SOR") {
            this->_configs["SOR_omega"] = any;
        }
    }

protected:
    void new_field(const std::string& name){
        if(!(this->has_field(name))){
            this->_fields[name] = spFieldCenter(new FieldCenter(
                    this->_spgrid,
                    this->_spghost,
                    this->_sporder));
        }
    }

    spFieldCenterExp new_field_exp() const{
        return spFieldCenterExp(new FieldCenterExp(
                    this->_spgrid,
                    this->_spghost,
                    this->_sporder));
    }

    template<class Container>
    void new_fields(const Container& list){
        auto isstring = std::is_same<typename Container::value_type,
                                     std::string>::value;
        ASSERT(isstring == true);
        for(auto& str : list){
            this->new_field(str);
        }
    }

    virtual spSolver _init_solver() {
        // initial solver
        spSolver spsolver;
        if (this->has_flag("set_solver")) {
            std::string sn = any_cast<std::string>(
                    this->_configs["set_solver"]);
            Vt  tol      = any_cast<Vt>(this->_configs["set_solver_tolerence"]);
            int max_iter = any_cast<int>(this->_configs["set_solver_max_iter"]);
            if (sn == "Jacobi") {
                spsolver = spSolver(new Solver_Jacobi(max_iter, tol));
            } else if (sn == "CG") {
                spsolver = spSolver(new Solver_CG(max_iter, tol));
            } else if (sn == "SOR") {
                ASSERT(this->has_flag("SOR_omega"));
                Vt omega = any_cast<Vt>(this->_configs["SOR_omega"]);
                spsolver = spSolver(new Solver_SOR(max_iter, tol, omega));
            }
        } else {
            // default solver
            spsolver = spSolver(new Solver_Jacobi(500, 1e-4));
        }
        return spsolver;
    }
};



}

#endif