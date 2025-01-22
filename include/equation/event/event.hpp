#ifndef _EVENT_HPP
#define _EVENT_HPP

#include <map>
#include <utility>
#include <iostream>
#include "type_define.hpp"
#include "equation/equation_base.hpp"

namespace carpio{

// template<class D> class EquationBase_;

template<class D>
class Event_ {
public:
    // static const St Dim         = DIM;

    // typedef EQU   Equ;
    // typedef Equ* pEqu;
    // typedef const Equ* const_pEqu;
    typedef D Domain;
    typedef EquationBase_<D> EquationBase;
    

    static const int START  = 0x10;
    static const int END    = 0x20;
    static const int BEFORE = 0x100;
    static const int AFTER  = 0x200;

protected:

    int _flag;
    int _istart, _iend, _istep;

public:
    /**
     *  @brief Event constructor
     *
     *  default constructor is running event after each step
     */

    Event_(int is = -1, int ie = -1, int istep = -1, int flag = 0) {
        _flag   = flag;
        _istart = is;
        _iend   = ie;
        _istep  = istep;
    }
    

    virtual int execute(St step, Vt t, int fob, EquationBase& equ) {
        std::cout << "Event : execute equ\n";
        return -1;
    }

    virtual int flag() const {
        return _flag;
    }

    virtual void set_flag(int i) {
        _flag = i;
    }

    virtual std::string name() const{
        return "Event Base";
    }

    virtual std::string flag_string() const {
        std::string str = "";
        if (this->has_flag(START)) {
            str += "START ";
        }
        if (this->has_flag(END)) {
            str += "END ";
        }
        if (this->has_flag(BEFORE)) {
            str += "BEFORE ";
        }
        if (this->has_flag(AFTER)) {
            str += "AFTER";
        }
        return str;
    }

    virtual ~Event_() {
    }

    /**
     *  @brief Dose the event need to be executed
     *
     *  @param [step] is step right now
     *  @param [t]    is time right now
     *  @param [fob]  one of START END BEFORE or AFTER
     */
    bool do_execute(St step, Vt t, int fob) const {
        if (this->has_flag(fob)) {
            bool res = ((step - this->_istart) % this->_istep == 0);
            if (this->_iend != -1) {
                res = res && int(step) <= this->_iend;
            }
            if (this->_istart != -1) {
                res = res && int(step) >= this->_istart;
            }
            return res;
        } else {
            return false;
        }
    }

    bool has_flag(int f) const {
        return (this->_flag | f) == this->_flag ? true : false;
    }

    virtual bool is_condition_event() const{
        return false;
    }

    virtual void show() const{
        std::cout << "Event Base Show" << std::endl;
    }
};

template<class D>
class EventCondition_ : public Event_<D>{
public:
    typedef Event_<D> Event;
    typedef EquationBase_<D> EquationBase;
protected:
    bool _is_satisfied;
public:
    EventCondition_(int is    = -1, int ie   = -1,
                    int istep = -1, int flag = Event::AFTER ):
        _is_satisfied(false),
        Event(is, ie, istep, flag) {
    }

    virtual ~EventCondition_() {
    }

    virtual bool is_satisfied() const {
        return _is_satisfied;
    }

    virtual bool is_condition_event() const{
        return true;
    }

    virtual int execute(St step, Vt t, int fob, EquationBase& equ) {
        std::cout << "Event : execute equ\n";
        return -1;
    }
};


}

#endif