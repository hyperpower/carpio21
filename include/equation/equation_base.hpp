#ifndef _EQUATION_BASE_HPP_
#define _EQUATION_BASE_HPP_

#include "type_define.hpp"
#include "utility/any.hpp"
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
    typedef typename Domain::Grid        Grid;
    typedef typename Domain::Ghost       Ghost;
    typedef typename Domain::Order       Order;
    typedef typename Domain::FieldCenter FieldCenter;

    typedef Event_<DIM, D>       Event;
    typedef std::shared_ptr<Event>       spEvent;
    typedef std::shared_ptr<FieldCenter> spFieldCenter;

    typedef std::map<std::string, Any>           Configures;
    typedef std::map<std::string, spFieldCenter> Fields;

protected:
    Configures _config;


};



}

#endif