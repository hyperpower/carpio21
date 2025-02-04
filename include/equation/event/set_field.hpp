#ifndef _EVENT_SET_FIELD_HPP
#define _EVENT_SET_FIELD_HPP

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
class EventSetFieldCenter_ : public Event_<D>{
public:
    typedef D Domain;
    typedef Event_<D> Event;
    typedef EquationBase_<D> EquationBase;

    typedef typename Domain::FieldCenter   FieldCenter;
    typedef typename Domain::FunXYZT_Value Fun;


protected:
    std::string _sn;    // scalar name
    Fun         _fun;           // function
public:

    /**
     *  @brief Event constructor
     *
     *  default constructor is running event after each step
     */
    EventSetFieldCenter_(const std::string& fieldn, 
                     int is    = -1,    int ie = -1,
                     int istep = 1,     int flag = Event::AFTER) :
                         _sn(fieldn),
                         Event(is, ie, istep, flag) {
         _fun = [](Vt, Vt, Vt, Vt){return 0.0;};
    }
    
    EventSetFieldCenter_(const std::string& fieldn, Fun fun, 
                     int is    = -1,    int ie = -1,
                     int istep = 1,     int flag = Event::AFTER) :
                         _sn(fieldn), _fun(fun),
                         Event(is, ie, istep, flag) {
    }


    virtual int execute(St step, Vt t, int fob, EquationBase& equ) {
        if(equ.has_field(_sn)){
            auto& f = equ.field(_sn);
            f.assign(_fun, t);            
        }else{
            std::cerr<< "EventSetFieldCenter : Field " << _sn << " not found!" << std::endl;
        }
        return -1;
    }
    std::string name() const {
        return "SetFieldCenter";
    }
    

    

    



};


}

#endif