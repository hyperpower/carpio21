#ifndef _STOP_CONTROL_HPP
#define _STOP_CONTROL_HPP

#include "type_define.hpp"
#include <map>
#include <utility>
#include <memory>
#include <list>

namespace carpio {

template<class D> class EventCondition_;

template<class D>
class StopControl_ {
public:
    typedef StopControl_<D>      Self;
    typedef StopControl_<D>& ref_Self;
    typedef StopControl_<D>* pSelf;
    typedef std::shared_ptr<Self> spStopManager;

    typedef EventCondition_<D> EventCondition;
    typedef std::shared_ptr<EventCondition> spEventCondition;

    typedef std::list<spEventCondition> Container;

protected:
    Container _conditions;

    std::string _relation;  // ralation can be "and" or "or"
                            // Default is "and"
public:
    /*
     * constructor
     */
    StopControl_() : _relation("and"){
    }

    virtual ~StopControl_() {
    }

    void set_relation_and(){
        _relation = "and";
    }
    void set_relation_or(){
        _relation = "or";
    }
    bool is_stop() const{
        if(_conditions.empty()){
            return false;
        }
        if(_relation == "and"){
            return _is_stop_and();
        }else if(_relation == "or"){
            return _is_stop_or();
        }else{
            SHOULD_NOT_REACH;
            return true;
        }
    }

    void add_condition(spEventCondition spec){
        _conditions.push_back(spec);
    }

protected:
    bool _is_stop_and() const{
        for(auto& c : _conditions){
            if(!c->is_satisfied()){
                return false;
            }
        }
        return true;
    }

    bool _is_stop_or() const {
        for (auto& c : _conditions) {
            if (c->is_satisfied()) {
                return true;
            }
        }
        return false;
    }

};

}

#endif