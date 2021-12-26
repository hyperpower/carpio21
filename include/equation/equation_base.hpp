#ifndef _EQUATION_BASE_HPP_
#define _EQUATION_BASE_HPP_

#include "type_define.hpp"
#include "utility/any.hpp"
#include "time_control.hpp"
#include "domain/boundary/boundary_index.hpp"
#include <memory>
#include <map>

namespace carpio {

template<St DIM, class D> class Event_;

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

    typedef Event_<DIM, D>       Event;
    typedef std::shared_ptr<Event>       spEvent;
    typedef std::shared_ptr<FieldCenter> spFieldCenter;
    typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

    typedef std::map<std::string, Any>           Configures;
    typedef std::map<std::string, spFieldCenter> Fields;
    typedef std::map<std::string, spBoundaryIndex> BIs;

    typedef TimeControl_<DIM> TimeControl;
    typedef std::shared_ptr<TimeControl> spTimeControl;

protected:
    spGrid  _spgrid; 
    spGhost _spghost; 
    spOrder _sporder;

    Configures _config;
    Fields     _fields;
    BIs        _bis; 

    spTimeControl _time;

public:
    EquationBase_():{
    } 

    EquationBase_(spGrid spgrid, spGhost spghost, spOrder sporder):
        _spgrid(spgrid), _spghost(spghost), _sporder(sporder), _time(nullptr){
    }

    virtual int run_one_step(St step) {
        std::cout << step << "  Equation: run one step \n";
        return -1;
    }

    virtual int initialize() {
        std::cout << "  Equation: initial \n";
        return -1;
    }

    virtual int finalize() {
        std::cout << "  Equation: finalize \n";
        return -1;
    }

    virtual int solve() {
        std::cout << "  Equation: solve \n";
        return -1;
    }

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
            while (!this->_time->is_end() && (!_stop_manager.is_stop())) {
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
            if(this->has_event("_STOP_")){
                this->_events["_STOP_"]->show();
            }
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
};



}

#endif