#ifndef _BEN_OPOINTPOINT_SPOINTRUCPOINTURES_HPP_
#define _BEN_OPOINTPOINT_SPOINTRUCPOINTURES_HPP_

#include <memory>
#include <queue>
#include <iostream>
#include <set>

#include "geometry/operation/boolean/segment_segment.hpp"


namespace carpio{

template<class POINT>
class SweepEvent_ {
private:
    typedef POINT Obj;
    typedef std::shared_ptr<Obj> spObj;
    spObj obj; 
public:
    SweepEvent_() : obj() {};

    SweepEvent_(const SweepEvent_& e) : obj(e.obj) {};

    SweepEvent_(const POINT& o) : obj(std::make_shared<Obj>(o)) {};

    const Obj& get_obj() const{
        return *obj;
    }
    // bool operator<(const SweepEvent_& e) const{
        // return COMPARE{}(*obj, *(e.obj));
    // }
};



template <class KEY, class VALUE, class COMP> 
class SweepEventQueue_ : public std::map<KEY, std::list<VALUE>, COMP> {
public:
    typedef std::map<KEY, std::list<VALUE>, COMP> Base;

    void push(const KEY& value) {
        std::list<VALUE> v; //empty vector
        this->insert(std::pair<KEY, std::list<VALUE> > (value, v));
    }

    void push(const KEY& value, const VALUE& assoc) {
        std::list<VALUE> v; 
        v.push_back(assoc);
        std::pair<typename Base::iterator, bool> ret =
        this->insert(std::pair<KEY, std::list<VALUE> > (value, v));
        if(false == ret.second){
            ret.first->second.push_back(assoc);
        }
        return;
    }

    KEY top() {
        return this->begin()->first;
    }

    void pop() {
        this->erase(this->begin());
    }

    bool mem(const KEY& value) {
        return (this->find(value) != this->end());
    }
};



}

#endif