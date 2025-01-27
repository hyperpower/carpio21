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
class EventErrorNorm_ : public Event_<D>{
public:
    typedef D Domain;
    typedef Event_<D> Event;
    typedef EquationBase_<D> EquationBase;

    typedef typename Domain::FieldCenter FieldCenter;
protected:
    FieldCenter _exact;
    std::string _sn;   // field name to compare

    std::list<Vt> _l1;  // Norm1 
    std::list<Vt> _l2;  // Norm2
    std::list<Vt> _li;  // Norm_inf
public:

    /**
     *  @brief Event constructor
     *
     *  default constructor is running event after each step
     */
    EventErrorNorm_(const FieldCenter& exact, const std::string& fieldn, 
                     int is    = -1,    int ie = -1,
                     int istep = 1,     int flag = Event::AFTER) :
                         _exact(exact), _sn(fieldn),
                         Event(is, ie, istep, flag) {
    }

    virtual int execute(St step, Vt t, int fob, EquationBase& equ) {
        this->record_execute_info(step, t, fob);
        if(equ.has_field(_sn)){
            auto error = _exact - equ.field(_sn);
            auto n1 = Norm1(error);
            auto n2 = Norm2(error);
            auto ni = NormInf(error);
            _l1.push_back(n1);
            _l2.push_back(n2);
            _li.push_back(ni);
        }else{
            std::cerr<< "EventErrorNorm : Field " << _sn << " not found!" << std::endl;
        }
        return -1;
    }
    std::string name() const {
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
// protected:
    

    



};


}

#endif