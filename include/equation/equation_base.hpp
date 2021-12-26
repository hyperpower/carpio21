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

    typedef EquationBase_<DIM, D> Self;

    typedef Event_<Self>                   Event;
    typedef std::shared_ptr<Event>       spEvent;
    typedef std::shared_ptr<FieldCenter> spFieldCenter;
    typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

    typedef std::map<std::string, Any>             Configures;
    typedef std::map<std::string, spFieldCenter>   Fields;
    typedef std::map<std::string, spBoundaryIndex> BIs;
    typedef std::map<std::string, spEvent>         Events;

    typedef TimeControl_<DIM> TimeControl;
    typedef std::shared_ptr<TimeControl> spTimeControl;

    typedef StopControl_<Self> StopControl;
    typedef std::shared_ptr<StopControl> spStopControl;

protected:
    spGrid  _spgrid; 
    spGhost _spghost; 
    spOrder _sporder;

    Configures _config;
    Fields     _fields;
    BIs        _bis;
    Events     _events; 

    spTimeControl _time;
    spStopControl _stop;
public:
    EquationBase_():{} 

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
            while (!this->_time->is_end() && (!_stop.is_stop())) {
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

    // void run_events(St step, Vt t, int fob) {
    //     for (auto& event : this->_events) {
    //         if (event.second->do_execute(step, t, fob)) {
    //             event.second->execute(step, t, fob, this);
    //         }
    //     }
    // }

    virtual void run_events(St step, Vt t, int fob) = 0; 
    

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
};



}

#endif