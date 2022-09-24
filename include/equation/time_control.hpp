#ifndef _TIME_CONTROL_HPP
#define _TIME_CONTROL_HPP

#include "type_define.hpp"
#include <map>
#include <utility>
#include <memory>

namespace carpio {

template<class D>
class TimeControl_ {
public:
    typedef TimeControl_<D> Self;
    typedef TimeControl_<D>& ref_Self;
    typedef TimeControl_<D>* pSelf;
    typedef std::shared_ptr<Self> spTime;

protected:
    St _max_step;
    Vt _dt; // dt

    //ArrayListV<St> _arr_idx;
    //St _inner_step;

    Vt _tau; // coefficient

    St _cur_step;  // current step
    Vt _cur_time;  // current time

public:
    /*
     * constructor
     */
    TimeControl_(St n, Vt dt, Vt tau = 1) :
            _max_step(n), _dt(dt), _tau(tau) {
        this->_cur_step = 0;
        this->_cur_time = 0;
    }

    virtual ~TimeControl_() {
    }

    const Vt& dt() const {
        return _dt;
    }

    const St& max_step() const {
        return this->_max_step;
    }

    void set_dt(Vt dt) {
        this->_dt = dt;
    }

    void set_max_step(St ms) {
        this->_max_step = ms;
    }

    void advance() {
        this->_cur_step++;
        this->_cur_time += _dt;
    }

    St current_step() const{
        return this->_cur_step;
    }

    Vt current_time() const{
        return this->_cur_time;
    }

    bool is_end() const{
        if (this->_cur_step > this->_max_step){
            return true;
        }else{
            return false;
        }
    }
    //vt tau() const {
    //    return _tau;
    //}
};

}

#endif