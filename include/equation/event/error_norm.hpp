#ifndef _EVENT_ERROR_NORM_HPP
#define _EVENT_ERROR_NORM_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"
#include "domain/base/base_operator.hpp"
#include "event.hpp"
#include "utility/clock.hpp"
#include "utility/tinyformat.hpp"

namespace carpio{

template<class D>
class EventNorm_ : public Event_<D>{
public:
    typedef D Domain;
    typedef Event_<D> Event;
    typedef EquationBase_<D> EquationBase;

    typedef typename Domain::FieldCenter FieldCenter;
protected:
    std::string   _fname;

    std::list<Vt> _l1;  // Norm1 
    std::list<Vt> _l2;  // Norm2
    std::list<Vt> _li;  // Norm_inf
public:

    /**
     *  @brief Event constructor
     *
     *  default constructor is running event after each step
     */
    EventNorm_(const std::string& fieldn, 
               int is    = -1,    int ie = -1,
               int istep = 1,     int flag = Event::AFTER) :
               _fname(fieldn),
        Event(is, ie, istep, flag) {
    }

    virtual int execute(St step, Vt t, int fob, EquationBase& equ) = 0;

    virtual std::string name() const {
        return "ErrorNorm";
    }
    const std::list<Vt>& get_list_norm1() const{
        return _l1;
    }
    const std::list<Vt>& get_list_norm2() const{
        return _l2;
    }
    const std::list<Vt>& get_list_norminf() const{
        return _li;
    }
};

template<class D>
class EventNormExactFieldCenter_ : public EventNorm_<D>{
public:
    typedef D Domain;
    typedef Event_<D> Event;
    typedef EventNorm_<D> Base;
    typedef EquationBase_<D> EquationBase;

    typedef typename Domain::FieldCenter FieldCenter;
protected:
    const FieldCenter* _exact;
public:
    EventNormExactFieldCenter_(const FieldCenter* exact, const std::string& fieldn, 
                     int is    = -1,    int ie = -1,
                     int istep = 1,     int flag = Event::AFTER) :
                         _exact(exact), Base(fieldn, is, ie, istep, flag){}

    virtual int execute(St step, Vt t, int fob, EquationBase& equ) {
        this->record_execute_info(step, t, fob);
        if(equ.has_field(this->_fname)){
            auto& exact = *_exact;
            auto error = equ.field(this->_fname) - exact;
            auto n1 = Norm1(error);
            auto n2 = Norm2(error);
            auto ni = NormInf(error);
            this->_l1.push_back(n1);
            this->_l2.push_back(n2);
            this->_li.push_back(ni);
        }else{
            std::cerr<< "EventErrorNorm : Field " << this->_fname << " not found!" << std::endl;
        }
        return -1;
    }
    std::string name() const {
        return "ErrorNormFieldCenter";
    }
};

template<class D>
class EventNormExactFunction_ : public EventNorm_<D>{
public:
    typedef D Domain;
    typedef Event_<D> Event;
    typedef EventNorm_<D> Base;
    typedef EquationBase_<D> EquationBase;

    typedef typename Domain::FieldCenter FieldCenter;
    typedef typename Domain::FunXYZT_Value Fun;
protected:
    Fun _fun;
public:
    EventNormExactFunction_(Fun f, const std::string& fieldn, 
                     int is    = -1,    int ie = -1,
                     int istep = 1,     int flag = Event::AFTER) :
                    _fun(f), Base(fieldn, is, ie, istep, flag){}

    virtual int execute(St step, Vt t, int fob, EquationBase& equ) {
        this->record_execute_info(step, t, fob);
        
        if(equ.has_field(this->_fname)){
            auto& f = equ.field(this->_fname);
            auto n1 = Norm1(f, _fun);
            auto n2 = Norm2(f, _fun);
            auto ni = NormInf(f, _fun);
            this->_l1.push_back(n1);
            this->_l2.push_back(n2);
            this->_li.push_back(ni);
        }else{
            std::cerr<< "EventErrorNorm : Field " << this->_fname << " not found!" << std::endl;
        }
        return -1; 
    }
    std::string name() const {
        return "ErrorNormExactFunction";
    }
};

template<class D>
class EventConditonNormPrevious_ : 
    public EventNormExactFieldCenter_<D>, public Condition_<D>{
public:
    typedef D Domain;
    typedef Event_<D> Event;
    typedef EventNormExactFieldCenter_<D> EventBase;
    typedef Condition_<D> Contdition;
    typedef EquationBase_<D> EquationBase;

    typedef typename Domain::FieldCenter     FieldCenter;
    typedef typename Domain::spFieldCenter spFieldCenter;
protected:
    Vt          _cv1;     // critical value,
                          //  a small value > 0.
                          //  if it's less than 0,
                          //  the critical value will not be checked
    Vt          _cv2;     // critical value
    Vt          _cvinf;   // critical value

    spFieldCenter _fp;   //field previous
    int         _stepp;  //step previous
    int         _count;  //count

    std::string _relation; // "or" or "and"
public:
    EventConditonNormPrevious_(const std::string& fname,
        const Vt& cv1, const Vt& cv2, const Vt& cvinf,
        int istep = 1) :
            _fp(nullptr),
            _cv1(cv1), _cv2(cv2), _cvinf(cvinf),
            _count(0),_stepp(istep),
            EventBase(nullptr, fname,-1, -1, istep, Event::AFTER){
        
        }
        
    bool is_satisfied() const {
        return this->_is_satisfied;
    }

    int execute(St step, Vt t, int fob, EquationBase& equ) {
        auto& fc = equ.field(this->_fname);
        if(_fp == nullptr){
            _fp = std::make_shared<FieldCenter>(fc.new_compatible_zero());
            this->_exact = _fp.get();
        }
        if (this->_count == 0) {
            *_fp = fc;                   // store previous field
            _stepp = step;              // previous step
            _count++;
            return _SUCCESS;
        } else {
            // norm1
            EventBase::execute(step, t, fob, equ); 
            _check_satisfied(this->_relation);

            *_fp = fc;
            _stepp = step;
            _count++;
        }
        return -1;
    }
    virtual bool is_condition_event() const{
        return true;
    }
protected:
    void _check_satisfied(const std::string& relation = "and") {  // default relation is and
        std::array<int, 3> arrs = {_check_satisfied_norm1(),
                                   _check_satisfied_norm2(),
                                   _check_satisfied_norminf()};
        if(relation == "and"){
            for(auto& state : arrs){
                if(state <= 0){
                    this->_is_satisfied = false;
                    return;
                }
            }
            this->_is_satisfied = true;
        }else{ // relation == "or"
            for (auto& state : arrs) {
                if (state == 1) {
                    this->_is_satisfied = true;
                    return;
                }
            }
            this->_is_satisfied = false;
        }
    }

    int _check_satisfied_norm1() const {
        if (_cv1 > 0) {
            return this->_l1.back() < _cv1 ? 1 : 0;
        }
        return -1;
    }
    int _check_satisfied_norm2() const {
        if (_cv2 > 0) {
            return this->_l2.back() < _cv2 ? 1 : 0;
        }
        return -1;
    }
    int _check_satisfied_norminf() const {
        if (_cvinf > 0) {
            return this->_li.back() < _cvinf ? 1 : 0;
        }
        return -1;
    }

};




}

#endif